#ifndef _MATH_3D_H_
#define _MATH_3D_H_

#include <SFML/Graphics.h>
#include <math.h>
#include "config.h"

// Do not include this here
// #include "stb_ds.h"

/*
 TODO:
    - add "destructors" to free for example meshes' dynamic arrays
    - tests of everything XD

 */

#define true 1
#define false 0

#define bool int

// Matrix 4x4:
typedef struct {
	float m[4][4];// = { 0 };	// rows, columns
} mat4x4_t;

// Vector 3D structure:
typedef struct {
	float x;// = 0;
    float y;// = 0;
    float z;// = 0;
    float w;// = 1;
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

// Binary tree map for storing 3D vectors and their visibility flags
// vmap is short for "vector 3D map"
typedef struct vmap_t {
    int key;    // used as vector's ID
    vec3d_t v;
#ifdef REMOVE_HIDDEN_LINES
    bool visible;    // set if vector v is visible
#endif
    struct vmap_t* left;
    struct vmap_t* right;
} vmap_t;

// Mesh:
// Vertex and face count limits are the limits of
// types of vertex_cnt and face_cnt variables
// Same for polygon struct
typedef struct {
    // vector<polygon> faces;    // std::vector<...>, contains IDs of vertices
    polygon_t* faces;
    // vector<vec3d> vertices;
    vec3d_t* vertices;
    int face_cnt;//=0;
    int vertex_cnt;//=0;

    mat4x4_t matRotZ, matRotX, matRotY;
    mat4x4_t matTrans;
    mat4x4_t matWorld;

    // vector<vec3d> transformedVertices;
    vec3d_t* transformedVertices;
    // vector<int> visFaceIDs;
    int* visFaceIDs;
    // map<int, vec3d> vert2DSpaceMap; // vertex ID, vertex object
    vmap_t* vert2DSpaceMap;
    
    // vis_edge_vec:
    // Each entry is 4x int
    // 1: start_vert_ID
    // 2: end_vert_ID
    // 3: num_of_faces_which_the_edge_belongs_to (1-2)
    // 4: ID_of_1st_face (from visFaceIDs)
    // vector<int> vis_edge_vec;
    int* vis_edge_vec;

// #ifdef REMOVE_HIDDEN_LINES
    // map<int, bool> visVertexMap; // vertex ID, vertex visibility flag
    // vmap_t* visVertexMap;
    // Functionality of the visVertexMap replaced by
    // storing "visible" flag in vmap_t type
// #endif
} mesh_t;

polygon_t polygonMakeEmpty( void );
void polygon_print( polygon_t* poly );
void polygon_free( polygon_t* poly );

// Idk if it makes sense because a mesh should be loaded
// with info just after its declaration
mesh_t mesh_makeEmpty();
void mesh_free( mesh_t* mesh );

bool mesh_loadFromObjFile( mesh_t* mesh, char* filename );
void mesh_printVisFaceIDs( mesh_t* mesh );
void mesh_printVisEdgeVec( mesh_t* mesh );

vmap_t* vmap_createNode( int key, vec3d_t v, int vis_flag );
vmap_t* vmap_search( vmap_t* root, int key );
vmap_t* vmap_insertNode( vmap_t* root, int key, vec3d_t v, int vis_flag );
void vmap_print( vmap_t* root );
void vmap_free( vmap_t* root );

void printVec3D( vec3d_t* v, char* name );
void printMatrix( mat4x4_t* mat );

// min() is built in for C++ I guess but to be sure I made it myself
//#define min( a, b ) (a>b ? a : b)
float minF( float a, float b );
float maxF( float a, float b );

// Add:
vec3d_t vectorAdd( vec3d_t* v1, vec3d_t* v2 );
// Subtract:
vec3d_t vectorSub( vec3d_t* v1, vec3d_t* v2 );
// Multiply:
vec3d_t vectorMul( vec3d_t* v, float k );
// Divide:
vec3d_t vectorDiv( vec3d_t* v, float k );
// Dot product:
float vectorDotProduct( vec3d_t* v1, vec3d_t* v2 );
// Cross product:
vec3d_t vectorCrossProduct( vec3d_t* v1, vec3d_t* v2 );
// Length:
float vectorLength( vec3d_t* v );
// Normalise:
vec3d_t vectorNormalise( vec3d_t* v );

// Matrix operator functions:
// Multiply matrix by vector ( m - input matrix, i - input vector ):
vec3d_t matrixMulVector( mat4x4_t* m, vec3d_t* i );

mat4x4_t matrix_makeEmpty();

mat4x4_t matrixMakeIdentity();

mat4x4_t matrixMakeRotZ( float fAngleRad );

mat4x4_t matrixMakeRotX( float fAngleRad );

#ifdef USE_CAMERA
mat4x4_t matrixMakeRotY(float fAngleRad);
#endif

mat4x4_t matrixMakeTranslation( float x, float y, float z );

mat4x4_t matrixMakeProjection( float fFovDegrees, float fAspectRatio, float fNear, float fFar );

mat4x4_t matrixMulMatrix( mat4x4_t* m1, mat4x4_t* m2 );

#ifdef USE_CAMERA
//  pos - where the object should be
//  target - "forward" vector for that object
//  up - "up" vector
mat4x4_t matrixPointAt( vec3d_t* pos, vec3d_t* target, vec3d_t* up );

// Works only for Rotation/Translation Matrices
mat4x4_t matrixQuickInverse(mat4x4_t* m);
#endif

bool areCollinear( vec3d_t* v1, vec3d_t* v2, vec3d_t* v3, bool object_num, sfRenderWindow* windowToDrawOnto );
void calcFaceBoundaries( mesh_t* mesh, int face_id, int* min_x, int* max_x, int* min_y, int* max_y );
void computePlaneCoefficients( float* A, float* B, float* C, float* D, vec3d_t* vert1, vec3d_t* vert2, vec3d_t* vert3 );

#endif /* _MATH_3D_H_ */