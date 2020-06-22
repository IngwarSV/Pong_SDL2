#pragma once

#include <chrono> 

class Timer
{
private:
	// Aliases are used for convenience
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<int, std::ratio<1>>;
	using millisecond_t = std::chrono::duration<int, std::milli>;

	std::chrono::time_point<clock_t> _start;

public:
	Timer() : _start(clock_t::now())
	{
	}

	void startTimer()
	{
		_start = clock_t::now();
	}

	int timeElapsed() const
	{
		return std::chrono::duration_cast<millisecond_t>(clock_t::now() - _start).count();
	}
};
