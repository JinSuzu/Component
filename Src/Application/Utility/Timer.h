#pragma once

class Timer
{
public:
	void PreUpdate() 
	{
		m_frame++;
		m_frame %= 600;
	}

	std::function<bool(int)> GetAlarmFn() 
	{	
		return std::bind(&Timer::GetAlarm,this, std::placeholders::_1);
	}

	bool GetAlarm(int _frame)const
	{
		if (_frame == 0)return true;
		return (m_frame % _frame) == 0;
	}
private:
	int m_frame = 0;
private:
	Timer() {}
public:
	static Timer& Instance() 
	{
		static Timer inst;
		return inst;
	}
};