#pragma once
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"

#pragma comment(lib,"fmod_vc.lib")

enum effect {
	BOMB_CREATE,
	BOMB_EXPLODE,
	LEVEL_UP,
	CLICK,
	HIT,
	APPLAUSE
};

using namespace FMOD;


bool check_bgm();

bool check_bgm_ingame();

// ���� �ý����� �̴ϼȶ���¡ �ϴ� �Լ�
void SoundSystem();

void SoundSystem2();

// BGM�� Ű�� �Լ� (����)
void PlayBGM(int BGM_NUM);

// EFFECT�� Ű�� �Լ� (�� ����)
void PlayEFFECT(int EFFECT_NUM);

// BGM�� ���� �Լ�
void StopBGM(int);