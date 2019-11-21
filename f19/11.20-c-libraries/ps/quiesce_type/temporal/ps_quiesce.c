#include <ps_smr.h>

void
ps_quiesce(struct parsec *parsec, coreid_t curr, ps_tsc_t *qsc)
{
	(void)parsec, (void)curr;
	*qsc = ps_tsc() - (ps_tsc_t)MAX_PARSEC_RESPONSE;
}
