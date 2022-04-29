#include "stdafx.h"
#include "Socket.h"
#include <rpc.h>
#include "Endpoint.h"
#include "Socketinit.h"
#include "Exception.h"

Socket::Socket(SocketType socketType)
{
	// Touch는 비어있는데 왜 하는 걸까?
	// 전에 리눅스 터미널 써봤을 때 shell 명령어로 Touch를 사용하면
	// 파일이 없었을 때 생성되게해주는 기능도 있었는데
	// 리눅스 환경에서 Touch함수를 호출할 때 무슨 기능이 있는 것일까?
	// 아니면 내가 뭔가 작성해야하는 부분이 있는건가..
	// 책 3장까지는 그런 내용이 없었는데
	g_socketInit.Touch();

	// UDP는 지금 안쓸거니까 생략한다.
	// UDP가 오히려 쉬울 수도 있지 않을까 하는 생각이 문득 들긴한다..
	if (socketType == SocketType::TCP)
	{
		// 소켓 생성함수에서 프로토콜 인수가 왜 0으로 적혀있을까
		// 0인 이유는 0으로 적어두면 해당 타입의 디폴트 프로토콜로 적용된다고 한다.
		// 그래서 그냥 명시적으로 0대신 IPPROTO_TCP를 적어주었다.
		// lpProtocol은 생성할 소켓의 특성을 정의할 때 사용한다는데 모르겠다..
		// g는 소켓 그룹이라는데 이것도 잘 모르겠음
		// 추가 속성 플래그.. 다른건 모르겠지만 일단 IOCP를 위해 Overlapped I/O를 설정한다.
		m_fd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	}

	ZeroMemory(&m_readOverlappedStruct, sizeof(m_readOverlappedStruct));
}

