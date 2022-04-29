#include "stdafx.h"
#include "Socket.h"
#include <rpc.h>
#include "Endpoint.h"
#include "Socketinit.h"
#include "Exception.h"

Socket::Socket(SocketType socketType)
{
	// Touch�� ����ִµ� �� �ϴ� �ɱ�?
	// ���� ������ �͹̳� ����� �� shell ��ɾ�� Touch�� ����ϸ�
	// ������ ������ �� �����ǰ����ִ� ��ɵ� �־��µ�
	// ������ ȯ�濡�� Touch�Լ��� ȣ���� �� ���� ����� �ִ� ���ϱ�?
	// �ƴϸ� ���� ���� �ۼ��ؾ��ϴ� �κ��� �ִ°ǰ�..
	// å 3������� �׷� ������ �����µ�
	g_socketInit.Touch();

	// UDP�� ���� �Ⱦ��Ŵϱ� �����Ѵ�.
	// UDP�� ������ ���� ���� ���� ������ �ϴ� ������ ���� ����Ѵ�..
	if (socketType == SocketType::TCP)
	{
		// ���� �����Լ����� �������� �μ��� �� 0���� ����������
		// 0�� ������ 0���� ����θ� �ش� Ÿ���� ����Ʈ �������ݷ� ����ȴٰ� �Ѵ�.
		// �׷��� �׳� ��������� 0��� IPPROTO_TCP�� �����־���.
		// lpProtocol�� ������ ������ Ư���� ������ �� ����Ѵٴµ� �𸣰ڴ�..
		// g�� ���� �׷��̶�µ� �̰͵� �� �𸣰���
		// �߰� �Ӽ� �÷���.. �ٸ��� �𸣰����� �ϴ� IOCP�� ���� Overlapped I/O�� �����Ѵ�.
		m_fd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	}

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

Socket::Socket(SOCKET fd)
{
	g_socketInit.Touch();

	// �ٸ� ������ �����ϴ°� ������ ��� �����ؼ� ����ϴ� �ɱ�
	m_fd = fd;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

Socket::Socket()
{
	static_assert(-1 == INVALID_SOCKET, "");

	m_fd = -1;

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

Socket::~Socket()
{
	Close();
}

void Socket::Bind(const Endpoint& endpoint)
{
	// ���ϰ� ������ ���´�.
	// ��Ʈ��ũ �ּҿ� ��Ʈ�� ���� Ŭ�������� ��������.
	// bind�� �ƹ�ư ���´ٰ� �����ߴµ� m_fd�� ��� Unsigned int��.
	// ���⿡ ��� ���� �ɱ�?
	// �� ������ ���Ƶ� ������ ������ �� �� ������ �����ϱ����Ѱ� �ʿ����ٵ�
	// ������ ����Ű�� �������ϱ�?
	// SOCKET -> UINT_PTR -> unsigned __int64 �ε� ������?
	// �޸� ũ�⵵ ����, int���� int* �޸� �ּҸ� ������ ����ó�� ���� �� �����ϱ�
	// ������ ���ٵ� ��¥ �̷������� �����ϴ� ������ �ñ��ѵ� �� ���� ����.
	// ������ �˻��غ��� ��Ʈ��ũ���� ������������� std::bind()�Լ���°� �� �ִ�.
	if (bind(m_fd, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0)
	{
		// bind �������� ���� ó���ε�
		std::stringstream ss;
		ss << "bind failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

bool Socket::Connect(const Endpoint& endpoint)
{
	// Ŭ���̾�Ʈ�� ������ ���� ��û�ϴ� �Լ�
	// ���ڷ� ���� ������ ����õ��ϰ� ����Ǹ� m_fd�� �ǳ��ִ� �����ϵ�
	// �ٵ� connectEx�� �����ʾƵ� ������ �ǰ�
	if (connect(m_fd, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0)
	{
		return false;
	}

	return true;
}

int Socket::Send(const char* data, int length)
{
	// ���Ͼտ� �ݷ� 2���� ���� �ǹ��ϱ�
	// ���ӽ����̽��� Ŭ����, ����ü, ���� Ŭ���� ���� ������
	// ������ ����ϴ� �뵵�� ��������� �ִµ� �ƹ��͵� ���� �̷��� �Ǵ� ���� ó������
	// ã�ƺ��Ҵ��� �տ� �ٴ°� ���ٸ� �����̶� ���̶���Ѵ�.
	// ������ �������� �����ؾ��� ������ ���⼭�� �ʹ��� �ʴ´�.
	// int i = 0;
	// int main { int i = 0;
	// i = 10; // ���� ������ 10��
	// ::i = 10; } // ���� ������ 10 ��
	// ��� �˰ڴµ� �� send�տ� �̰� �ٿ��� ����ϴ� �ɱ�
	// flag�� �����͸� ���� Ȥ�� �д� ����� �ɼ��� �Ŵ� ���̴�.
	// ������ �о�� MSG_DONTWAIT �̶�� ���Ͽ��� ����ؾ��� �� ���� �ɼ��� �ִµ�
	// �������� ������� �ʾҴ� ������ ����
	return ::send(m_fd, data, length, 0);
}

void Socket::Close()
{
	// �� �𸣰����� ���������� �� ���� ������
	// �̷������� ���Ǵ°� ���� m_fd�� ����� ���� �����Ϳ� ����غ��δ�.
	closesocket(m_fd);
}

void Socket::Listen()
{
	// m_fd �������� ���� ���� ��û�� �ִ� 5000������ ����ϰ� �ްڴٴ� ��
	// �̰� ��⿭�ΰ�?
	listen(m_fd, 5000);
}

// �Լ��� �����صξ����� Overlapped I/O �ϴ°� �����ε� �Ӹ� �����ϱ�
// �м��� ���߿�
int Socket::Accept(Socket& acceptedSocket, std::string& errorText)
{
	// �м��� ���߿� �Ϸ��� �ߴµ� �� addrlen�� �����ͷ� �޴°ž�
	acceptedSocket.m_fd = accept(m_fd, nullptr, nullptr);
	if (acceptedSocket.m_fd == -1)
	{
		errorText = GetLastErrorAsString();
		return -1;
	}

	return 0;
}

bool Socket::AcceptOverlapped(Socket& acceptCandidateSocket)
{
	if (AcceptEx == nullptr)
	{
		DWORD bytes;
		GUID uuid_v = UUID(WSAID_ACCEPTEX);

		// Window Socket API...
		// input output control? �³�? ����
		WSAIoctl(m_fd, // ����
			// ���� �����ڵ�? The control code of operation to perform
			// ���ۿ� �˻��غ��� �ڷᰡ �� ������ �ʴ´�.
			// ���� ���� Ȯ�� �Լ� ������ �ޱ�ϱ� �Լ������Ϳ� �� �־��شٴ� �Ҹ��ϵ�
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			// UUID ���� �ĺ��ڶ�µ�..
			// �ϴ� MSDN���� �Է� ������ �����Ͷ�� �����ϰ� �����ִ�
			&uuid_v, sizeof(UUID),
			// UUID���� ������ �����ͼ� ���⿡ �־��ִ� �� �ʹ�.
			&AcceptEx, sizeof(AcceptEx),
			// ��� ���ϴ°ź��� ���� �߿��� ������ �ƴѰɷ� ����
			// 1. ������ ��¹��� byte ��ġ
			// 2. ����ü ������, �񵿱� ���Ͽ��� ���õȴٰ���
			// 3. ���� �� ������ ���Ͽ����� ���õȴٰ��ϴµ�
			// ����.. ������ �����ε�.. �� null���� ������..
			// �ƹ�ư ���� �Ϸ�� �� ȣ��� �Ϸ� ��ƾ�� �����Ͷ�� �Ѵ�.
			&bytes, nullptr, nullptr);

		// ������ �ߴµ��� nullptr�� ���
		if (AcceptEx == nullptr)
		{
			throw Exception("Getting AcceptEx ptr failed.");
		}
	}

	// accept�� ������ ���� �ּҿ� ����Ʈ �ּҰ� ä�����µ�
	// ������ ���ظ� �������� �׳� �����ٰ� �Ѵ�..
	// ���� �ּҴ� Ŭ���̾�Ʈ�� �ּ� ��Ⱑ �ƴұ� ������
	// ����Ʈ �ּҴ� ����..
	char ignored[200];
	DWORD ignored2 = 0;

	bool ret = AcceptEx(
		// Listen ���� - ���� ����
		m_fd,
		// Accept ���� - Ŭ���̾�Ʈ ����
		// bind, connect�� �Ǿ����� �ʾƾ� �Ѵٰ� �մϴ�.
		// bind�� �Ǿ���ϴ°� �ƴѰ� �ͱ�
		acceptCandidateSocket.m_fd,
		// ��� ����
		// ���� �ּҿ�, ����Ʈ �ּ�, ����� �ްԵǴ� ù ������ ��� ������ �����ϴ� ����
		&ignored,
		// ���� ������ ����
		// �ٵ� �� 0����
		// ��.. ������ �����ٰ��ؼ� 0�ΰŰ���.
		// ù �����ʹ� ������ ���ð� ����Ʈ �ּҴ� �����Ѵٰ� �Ѵ�.
		// ù ������ ������ ��ٸ��� �ʱ� ������ 0���� �� �� ���� ������ �ִ�
		0,
		// ���� �ּ� ����
		// �ּ� 16�� �����ؾ��Ѵ�.
		50,
		// ����Ʈ �ּ� ����
		// ���������� �ּ� 16�� �����ؾ��Ѵ�.
		50,
		// �Ϸ�� ������ overlapped ����ü�� �ּ�
		// ������ ���� ���Ҹ��� �𸣰ڴ�.
		&ignored2,
		&m_readOverlappedStruct) == true;

	return ret;
}

// AcceptEx�� I/O �ϷḦ �ϴ��� ���� TCP ���� �ޱ� ó���� �� ���� ���� �ƴϴ�.
// �� �Լ��� ȣ�����־�߸� �Ϸᰡ �ȴ�.
// ��� ������ �����ִ�. å�� �ٽ� �о���ϳ� ���Ҹ��� �𸣰ڴ�.
// �� ����� ���� ������ ��ȯ�ޱ� ���Ѱǰ�
int Socket::UpdateAcceptContext(Socket& listenSocket)
{
	sockaddr_in ignore1;
	INT			ignore2;
	sockaddr_in ignore3;
	INT			ignore4;

	char ignore[1000];
	// �޴� ����, ���� ũ��
	// ���� �ּ�, ���� �ּ� ����, ���� �Էµ� ����
	// ����Ʈ �ּ�, ����Ʈ �ּ� ����, ���� �Էµ� ����Ʈ �ּ� ����
	// �н��� ���� ������ ������ �𸣰ڴ�
	GetAcceptExSockaddrs(ignore,
		0,
		50,
		50,
		(sockaddr**)&ignore1,
		&ignore2,
		(sockaddr**)&ignore3,
		&ignore4);

	return setsockopt(m_fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
		(char*)&listenSocket.m_fd, sizeof(listenSocket.m_fd));
}

Endpoint Socket::GetPeerAddr()
{
	// ���� �� ���� ����
	Endpoint ret;
	socklen_t retLength = sizeof(ret.m_ipv4Endpoint);
	
	// m_fd�� �ּҸ� ���� ������ �Ű� ��´�.
	if (getpeername(m_fd, (sockaddr*)&ret.m_ipv4Endpoint, &retLength) < 0)
	{
		std::stringstream ss;
		ss << "getPeerAddr failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}

	// �ּҰ� ������ �������� Ŭ ��쿡 ������ �����.
	if (retLength > sizeof(ret.m_ipv4Endpoint))
	{
		std::stringstream ss;
		ss << "getPeerAddr buffer overrun: " << retLength;
		throw Exception(ss.str().c_str());
	}

	return ret;
}

int Socket::Receive()
{
	return recv(m_fd, m_receiveBuffer, MaxReceiveLength, 0);
}

int Socket::ReceiveOverlapped()
{
	WSABUF b;
	b.buf = m_receiveBuffer;
	b.len = MaxReceiveLength;

	m_readflags = 0;

	return WSARecv(m_fd, &b, 1, nullptr, &m_readflags, &m_readOverlappedStruct, nullptr);
}

void Socket::SetNonblocking()
{
	u_long val = 1;
	int ret = ioctlsocket(m_fd, FIONBIO, &val);

	if (ret != 0)
	{
		std::stringstream ss;
		ss << "bind failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

// ������ ���� �޽��� �޾Ƽ� ����ϱ����� �ۼ��� �Լ� ������
// ��Ʈ��ũ�� �ص� �Ӹ� ������ �����ϱ� �ۼ��� �Ѵ�.
std::string GetLastErrorAsString()
{
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
	{
		return std::string();
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);

	std::string message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}