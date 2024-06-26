#pragma once

class Score
{
public:
	void Add() { cnt++; };
	void Reset() { cnt = 0; };
	int Get() const { return cnt; };
private:
	int cnt = 0;

	Score() {}
public:
	static Score& Instance()
	{
		static Score inst;
		return inst;
	}
};
