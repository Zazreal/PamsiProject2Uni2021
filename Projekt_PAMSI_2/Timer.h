#pragma once
#include <chrono>
#include <iostream>

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start_Timepoint;
public:
	Timer() 
	{
		m_Start_Timepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		Stop();
	}
	inline void Stop()
	{
		auto EndTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start_Timepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(EndTimepoint).time_since_epoch().count();
		
		auto duration = end - start;
		auto ms = duration * 0.001;
		std::cout << "\n duration: " << duration << "us (" << ms << "ms)\n";
	}
};

