#ifndef FOG_H_INCLUDED
#define FOG_H_INCLUDED

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void fog_setenabled(bool_t enable);
bool_t fog_enabled();
void fog_setcolor(int color);
int fog_color();
void fog_setmindistance(float distance);
float fog_mindistance();
void fog_setmaxdistance(float distance);
float fog_maxdistance();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FOG_H_INCLUDED */
