#ifndef __MCELIECE_PARAMS_H
#define __MCELIECE_PARAMS_H

#define GFBITS 13
#define SYS_N 4608
#define SYS_T 96

#define COND_BYTES ((1 << (GFBITS-4))*(2*GFBITS - 1))
#define IRR_BYTES (SYS_T * 2)

#define PK_NROWS (SYS_T*GFBITS) 
#define PK_NCOLS (SYS_N - PK_NROWS)
#define PK_ROW_BYTES ((PK_NCOLS + 7)/8)

#define SK_BYTES (SYS_N/8 + IRR_BYTES + COND_BYTES)
#define SYND_BYTES ((PK_NROWS + 7)/8)

#define GFMASK ((1 << GFBITS) - 1)

#endif

