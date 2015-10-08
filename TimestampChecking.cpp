#include "stdafx.h"
#include "TimestampChecking.h"
#include <iostream>
#include <sstream>
using namespace std;

/***********************************************************************************************************
 * 程序作者：赵进军
 * 函数功能：构造函数
 * 参数说明：null
 * 注意事项：null
 * 修改日期：2015/10/08 17:56:00
 ***********************************************************************************************************/
TimestampChecking::TimestampChecking()
{
}


/***********************************************************************************************************
 * 程序作者：赵进军
 * 函数功能：析构函数
 * 参数说明：null
 * 注意事项：null
 * 修改日期：2015/10/08 17:57:00
 ***********************************************************************************************************/
TimestampChecking::~TimestampChecking()
{
}


/***********************************************************************************************************
 * 程序作者：赵进军
 * 函数功能：检查当年是闰年还是平年
 * 参数说明：
 *     year：需要检查的年份
 * 注意事项：null
 * 修改日期：2015/10/08 17:58:00
 ***********************************************************************************************************/
int TimestampChecking::IsLeap(unsigned short year)
{
	return ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0));
}

/***********************************************************************************************************
 * 程序作者：赵进军
 * 函数功能：检查时间戳转换成时间格式是否正常
 * 参数说明：
 *     time：时间戳
 * timezone：默认为8，暂时不用修改
 * 注意事项：null
 * 修改日期：2015/10/08 18:01:00
 ***********************************************************************************************************/
bool TimestampChecking::ActionTimestampCheck(long long time, long timezone)
{
	ostringstream os;																								// 将long 类型的时间戳转换成字符串类型的时间戳
	os << time;
	string result;
	istringstream is(os.str());
	is >> result;

	bool isMilsecond = false;																						// 标记当前输入的时间戳是毫秒级
	int timeLen = result.size();
	switch (timeLen)
	{
	case 13:
		isMilsecond = true;
		break;
	case 10:
		isMilsecond = false;
		break;
	default:
		return false;
		break;
	}

	const int monthLengths[2][13] =																					// 定义每个月天数累计
	{
		{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
		{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
	};
	const int yearLengths[2] = { 365, 366 };																		// 定义平年、闰年的天数
	int year(0), month(0), minMonth(0), maxMonth(0), days(0), clock(0), isLeap(0), day(0), hour(0), minute(0), second(0);
	if (isMilsecond)
		time /= 1000;																							    // 把毫秒转换成秒
	time += timezone * 60 * 60;
	days = time / 86400;																							// 天数
	clock = time % 86400;																							// 小时数

	if (clock < 0)
	{
		clock += 86400;
		days -= 1;
	}

	if (days >= 0)
	{
		year = days / 366;
		days -= year * 365 + (year + 1) / 4 - (year + 69) / 100 + (year + 369) / 400;
		for (year = year + 1970;; year++)
		{
			isLeap = IsLeap(year);
			if (days < yearLengths[isLeap])
			{
				break;
			}
			days -= yearLengths[isLeap];
		}
	}
	else
	{
		year = days / 366;
		days -= year * 365 + (year - 2) / 4 - (year - 30) / 100 + (year - 30) / 400;
		for (year = year + 1970 - 1;; year--)
		{
			isLeap = false;
			days += yearLengths[isLeap];
			if (days >= 0)
			{
				break;
			}
		}
	}

	minMonth = 0;
	maxMonth = 12;
	for (month = 5; month < 12 && month>0; month = (minMonth + maxMonth) / 2)
	{
		if (days < monthLengths[isLeap][month])
		{
			maxMonth = month;
		}
		else if (days >= monthLengths[isLeap][month + 1])
		{
			minMonth = month;
		}
		else
		{
			break;
		}
	}
	days -= monthLengths[isLeap][month];
	month++;
	day = days + 1;

	hour = clock / 3600;																							// 获取当前时间戳的小时部分
	clock = clock % 3600;
	minute = clock / 60;																							// 获取当前时间戳的分钟部分
	second = clock % 60;																							// 获取当前时间戳的秒钟部分

	if (year < 1970 && year>2099)																					// 检查当前时间戳的年部分是否合法
		return false;

	if (month < 1 && month>12)																						// 检查当前时间戳的月部分是否合法
		return false;

	if (day < 1 && day>31)																							// 检查当前时间戳的天部分是否合法
		return false;

	if (hour < 1 && hour>24)																						// 检查当前时间戳的小时部分是否合法
		return false;

	if (minute < 1 && minute>60)																					// 检查当前时间戳的分钟部分是否合法
		return false;

	if (second < 1 && second>60)																					// 检查当前时间戳的秒钟部分是否合法
		return false;

	//printf("%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);                                // 暂时注释该句话
	return true;
}