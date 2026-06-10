#include "SoundManager.h"

vector<int>SoundManager::m_Hndl;
vector<int>SoundManager::m_ThreeDimensionHndl;

const char* FilePath1[SoundManager::TagID::NUM_MAX] = {
	"Data/Sound/SE/1.Sword.mp3",
	"Data/Sound/SE/7.SwordMiss.mp3",
	"Data/Sound/SE/8.FlashofLight.mp3",
	"Data/Sound/SE/2.Guard.mp3",
	"Data/Sound/SE/3.Parry.mp3",
	"Data/Sound/SE/4.WeakAtk.mp3",
	"Data/Sound/SE/5.MediumAtk.mp3",
	"Data/Sound/SE/6.StrongAtk.mp3",
	"Data/Sound/SE/9.Lunges.mp3",
	"Data/Sound/SE/10.ElephantCry.mp3",
	"Data/Sound/SE/11.Explosion.mp3",
	"Data/Sound/BGM/Title.mp3",
	"Data/Sound/BGM/Select.mp3",
	"Data/Sound/BGM/GameOver.mp3",
	"Data/Sound/BGM/GameClear.mp3",
};

const char* FilePath2[SoundManager::TagThreeDimensionID::THIRD_NUM_MAX] = {
	"Data/Sound/SE/100.Three.mp3",
};


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
	m_ThreeDimensionHndl.clear();
}

//データ読み込み処理
void SoundManager::Load() {
	for (int i = 0; i < NUM_MAX; i++) {
		int Hndl = LoadSoundMem(FilePath1[i]);
		m_Hndl.push_back(Hndl);
	}

	for (int i = 0; i < THIRD_NUM_MAX; i++) {
		SetCreate3DSoundFlag(TRUE);
		int Hndl = LoadSoundMem(FilePath2[i]);
		m_ThreeDimensionHndl.push_back(Hndl);
		SetCreate3DSoundFlag(FALSE);
	}

	Set3DRadiusSoundMem(250.0f, m_ThreeDimensionHndl[SE_ROT]);
}

//音量調節
void SoundManager::Volume() {
	for (int i = 0; i < SE_WEAKATK; i++) {
		ChangeVolumeSoundMem(255 * 50 / 100, m_Hndl[i]);
	}
	ChangeVolumeSoundMem(255 * 40 / 100, m_Hndl[SE_ATK]);
	ChangeVolumeSoundMem(255 * 40 / 100, m_Hndl[SE_PARRY]);

	for (int i = SE_WEAKATK; i < NUM_MAX; i++) {
		ChangeVolumeSoundMem(255 * 60 / 100, m_Hndl[i]);
	}
	ChangeVolumeSoundMem(255 * 50 / 100, m_Hndl[SE_STRONGATK]);
	ChangeVolumeSoundMem(255 * 50 / 100, m_Hndl[SE_LUNGES]);
	ChangeVolumeSoundMem(255, m_Hndl[SE_ELEPHANTCRY]);
}

//データ破棄
void SoundManager::Exit() {
	for (int i = 0; i < m_Hndl.size(); i++) {
		if (m_Hndl[i] != -1) {
			DeleteSoundMem(m_Hndl[i]);
			m_Hndl[i] = -1;
		}
	}

	for (int i = 0; i < m_ThreeDimensionHndl.size(); i++) {
		if (m_ThreeDimensionHndl[i] != -1) {
			DeleteSoundMem(m_ThreeDimensionHndl[i]);
			m_ThreeDimensionHndl[i] = -1;
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
	for (int i = 0; i < m_Hndl.size(); i++) {
		StopSoundMem(m_Hndl[i]);
	}

	for (int i = 0; i < m_ThreeDimensionHndl.size(); i++) {
		StopSoundMem(m_ThreeDimensionHndl[i]);
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

//音楽再生
bool SoundManager::ThreeDimension_Play(TagThreeDimensionID _ID, float _Rad, VECTOR _Pos, int _Type, bool _IsStart) {
	Set3DRadiusSoundMem(_Rad, m_ThreeDimensionHndl[_ID]);
	Set3DPositionSoundMem(_Pos, m_ThreeDimensionHndl[_ID]);

	int Res = PlaySoundMem(m_ThreeDimensionHndl[_ID], _Type, _IsStart);
	if (Res == -1) {
		return false;
	}
	else {
		return true;
	}
}

//指定音楽停止
void SoundManager::ThreeDimension_Stop(TagThreeDimensionID _ID) {
	StopSoundMem(m_ThreeDimensionHndl[_ID]);
}

//音楽を再生中か
bool SoundManager::ThreeDimension_IsPlay(TagThreeDimensionID _ID) {
	int Res = CheckSoundMem(m_ThreeDimensionHndl[_ID]);
	if (Res == 1) {
		return true;
	}
	else {
		return false;
	}
}

//3Dサウンドの再生座標を設定する
void SoundManager::SetThreeDimension_MemPos(VECTOR _Pos, TagThreeDimensionID _ID) {
	Set3DPositionSoundMem(_Pos, m_ThreeDimensionHndl[_ID]);
}
