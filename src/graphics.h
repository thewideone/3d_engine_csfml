#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML/Graphics.h>

#include "math_3d.h"
#include "config.h"

// #include <vector>
#include <math.h>
// #include <strstream>
// #include <sstream>
#include <string.h>   // for text

// for random stuff:
#include <stdlib.h>     /* srand, rand */

int initGraphics( void );
void drawLine( float x0, float y0, float x1, float y1, sfColor colour, sfRenderWindow* windowToDrawOnto );
int putText( char* str, float x, float y, int size, sfColor colour, sfRenderWindow* windowToDrawOnto );
vec3d_t findCentre( mesh_t* mesh, int face_ID );
int getOutlineEdgeCount( mesh_t* mesh );
// void paintPolygonContours( mesh& mesh, int face_ID, sf::Color colour, sf::RenderWindow &windowToDrawOnto );

#endif /* _GRAPHICS_H_ */