/* Copyright (C) 2010 by Tommi Rantala <tt.rantala@gmail.com>
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
test_clock_process_cputime_id(void)
{
#ifdef CLOCK_PROCESS_CPUTIME_ID
	int i;
	struct timespec t1, t2;
	long long s1, s2;
	printf("clock_gettime(CLOCK_PROCESS_CPUTIME_ID)\n");
	for (i=0; i < ROUNDS; ++i) {
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1) < 0) abort();
		while (1) {
			if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2) < 0) abort();
			if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) break;
		}
		s1 = t1.tv_sec * 1000000000 + t1.tv_nsec; 
		s2 = t2.tv_sec * 1000000000 + t2.tv_nsec; 
		printf("   %lld.%09lld seconds\n",
				(s2-s1)/1000000000,
				(s2-s1)%1000000000);
	}
#endif
}

void
test_clock_thread_cputime_id(void)
{
#ifdef CLOCK_THREAD_CPUTIME_ID
	int i;
	struct timespec t1, t2;
	long long s1, s2;
	printf("clock_gettime(CLOCK_THREAD_CPUTIME_ID)\n");
	for (i=0; i < ROUNDS; ++i) {
		if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t1) < 0) abort();
		while (1) {
			if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t2) < 0) abort();
			if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) break;
		}
		s1 = t1.tv_sec * 1000000000 + t1.tv_nsec; 
		s2 = t2.tv_sec * 1000000000 + t2.tv_nsec; 
		printf("   %lld.%09lld seconds\n",
				(s2-s1)/1000000000,
				(s2-s1)%1000000000);
	}
#endif
}

void
test_clock_realtime(void)
{
#ifdef CLOCK_REALTIME
	int i;
	struct timespec t1, t2;
	long long s1, s2;
	printf("clock_gettime(CLOCK_REALTIME)\n");
	for (i=0; i < ROUNDS; ++i) {
		if (clock_gettime(CLOCK_REALTIME, &t1) < 0) abort();
		while (1) {
			if (clock_gettime(CLOCK_REALTIME, &t2) < 0) abort();
			if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) break;
		}
		s1 = t1.tv_sec * 1000000000 + t1.tv_nsec; 
		s2 = t2.tv_sec * 1000000000 + t2.tv_nsec; 
		printf("   %lld.%09lld seconds\n",
				(s2-s1)/1000000000,
				(s2-s1)%1000000000);
	}
#endif
}


void
test_clock_monotonic(void)
{
#ifdef CLOCK_MONOTONIC
	int i;
	struct timespec t1, t2;
	long long s1, s2;
	printf("clock_gettime(CLOCK_MONOTONIC)\n");
	for (i=0; i < ROUNDS; ++i) {
		if (clock_gettime(CLOCK_MONOTONIC, &t1) < 0) abort();
		while (1) {
			if (clock_gettime(CLOCK_MONOTONIC, &t2) < 0) abort();
			if (t1.tv_sec != t2.tv_sec || t1.tv_nsec != t2.tv_nsec) break;
		}
		s1 = t1.tv_sec * 1000000000 + t1.tv_nsec; 
		s2 = t2.tv_sec * 1000000000 + t2.tv_nsec; 
		printf("   %lld.%09lld seconds\n",
				(s2-s1)/1000000000,
				(s2-s1)%1000000000);
	}
#endif
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

int main(void)
{
	test_clock_realtime();
	test_clock_monotonic();
	test_clock_process_cputime_id();
	test_clock_thread_cputime_id();
	test_getrusage();
	test_gettimeofday();
	return 0;
}
