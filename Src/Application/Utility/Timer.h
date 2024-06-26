#pragma once

class Timer
{
public:
	void PreUpdate()
	{
		m_frame++;
		m_frame %= 600;

		for (auto& timer : m_timers)
		{
			if (timer.second.start)
			{
				timer.second.frame++;
			}
		}
	}


	std::function<bool(int)> GetAlarmFn()
	{
		return std::bind(&Timer::GetAlarm, this, std::placeholders::_1);
	}

	bool GetAlarm(int _frame)const
	{
		if (_frame == 0)return true;
		return (m_frame % _frame) == 0;
	}

	void Start(std::string _tag) { m_timers[_tag] = Time(); }
	void Stop(std::string  _tag) { if(m_timers.find(_tag) != m_timers.end())m_timers[_tag].start = false; }
	int GetCnt(std::string _tag) { return m_timers[_tag].frame; }

	void ImGuiUpdate() 
	{
		for (auto& timer:m_timers) 
		{
			ImGui::Text((timer.first + " %d : %d").c_str(), timer.second.start, timer.second.frame);
		}
	}
private:
	int m_frame = 0;

	struct Time
	{
		bool start = true;
		int frame = 0;
	};
	std::map<std::string, Time>m_timers;

	Timer() {}
public:
	static Timer& Instance()
	{
		static Timer inst;
		return inst;
	}
};