Socket::Socket(SOCKET fd)
{
	g_socketInit.Touch();

	// 다른 소켓을 복사하는거 같은데 어떨때 복사해서 사용하는 걸까
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
	// 소켓과 끝점을 묶는다.
	// 네트워크 주소와 포트는 끝점 클래스에서 묶어진다.
	// bind라서 아무튼 묶는다고 생각했는데 m_fd는 사실 Unsigned int다.
	// 여기에 어떻게 묶는 걸까?
	// 또 끝점이 같아도 소켓은 여러개 일 수 있으니 구분하기위한게 필요할텐데
	// 끝점을 가리키는 포인터일까?
	// SOCKET -> UINT_PTR -> unsigned __int64 인데 포인터?
	// 메모리 크기도 같고, int에도 int* 메모리 주소를 강제로 숫자처럼 넣을 순 있으니까
	// 가능은 할텐데 진짜 이런식으로 동작하는 것일지 궁금한데 알 수가 없다.
	// 심지어 검색해보니 네트워크랑은 연관없어보이지만 std::bind()함수라는게 또 있다.
	if (bind(m_fd, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0)
	{
		// bind 실패했을 때의 처리인듯
		std::stringstream ss;
		ss << "bind failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}
}

bool Socket::Connect(const Endpoint& endpoint)
{
	// 클라이언트가 서버에 연결 요청하는 함수
	// 인자로 들어온 끝점에 연결시도하고 연결되면 m_fd를 건네주는 형식일듯
	// 근데 connectEx는 쓰지않아도 괜찮은 건가
	if (connect(m_fd, (sockaddr*)&endpoint.m_ipv4Endpoint, sizeof(endpoint.m_ipv4Endpoint)) < 0)
	{
		return false;
	}

	return true;
}

int Socket::Send(const char* data, int length)
{
	// 리턴앞에 콜론 2개는 무슨 의미일까
	// 네임스페이스나 클래스, 구조체, 열거 클래스 같은 곳에서
	// 지역을 명시하는 용도로 사용한적은 있는데 아무것도 없이 이렇게 되는 것은 처음봐서
	// 찾아보았더니 앞에 붙는게 없다면 전역이란 뜻이라고한다.
	// 전역을 지역으로 구분해야할 이유가 여기서는 와닿지 않는다.
	// int i = 0;
	// int main { int i = 0;
	// i = 10; // 지역 변수에 10들어감
	// ::i = 10; } // 전역 변수에 10 들어감
	// 요건 알겠는데 왜 send앞에 이걸 붙여서 사용하는 걸까
	// flag는 데이터를 전송 혹은 읽는 방법에 옵션을 거는 것이다.
	// 종류를 읽어보니 MSG_DONTWAIT 이라고 논블록에서 사용해야할 것 같은 옵션이 있는데
	// 예제에서 사용하지 않았다 이유가 뭘까
	return ::send(m_fd, data, length, 0);
}

void Socket::Close()
{
	// 잘 모르겠지만 내부적으로 잘 닫지 않을까
	// 이런식으로 사용되는걸 보면 m_fd의 사용은 정말 포인터와 비슷해보인다.
	closesocket(m_fd);
}

void Socket::Listen()
{
	// m_fd 소켓으로 오는 연결 요청을 최대 5000개까지 대기하게 받겠다는 뜻
	// 이게 대기열인가?
	listen(m_fd, 5000);
}

// 함수는 선언해두었지만 Overlapped I/O 하는것 만으로도 머리 터지니까
// 분석은 나중에
int Socket::Accept(Socket& acceptedSocket, std::string& errorText)
{
	// 분석은 나중에 하려고 했는데 왜 addrlen을 포인터로 받는거야
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
		// input output control? 맞나? 뭐지
		WSAIoctl(m_fd, // 소켓
			// 연산 제어코드? The control code of operation to perform
			// 구글에 검색해봐도 자료가 잘 보이지 않는다.
			// 대충 뜻이 확장 함수 포인터 받기니까 함수포인터에 뭘 넣어준다는 소리일듯
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			// UUID 고유 식별자라는데..
			// 일단 MSDN에는 입력 버퍼의 포인터라고 심플하게 적혀있다
			&uuid_v, sizeof(UUID),
			// UUID에서 뭔가를 가져와서 여기에 넣어주는 듯 싶다.
			&AcceptEx, sizeof(AcceptEx),
			// 사용 안하는거보면 지금 중요한 내용은 아닌걸로 보임
			// 1. 실제로 출력받은 byte 수치
			// 2. 구조체 포인터, 비동기 소켓에서 무시된다고함
			// 3. 설명에 논 오버랩 소켓에서는 무시된다고하는데
			// 나는.. 오버랩 소켓인데.. 왜 null값이 들어가있지..
			// 아무튼 연산 완료될 때 호출될 완료 루틴의 포인터라고 한다.
			&bytes, nullptr, nullptr);

		// 설정을 했는데도 nullptr인 경우
		if (AcceptEx == nullptr)
		{
			throw Exception("Getting AcceptEx ptr failed.");
		}
	}

	// accept된 소켓의 로컬 주소와 리모트 주소가 채워지는데
	// 독자의 이해를 돕기위해 그냥 버린다고 한다..
	// 로컬 주소는 클라이언트의 주소 얘기가 아닐까 싶은데
	// 리모트 주소는 뭘까..
	char ignored[200];
	DWORD ignored2 = 0;

	bool ret = AcceptEx(
		// Listen 소켓 - 서버 소켓
		m_fd,
		// Accept 소켓 - 클라이언트 소켓
		// bind, connect가 되어있지 않아야 한다고 합니다.
		// bind는 되어야하는거 아닌가 왤까
		acceptCandidateSocket.m_fd,
		// 출력 버퍼
		// 로컬 주소와, 리모트 주소, 연결시 받게되는 첫 데이터 블록 내용을 저장하는 버퍼
		&ignored,
		// 받을 데이터 길이
		// 근데 왜 0이지
		// 아.. 위에서 버린다고해서 0인거같다.
		// 첫 데이터는 버리고 로컬과 리모트 주소는 저장한다고 한다.
		// 첫 데이터 수신을 기다리지 않기 때문에 0으로 할 때 성능 이점이 있다
		0,
		// 지역 주소 길이
		// 최소 16을 보장해야한다.
		50,
		// 리모트 주소 길이
		// 마찬가지로 최소 16을 보장해야한다.
		50,
		// 완료시 전달할 overlapped 구조체의 주소
		// 설명을 봐도 뭔소린지 모르겠다.
		&ignored2,
		&m_readOverlappedStruct) == true;

	return ret;
}

// AcceptEx가 I/O 완료를 하더라도 아직 TCP 연결 받기 처리가 다 끝난 것이 아니다.
// 이 함수를 호출해주어야만 완료가 된다.
// 라고 예제에 적혀있다. 책을 다시 읽어야하나 뭔소린지 모르겠다.
// 아 연결된 소켓 정보를 반환받기 위한건가
int Socket::UpdateAcceptContext(Socket& listenSocket)
{
	sockaddr_in ignore1;
	INT			ignore2;
	sockaddr_in ignore3;
	INT			ignore4;

	char ignore[1000];
	// 받는 버퍼, 버퍼 크기
	// 로컬 주소, 로컬 주소 길이, 실제 입력된 길이
	// 리모트 주소, 리모트 주소 길이, 실제 입력된 리모트 주소 길이
	// 학습을 위해 생략된 것인지 모르겠다
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
	// 리턴 값 담을 변수
	Endpoint ret;
	socklen_t retLength = sizeof(ret.m_ipv4Endpoint);
	
	// m_fd의 주소를 리턴 변수에 옮겨 담는다.
	if (getpeername(m_fd, (sockaddr*)&ret.m_ipv4Endpoint, &retLength) < 0)
	{
		std::stringstream ss;
		ss << "getPeerAddr failed:" << GetLastErrorAsString();
		throw Exception(ss.str().c_str());
	}

	// 주소가 지정된 공간보다 클 경우에 문제가 생긴다.
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

// 윈도우 오류 메시지 받아서 출력하기위해 작성한 함수 같은데
// 네트워크만 해도 머리 깨질거 같으니까 작성만 한다.
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