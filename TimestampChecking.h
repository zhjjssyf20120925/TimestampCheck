#pragma once
class TimestampChecking
{
public:
	TimestampChecking();
	~TimestampChecking();

public:
	bool ActionTimestampCheck(long long time, long timezone);
private:
	int IsLeap(unsigned short year);
};

