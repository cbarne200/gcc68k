/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vpcmpgtq\[ \\t\]+\[^\n\]*%zmm\[0-9\]\[^\n^k\]*%k\[0-9\]\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpcmpgtq\[ \\t\]+\[^\n\]*%zmm\[0-9\]\[^\n^k\]*%k\[0-9\]\{" 1 } } */

#include <immintrin.h>

volatile __m512i x;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  m = _mm512_cmpgt_epi64_mask (x, x);
  m = _mm512_mask_cmpgt_epi64_mask (3, x, x);
}
