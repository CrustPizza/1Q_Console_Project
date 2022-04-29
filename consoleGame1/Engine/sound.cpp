#include "stdafx.h"

System* pSoundSystem;

System* pSoundSystem2;

Sound* pBGM[2];// BGM용 배열

Sound* pEFFECT[10]; // EFFECT용 배열

Channel* pBGMChannel[1]; // BGM용 채널, LOOP를 기본적으로 키고 실행함 (pause 신경 써야함)

Channel* pEFFECTChannel[1]; // EFFECT용 채널, LOOP를 기본적으로 끄고 실행함 (pause 신경 안 써야함)

bool bgm_on = false;

bool bgm_ingame_on = false;

bool check_bgm()
{
	return bgm_on;
}

bool check_bgm_ingame()
{
	return bgm_ingame_on;
}

void SoundSystem()
{
	System_Create(&pSoundSystem); // 시스템을 만들어준다.

	pSoundSystem->init(10, FMOD_INIT_NORMAL, NULL); // 시스템의 Init을 통해 설정한다.

	// BGM INIT() 
	//C:\GK\GA2021\C_Programming_Miniproject\봄버맨레볼루션_신강호\4_Rescources
	//FMOD_LOOP_NORMAL;
	//pSoundSystem->createSound("Sounds\\BGM\\bgm.mp3", FMOD_LOOP_NORMAL, 0, &pBGM[0]);
	//pSoundSystem->createSound("Sounds\\BGM\\bgm_ingame.mp3", FMOD_LOOP_NORMAL, 0, &pBGM[1]);

	// EFFECT INIT()
	FMOD_DEFAULT;
	pSoundSystem->createSound("Sounds\\EFFECT\\bomb_create.wav", FMOD_DEFAULT, 0, &pEFFECT[BOMB_CREATE]);
	pSoundSystem->createSound("Sounds\\EFFECT\\bomb_explode.wav", FMOD_DEFAULT, 0, &pEFFECT[BOMB_EXPLODE]);
	pSoundSystem->createSound("Sounds\\EFFECT\\level_up.wav", FMOD_DEFAULT, 0, &pEFFECT[LEVEL_UP]);
	pSoundSystem->createSound("Sounds\\EFFECT\\click.wav", FMOD_DEFAULT, 0, &pEFFECT[CLICK]);
	pSoundSystem->createSound("Sounds\\EFFECT\\hit.wav", FMOD_DEFAULT, 0, &pEFFECT[HIT]);
	pSoundSystem->createSound("Sounds\\EFFECT\\applause.wav", FMOD_DEFAULT, 0, &pEFFECT[APPLAUSE]);

	//pSoundSystem->createSound("..\\..\\..\\Sounds\\EFFECT\\WALKING_RIGHT.wav", FMOD_DEFAULT, 0, &pEFFECT[0]);
}

void SoundSystem2()
{
	System_Create(&pSoundSystem2); // 시스템을 만들어준다.

	pSoundSystem2->init(1, FMOD_INIT_NORMAL, NULL); // 시스템의 Init을 통해 설정한다.

	// BGM INIT() 
	//C:\GK\GA2021\C_Programming_Miniproject\봄버맨레볼루션_신강호\4_Rescources
	FMOD_LOOP_NORMAL;
	pSoundSystem2->createSound("Sounds\\BGM\\bgm.mp3", FMOD_LOOP_NORMAL, 0, &pBGM[0]);
	pSoundSystem2->createSound("Sounds\\BGM\\bgm-ingame.mp3", FMOD_LOOP_NORMAL, 0, &pBGM[1]);
}

void PlayBGM(int BGM_NUM)
{
	switch (BGM_NUM)
	{
		case 0:
		{
			pSoundSystem2->playSound(pBGM[BGM_NUM], NULL, 0, pBGMChannel);
			bgm_on = true;
			break;
		}
		case 1:
		{
			pSoundSystem2->playSound(pBGM[BGM_NUM], NULL, 0, pBGMChannel);
			pBGMChannel[0]->setVolume(0.25f);
			bgm_ingame_on = true;
			break;
		}
	}
	
}

void PlayEFFECT(int EFFECT_NUM)
{
	//pSoundSystem->update();
	pSoundSystem->playSound(pEFFECT[EFFECT_NUM], NULL, 0, pEFFECTChannel);
}

void StopBGM(int BGM_NUM)
{
	switch (BGM_NUM)
	{
		case 0:
		{
			pBGMChannel[0]->stop();
			bgm_on = false;
			break;
		}
		case 1:
		{
			pBGMChannel[0]->stop();
			bgm_ingame_on = false;
			break;
		}
	}
}