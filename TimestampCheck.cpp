// TimestampCheck.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TimestampChecking.h"


int _tmain(int argc, _TCHAR* argv[])
{
	TimestampChecking t;
	long long time = 1444294302000;
	t.ActionTimestampCheck(time, 8);
	return 0;
}

