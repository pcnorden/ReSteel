#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <atomic> // std::atomic, std::memory_order_relaxed
#include <iostream> // std::cout
#include <thread> // std::thread

#ifdef _WIN32
#include "TCHAR.h"
#include "pdh.h"
#endif

class CPUMetric{
	#ifdef __linux__
	static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
	static std::atomic<double> last_percent;
	void sampleData();
	static std::thread background_thread;
	#elif _WIN32
	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;
	#endif
	public:
	void init();
	double getCurrentValue();
};