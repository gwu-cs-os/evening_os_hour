#ifndef PS_QUIESCE_IMPL_H
#define PS_QUIESCE_IMPL_H

#ifndef PS_QLIST_BATCH
#define PS_QLIST_BATCH 128
#endif

struct ps_quiescence_timing {
	volatile ps_tsc_t     time_in, time_out;
	volatile ps_tsc_t     last_known_quiescence;
	char __padding[PS_CACHE_PAD_SZ(3*sizeof(ps_tsc_t))];
} PS_ALIGNED PS_PACKED;

struct __ps_other_core {
	ps_tsc_t time_in, time_out, time_updated;
};

struct ps_smr_percore {
	/* ps_quiescence_timing info of this CPU */
	struct ps_quiescence_timing timing;
	/* ps_quiescence_timing info of other CPUs known by this CPU */
	struct __ps_other_core timing_others[PS_NUMCORES];
	/* padding an additional cacheline for prefetching */
	char __padding[PS_CACHE_PAD_SZ(sizeof(struct __ps_other_core)*PS_NUMCORES + sizeof(struct ps_quiescence_timing))];
} PS_ALIGNED PS_PACKED;

struct ps_qsc_account {
	size_t             qmemcnt;    /* # of items in the qsc_list */
	size_t             qmemtarget; /* # of items in qsc_list before we attempt to quiesce */
};

struct parsec;
int ps_quiesce_wait(struct parsec *p, ps_tsc_t tsc, ps_tsc_t *qsc);
int ps_try_quiesce (struct parsec *p, ps_tsc_t tsc, ps_tsc_t *qsc);

#endif	/* PS_QUIESCE_IMPL_H */
