#include <ps_smr.h>

static inline void
__ps_timing_update_remote(struct parsec *parsec, struct ps_quiescence_timing *curr, int remote_cpu)
{
	struct ps_quiescence_timing *cpu_i;

	cpu_i = &(parsec->timing_info[remote_cpu].timing);
	curr->time_in  = cpu_i->time_in;
	curr->time_out = cpu_i->time_out;

	ps_mem_fence();
	return;
}

void
ps_quiesce(struct parsec *parsec, coreid_t curr, ps_tsc_t *qsc)
{
	int i, qsc_cpu;
	ps_tsc_t min_known_qsc;
	struct ps_quiescence_timing t;

	min_known_qsc = ps_tsc();
	for (i = 1 ; i < PS_NUMCORES; i++) {
		/* Make sure we don't all hammer core 0... */
		qsc_cpu = (curr + i) % PS_NUMCORES;
		assert(qsc_cpu != curr);

		__ps_timing_update_remote(parsec, &t, qsc_cpu);
		if (__ps_in_lib(&t)) {
			if (min_known_qsc > t.time_in) min_known_qsc = t.time_in;
		}
	}
	*qsc = min_known_qsc;
	ps_mem_fence();
}
