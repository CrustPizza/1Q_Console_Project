#include "stdafx.h"
#include "Socketinit.h"

SocketInit g_socketInit;

SocketInit::SocketInit()
{
	// WSAStartup 함수에서 반환값을 받기위해 사용되는 변수인데
	// 현재 버전, 상위 버전, 윈속 설명, 상태 문자열, 최대 소켓
	// 데이터 그렘의 최대 크기, 벤더 정보 로 구성된 구조체변수다.
	// 하위버전과의 호환성 때문에 남아있는 것으로 당장에 중요한 변수는 아닌듯하다.
	WSADATA w;

	// Window Socket API
	// WSAStartup 함수 내부적으로 뭔지 잘 모르겠지만
	// 소켓 프로그래밍할 때 초기화를 위해 최우선적으로 호출하는 함수라고 한다.
	// 2Byte WORD에서 상위 바이트는 주 버전, 하위 바이트는 부 버전이라고 한다.
	// 2, 2가 들어가니까 winsock 2.2version인건가? 했는데
	// 부 버전이 내가 생각하는 그런 의미가 아닌건지 2.0version이라고 한다.
	WSAStartup(MAKEWORD(2, 2), &w);
}

// 비어있다..
void SocketInit::Touch()
{
}