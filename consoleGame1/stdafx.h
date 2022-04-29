#pragma once

#include <SDKDDKVer.h>

// Socket.cpp�� Bind �Լ����� bind �Լ��� 2��° ���ڰ�
// Ambiguous symbol ������ �����鼭 ����ȯ�� ���� �ʴµ�
// �Ʒ��� ��ũ�θ� �������ָ� ����� �����ϴ�.
#define WIN32_LEAN_AND_MEAN

// ��Ʈ��ũ
#include "Server.h"
#include "client.h"
#include "Exception.h"

// ����
#include "ui.h"
#include "map.h"
#include "game.h"
#include "bomb.h"
#include "scene.h"
#include "character.h"

// ����
#include "sound.h"
#include "render.h"
#include "controller.h"

// ��� ���
#include <string>
#include <time.h>
#include <conio.h>

// Socket.cpp�� Bind �Լ� �ȿ��� StringStream ������ ������ �� �ʿ��ϴ�.
#include <sstream>

// ��Ʈ��ũ ���� �����
#include <WS2tcpip.h>
#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>

// �̰� �߰� ���ߴ��� ��Ʈ��ũ ���� �Լ����� ��ũ������ ����.
// �� �������� ���� �ȵǾ��ֳ� �ߴµ� Socket.h �ؿ� ����Ǿ� �־���..
#pragma comment(lib, "ws2_32")

// ws2_32 ���̺귯���� �Բ� �̰͵� ����Ǿ� �־��µ�
// �� ���� �����صδ� ���� �𸣰����� �ϴ� �ּ�ó�� �صд�
// Socket.cpp�� UpdateAcceptContext�Լ����� ��ũ ������ ���Դ�.
// �ּ��� �����ϴ� �۵��ȴ�.
#pragma comment(lib, "mswsock.lib")

extern Character player[2];