#include "stdafx.h"
#include "Server.h"

Server::Server(int threadCount)
{
	frame = 0;

	// IOCP 생성하고 스레드 개수를 입력
	iocp = make_shared<Iocp>(threadCount);

	// socket 생성하고 호스트 설정
	listen = make_shared<Socket>(SocketType::TCP);
	listen->Bind(Endpoint("0.0.0.0", 4823));
	listen->Listen();

	// IOCP에 이벤트 발생 확인할 소켓 등록
	iocp->Add(*listen, nullptr);

	// 클라이언트 받을 소켓 생성
	ProspectiveClient = make_shared<RemoteClient>(SocketType::TCP);

	// 클라이언트 요청이 있는지 확인한다.
	Check_Accept_Request();

	// 이벤트 대기열 생성
	readEvents = make_shared<IocpEvents>();

	// 서버 시작
	updater = make_shared<thread>([this] { while (true) Update(); });
}

Server::~Server()
{
	Release();
}

/// <summary>
/// 호스트의 IP 얻기
/// </summary>
string Server::get_host_ip()
{
	// IP를 얻기전에 먼저 호스트의 이름을 얻는다.
	char buf[32];
	gethostname(buf, 32);

	// 호스트의 이름으로 IP 주소를 얻는다.
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	getaddrinfo(buf, NULL, &hints, &res);

	// IP를 문자열로 저장하여 반환한다.
	string host_ip = "";
	for (int i = 2; i < 6; i++)
	{
		_itoa_s((unsigned char)res->ai_addr->sa_data[i], buf, 10);
		host_ip += buf;

		if (i != 5)
		{
			host_ip += '.';
		}
	}

	return host_ip;
}

/// <summary>
/// 연결된 클라이언트의 수
/// </summary>
int Server::get_client_count()
{
	return remoteClients.size();
}

/// <summary>
/// Listen Socket으로 연결 요청이 들어오는 것을 확인하기 위한 Event 설정을 한다.
/// </summary>
void Server::Check_Accept_Request()
{
	if (listen->AcceptOverlapped(ProspectiveClient->tcpConnection)
		&& WSAGetLastError() != ERROR_IO_PENDING)
	{
		throw Exception("Overlapped AcceptEx failed.");
	}

	// Overlapped 설정
	listen->m_isReadOverlapped = true;
}

/// <summary>
/// 클라이언트와 연결된 소켓의 버퍼에 데이터 수신이 되는 것을 확인하기 위한 Event 설정
/// </summary>
bool Server::Check_Receive_Request(shared_ptr<RemoteClient> client)
{
	// 수신 오류가 생겼다면 닫아버린다.
	if (client->tcpConnection.ReceiveOverlapped() != 0
		&& WSAGetLastError() != ERROR_IO_PENDING)
	{
		client->tcpConnection.Close();
		return false;
	}

	return true;
}

