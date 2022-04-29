#pragma once
#include <string>
#include <WinSock2.h>

class Endpoint
{
public:
	Endpoint();
	Endpoint(const char* address, int port);
	~Endpoint();

	// ��Ʈ��ũ �ּ�
	sockaddr_in m_ipv4Endpoint;

	// �̰� ������� �ɱ�
	// Ŭ���̾�Ʈ �ʿ��� ���Ǿ���.
	static Endpoint Any;

	std::string ToString();
};