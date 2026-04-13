#pragma once
#include "Game/Object/Actor/Character/Player/Player.h"
#include "Lib/Input/InputKey.h"
#include "Lib/Input/InputPad.h"

class ItemBase {
protected:
	float	m_PosX;			//座標X
	float	m_PosY;			//座標Y

	int		m_Quantity;		//アイテム個数
	int		m_MaxQuantity;	//アイテム最大個数

	bool	m_IsUse;		//使用判定

	int		m_Hndl;			//画像ハンドル

	// データ読み込み処理
	void Load(const char* FilePath);

public:
	//コンストラクタ・デストラクタ
	ItemBase();
	virtual ~ItemBase();

	//初期化処理
	virtual void Init();

	//データ破棄処理
	virtual void Exit();

	// データ読み込み処理
	virtual void Load() = 0;

	//アイテム使用処理
	virtual void Use(Player& _Player) = 0;

	//描画処理
	virtual void Draw();

	//セット
	void SetIsUse(bool _IsUse) { m_IsUse = _IsUse; };
};
