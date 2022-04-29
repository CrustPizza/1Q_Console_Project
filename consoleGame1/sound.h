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

// 사운드 시스템을 이니셜라이징 하는 함수
void SoundSystem();

void SoundSystem2();

// BGM을 키는 함수 (루프)
void PlayBGM(int BGM_NUM);

// EFFECT를 키는 함수 (노 루프)
void PlayEFFECT(int EFFECT_NUM);

// BGM을 끄는 함수
void StopBGM(int);