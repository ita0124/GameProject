#pragma once


class CMouseInput
{
private:
	static unsigned int m_nowKey;		//現在のボタン情報
	static unsigned int m_beforeKey;	//1フレーム前のボタン

public:
	CMouseInput();

	//キー入力初期化
	static void Init();
	//キー入力情報更新
	static void Update();
	//キー入力判定（通常判定）
	static bool IsRep(unsigned int _key);
	//キー入力判定（トリガー判定）
	static bool IsTrg(unsigned int _key);

};

/*マウス入力一覧
MOUSE_INPUT_LEFT　　マウス左ボタン
MOUSE_INPUT_RIGHT　 マウス右ボタン
MOUSE_INPUT_MIDDLE　マウス中央ボタン
*/
