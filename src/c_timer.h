#ifndef __C_TIMER_H__
#define __C_TIMER_H__

#include <stdint.h>
#include <stdio.h>
#include <time.h>

typedef struct time_probe {
	struct timespec start_time;

	// Numbers collected on the fly.
	int64_t sum_nano_lapse;
	int64_t min_nano_lapse;
	int64_t max_nano_lapse;
	double sq_sum_micro_lapse;
	int64_t num_iter;

	// Numbers generated afterwards.
	double avg_micro; 	// avg latency
	double stdev_micro; 	// stdev of latency
	double throughput;
	
	int stats_gen;
} time_probe_t;

void *timer_init();
void timer_start(time_probe_t *tp);
int64_t timer_stop(time_probe_t *tp);
void timer_gen_stats(time_probe_t *tp);
void timer_combine_latency_stats(time_probe_t *tp_dst, time_probe_t *tp_src);
void timer_add_throughput_stats(time_probe_t *tp_dst, time_probe_t *tp_src);
void timer_print_stats(FILE *fp, time_probe_t *tp);


#endif // end of #ifndef __C_TIMER_H__
