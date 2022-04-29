#include "stdafx.h"
#include "Server.h"

Server::Server(int threadCount)
{
	frame = 0;

	// IOCP �����ϰ� ������ ������ �Է�
	iocp = make_shared<Iocp>(threadCount);

	// socket �����ϰ� ȣ��Ʈ ����
	listen = make_shared<Socket>(SocketType::TCP);
	listen->Bind(Endpoint("0.0.0.0", 4823));
	listen->Listen();

	// IOCP�� �̺�Ʈ �߻� Ȯ���� ���� ���
	iocp->Add(*listen, nullptr);

	// Ŭ���̾�Ʈ ���� ���� ����
	ProspectiveClient = make_shared<RemoteClient>(SocketType::TCP);

	// Ŭ���̾�Ʈ ��û�� �ִ��� Ȯ���Ѵ�.
	Check_Accept_Request();

	// �̺�Ʈ ��⿭ ����
	readEvents = make_shared<IocpEvents>();

	// ���� ����
	updater = make_shared<thread>([this] { while (true) Update(); });
}

Server::~Server()
{
	Release();
}

/// <summary>
/// ȣ��Ʈ�� IP ���
/// </summary>
string Server::get_host_ip()
{
	// IP�� ������� ���� ȣ��Ʈ�� �̸��� ��´�.
	char buf[32];
	gethostname(buf, 32);

	// ȣ��Ʈ�� �̸����� IP �ּҸ� ��´�.
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	getaddrinfo(buf, NULL, &hints, &res);

	// IP�� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�.
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
/// ����� Ŭ���̾�Ʈ�� ��
/// </summary>
int Server::get_client_count()
{
	return remoteClients.size();
}

/// <summary>
/// Listen Socket���� ���� ��û�� ������ ���� Ȯ���ϱ� ���� Event ������ �Ѵ�.
/// </summary>
void Server::Check_Accept_Request()
{
	if (listen->AcceptOverlapped(ProspectiveClient->tcpConnection)
		&& WSAGetLastError() != ERROR_IO_PENDING)
	{
		throw Exception("Overlapped AcceptEx failed.");
	}

	// Overlapped ����
	listen->m_isReadOverlapped = true;
}

/// <summary>
/// Ŭ���̾�Ʈ�� ����� ������ ���ۿ� ������ ������ �Ǵ� ���� Ȯ���ϱ� ���� Event ����
/// </summary>
bool Server::Check_Receive_Request(shared_ptr<RemoteClient> client)
{
	// ���� ������ ����ٸ� �ݾƹ�����.
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
	// �̺�Ʈ ���
	iocp->Wait(*readEvents, 100);

	// �̺�Ʈ �迭 ����
	for (int i = 0; i < readEvents->m_eventCount; i++)
	{
		// ���� �̺�Ʈ �迭 ����
		auto& readEvent = readEvents->m_events[i];

		// Key�� 0, �� Listen Socket�� ���
		if (readEvent.lpCompletionKey == 0)
		{
			// false�ϴ� ������ �� �𸣰�����..
			// �̺�Ʈ�� �߻������Ƿ� �ϴ� ���ΰ� �ٽ� Ű�� �뵵 ����.
			listen->m_isReadOverlapped = false;

			// Accept ���ε� 0�� �ƴ� ���� ��ȯ�ȴٸ� ������ �����Ȳ�̹Ƿ� �ݾƹ�����.
			if (ProspectiveClient->tcpConnection.UpdateAcceptContext(*listen) != 0)
			{
				listen->Close();

				continue;
			}

			// ����� Ŭ���̾�Ʈ�� IOCP�� �߰��ϰ� ���� ����� ���� �۾��� �Ѵ�.
			shared_ptr<RemoteClient> newClient = ProspectiveClient;

			// ����� Ŭ���̾�Ʈ IOCP�� �߰�
			iocp->Add(newClient->tcpConnection, newClient.get());

			// �� Ŭ���̾�Ʈ�� ���� ��û�� �޴´�.
			if (Check_Receive_Request(newClient) == true)
			{
				// ��û ��Ⱑ �Ǿ� true
				newClient->tcpConnection.m_isReadOverlapped = true;

				// ���� �̿����� Ŭ���̾�Ʈ ����Ʈ�� ����Ѵ�.
				remoteClients.insert({ newClient.get(), newClient });
				ReceivedData[newClient->tcpConnection.m_fd] = "";

				// Ŭ���̾�Ʈ�� ����ȰͿ� ���� ��ȯ ���� �༭
				// ��¿� ����Ѵٸ� ���⸦ �̿��� ����
			}

			// �� �ٸ� Ŭ���̾�Ʈ�� ��û�� �ޱ� ���� �غ� �Ѵ�.
			ProspectiveClient = make_shared<RemoteClient>(SocketType::TCP);
			Check_Accept_Request();

			continue;
		}

		// ����� ���� ������ �ƴ� �̺�Ʈ�� ó���ϴ� ��

		// ���� Ŭ���̾�Ʈ�� ������ �����´�.
		shared_ptr<RemoteClient> curClient = remoteClients[(RemoteClient*)readEvent.lpCompletionKey];

		// Ű ���� �߸� ���ٰų��ؼ� Ȥ�� �� ����ó��
		if (curClient != nullptr)
		{
			curClient->tcpConnection.m_isReadOverlapped = false;

			// ec�� ���� ���Ӹ��ϱ�
			// ���ڼ��� �������� echo���� �� ����.
			int dataSize = readEvent.dwNumberOfBytesTransferred;

			if (dataSize <= 0)
			{
				// ���� �����Ͱ� 0������ �� � ó���� �ؾ��ұ�
				// ���� ���������� int a = 0;�� �� �ִ� �ͱ�
				// �ƹ����� �ؿ��� ����ó���� �ߺ��Ǵ°� ���Ƽ� �ܼ��� ��Ÿ�� ���� �ڵ带 �Ű��.
				ProcessClientLeave(curClient);

				continue;
			}

			// ���۸� �����Ѵ�.
			// ���� �ٲ�� �ȵǼ� �׷��� ����.
			// TCP ��Ʈ�� Ư���� �Ϻθ� �۽ŵǴ� ��쵵 ���� ����ؾ��ϴµ�
			// ���ظ� �������� �ȵǾ��ִٰ��Ѵ�.
			// �׷� �����ϱ� ���ؼ� �۽��� �� �۽ŵǴ� �������� ���� �� �� �ְ� �ؾ߰ڴ�.
			// ������ TCP�� ������� ���۵Ǵϱ�


			// �ϴ� �ӽ� String�� ���� ���۸� ���� ��
			// ���ŵ� ���̸�ŭ ���� ���� ���ڿ��� �ִ´�.
			string temp = curClient->tcpConnection.m_receiveBuffer;
			ReceivedData[curClient->tcpConnection.m_fd] += temp.substr(0, dataSize);

			// ���ŵ� �����͸� �м��ؼ� ������.
			analysis(curClient->tcpConnection.m_fd);

			 // �ٽ� ���Źޱ����� ��û �����Ѿ� �Ѵٴµ�
			 // �����Ͱ� �ɰ����� ���� ���� ��� �ؾ��ϴ°���?
			 // �ɰ����� ���� ���� ��û ����Ű�� ���� �����͸� ������ �صδ� �ǰ�
			if (Check_Receive_Request(curClient) == true)
			{
				// ���������� ��û ����
				curClient->tcpConnection.m_isReadOverlapped = true;
			}
			else
			{
				// ��û ��� ����
				remoteClients.erase(curClient.get());
			}
		}
	}
}

void Server::Release()
{
	// ���� ����

	// listen �ݱ�
	listen->Close();

	// Ŭ���̾�Ʈ ���� ����
	for (auto i : remoteClients)
	{
		i.second->tcpConnection.Close();
	}

	while (remoteClients.size() > 0 || listen->m_isReadOverlapped == true)
	{
		// iterator
		for (auto i = remoteClients.begin(); i != remoteClients.end();)
		{
			// Overlapped���� ���� Ŭ���̾�Ʈ�� �����.
			if (i->second->tcpConnection.m_isReadOverlapped == false)
			{
				i = remoteClients.erase(i);
			}
			else
			{
				i++;
			}
		}

		// �ԷµǴ� �̺�Ʈ���� �����Ѵ�
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

	// ������ �������� ���� �ȵǴ� ��?
	// ������ �ݾҾ �̺�Ʈ�� ���� ���� �� �ִ� ����
	// �ƴϸ� ������ �̺�Ʈ�� �����ϰ� ����ڴٴ� ���� �� �𸣰ڴ�.
	// �׸��� Overlapped ���¿��� ������ �� ����� �������� ����
}

/// <summary>
/// ���ŵ� �����͸� �м��Ѵ�.
/// </summary>
void Server::analysis(SOCKET cur_sock)
{
	// ���ŵ� �������� ����Ʈ ũ�⸦ ���Ѵ�
	int dataLength = (ReceivedData[cur_sock][0] - '0') * 100
		+ (ReceivedData[cur_sock][1] - '0') * 10
		+ (ReceivedData[cur_sock][2] - '0');

	// ��Ŷ�� ��� ������ ���
	while (dataLength <= ReceivedData[cur_sock].length())
	{
		// ĳ���� �� ��ź
		if (ReceivedData[cur_sock][4] == (char)Type::character_type
			|| ReceivedData[cur_sock][4] == (char)Type::bomb_type)
		{
			// ĳ���� ������ or ��ź �����͸� Queue�� �����Ѵ�.
			// ReceivedData[3]�� frame�� ���߱����� ������(������)
			string buf = ReceivedData[cur_sock].substr(4, dataLength - 4);

			todo_list.push(std::make_pair(ReceivedData[cur_sock][3] - '0', buf));
		}
		// �ð�
		else if (ReceivedData[cur_sock][4] == (char)Type::time_type)
		{
			// ����ȭ
			timer = atoi(ReceivedData[cur_sock].substr(5, dataLength - 5).c_str());
		}

		// �۾��� ���� ���ڿ��� �������ش�.
		int subLength = ReceivedData[cur_sock].length() - dataLength;

		if (subLength > 0)
		{
			ReceivedData[cur_sock] = ReceivedData[cur_sock].substr(dataLength, subLength);

			// ������ ���۵� ��Ŷ�� �� �ִ��� Ȯ���ϱ� ����
			// ����Ʈ ũ�⸦ ����Ѵ�.
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
/// Ŭ���̾�Ʈ�� ������ �������� �� ó��
/// </summary>
void Server::ProcessClientLeave(shared_ptr<RemoteClient> client)
{
	client->tcpConnection.Close();
	remoteClients.erase(client.get());

	// Ŭ���̾�Ʈ�� ������ �� �޽���
}

/// <summary>
/// ���� �÷��̱��� �����ϰ� ����� �ߴ� ����
/// </summary>
void Server::Syncro(SOCKET to, string data)
{
	// ��ũ�θ� ���� Ŭ���̾�Ʈ�鿡�� ������ ����
	for (auto i : remoteClients)
	{
		// �۽��ڿ� �����ڰ� ��ġ���� �ʵ��� ����
		if (to == i.second->tcpConnection.m_fd)
		{
			continue;
		}

		i.second->tcpConnection.Send(data.c_str(), data.length());
	}

	// ȣ��Ʈ�� ���� ��ũ�� �۾�
	// ����
}

/// <summary>
/// ��� Ŭ���̾�Ʈ���� ����
/// </summary>
void Server::SendToAll(string data)
{
	// ȣ��Ʈ�� �Է��� Ŭ���̾�Ʈ ��ü���� ����
	for (auto i : remoteClients)
	{
		i.second->tcpConnection.Send(data.c_str(), data.length());
	}
}