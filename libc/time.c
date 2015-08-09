#include "time.h"
#include "stdint.h"

// Include "common.h" to use this variable, you should be using time(NULL) though
// This variable stores the UNIX EPOCH time.
time_t SystemTime = 0;


// Don't ask me. It just works. - Justasic
static __inline__ unsigned long _mktime (unsigned int year, unsigned int mon, unsigned int day, unsigned int hour, unsigned int min, unsigned int sec)
{
	if (0 >= (int)(mon -= 2))
	{                               /* 1..12 -> 11,12,1..10 */
		mon += 12;              /* Puts Feb last since it has leap day */
		year -= 1;
	}
	return ((((unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) + year*365 - 719499)*24 + hour /* now have hours */)*60 + min /* now have minutes */)*60 + sec; /* finally seconds */
}

time_t mktime(struct tm *tm)
{
	if (!tm)
		return -1;

	return _mktime(tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}


// Easiest function ever.
time_t time(time_t *t)
{
	if (t)
		*t = SystemTime;
	return SystemTime;
}
