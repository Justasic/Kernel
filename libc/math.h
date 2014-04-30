/*
 * Copyright (c) 2014, Justin Crawford <Justasic@gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice 
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// All math functions defined as C90 functions.
#ifndef __MATH_H__
#define __MATH_H__

////////////////////////////////////////
// Trigonometric functions
extern double cos(double x);
extern double sin(double x);
extern double tan(double x);


////////////////////////////////////////
// Exponential and logarithmic functions
extern double modf(double x, double* intpart);
extern double log(double x);
extern double log10(double x);

////////////////////////////////////////
// Power functions
extern double pow(double base, double exponent);
extern double sqrt(double x);

////////////////////////////////////////
// Rounding and remainder functions
extern double ceil(double x);
extern double floor(double x);
extern double fmod(double numer, double denom);

extern double trunc(double x);
extern float truncf(float x);
extern long double truncl(long double x);

extern double round(double x);
extern float roundf(float x);
extern long double roundl(long double x);

////////////////////////////////////////
// Minimum, Maximum, and difference functions
extern double fmin(double x, double y);
extern float fminf(float x, float y);
extern long double fminl(long double x, long double y);

extern double fmax(double x, double y);
extern float fmaxf(float x, float y);
extern long double fmaxl(long double x, long double y);

////////////////////////////////////////
// Other functions.
extern double fabs(double x);

extern double abs(double x);
extern float absf(float x);
extern long double absld(long double x);

//##################################################################
//##################################################################
//##################################################################
// MACROS/FUNCTIONS
//##################################################################
//##################################################################
//##################################################################

// Not sure if this is too large, truncate some numbers
// if compiler warnings.
#define M_PI 3.14159265358979323846264338327950288419


#endif // __MATH_H__
