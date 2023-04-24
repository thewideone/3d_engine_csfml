#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML/Graphics.h>

#include "config.h"
#include "math_3d.h"
#include "mesh_3d.h"

// #include <vector>
#include <math.h>
// #include <strstream>
// #include <sstream>
#include <string.h>   // for text

// for random stuff:
#include <stdlib.h>     /* srand, rand */

int initGraphics( sfRenderWindow* render_window );
void freeGraphics( void );

int putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

void drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

vec3d_t findCentre( mesh_t* mesh, int face_ID );
int getOutlineEdgeCount( mesh_t* mesh );
// void paintPolygonContours( mesh& mesh, int face_ID, sf::Color colour, sf::RenderWindow &renderWindow );

#endif /* _GRAPHICS_H_ */