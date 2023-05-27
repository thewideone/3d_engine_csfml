#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML/Graphics.h>

#include "3d_engine/config.h"
#include "3d_engine/math_3d.h"
#include "3d_engine/mesh_3d.h"

// #include <vector>
#include <math.h>
// #include <strstream>
// #include <sstream>
#include <string.h>   // for text

// for random stuff:
#include <stdlib.h>     /* srand, rand */

int SFML_initGraphics( sfRenderWindow* render_window );
void SFML_freeGraphics( void );

void CSFML_putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

void CSFML_drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

#endif /* _GRAPHICS_H_ */