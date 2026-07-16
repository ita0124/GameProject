#pragma once
#include <DxLib.h>
#include <vector>
#include <iterator>

class InputManager {
public:
	enum {
		INPUT_MOVE_UP,			//上移動
		INPUT_MOVE_DOWN,		//下移動
		INPUT_MOVE_LEFT,		//左移動
		INPUT_MOVE_RIGHT,		//右移動
		INPUT_ROLLING,			//ローリング
		INPUT_JUMP,				//ジャンプ
		INPUT_GUARD,			//ガード
		INPUT_SKILL_ATTACK,		//スキル攻撃
		INPUT_NORMAL_ATTACK,	//通常攻撃

		INPUT_MAX
	};

private:
	static std::vector<int>m_Key;	//キーボード用
	static std::vector<int>m_Pad;	//コントローラー用
public:
	//初期化
	static void Init();
	//更新処理
	static void Update();

};

