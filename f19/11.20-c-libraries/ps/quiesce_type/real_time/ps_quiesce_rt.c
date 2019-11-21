#include <limits.h>
#include <ps_smr.h>

void ps_quiesce(struct parsec *parsec, coreid_t curr, ps_tsc_t *qsc);
int __ps_smr_reclaim_batch(int batch, ps_tsc_t qsc, coreid_t curr, struct ps_qsc_list *ql, struct ps_mem *m, ps_free_fn_t ffn);

void
__ps_timing_info_init(struct parsec *ps, ps_tsc_t now)
{ (void)ps, (void)now; return ; }

void
__ps_smr_account_init(struct ps_smr_info *si)
{ si->account.qmemcnt = 0; }

void
__ps_invoke_smr(coreid_t curr, struct ps_qsc_list *ql, struct ps_smr_info *si, struct ps_mem *mem, ps_free_fn_t ffn)
{ (void)curr, (void)ql, (void)si, (void)mem, (void)ffn; return ; }

void
__ps_smr_reclaim(coreid_t curr, struct ps_qsc_list *ql, struct ps_smr_info *si,
		 struct ps_mem *m, ps_free_fn_t ffn)
{
	struct parsec    *ps = m->percore[curr].smr_info.ps;
	struct ps_mheader *a = __ps_qsc_peek(ql);
	ps_tsc_t qsc;
	assert(ps && ql && si);

	if (!a) return ;
	ps_quiesce(ps, curr, &qsc);
	__ps_smr_reclaim_batch(INT_MAX,  qsc, curr, ql, m, ffn);

	return;
}
