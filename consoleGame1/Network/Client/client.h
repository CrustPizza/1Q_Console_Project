#pragma once
#include "Network/Socket.h"
#include "Game/character.h"
#include <memory>
#include <mutex>

using std::shared_ptr;
using std::recursive_mutex;

class Client
{
private:
	shared_ptr<Socket>	user;
	shared_ptr<thread>	updater;
	shared_ptr<thread>	sender;
	string				ReceivedData;

public:
	bool				gameStart;
	bool				send_on;
	int					timer;
	unsigned int		frame;

	recursive_mutex buffer_mutex;
	string				buffer;
	queue<pair<char, string>> todo_list;

	Client();
	~Client();

	bool Check_Receive_Request(shared_ptr<Socket> client);
	bool Connect(const Endpoint& endpoint);
	void Sender();
	void Update();
	void Release();
};