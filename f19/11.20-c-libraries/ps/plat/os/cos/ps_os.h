/***
 * Copyright 2015 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

#ifndef PS_OS_H
#define PS_OS_H

#include <ps_config.h>
#include <ps_plat.h>

#include <stdlib.h>
#include <cos_debug.h>
#include <string.h>

/* Default allocation and deallocation functions */
static inline void *
ps_plat_alloc(size_t sz, coreid_t coreid)
{
	(void)sz;
	(void)coreid;

	return NULL;
}

static inline void
ps_plat_free(void *s, size_t sz, coreid_t coreid)
{
	(void)s; (void)coreid; (void)sz;
}

/*
 * These functions are ambiguously os vs. x86 as the locality API
 * requires that the underlying OS set up the locality information
 * readable with rdtscp correctly.  Arbitrary decision to put it here
 * for now.
 */
static inline ps_tsc_t
ps_tsc_locality(coreid_t *coreid, localityid_t *numaid)
{
	*coreid = 0;
	*numaid = 0;

	return ps_tsc();
}

static inline unsigned int
ps_coreid(void)
{
	coreid_t coreid, numaid;

	if (PS_NUMCORES == 1) return 0;
	ps_tsc_locality(&coreid, &numaid);

	return coreid;
}


#endif	/* PS_OS_H */
