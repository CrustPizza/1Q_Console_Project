#pragma once
#include <WinSock2.h>
#include <MSWSock.h>
#include <string>

// 끝점, ip주소와 포트 정보가 저장되어있다.
class Endpoint;

enum class SocketType
{
	// TCP
	// 데이터가 변조될 가능성이 낮은거 같다.
	// 송신하고서 ack인가하는 수신 확인 신호를 받지 않으면 재전송함
	// 비교적 전송횟수가 적은 데이터를 보낼때 사용하는 것 같다.
	TCP,
	// UDP
	// 데이터가 유실되거나 변조되는 가능성이 높은거 같다.
	// 일단 데이터보내는데 수신 버퍼가 가득차있으면 버려지거나 하는 문제가 생기는거 같다.
	// 안정성이 떨어지는 대신 지속적인 데이터 갱신이 필요한 상황에서 사용하는 거 같다.
	// 책에서는 캐릭터 이동이나 영상재생 같은 예를 들어주었다.
	UDP
};

class Socket
{
public:
	// 대충 패킷? 아무튼 한번에 전송되는 수신 데이터가
	// 최대 8메가바이트로 받겠다는 것 같다.
	//
	// 그게 아니고 Overlapped I/O 환경에서 일단 수신해놓을 버퍼 사이즈인듯..
	static const int MaxReceiveLength = 8192;

	// 멤버 변수 소켓
	// fd는 리눅스에서 File Descripter 줄임말이라는데
	// 그게 뭔지 모르겠다...
	SOCKET m_fd;

	// 포인터 함수_ AcceptEx
	// Accept가 연결요청된 클라이언트를 받는 함수였는데
	// 얘는 윈도우에서 제공되는 Overlapped I/O 기능을 사용하기위해 쓰는
	// AcceptEx 함수를 개량시킨 함수를 받기위한 포인터로 보인다.
	// 음 받는 함수명이 AcceptEx도 아니었고 개량시키는 것도 아니었다.
	// WSAIoctl 이 함수의 리턴을 받는 함수 포인터였다.
	//
	// WSAIoctl이 리턴해주는게 AcceptEx인듯.. 머리 아프다
	LPFN_ACCEPTEX AcceptEx = NULL;

	// 윈도우 환경에서 Overlapped I/O나 IOCP를 사용할 때 사용하는 변수
	// 원래는 윈도우가 아닌 환경도 고려해서 변수를 사용하고
	// 디폴트 값도 false가 들어가있는데 나는 윈도우에서만 할것이므로
	// 음.. 그래도 변수는 쓰고 디폴트를 true로 한다.
	bool m_isReadOverlapped = true;

	// 아 이거 책에서 봤었는데
	// 위에 변수들과 마찬가지로 논블록 기능 Overlapped I/O, IOCP와
	// 관련된 변수인데, 해당 기능들은 wouldblock인가하는 상태에서
	// 메모리안에 송/수신 버퍼를 만들어서 송/수신 요청한다고 하는데
	// 이미 요청이 된 상태라서 값이 바뀌면 안된다
	// 까지만 생각이 난다.
	WSAOVERLAPPED m_readOverlappedStruct;

	// 수신되는 데이터를 채워넣을 버퍼
	// 값이 바뀌면 안된다!
	char m_receiveBuffer[MaxReceiveLength];

	// 무슨 용도인지 잘 모르겠다.
	// 주석이 적혀있는데 얘도 값이 바뀌면 안되고 recv의 flags에 준하는 값이 채워진다고한다.
	// 그게 뭘까? 검색해봐도 이해가 잘안된다.
	DWORD m_readflags = 0;


	Socket();
	Socket(SOCKET fd);
	Socket(SocketType socketType);
	~Socket();

	// 소켓에 네트워크 주소와 포트를 연결하는 함수인듯
	void	Bind(const Endpoint& endpoint);

	// 서버에 연결요청 보내기
	bool	Connect(const Endpoint& endpoint);

	// 연결된 소켓으로 데이터 보내기
	int		Send(const char* data, int length);

	// 소켓 닫기. 릴리즈처럼, 실제로 소멸자에 이 함수를 호출하게 되어있음
	void	Close();

	// 서버에서 연결 요청하는 클라이언트를 확인?하는 함수
	void	Listen();

	// 연결 허용
	int		Accept(Socket& acceptedSocket, std::string& errorText);

	// 윈도우 환경에서 Overlapped I/O를 사용하기위해 연결허용할 때 이걸 쓰는듯
	bool	AcceptOverlapped(Socket& acceptCandidateSocket);
	int		UpdateAcceptContext(Socket& listenSocket);

	// 연결된 클라이언트의 네트워크 주소를 받는다.
	Endpoint GetPeerAddr();

	// 데이터를 받는다.
	int Receive();

	// 윈도우 환경에서 논블록으로 데이터 받는다.
	int		ReceiveOverlapped();

	// 논블록 세팅
	void	SetNonblocking();
};

// 에러를 출력하기위한 함수인듯?
std::string GetLastErrorAsString();