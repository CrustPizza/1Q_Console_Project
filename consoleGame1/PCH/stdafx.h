#pragma once

#include <SDKDDKVer.h>

// Socket.cpp의 Bind 함수에서 bind 함수의 2번째 인자가
// Ambiguous symbol 에러가 나오면서 형변환이 되지 않는데
// 아래의 매크로를 정의해주면 사용이 가능하다.
#define WIN32_LEAN_AND_MEAN

// 네트워크
#include "Network/Server/Server.h"
#include "Network/Client/client.h"
#include "Network/Exception/Exception.h"

// 게임
#include "Game/ui.h"
#include "Game/map.h"
#include "Game/game.h"
#include "Game/bomb.h"
#include "Game/scene.h"
#include "Game/character.h"

// 엔진
#include "Engine/sound.h"
#include "Engine/render.h"
#include "Engine/controller.h"

// 기능 헤더
#include <string>
#include <time.h>
#include <conio.h>

// Socket.cpp의 Bind 함수 안에서 StringStream 변수를 선언할 때 필요하다.
#include <sstream>

// 네트워크 관련 헤더들
#include <WS2tcpip.h>
#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>

// 이거 추가 안했더니 네트워크 관련 함수들이 링크에러가 났다.
// 왜 예제에선 포함 안되어있나 했는데 Socket.h 밑에 선언되어 있었다..
#pragma comment(lib, "ws2_32")

// ws2_32 라이브러리랑 함께 이것도 선언되어 있었는데
// 뭘 위해 선언해두는 건지 모르겠으니 일단 주석처리 해둔다
// Socket.cpp의 UpdateAcceptContext함수에서 링크 에러가 나왔다.
// 주석을 해제하니 작동된다.
#pragma comment(lib, "mswsock.lib")

extern Character player[2];