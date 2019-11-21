/***
 * Copyright 2017 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

#ifndef PS_CONFIG_H
#define PS_CONFIG_H

/*
 * How frequently do we check remote free lists when we make an
 * allocation?  This is in platform-specific code because it is
 * dependent on the hardware costs for cache-line contention on a
 * remote numa node.
 *
 * If that contention has 16x the cost of a normal allocation, for
 * example, then choosing to batch checking remote frees once every
 * 128 iterations increases allocation cost by a factor of (2^4/2^7 =
 * 2^-3) 1/8.
 */
#ifndef PS_REMOTE_BATCH
/* Needs to be a power of 2 */
#define PS_REMOTE_BATCH 64
#endif

/*
 * Core and socket counts.  We're concerned with servers and embedded
 * systems; both domains where the machine configuration is often
 * known apriori which gives use the opportunity to more efficiently
 * lay out data, and gives some flexibiliy in system design and
 * optimization (see the parsec paper on the optimization based on
 * different cores iterating through other cores by starting at
 * different offsets).
 */
#ifndef PS_NUMCORES
#define PS_NUMCORES      4
#endif
#ifndef PS_NUMLOCALITIES
#define PS_NUMLOCALITIES 1
#endif

#endif	/* PS_CONFIG_H */
