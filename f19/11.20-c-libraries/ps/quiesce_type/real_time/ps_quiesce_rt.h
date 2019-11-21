#ifndef PS_QUIESCE_RT_H
#define PS_QUIESCE_RT_H

struct ps_quiescence_timing {
	volatile ps_tsc_t time_in, time_out;
	char __padding[PS_CACHE_PAD_SZ(2*sizeof(ps_tsc_t))];
} PS_ALIGNED PS_PACKED;

struct ps_smr_percore {
	/* ps_quiescence_timing info of this CPU */
	struct ps_quiescence_timing timing;
} PS_ALIGNED PS_PACKED;

struct ps_qsc_account {
	size_t qmemcnt;    /* # of items in the qsc_list */
};

#endif	/* PS_QUIESCE_RT_H */
