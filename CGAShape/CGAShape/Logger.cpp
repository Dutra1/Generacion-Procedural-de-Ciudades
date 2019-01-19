#include "stdafx.h"
#include "Logger.h"

Logger::Logger(){
	start = std::chrono::high_resolution_clock::now();
	lastUpdate = start;
}

void Logger::log(const std::string & string) const {
	std::cout << string << std::endl;
}

void Logger::update() {
	lastUpdate = std::chrono::high_resolution_clock::now();
}

void Logger::updateAndLogDuration(const std::string & string) {
	std::chrono::high_resolution_clock::time_point oldUpdate = lastUpdate;
	update();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds> (lastUpdate - oldUpdate);
	std::cout << duration.count() / 1000.0 << " " << string << std::endl;
}

