#pragma once

class SocketInit
{
public:

	SocketInit();

	// NOTE: WSACleanup을 호출하는 시점이 있어야 하는 것이 일반적입니다.
	// 그러나, C++에서 전역 객체의 파괴자 호출의 순서를 
	// 알 수 없으므로, 어차피 프로세스를 종료하는 마당이므로 굳이 WSACleanup을 호출하지는 않습니다.

	// 위에는 적혀있던 주석 내용이다.
	// WSACleanup이 뭔지 모르겠지만 원래는 서버가 항상 가동되고 있고
	// 전역변수를 사용하지 않는 환경에서 저 함수인가 하는 것을 호출해야하는데
	// 그럴 필요가 없어서 구현하지 않았다는 것일까?
	void Touch();
};

extern SocketInit g_socketInit;