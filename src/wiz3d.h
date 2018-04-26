#ifndef WIZ3D_H_INCLUDED
#define WIZ3D_H_INCLUDED

#include "camera.h"
#include "color.h"
#include "image.h"
#include "input.h"
#include "light.h"
#include "log.h"
#include "object.h"
#include "physics.h"
#include "screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool_t wiz3d_init();
void wiz3d_finish();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WIZ3D_H_INCLUDED */
