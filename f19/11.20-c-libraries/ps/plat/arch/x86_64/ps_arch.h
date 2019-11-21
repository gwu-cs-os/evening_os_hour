/***
 * Copyright 2015 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

/*
 * TODO: most of this file should simply use the concurrency kit
 * versions.
 */

#ifndef PS_ARCH_H
#define PS_ARCH_H

#include <ps_config.h>

#define PS_PLAT_SHIFTR32(v) (v |= v >> 32)
#define PS_ATOMIC_POSTFIX "q"

#include "ps_arch_x86_common.h"

#endif /* PS_ARCH_H */
