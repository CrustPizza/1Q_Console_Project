#pragma once
#include <string>
#include <WinSock2.h>

class Endpoint
{
public:
	Endpoint();
	Endpoint(const char* address, int port);
	~Endpoint();

	// 네트워크 주소
	sockaddr_in m_ipv4Endpoint;

	// 이건 어따쓰는 걸까
	// 클라이언트 쪽에서 사용되었다.
	static Endpoint Any;

	std::string ToString();
};