void Server::Update()
{
	// 이벤트 대기
	iocp->Wait(*readEvents, 100);

	// 이벤트 배열 루프
	for (int i = 0; i < readEvents->m_eventCount; i++)
	{
		// 현재 이벤트 배열 받음
		auto& readEvent = readEvents->m_events[i];

		// Key가 0, 즉 Listen Socket일 경우
		if (readEvent.lpCompletionKey == 0)
		{
			// false하는 이유는 잘 모르겠지만..
			// 이벤트가 발생했으므로 일단 꺼두고 다시 키는 용도 같다.
			listen->m_isReadOverlapped = false;

			// Accept 중인데 0이 아닌 값이 반환된다면 오류가 생긴상황이므로 닫아버린다.
			if (ProspectiveClient->tcpConnection.UpdateAcceptContext(*listen) != 0)
			{
				listen->Close();

				continue;
			}

			// 연결된 클라이언트를 IOCP에 추가하고 새로 만들기 위한 작업을 한다.
			shared_ptr<RemoteClient> newClient = ProspectiveClient;

			// 연결된 클라이언트 IOCP에 추가
			iocp->Add(newClient->tcpConnection, newClient.get());

			// 새 클라이언트의 수신 요청을 받는다.
			if (Check_Receive_Request(newClient) == true)
			{
				// 요청 대기가 되어 true
				newClient->tcpConnection.m_isReadOverlapped = true;

				// 현재 이용중인 클라이언트 리스트에 등록한다.
				remoteClients.insert({ newClient.get(), newClient });
				ReceivedData[newClient->tcpConnection.m_fd] = "";

				// 클라이언트가 연결된것에 대해 반환 값을 줘서
				// 출력에 사용한다면 여기를 이용할 예정
			}

			// 또 다른 클라이언트의 요청을 받기 위한 준비를 한다.
			ProspectiveClient = make_shared<RemoteClient>(SocketType::TCP);
			Check_Accept_Request();

			continue;
		}

		// 여기는 리슨 소켓이 아닌 이벤트를 처리하는 곳

		// 현재 클라이언트의 정보를 가져온다.
		shared_ptr<RemoteClient> curClient = remoteClients[(RemoteClient*)readEvent.lpCompletionKey];

		// 키 값이 잘못 들어간다거나해서 혹시 모를 예외처리
		if (curClient != nullptr)
		{
			curClient->tcpConnection.m_isReadOverlapped = false;

			// ec는 무슨 줄임말일까
			// 에코서버 예제여서 echo였을 것 같다.
			int dataSize = readEvent.dwNumberOfBytesTransferred;

			if (dataSize <= 0)
			{
				// 전송 데이터가 0이하일 때 어떤 처리를 해야할까
				// 원문 예제에서는 int a = 0;이 들어가 있다 왤까
				// 아무래도 밑에서 예외처리가 중복되는거 같아서 단순한 오타로 보고 코드를 옮겼다.
				ProcessClientLeave(curClient);

				continue;
			}

			// 버퍼를 복사한다.
			// 값이 바뀌면 안되서 그런것 같다.
			// TCP 스트림 특성상 일부만 송신되는 경우도 원래 고려해야하는데
			// 이해를 돕기위해 안되어있다고한다.
			// 그럼 구현하기 위해서 송신할 때 송신되는 데이터의 끝을 알 수 있게 해야겠다.
			// 어차피 TCP는 순서대로 전송되니까


			// 일단 임시 String에 수신 버퍼를 받은 후
			// 수신된 길이만큼 수신 저장 문자열에 넣는다.
			string temp = curClient->tcpConnection.m_receiveBuffer;
			ReceivedData[curClient->tcpConnection.m_fd] += temp.substr(0, dataSize);

			// 수신된 데이터를 분석해서 나눈다.
			analysis(curClient->tcpConnection.m_fd);

			 // 다시 수신받기위해 요청 대기시켜야 한다는데
			 // 데이터가 쪼개져서 왔을 때는 어떻게 해야하는거지?
			 // 쪼개져서 왔을 때도 요청 대기시키고 그전 데이터를 보관만 해두는 건가
			if (Check_Receive_Request(curClient) == true)
			{
				// 성공적으로 요청 대기됨
				curClient->tcpConnection.m_isReadOverlapped = true;
			}
			else
			{
				// 요청 대기 실패
				remoteClients.erase(curClient.get());
			}
		}
	}
}

