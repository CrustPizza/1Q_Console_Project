#pragma once
#include "Iocp.h"
#include "Socket.h"
#include "Endpoint.h"
#include "character.h"
#include <thread>
#include <unordered_map>
#include <map>
#include <queue>

using std::shared_ptr;
using std::thread;
using std::make_shared;
using std::unordered_map;
using std::map;
using std::string;
using std::pair;
using std::queue;

class RemoteClient
{
public:
	Socket tcpConnection;

	RemoteClient(SocketType socketType)
		:tcpConnection(socketType) {}
	~RemoteClient() {}
};

class Server
{
private:
	shared_ptr<Socket> listen;
	shared_ptr<thread> updater;
	map<SOCKET, string> ReceivedData;
	shared_ptr<Iocp> iocp;
	shared_ptr<IocpEvents> readEvents;
	shared_ptr<RemoteClient> ProspectiveClient;
	unordered_map<RemoteClient*, shared_ptr<RemoteClient>> remoteClients;

public:
	int timer;
	unsigned int frame;
	queue<pair<char, string>> todo_list;

	Server(int threadCount);
	~Server();

	string get_host_ip();
	int	get_client_count();
	void Check_Accept_Request();
	bool Check_Receive_Request(shared_ptr<RemoteClient> client);
	void Update();
	void Release();
	void analysis(SOCKET cur_sock);
	void ProcessClientLeave(shared_ptr<RemoteClient> client);
	void Syncro(SOCKET to, string data);
	void SendToAll(string data);
};