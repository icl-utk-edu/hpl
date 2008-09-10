/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 2.0 - September 10, 2008                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratory                                 
 *    (C) Copyright 2000-2008 All Rights Reserved                       
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
 * Tennessee, Knoxville, Innovative Computing Laboratory.             
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

#ifdef STDC_HEADERS
void HPL_lmul
(
   int *                            K,
   int *                            J,
   int *                            I
)
#else
void HPL_lmul
( K, J, I )
   int *                            K;
   int *                            J;
   int *                            I;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_lmul multiplies  without carry two long positive integers K and J
 * and put the result into I.  The long integers  I, J, K are encoded on
 * 31 bits using an array of 2 integers. The 16-lower bits are stored in
 * the first entry of each array, the 15-higher bits in the second entry
 * of each array. For efficiency purposes, the  intrisic modulo function
 * is inlined.
 *
 * Arguments
 * =========
 *
 * K       (local input)                 int *
 *         On entry, K is an integer array of dimension 2 containing the
 *         encoded long integer K.
 *
 * J       (local input)                 int *
 *         On entry, J is an integer array of dimension 2 containing the
 *         encoded long integer J.
 *
 * I       (local output)                int *
 *         On entry, I is an integer array of dimension 2. On exit, this
 *         array contains the encoded long integer result.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        r, c;
   unsigned int               kk[4], jj[4], res[5];
/* ..
 * .. Executable Statements ..
 */
/*
 *    K[1] K[0] K  kt = K[0]*J[0]
 *    0XXX XXXX    if(kt < 0) kt += 2^31
 * x               I[0] = kt % 2^16
 *                 lt = K[0]*J[1] + K[1]*J[0]
 *    J[1] J[0] J  if(lt < 0) lt += 2^31
 *    0XXX XXXX    kt = (kt / 2^16) + lt
 * --------------  if(kt < 0) kt += 2^31
 *    I[1] I[0]    I[1] = kt % 2^15
 *    0XXX XXXX I
 */
   for (c = 0; c < 2; ++c) {
     kk[2*c] = K[c] & 65535;
     kk[2*c+1] = ((unsigned)K[c] >> 16) & 65535;
     jj[2*c] = J[c] & 65535;
     jj[2*c+1] = ((unsigned)J[c] >> 16) & 65535;
   }

   res[0] = 0;
   for (c = 0; c < 4; ++c) {
     res[c+1] = (res[c] >> 16) & 65535;
     res[c] &= 65535;
     for (r = 0; r < c+1; ++r) {
       res[c] = kk[r] * jj[c-r] + (res[c] & 65535);
       res[c+1] += (res[c] >> 16) & 65535;
     }
   }

   for (c = 0; c < 2; ++c)
     I[c] = (int)(((res[2*c+1] & 65535) << 16) | (res[2*c] & 65535));
/*
 * End of HPL_lmul
 */
}
