#pragma once

class Coin {
private:
	static int m_CoinCount;

public:
	//‰Šú‰»ˆ—
	static void Init() { m_CoinCount = 0; }
	//Get
	static int GetCoinCount() { return m_CoinCount; }
	//Set
	static void SetCoinCount(int _CoinCount) { m_CoinCount = _CoinCount; }
	//ŒvZ
	static void CalcCoinCount(int _CoinCount) { m_CoinCount += _CoinCount; }
};