void Server::Release()
{
	// 서버 종료

	// listen 닫기
	listen->Close();

	// 클라이언트 연결 끊기
	for (auto i : remoteClients)
	{
		i.second->tcpConnection.Close();
	}

	while (remoteClients.size() > 0 || listen->m_isReadOverlapped == true)
	{
		// iterator
		for (auto i = remoteClients.begin(); i != remoteClients.end();)
		{
			// Overlapped되지 않은 클라이언트를 지운다.
			if (i->second->tcpConnection.m_isReadOverlapped == false)
			{
				i = remoteClients.erase(i);
			}
			else
			{
				i++;
			}
		}

		// 입력되는 이벤트들을 정리한다
		IocpEvents readEvents;
		iocp->Wait(readEvents, 100);

		for (int i = 0; i < readEvents.m_eventCount; i++)
		{
			auto& readEvent = readEvents.m_events[i];

			if (readEvent.lpCompletionKey == 0)
			{
				listen->m_isReadOverlapped = false;
			}
			else
			{
				shared_ptr<RemoteClient> curClient = remoteClients[(RemoteClient*)readEvent.lpCompletionKey];

				if (curClient != nullptr)
				{
					curClient->tcpConnection.m_isReadOverlapped = false;
				}
			}
		}
	}

	// 릴리즈 과정에서 이해 안되는 점?
	// 소켓을 닫았어도 이벤트가 새로 들어올 수 있는 건지
	// 아니면 누적된 이벤트를 정리하고 지우겠다는 건지 잘 모르겠다.
	// 그리고 Overlapped 상태에서 지웠을 때 생기는 문제점이 뭘까
}

/// <summary>
/// 수신된 데이터를 분석한다.
/// </summary>
void Server::analysis(SOCKET cur_sock)
{
	// 수신된 데이터의 바이트 크기를 구한다
	int dataLength = (ReceivedData[cur_sock][0] - '0') * 100
		+ (ReceivedData[cur_sock][1] - '0') * 10
		+ (ReceivedData[cur_sock][2] - '0');

	// 패킷이 모두 들어왔을 경우
	while (dataLength <= ReceivedData[cur_sock].length())
	{
		// 캐릭터 및 폭탄
		if (ReceivedData[cur_sock][4] == (char)Type::character_type
			|| ReceivedData[cur_sock][4] == (char)Type::bomb_type)
		{
			// 캐릭터 데이터 or 폭탄 데이터를 Queue에 예약한다.
			// ReceivedData[3]은 frame을 맞추기위한 데이터(였던것)
			string buf = ReceivedData[cur_sock].substr(4, dataLength - 4);

			todo_list.push(std::make_pair(ReceivedData[cur_sock][3] - '0', buf));
		}
		// 시간
		else if (ReceivedData[cur_sock][4] == (char)Type::time_type)
		{
			// 동기화
			timer = atoi(ReceivedData[cur_sock].substr(5, dataLength - 5).c_str());
		}

		// 작업이 끝난 문자열을 제거해준다.
		int subLength = ReceivedData[cur_sock].length() - dataLength;

		if (subLength > 0)
		{
			ReceivedData[cur_sock] = ReceivedData[cur_sock].substr(dataLength, subLength);

			// 온전히 전송된 패킷이 또 있는지 확인하기 위해
			// 바이트 크기를 계산한다.
			dataLength = (ReceivedData[cur_sock][0] - '0') * 100
						+ (ReceivedData[cur_sock][1] - '0') * 10
						+ (ReceivedData[cur_sock][2] - '0');
		}
		else
		{
			ReceivedData[cur_sock].clear();
		}
	}
}

/// <summary>
/// 클라이언트와 연결이 끊어졌을 때 처리
/// </summary>
void Server::ProcessClientLeave(shared_ptr<RemoteClient> client)
{
	client->tcpConnection.Close();
	remoteClients.erase(client.get());

	// 클라이언트가 떠났을 때 메시지
}

/// <summary>
/// 다인 플레이까지 가정하고 만들까 했던 흔적
/// </summary>
void Server::Syncro(SOCKET to, string data)
{
	// 싱크로를 위해 클라이언트들에게 데이터 전송
	for (auto i : remoteClients)
	{
		// 송신자와 수신자가 일치하지 않도록 설정
		if (to == i.second->tcpConnection.m_fd)
		{
			continue;
		}

		i.second->tcpConnection.Send(data.c_str(), data.length());
	}

	// 호스트를 위한 싱크로 작업
	// 예정
}

/// <summary>
/// 모든 클라이언트에게 전송
/// </summary>
void Server::SendToAll(string data)
{
	// 호스트의 입력을 클라이언트 전체에게 전송
	for (auto i : remoteClients)
	{
		i.second->tcpConnection.Send(data.c_str(), data.length());
	}
}