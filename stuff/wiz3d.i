%module wiz3d
%{
  #include "wiz3d.h"
%}

%import "types.i"

%rename("%(regex:/(\\w+)_(.*)/\\2/)s", %$isfunction) "";

%include "src/camera.h"
%include "src/color.h"
%include "src/image.h"
%include "src/input.h"
%include "src/light.h"
%include "src/log.h"
%include "src/object.h"
%include "src/screen.h"
