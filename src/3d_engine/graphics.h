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

int initGraphics( void );
void freeGraphics( void );
#ifdef USE_FIXED_POINT_ARITHMETIC
int putText( char* str, fxp_t x, fxp_t y, int size, sfColor colour, sfRenderWindow* renderWindow );
void drawLine( fxp_t x0, fxp_t y0, fxp_t x1, fxp_t y1, sfColor colour, sfRenderWindow* renderWindow );
#else
int putText( char* str, float x, float y, int size, sfColor colour, sfRenderWindow* renderWindow );
void drawLine( float x0, float y0, float x1, float y1, sfColor colour, sfRenderWindow* renderWindow );
#endif
vec3d_t findCentre( mesh_t* mesh, int face_ID );
int getOutlineEdgeCount( mesh_t* mesh );
// void paintPolygonContours( mesh& mesh, int face_ID, sf::Color colour, sf::RenderWindow &renderWindow );

#endif /* _GRAPHICS_H_ */