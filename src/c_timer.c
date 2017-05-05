#include "c_timer.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define KILO_F	1000.0F
#define MEGA_F	1000000.0F
#define GIGA_F	1000000000.0F

#define KILO_LL	1000LL
#define MEGA_LL	1000000LL
#define GIGA_LL	1000000000LL

static int64_t elapsed_in_nano(struct timespec *start, struct timespec *end)
{
	int64_t elapsed_sec;
	int64_t elapsed_nano;

	elapsed_sec = end->tv_sec;
	elapsed_sec -= start->tv_sec;
	// Let's assume elapsed second is less than 24 hours all the time.
	assert(elapsed_sec >= 0 && elapsed_sec < 86400);

	elapsed_nano = elapsed_sec * GIGA_LL;
	elapsed_nano += end->tv_nsec;
	elapsed_nano -= start->tv_nsec;
	assert(elapsed_nano >= 0);

	return elapsed_nano;
}


void *timer_init()
{
	time_probe_t *tp = (time_probe_t *) malloc(sizeof(*tp));
	if (tp == NULL) {
		fprintf(stderr, "ERROR: timer_init failed\n");
	} else {
		memset(tp, 0x00, sizeof(*tp));	
	}
	return tp;
}

void timer_start(time_probe_t *tp)
{
	clock_gettime(CLOCK_REALTIME, &tp->start_time);
}

int64_t timer_stop(time_probe_t *tp)
{
	int64_t elapsed_nano;
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	elapsed_nano = elapsed_in_nano(&tp->start_time, &now);
	tp->num_iter++;

	tp->sum_nano_lapse += elapsed_nano;
	tp->sq_sum_micro_lapse += (((double) elapsed_nano / KILO_F) *
		((double) elapsed_nano / KILO_F));
	if (tp->max_nano_lapse < elapsed_nano) {
		tp->max_nano_lapse = elapsed_nano;
	}
	if (tp->min_nano_lapse > elapsed_nano) {
		tp->min_nano_lapse = elapsed_nano;
	}
	return elapsed_nano;
}

static void timer_gen_latency_stats(time_probe_t *tp)
{
	double sq_avg_micro;
	double var_micro;

	tp->avg_micro = ((double) tp->sum_nano_lapse) / KILO_F /
		((double) tp->num_iter);

	sq_avg_micro = ((double) tp->sq_sum_micro_lapse) /
		((double) tp->num_iter);
	var_micro = sq_avg_micro - (tp->avg_micro * tp->avg_micro);
	assert(var_micro >= 0);

	tp->stdev_micro = sqrt(var_micro);
}

static void timer_gen_throughput_stats(time_probe_t *tp)
{
	if (tp->num_iter != 0) {
		tp->throughput = (double) tp->num_iter /
			((double) tp->sum_nano_lapse / GIGA_F);
	}
}

void timer_gen_stats(time_probe_t *tp)
{
	if (tp->stats_gen) {
		fprintf(stderr, "ERROR: stats already generated.\n");
	} else {
		timer_gen_throughput_stats(tp);
		timer_gen_latency_stats(tp);
		tp->stats_gen = 1;
	}
}

void timer_combine_latency_stats(time_probe_t *tp_dst, time_probe_t *tp_src)
{
	if (!tp_dst->stats_gen || !tp_src->stats_gen) {
		fprintf(stderr, "ERROR: stats should be generated first.\n");
	}
	tp_dst->num_iter += tp_src->num_iter;
	tp_dst->sum_nano_lapse += tp_src->sum_nano_lapse;
	tp_dst->sq_sum_micro_lapse += tp_src->sq_sum_micro_lapse;
	if (tp_dst->max_nano_lapse < tp_src->max_nano_lapse) {
		tp_dst->max_nano_lapse = tp_src->max_nano_lapse;
	}
	if (tp_dst->min_nano_lapse > tp_src->min_nano_lapse) {
		tp_dst->min_nano_lapse = tp_src->min_nano_lapse;
	}
	timer_gen_latency_stats(tp_dst);
}

void timer_add_throughput_stats(time_probe_t *tp_dst, time_probe_t *tp_src)
{
	tp_dst->throughput += tp_src->throughput;
}

void timer_print_stats(FILE *fp, time_probe_t *tp)
{

	fprintf(fp, "Type\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
			"op", "avg(us)", "min(us)", "max(us)", "stdev", "num_iter", "tput");
	if (tp->num_iter == 0) {
		fprintf(fp, "No measured data\n");
	} else {
		fprintf(fp, "Numbers\t%.3f\t%.3f\t%.3f\t%.3f\t%ld\t%.3f\n",
				tp->avg_micro,
				((double) tp->min_nano_lapse) / KILO_F,
				((double) tp->max_nano_lapse) / KILO_F,
				tp->stdev_micro,
				tp->num_iter,
				tp->throughput);
	}
	fflush(fp);
}

void timer_destroy(time_probe_t *tp)
{
	if (tp != NULL) {
		free(tp);
	}
}


