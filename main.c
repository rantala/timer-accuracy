/* Copyright (C) 2010 by Tommi Rantala <tt.rantala@gmail.com>
 * Copyright (C) 2010 by Leonid Moiseichuk <leonid.moiseichuk@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef ROUNDS
#define ROUNDS 10
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

void
test_clock_id(clockid_t clk_id, const char *clk_id_str)
{
	int i;
	struct timespec t1, t2;
	long long s1, s2;
	printf("clock_gettime(%s)\n", clk_id_str);
	for (i=0; i < ROUNDS; ++i) {
		if (clock_gettime(clk_id, &t1) < 0) abort();
		while (1) {
			if (clock_gettime(clk_id, &t2) < 0) abort();
			if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) break;
		}
		s1 = t1.tv_sec * 1000000000 + t1.tv_nsec; 
		s2 = t2.tv_sec * 1000000000 + t2.tv_nsec; 
		printf("   %lld.%09lld seconds\n",
				(s2-s1)/1000000000,
				(s2-s1)%1000000000);
	}
}

void
test_getrusage(void)
{
	int i;
	struct rusage t1, t2;
	struct timeval tv1, tv2;
	printf("getrusage()\n");
	for (i=0; i < ROUNDS; ++i) {
		if (getrusage(RUSAGE_SELF, &t1) < 0) abort();
		timeradd(&t1.ru_utime, &t1.ru_stime, &tv1);
		while (1) {
			if (getrusage(RUSAGE_SELF, &t2) < 0) abort();
			timeradd(&t2.ru_utime, &t2.ru_stime, &tv2);
			if (timercmp(&tv1, &tv2, !=)) break;
		}
		timersub(&tv2, &tv1, &tv2);
		printf("   %lld.%06lld000 seconds\n",
				(long long)tv2.tv_sec,
				(long long)tv2.tv_usec);
	}
}

void
test_gettimeofday(void)
{
	int i;
	struct timeval tv1, tv2;
	printf("gettimeofday()\n");
	for (i=0; i < ROUNDS; ++i) {
		if (gettimeofday(&tv1, NULL) < 0) abort();
		while (1) {
			if (gettimeofday(&tv2, NULL) < 0) abort();
			if (timercmp(&tv1, &tv2, !=)) break;
		}
		timersub(&tv2, &tv1, &tv2);
		printf("   %lld.%06lld000 seconds\n",
				(long long)tv2.tv_sec,
				(long long)tv2.tv_usec);
	}
}

void
test_clock(void)
{
	int i;
	clock_t c1, c2;
	printf("clock()\n");
	for (i=0; i < ROUNDS; ++i) {
		c1 = clock();
		if (c1 == (clock_t)-1) abort();
		while (1) {
			c2 = clock();
			if (c1 != c2) break;
		}
		printf("   %.9f seconds\n", (double)(c2 - c1) / (double)CLOCKS_PER_SEC);
	}
}

int main(void)
{
#ifdef CLOCK_REALTIME
	test_clock_id(CLOCK_REALTIME, "CLOCK_REALTIME");
#endif
#ifdef CLOCK_MONOTONIC
	test_clock_id(CLOCK_MONOTONIC, "CLOCK_MONOTONIC");
#endif
#ifdef CLOCK_PROCESS_CPUTIME_ID
	test_clock_id(CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID");
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
	test_clock_id(CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID");
#endif
	test_getrusage();
	test_gettimeofday();
	test_clock();
	return 0;
}
