//
// Created by 易国磐 on 15-3-14.
//

#ifndef _NIMO_MEM_H_
#define _NIMO_MEM_H_

#define NM_MALLOC(x) malloc(sizeof(x))
#define NM_CALLOC(z,x) calloc(z, x)
#define NM_FREE(x) if((x)) free(x);(x)=NULL
#define NM_REALLOC(x,z) realloc((x),(z))
#endif //_NIMO_MEM_H_
