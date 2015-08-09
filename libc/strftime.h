//
// Created by justasic on 8/8/15.
//

#ifndef KERNEL_STRFTIME_H
#define KERNEL_STRFTIME_H
#include <stdint.h>
#include <time.h>

extern size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t);
extern struct tm *localtime_r(const time_t *timer, struct tm *tmbuf);
extern struct tm *gmtime_r(const time_t *timer, struct tm *tmbuf);

#endif //KERNEL_STRFTIME_H
