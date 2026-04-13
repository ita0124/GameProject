#pragma once
#include <DxLib.h>

//Dxlibにあるフォントを指定するやつのクラス
class DxLibFont {
public:
	static int FONTHNDL_N15;	//ノーマルフォント,サイズ15
	static int FONTHNDL_N30;	//ノーマルフォント,サイズ30
	static int FONTHNDL_N50;	//ノーマルフォント,サイズ50

	static void Init();
};

//DrawFormatStringToHandle(X座標, Y座標, 色指定, フォントハンドル, "描画したい文字", 入れる数値);
