#include "Select.h"

int Select::m_Hndl[SELECTHANDL_MAX];

static const char* FilePath1[SELECTHANDL_MAX] = {
	"Data/Select/Elephant.png",		//0
	"Data/Select/BossSelect.png",	//1
	"Data/Select/ElephantText.png",	//2
};

void Select::Init() { 
	for (int i = 0; i < SELECTHANDL_MAX; i++) {
		m_Hndl[i] = -1;
	}
}

void Select::Load() {
	for (int i = 0; i < SELECTHANDL_MAX; i++) {
		if (m_Hndl[i] == -1) {
			m_Hndl[i] = LoadGraph(FilePath1[i]);
		}
	}
}

void Select::Exit() {
	for (int i = 0; i < SELECTHANDL_MAX; i++) {
		if (m_Hndl[i] != -1) {
			DeleteGraph(m_Hndl[i]);
			m_Hndl[i] = -1;
		}
	}
}

void Select::Draw() {
	DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_LOWER, 4.0f, 0.0f, m_Hndl[2], TRUE);
	DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_HALF_Y, 10.0f, 0.0f, m_Hndl[0], TRUE);
	DrawRotaGraph((int)SCREEN_HALF_X, (int)SCREEN_UPPER, 1.0f, 0.0f, m_Hndl[1], TRUE);
}

void Select::Step() {
	
}
