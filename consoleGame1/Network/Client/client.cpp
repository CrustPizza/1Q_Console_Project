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
	// ���� ������ ����ٸ� �ݾƹ�����.
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
		// ȣ��Ʈ�� ������ �������� ���
		// Update�� Sender�� ���� Thread�� �ϳ��� �����Ѵ�.
		updater = make_shared<thread>([this] { while (true) Update(); });
		sender = make_shared<thread>([this] { while (true) Sender(); });

		return true;
	}

	return false;
}

void Client::Update()
{
	// ���� ���
	int receiveLength = user->Receive();

	// ����ó�� �ؾ��ϴ� ��
	if (receiveLength < 0)
	{
		return;
	}

	// �ϴ� �ӽ� String�� ���� ���۸� ���� ��
	// ���ŵ� ���̸�ŭ ���� ���� ���ڿ��� �ִ´�.
	string temp = user->m_receiveBuffer;
	ReceivedData += temp.substr(0, receiveLength);

	// ��Ŷ�� ����Ʈ ũ�⸦ ����Ѵ�.
	int dataLength = (ReceivedData[0] - '0') * 100
		+ (ReceivedData[1] - '0') * 10
		+ (ReceivedData[2] - '0');

	// ��Ŷ�� ��� ������ ���
	while (dataLength <= ReceivedData.length())
	{
		// ��
		if (ReceivedData[4] == (char)Type::map_type)
		{
			// ���� ����ȭ �Ѵ�.
			for (int i = 0; i < map_h; i++)
			{
				for (int j = 0; j < map_w; j++)
				{
					tempmap[i][j] = ReceivedData[5 + i * map_w + j];
				}
			}
		}
		// ĳ���� �� ��ź
		else if (ReceivedData[4] == (char)Type::character_type
				|| ReceivedData[4] == (char)Type::bomb_type)
		{
			// ĳ���� ������ or ��ź �����͸� Queue�� �����Ѵ�.
			// ReceivedData[3]�� frame�� ���߱����� ������(������)
			string buf = ReceivedData.substr(4, dataLength - 4);

			todo_list.push(std::make_pair(ReceivedData[3] - '0', buf));
		}
		// �ð�
		else if (ReceivedData[4] == (char)Type::time_type)
		{
			// ����ȭ�� �Ѵ�.
			gameStart = true;
			timer = atoi(ReceivedData.substr(5, dataLength - 5).c_str());
		}

		// �۾��� ���� ���ڿ��� �������ش�.
		int subLength = ReceivedData.length() - dataLength;

		if (subLength > 0)
		{
			ReceivedData = ReceivedData.substr(dataLength, subLength);

			// ������ ���۵� ��Ŷ�� �� �ִ��� Ȯ���ϱ� ����
			// ����Ʈ ũ�⸦ ����Ѵ�.
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
	// �ϳ��� Thread�� ���۸��� ���� �۾��� �Ѵ�.
	if (send_on == true)
	{
		// buffer�� ����ϴ� Ÿ�̹��� ��ġ�� �ʰ� �ϱ����� Lock
		std::lock_guard<recursive_mutex> lock(buffer_mutex);
		user->Send(buffer.c_str(), buffer.length());
		send_on = false;
	}
}