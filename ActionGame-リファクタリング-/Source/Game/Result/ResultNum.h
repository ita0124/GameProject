#pragma once

class ResultNum {
private:
	static int m_ResultNum;

public:
	static void Init() { m_ResultNum = -1; };

	static int GetNum() { return m_ResultNum; }

	static void SetNum(int _ResultNum) { m_ResultNum = _ResultNum; }
};

//”s–k‚P@Ÿ—˜‚Q
