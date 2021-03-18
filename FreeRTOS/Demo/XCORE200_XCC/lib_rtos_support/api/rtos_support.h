<<<<<<< HEAD
// Copyright (c) 2019, XMOS Ltd, All rights reserved
=======
// Copyright 2019 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
>>>>>>> d65f82bc8d6d34a00091a8191f4ba9c4f97d4588

#ifndef RTOS_SUPPORT_H_
#define RTOS_SUPPORT_H_

/* Config file to be provided by the RTOS */
#include "rtos_support_rtos_config.h"

/* Library header files */
#include "rtos_cores.h"
#include "rtos_interrupt.h"
#include "rtos_locks.h"
#include "rtos_time.h"
#include "rtos_macros.h"
#include "rtos_printf.h"

#ifndef __XC__
#include "rtos_irq.h"
#endif

#endif /* RTOS_SUPPORT_H_ */
