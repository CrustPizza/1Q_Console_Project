#pragma once
#include <WinSock2.h>
#include <MSWSock.h>
#include <string>

// ����, ip�ּҿ� ��Ʈ ������ ����Ǿ��ִ�.
class Endpoint;

enum class SocketType
{
	// TCP
	// �����Ͱ� ������ ���ɼ��� ������ ����.
	// �۽��ϰ� ack�ΰ��ϴ� ���� Ȯ�� ��ȣ�� ���� ������ ��������
	// ���� ����Ƚ���� ���� �����͸� ������ ����ϴ� �� ����.
	TCP,
	// UDP
	// �����Ͱ� ���ǵǰų� �����Ǵ� ���ɼ��� ������ ����.
	// �ϴ� �����ͺ����µ� ���� ���۰� ������������ �������ų� �ϴ� ������ ����°� ����.
	// �������� �������� ��� �������� ������ ������ �ʿ��� ��Ȳ���� ����ϴ� �� ����.
	// å������ ĳ���� �̵��̳� ������� ���� ���� ����־���.
	UDP
};

class Socket
{
public:
	// ���� ��Ŷ? �ƹ�ư �ѹ��� ���۵Ǵ� ���� �����Ͱ�
	// �ִ� 8�ް�����Ʈ�� �ްڴٴ� �� ����.
	//
	// �װ� �ƴϰ� Overlapped I/O ȯ�濡�� �ϴ� �����س��� ���� �������ε�..
	static const int MaxReceiveLength = 8192;

	// ��� ���� ����
	// fd�� ���������� File Descripter ���Ӹ��̶�µ�
	// �װ� ���� �𸣰ڴ�...
	SOCKET m_fd;

	// ������ �Լ�_ AcceptEx
	// Accept�� �����û�� Ŭ���̾�Ʈ�� �޴� �Լ����µ�
	// ��� �����쿡�� �����Ǵ� Overlapped I/O ����� ����ϱ����� ����
	// AcceptEx �Լ��� ������Ų �Լ��� �ޱ����� �����ͷ� ���δ�.
	// �� �޴� �Լ����� AcceptEx�� �ƴϾ��� ������Ű�� �͵� �ƴϾ���.
	// WSAIoctl �� �Լ��� ������ �޴� �Լ� �����Ϳ���.
	//
	// WSAIoctl�� �������ִ°� AcceptEx�ε�.. �Ӹ� ������
	LPFN_ACCEPTEX AcceptEx = NULL;

	// ������ ȯ�濡�� Overlapped I/O�� IOCP�� ����� �� ����ϴ� ����
	// ������ �����찡 �ƴ� ȯ�浵 ����ؼ� ������ ����ϰ�
	// ����Ʈ ���� false�� ���ִµ� ���� �����쿡���� �Ұ��̹Ƿ�
	// ��.. �׷��� ������ ���� ����Ʈ�� true�� �Ѵ�.
	bool m_isReadOverlapped = true;

	// �� �̰� å���� �þ��µ�
	// ���� ������� ���������� ���� ��� Overlapped I/O, IOCP��
	// ���õ� �����ε�, �ش� ��ɵ��� wouldblock�ΰ��ϴ� ���¿���
	// �޸𸮾ȿ� ��/���� ���۸� ���� ��/���� ��û�Ѵٰ� �ϴµ�
	// �̹� ��û�� �� ���¶� ���� �ٲ�� �ȵȴ�
	// ������ ������ ����.
	WSAOVERLAPPED m_readOverlappedStruct;

	// ���ŵǴ� �����͸� ä������ ����
	// ���� �ٲ�� �ȵȴ�!
	char m_receiveBuffer[MaxReceiveLength];

	// ���� �뵵���� �� �𸣰ڴ�.
	// �ּ��� �����ִµ� �굵 ���� �ٲ�� �ȵǰ� recv�� flags�� ���ϴ� ���� ä�����ٰ��Ѵ�.
	// �װ� ����? �˻��غ��� ���ذ� �߾ȵȴ�.
	DWORD m_readflags = 0;


	Socket();
	Socket(SOCKET fd);
	Socket(SocketType socketType);
	~Socket();

	// ���Ͽ� ��Ʈ��ũ �ּҿ� ��Ʈ�� �����ϴ� �Լ��ε�
	void	Bind(const Endpoint& endpoint);

	// ������ �����û ������
	bool	Connect(const Endpoint& endpoint);

	// ����� �������� ������ ������
	int		Send(const char* data, int length);

	// ���� �ݱ�. ������ó��, ������ �Ҹ��ڿ� �� �Լ��� ȣ���ϰ� �Ǿ�����
	void	Close();

	// �������� ���� ��û�ϴ� Ŭ���̾�Ʈ�� Ȯ��?�ϴ� �Լ�
	void	Listen();

	// ���� ���
	int		Accept(Socket& acceptedSocket, std::string& errorText);

	// ������ ȯ�濡�� Overlapped I/O�� ����ϱ����� ��������� �� �̰� ���µ�
	bool	AcceptOverlapped(Socket& acceptCandidateSocket);
	int		UpdateAcceptContext(Socket& listenSocket);

	// ����� Ŭ���̾�Ʈ�� ��Ʈ��ũ �ּҸ� �޴´�.
	Endpoint GetPeerAddr();

	// �����͸� �޴´�.
	int Receive();

	// ������ ȯ�濡�� �������� ������ �޴´�.
	int		ReceiveOverlapped();

	// ���� ����
	void	SetNonblocking();
};

// ������ ����ϱ����� �Լ��ε�?
std::string GetLastErrorAsString();