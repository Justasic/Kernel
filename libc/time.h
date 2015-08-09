//
// Created by justasic on 8/8/15.
//

#ifndef KERNEL_TIME_H
#define KERNEL_TIME_H
#include "stdint.h"

#ifndef time_t
typedef uint64_t time_t;
#endif

struct tm {
	int tm_sec;    /* Seconds (0-60) */
	int tm_min;    /* Minutes (0-59) */
	int tm_hour;   /* Hours (0-23) */
	int tm_mday;   /* Day of the month (1-31) */
	int tm_mon;    /* Month (0-11) */
	int tm_year;   /* Year - 1900 */
	int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
	int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
	int tm_isdst;  /* Daylight saving time */
	char *tm_zone;    /* Timezone name */
	int tm_gmtoff; /* GMT offset */
};

extern time_t mktime(struct tm *tm);
extern time_t time(time_t *t);

#include "strftime.h"

#endif //KERNEL_TIME_H
