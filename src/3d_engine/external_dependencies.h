#ifndef _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_
#define _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_

// 
// To make this engine universal,
// this and *.c files contain all dependencies
// on external features like drawing a line on a screen,
// so that the engine can be used with any display.
// 

#include "config.h"     // for rtnl_t
#include "math_3d.h"    // for colour_t

void engine3D_register_drawLine( void *function_ptr );
void engine3D_register_putText( void *function_ptr );

void putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

void drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );


#endif // _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_