#ifndef _MATH_3D_H_
#define _MATH_3D_H_

#include <math.h>

// For fixed point arithmetic
#include <inttypes.h>

#include <SFML/Graphics.h>

#include "config.h"

// Do not include this here
// #include "stb_ds.h"

// 
// Config in "config.h".
// 

/*
 TODO:
    - add "destructors" to free for example meshes' dynamic arrays
    - tests of everything XD

 */

#ifdef USE_FIXED_POINT_ARITHMETIC
fxp_t floatingToFixed( flp_t num );
flp_t fixedToFloating( fxp_t num );
fxp_t fixedMul( fxp_t a, fxp_t b );
fxp_t fixedDiv( fxp_t a, fxp_t b );
#endif

// Matrix 4x4:
typedef struct {
	rtnl_t m[4][4];// = { 0 };	// rows, columns
} mat4x4_t;

// Vector 3D structure:
typedef struct {
	rtnl_t x;// = 0;
    rtnl_t y;// = 0;
    rtnl_t z;// = 0;
    rtnl_t w;// = 1;
} vec3d_t;

// Polygon holds only IDs of its vertices
typedef struct {
    // Vector of vertex IDs
    // vector<int> p;
    int* p;
    // Number of vertices;
    // ( faster access than arrlen() I guess )
	int p_count;
} polygon_t;

void vec3d_makeEmpty( vec3d_t* v );

void vec3d_print( vec3d_t* v, int new_line_flag );

#ifdef USE_FIXED_POINT_ARITHMETIC
void vec3d_printAsFixed( vec3d_t* v, int new_line_flag );
#endif

polygon_t polygonMakeEmpty( void );
void polygon_print( polygon_t* poly );
void polygon_free( polygon_t* poly );

void printVec3D( vec3d_t* v, char* name );
void printMatrix( mat4x4_t* mat );

// min() is built in for C++ I guess but to be sure I made it myself
//#define min( a, b ) (a>b ? a : b)
rtnl_t minF( rtnl_t a, rtnl_t b );
rtnl_t maxF( rtnl_t a, rtnl_t b );

// Add:
vec3d_t vectorAdd( vec3d_t* v1, vec3d_t* v2 );
// Subtract:
vec3d_t vectorSub( vec3d_t* v1, vec3d_t* v2 );
// Multiply:
vec3d_t vectorMul( vec3d_t* v, rtnl_t k );
// Divide:
vec3d_t vectorDiv( vec3d_t* v, rtnl_t k );
// Dot product:
rtnl_t vectorDotProduct( vec3d_t* v1, vec3d_t* v2 );
// Cross product:
vec3d_t vectorCrossProduct( vec3d_t* v1, vec3d_t* v2 );
// Length:
rtnl_t vectorLength( vec3d_t* v );
// Normalise:
vec3d_t vectorNormalise( vec3d_t* v );

// Matrix operator functions:
// Multiply matrix by vector ( m - input matrix, i - input vector ):
vec3d_t matrix_mulVector( mat4x4_t* m, vec3d_t* i );

mat4x4_t matrix_makeEmpty();

mat4x4_t matrix_makeIdentity();

mat4x4_t matrix_makeRotZ( rtnl_t fAngleRad );

mat4x4_t matrix_makeRotX( rtnl_t fAngleRad );

#ifdef USE_CAMERA
mat4x4_t matrix_makeRotY(rtnl_t fAngleRad);
#endif

mat4x4_t matrix_makeTranslation( rtnl_t x, rtnl_t y, rtnl_t z );

mat4x4_t matrix_makeProjection( rtnl_t fFovDegrees, rtnl_t fAspectRatio, rtnl_t fNear, rtnl_t fFar );

mat4x4_t matrix_mulMatrix( mat4x4_t* m1, mat4x4_t* m2 );

#ifdef USE_CAMERA
//  pos - where the object should be
//  target - "forward" vector for that object
//  up - "up" vector
mat4x4_t matrix_pointAt( vec3d_t* pos, vec3d_t* target, vec3d_t* up );

// Works only for Rotation/Translation Matrices
mat4x4_t matrix_quickInverse(mat4x4_t* m);
#endif

// Move these to a some other file, like hidden line related one:
// bool areCollinear( vec3d_t* v1, vec3d_t* v2, vec3d_t* v3, bool object_num, sfRenderWindow* windowToDrawOnto );
// void computePlaneCoefficients( float* A, float* B, float* C, float* D, vec3d_t* vert1, vec3d_t* vert2, vec3d_t* vert3 );

#endif /* _MATH_3D_H_ */