/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 1.0 - September 27, 2000                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratories                                 
 *    (C) Copyright 2000 All Rights Reserved                            
 *                                                                      
 * -- Copyright notice and Licensing terms:                             
 *                                                                      
 * Redistribution  and  use in  source and binary forms, with or without
 * modification, are  permitted provided  that the following  conditions
 * are met:                                                             
 *                                                                      
 * 1. Redistributions  of  source  code  must retain the above copyright
 * notice, this list of conditions and the following disclaimer.        
 *                                                                      
 * 2. Redistributions in binary form must reproduce  the above copyright
 * notice, this list of conditions,  and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. 
 *                                                                      
 * 3. All  advertising  materials  mentioning  features  or  use of this
 * software must display the following acknowledgement:                 
 * This  product  includes  software  developed  at  the  University  of
 * Tennessee, Knoxville, Innovative Computing Laboratories.             
 *                                                                      
 * 4. The name of the  University,  the name of the  Laboratory,  or the
 * names  of  its  contributors  may  not  be used to endorse or promote
 * products  derived   from   this  software  without  specific  written
 * permission.                                                          
 *                                                                      
 * -- Disclaimer:                                                       
 *                                                                      
 * THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ---------------------------------------------------------------------
 */ 
/*
 * Include files
 */
#include "hpl.h"
 
/* 
 * Purpose
 * =======
 *
 * HPL_timer_walltime returns the elapsed (wall-clock) time.
 *  
 * If HPL_USE_PentiumCPS  is defined to be the clock cycle per second on
 * an i[56]86 Intel processor (e.g., HPL_USE_PentiumCPS is defined to be
 * 300  on a  300 Mhz Pentium processor), we read the internal processor
 * counters. Otherwise we use the  gettimeofday()  function that returns
 * the wall-clock time of the system (default).
 *
 * ---------------------------------------------------------------------
 */ 

#if defined( HPL_USE_PentiumCPS )
 
#include <sys/time.h>
#include <sys/types.h>
 
#define    CPS     HPL_USE_PentiumCPS * 1e6
 
static unsigned               usec, sec, tusec, tsec, start = 0, startu;
static long long              tmp;
 
#ifdef STDC_HEADERS
static inline void microtime
(
   unsigned                   * LO,
   unsigned                   * HI
)
#else
static inline void microtime( LO, HI )
   unsigned                   * HI, * LO;
#endif
{
  __asm __volatile (
        ".byte 0x0f; .byte 0x31   # RDTSC instruction
        movl    %%edx,%0          # High order 32 bits
        movl    %%eax,%1          # Low order 32 bits"
                : "=g" (*HI), "=g" (*LO) :: "eax", "edx");
}

#ifdef STDC_HEADERS
double HPL_timer_walltime( void )
#else
double HPL_timer_walltime()
#endif
{
  if( !start ) { microtime( &startu, &start ); return( HPL_rzero ); }
 
  microtime( &usec, &sec );
 
  tmp = sec; tmp -= start; tmp = ( tmp << 32 ) + usec; tmp -= startu;
  return( ( (double)(tmp) ) / (double)(CPS) );
}
 
/* #elif defined( HPL_USE_GETTIMEOFDAY ) */
#else

#include <sys/time.h>
#include <sys/resource.h>

#ifdef STDC_HEADERS
double HPL_timer_walltime( void )
#else
double HPL_timer_walltime()
#endif
{
   struct timeval             tp;
   static long                start=0, startu;

   if( !start )
   {
      (void) gettimeofday( &tp, NULL );
      start  = tp.tv_sec;
      startu = tp.tv_usec;
      return( HPL_rzero );
   }
   (void) gettimeofday( &tp, NULL );

   return( (double)( tp.tv_sec - start ) +
           ( (double)( tp.tv_usec-startu ) / 1000000.0 ) );
}                                                                               

#endif
/*
 * End of HPL_timer_walltime
 */
