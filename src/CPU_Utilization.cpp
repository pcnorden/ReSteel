#include "CPU_Utilization.hxx"
#include <atomic>
#include <cstdio>

void CPUMetric::init(){
	// https://stackoverflow.com/a/6649992 : ifdef switches for different operating systems
	#ifdef __linux__
	FILE* file = fopen("/proc/stat", "r");
	fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
	fclose(file);
	//background_thread(sampleData);
	#elif _WIN32
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
	#endif
}


// This is just a exploration into mutli-threaded data collection.
// Huge shout out to Lanzelot on stackoverflow for this amazing answer:
// https://stackoverflow.com/a/64166
void CPUMetric::sampleData(){
	double percent;
	FILE* file;
	unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

	file = fopen("/proc/stat", "r");
	fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
	fclose(file);

	if(totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle){
		// This is a overflow prevention, so just skip this value
		percent = -1.0;
	}else{
		total = (totalUser-lastTotalUser)+(totalUserLow-lastTotalUserLow)+(totalSys-lastTotalSys);
		percent = total;
		total += (totalIdle-lastTotalIdle);
		percent /= total;
		percent *= 100;
	}
	lastTotalUser = totalUser;
	lastTotalUserLow = totalUserLow;
	lastTotalSys = totalSys;
	lastTotalIdle = totalIdle;
	last_percent.store(percent, std::memory_order_relaxed);
}

double CPUMetric::getCurrentValue(){
	#ifdef __linux__
	double percent;
	FILE* file;
	unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

	file = fopen("/proc/stat", "r");
	fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
	fclose(file);

	if(totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle){
		// This is a overflow prevention, so just skip this value
		percent = -1.0;
	}else{
		total = (totalUser-lastTotalUser)+(totalUserLow-lastTotalUserLow)+(totalSys-lastTotalSys);
		percent = total;
		total += (totalIdle-lastTotalIdle);
		percent /= total;
		percent *= 100;
	}
	lastTotalUser = totalUser;
	lastTotalUserLow = totalUserLow;
	lastTotalSys = totalSys;
	lastTotalIdle = totalIdle;
	return percent;
	#elif _WIN32
	PDH_FMT_COUNTERVALUE counterVal;
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
	#endif
}