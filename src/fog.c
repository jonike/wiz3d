#include "color.h"
#include "fog.h"

static bool_t _fog_enabled = FALSE;
static int _fog_color = _COLOR_DARKGRAY;
static float _fog_mindistance = 0;
static float _fog_finish = 1000;

void fog_setenabled(bool_t enable)
{
  _fog_enabled = enable;
}

bool_t fog_enabled()
{
  return _fog_enabled;
}

void fog_setcolor(int color)
{
  _fog_color = color;
}

int fog_color()
{
  return _fog_color;
}

void fog_setmindistance(float distance)
{
  _fog_mindistance = distance;
}

float fog_mindistance()
{
  return _fog_mindistance;
}

void fog_setmaxdistance(float distance)
{
  _fog_finish = distance;
}

float fog_maxdistance()
{
  return _fog_finish;
}
