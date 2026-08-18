#include "SoundManager.h"

using namespace std;

vector<int>SoundManager::m_Hndl;

namespace {
	constexpr const char* FilePath1[SoundManager::TagID::NUM_MAX] = {
		"Data/Sound/SE/1.Sword.mp3",
		"Data/Sound/SE/2.Shield.mp3",
		"Data/Sound/SE/3.Hit.mp3",
		"Data/Sound/SE/4.Jump.mp3",
		"Data/Sound/SE/5.SkillAttack.mp3",
		"Data/Sound/SE/6.ElephantNormalAttack.mp3",
		"Data/Sound/SE/7.ElephantStrongAttack.mp3",
		"Data/Sound/SE/8.ElephantLunges.mp3",
		"Data/Sound/SE/9.ElephantCry.mp3",
		"Data/Sound/SE/10.WolfAttack.mp3",
		"Data/Sound/SE/11.MobWolfDeath.mp3",
		"Data/Sound/SE/12.BossWolfDeath.mp3",
		"Data/Sound/SE/13.BoarLunges.mp3",
		"Data/Sound/SE/14.MobBoarDeath.mp3",
		"Data/Sound/SE/15.BossBoarDeath.mp3",
		"Data/Sound/BGM/1.Title.mp3",
		"Data/Sound/BGM/2.NormalMap.mp3",
		"Data/Sound/BGM/3.Main.mp3",
		"Data/Sound/BGM/4.GameClear.mp3",
		"Data/Sound/BGM/5.GameOver.mp3",
	};
}

//コンストラクタ
SoundManager::SoundManager() {
	Init();
}

//デストラクタ
SoundManager::~SoundManager() {
	Exit();
}

//初期化処理
void SoundManager::Init() {
	m_Hndl.clear();
}

//データ読み込み処理
void SoundManager::Load() {
	for (int Index = 0; Index < NUM_MAX; Index++) {
		int Hndl = LoadSoundMem(FilePath1[Index]);
		m_Hndl.push_back(Hndl);
	}
}

//音量調節
void SoundManager::Volume() {
	for (int Index = BGM_TITLE; Index < NUM_MAX; Index++) {
		ChangeVolumeSoundMem(255 *  50/ 100, m_Hndl[Index]);
	}
}

//データ破棄
void SoundManager::Exit() {
	for (int Index = 0; Index < m_Hndl.size(); Index++) {
		if (m_Hndl[Index] != -1) {
			DeleteSoundMem(m_Hndl[Index]);
			m_Hndl[Index] = -1;
		}
	}
}

//音楽再生
bool SoundManager::Play(TagID _ID, int _Type, bool _IsStart) {
	int Res = PlaySoundMem(m_Hndl[_ID], _Type, _IsStart);
	if (Res == -1) {
		return false;
	}
	else {
		return true;
	}
}

//指定音楽停止
void SoundManager::Stop(TagID _ID) {
	StopSoundMem(m_Hndl[_ID]);
}

//全音楽停止
void SoundManager::AllStop() {
	for (int Index = 0; Index < m_Hndl.size(); Index++) {
		StopSoundMem(m_Hndl[Index]);
	}
}

//音楽を再生中か
bool SoundManager::IsPlay(TagID _ID) {
	int Res = CheckSoundMem(m_Hndl[_ID]);
	if (Res == 1) {
		return true;
	}
	else {
		return false;
	}
}
