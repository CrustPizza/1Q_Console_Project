#include "stdafx.h"
#include "client.h"

Client::Client()
{
	frame = 0;

	user = make_shared<Socket>(SocketType::TCP);
	user->Bind(Endpoint::Any);
	updater = nullptr;
	sender = nullptr;
	gameStart = false;

	ReceivedData = "";
}

Client::~Client()
{
	Release();
}

bool Client::Check_Receive_Request(shared_ptr<Socket> client)
{
	// 수신 오류가 생겼다면 닫아버린다.
	if (user->ReceiveOverlapped() != 0
		&& WSAGetLastError() != ERROR_IO_PENDING)
	{
		user->Close();
		return false;
	}

	return true;
}

bool Client::Connect(const Endpoint& endpoint)
{
	if (user->Connect(endpoint) == true)
	{
		// 호스트와 연결이 성공했을 경우
		// Update와 Sender를 위한 Thread를 하나씩 생성한다.
		updater = make_shared<thread>([this] { while (true) Update(); });
		sender = make_shared<thread>([this] { while (true) Sender(); });

		return true;
	}

	return false;
}

void Client::Update()
{
	// 수신 대기
	int receiveLength = user->Receive();

	// 예외처리 해야하는 곳
	if (receiveLength < 0)
	{
		return;
	}

	// 일단 임시 String에 수신 버퍼를 받은 후
	// 수신된 길이만큼 수신 저장 문자열에 넣는다.
	string temp = user->m_receiveBuffer;
	ReceivedData += temp.substr(0, receiveLength);

	// 패킷의 바이트 크기를 계산한다.
	int dataLength = (ReceivedData[0] - '0') * 100
		+ (ReceivedData[1] - '0') * 10
		+ (ReceivedData[2] - '0');

	// 패킷이 모두 들어왔을 경우
	while (dataLength <= ReceivedData.length())
	{
		// 맵
		if (ReceivedData[4] == (char)Type::map_type)
		{
			// 맵을 동기화 한다.
			for (int i = 0; i < map_h; i++)
			{
				for (int j = 0; j < map_w; j++)
				{
					tempmap[i][j] = ReceivedData[5 + i * map_w + j];
				}
			}
		}
		// 캐릭터 및 폭탄
		else if (ReceivedData[4] == (char)Type::character_type
				|| ReceivedData[4] == (char)Type::bomb_type)
		{
			// 캐릭터 데이터 or 폭탄 데이터를 Queue에 예약한다.
			// ReceivedData[3]은 frame을 맞추기위한 데이터(였던것)
			string buf = ReceivedData.substr(4, dataLength - 4);

			todo_list.push(std::make_pair(ReceivedData[3] - '0', buf));
		}
		// 시간
		else if (ReceivedData[4] == (char)Type::time_type)
		{
			// 동기화를 한다.
			gameStart = true;
			timer = atoi(ReceivedData.substr(5, dataLength - 5).c_str());
		}

		// 작업이 끝난 문자열을 제거해준다.
		int subLength = ReceivedData.length() - dataLength;

		if (subLength > 0)
		{
			ReceivedData = ReceivedData.substr(dataLength, subLength);

			// 온전히 전송된 패킷이 또 있는지 확인하기 위해
			// 바이트 크기를 계산한다.
			dataLength = (ReceivedData[0] - '0') * 100
						+ (ReceivedData[1] - '0') * 10 
						+ (ReceivedData[2] - '0');
		}
		else
		{
			ReceivedData.clear();
		}
	}
}

void Client::Release()
{
	user->Close();

	if (updater != nullptr)
	{
		updater = nullptr;
	}

	if (sender != nullptr)
	{
		sender = nullptr;
	}
}

void Client::Sender()
{
	// 하나의 Thread로 전송만을 위한 작업을 한다.
	if (send_on == true)
	{
		// buffer를 사용하는 타이밍이 겹치지 않게 하기위한 Lock
		std::lock_guard<recursive_mutex> lock(buffer_mutex);
		user->Send(buffer.c_str(), buffer.length());
		send_on = false;
	}
}