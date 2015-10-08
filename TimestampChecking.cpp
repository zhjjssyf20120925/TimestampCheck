#include "stdafx.h"
#include "TimestampChecking.h"
#include <iostream>
#include <sstream>
using namespace std;

/***********************************************************************************************************
 * �������ߣ��Խ���
 * �������ܣ����캯��
 * ����˵����null
 * ע�����null
 * �޸����ڣ�2015/10/08 17:56:00
 ***********************************************************************************************************/
TimestampChecking::TimestampChecking()
{
}


/***********************************************************************************************************
 * �������ߣ��Խ���
 * �������ܣ���������
 * ����˵����null
 * ע�����null
 * �޸����ڣ�2015/10/08 17:57:00
 ***********************************************************************************************************/
TimestampChecking::~TimestampChecking()
{
}


/***********************************************************************************************************
 * �������ߣ��Խ���
 * �������ܣ���鵱�������껹��ƽ��
 * ����˵����
 *     year����Ҫ�������
 * ע�����null
 * �޸����ڣ�2015/10/08 17:58:00
 ***********************************************************************************************************/
int TimestampChecking::IsLeap(unsigned short year)
{
	return ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0));
}

/***********************************************************************************************************
 * �������ߣ��Խ���
 * �������ܣ����ʱ���ת����ʱ���ʽ�Ƿ�����
 * ����˵����
 *     time��ʱ���
 * timezone��Ĭ��Ϊ8����ʱ�����޸�
 * ע�����null
 * �޸����ڣ�2015/10/08 18:01:00
 ***********************************************************************************************************/
bool TimestampChecking::ActionTimestampCheck(long long time, long timezone)
{
	ostringstream os;																								// ��long ���͵�ʱ���ת�����ַ������͵�ʱ���
	os << time;
	string result;
	istringstream is(os.str());
	is >> result;

	bool isMilsecond = false;																						// ��ǵ�ǰ�����ʱ����Ǻ��뼶
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

	const int monthLengths[2][13] =																					// ����ÿ���������ۼ�
	{
		{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
		{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
	};
	const int yearLengths[2] = { 365, 366 };																		// ����ƽ�ꡢ���������
	int year(0), month(0), minMonth(0), maxMonth(0), days(0), clock(0), isLeap(0), day(0), hour(0), minute(0), second(0);
	if (isMilsecond)
		time /= 1000;																							    // �Ѻ���ת������
	time += timezone * 60 * 60;
	days = time / 86400;																							// ����
	clock = time % 86400;																							// Сʱ��

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

	hour = clock / 3600;																							// ��ȡ��ǰʱ�����Сʱ����
	clock = clock % 3600;
	minute = clock / 60;																							// ��ȡ��ǰʱ����ķ��Ӳ���
	second = clock % 60;																							// ��ȡ��ǰʱ��������Ӳ���

	if (year < 1970 && year>2099)																					// ��鵱ǰʱ������겿���Ƿ�Ϸ�
		return false;

	if (month < 1 && month>12)																						// ��鵱ǰʱ������²����Ƿ�Ϸ�
		return false;

	if (day < 1 && day>31)																							// ��鵱ǰʱ������첿���Ƿ�Ϸ�
		return false;

	if (hour < 1 && hour>24)																						// ��鵱ǰʱ�����Сʱ�����Ƿ�Ϸ�
		return false;

	if (minute < 1 && minute>60)																					// ��鵱ǰʱ����ķ��Ӳ����Ƿ�Ϸ�
		return false;

	if (second < 1 && second>60)																					// ��鵱ǰʱ��������Ӳ����Ƿ�Ϸ�
		return false;

	//printf("%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);                                // ��ʱע�͸þ仰
	return true;
}