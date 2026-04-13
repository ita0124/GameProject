#include "DxLibFont.h"

int DxLibFont::FONTHNDL_N15;
int DxLibFont::FONTHNDL_N30;
int DxLibFont::FONTHNDL_N50;

void DxLibFont::Init() {
	FONTHNDL_N15 = CreateFontToHandle(NULL, 15, 0, DX_FONTTYPE_NORMAL);	//ノーマルフォント,サイズ15
	FONTHNDL_N30 = CreateFontToHandle(NULL, 30, 0, DX_FONTTYPE_NORMAL);	//ノーマルフォント,サイズ30FONTHNDL_N30;	//ノーマルフォント,サイズ30
	FONTHNDL_N50 = CreateFontToHandle(NULL, 50, 0, DX_FONTTYPE_NORMAL);	//ノーマルフォント,サイズ50FONTHNDL_N50;	//ノーマルフォント,サイズ50
}