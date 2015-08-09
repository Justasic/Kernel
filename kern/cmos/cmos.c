//
// Created by justasic on 8/8/15.
//

#include "cmos.h"
#include "lib/common.h"
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#define BCD2BIN(n) (((n) >> 4) * 10 + ((n) & 0x0F))

uint8_t ReadCMOS(uint8_t flag)
{
	outb(PORT_CMOS_INDEX, flag);
	return inb(PORT_CMOS_DATA);
}

int CMOSUpdate()
{
	outb(PORT_CMOS_INDEX, CMOS_STATUS_UPDATE);
	return (inb(PORT_CMOS_DATA) & 0x80);
}

time_t GetCMOSTime()
{
	struct tm tm;

	// Assume that we already aren't in a CMOS update
	tm.tm_sec = ReadCMOS(CMOS_RTC_SECONDS);
	tm.tm_min = ReadCMOS(CMOS_RTC_MINUTES);
	tm.tm_hour = ReadCMOS(CMOS_RTC_HOURS);
	tm.tm_mday = ReadCMOS(CMOS_RTC_DAY_MONTH);
	tm.tm_wday = ReadCMOS(CMOS_RTC_DAY_WEEK);
	tm.tm_mon = ReadCMOS(CMOS_RTC_MONTH);
	tm.tm_year = ReadCMOS(CMOS_RTC_YEAR);
	int century = ReadCMOS(CMOS_CENTURY);
	int RegisterB = ReadCMOS(CMOS_STATUS_B);

	// Fix BCD crap
	if (!(RegisterB & 0x04))
	{
		tm.tm_sec  = BCD2BIN(tm.tm_sec);
		tm.tm_min  = BCD2BIN(tm.tm_min);
		tm.tm_hour = BCD2BIN(tm.tm_hour);
		tm.tm_mday = BCD2BIN(tm.tm_mday);
		tm.tm_wday = BCD2BIN(tm.tm_wday);
		tm.tm_mon  = BCD2BIN(tm.tm_mon);
		tm.tm_year = BCD2BIN(tm.tm_year);
		century = BCD2BIN(century);
	}

	// Convert from 12 hour to 24 hour if necessary
	if (!(RegisterB & 0x02) && (tm.tm_hour & 0x80))
		tm.tm_hour = ((tm.tm_hour & 0x7F) + 12) % 24;


	// Fix the year
	tm.tm_year += century * 100;

	return mktime(&tm);
}
