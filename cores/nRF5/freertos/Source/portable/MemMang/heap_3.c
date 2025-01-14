/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


 /*
  * Implementation of pvPortMalloc() and vPortFree() that relies on the
  * compilers own malloc() and free() implementations.
  *
  * This file can only be used if the linker is configured to to generate
  * a heap memory area.
  *
  * See heap_1.c, heap_2.c and heap_4.c for alternative implementations, and the
  * memory management pages of http://www.FreeRTOS.org for more information.
  */

#include <stdlib.h>

  /* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
  all the API functions to use the MPU wrappers.  That should only be done when
  task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

  /*-----------------------------------------------------------*/

  // link to libnano's malloc
  // require "-Wl,--wrap=malloc -Wl,--wrap=free" linker option
extern void* __real_malloc(size_t size);
extern void __real_free(void* ptr);
extern void* __real_realloc(void* ptr, size_t _size);
extern void* __real_calloc(size_t nmemb, size_t _size);

void* __wrap_malloc(size_t c)
{
	return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) ? pvPortMalloc(c) : __real_malloc(c);
}

void __wrap_free(void* ptr)
{
	return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) ? vPortFree(ptr) : __real_free(ptr);
}

void* __wrap_realloc(void* ptr, size_t c)
{
	return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) ? pvPortRealloc(ptr, c) : __real_realloc(ptr, c);
}

void* __wrap_calloc(size_t nmemb, size_t c)
{
	return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) ? pvPortCalloc(nmemb, c) : __real_calloc(nmemb, c);
}

void* pvPortMalloc(size_t xWantedSize)
{
	void* pvReturn;

	vTaskSuspendAll();
	{
		pvReturn = __real_malloc(xWantedSize);
		traceMALLOC(pvReturn, xWantedSize);
	}
	(void)xTaskResumeAll();

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if (pvReturn == NULL)
		{
			extern void vApplicationMallocFailedHook(void);
			vApplicationMallocFailedHook();
		}
	}
#endif

	return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree(void* pv)
{
	if (pv)
	{
		vTaskSuspendAll();
		{
			__real_free(pv);
			traceFREE(pv, 0);
		}
		(void)xTaskResumeAll();
	}
}

/*-----------------------------------------------------------*/

void* pvPortRealloc(void* ptr, size_t xWantedSize)
{
	void* pvReturn;

	vTaskSuspendAll();
	{
		pvReturn = __real_realloc(ptr, xWantedSize);
		traceMALLOC(pvReturn, xWantedSize);
	}
	(void)xTaskResumeAll();

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if (pvReturn == NULL)
		{
			extern void vApplicationMallocFailedHook(void);
			vApplicationMallocFailedHook();
		}
	}
#endif

	return pvReturn;
}
/*-----------------------------------------------------------*/

void* pvPortCalloc(size_t nmemb, size_t xWantedSize)
{
	void* pvReturn;

	vTaskSuspendAll();
	{
		pvReturn = __real_calloc(nmemb, xWantedSize);
		traceMALLOC(pvReturn, xWantedSize);
	}
	(void)xTaskResumeAll();

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if (pvReturn == NULL)
		{
			extern void vApplicationMallocFailedHook(void);
			vApplicationMallocFailedHook();
		}
	}
#endif

	return pvReturn;
}