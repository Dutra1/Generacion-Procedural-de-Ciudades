#pragma once
#ifndef LOGGER
#define LOGGER

class Logger {
private:
	std::chrono::high_resolution_clock::time_point start, lastUpdate;
public:
	Logger();
	void log(const std::string & string) const;
	void update();
	void updateAndLogDuration(const std::string & string);
};

#endif

