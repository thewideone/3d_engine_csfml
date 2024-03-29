#ifndef _MESH3D_H_
#define _MESH3D_H_

#include "config.h"
#include "vmap/vmap.h"

// 
// TODO:
//  - make transformedVertices static (dynamic functionality is not used)
//  - ifndef RENDER_VISIBLE_ONLY make visFaceIDs,
//    vert2DSpaceMap and vis_edge_vec also static,
//    maybe there are more optimization possibilities?
//  - instead of clearing and filling vert2DSpaceMap,
//    update it with new values
//  - X add loading from program memory

// Vertex ID separator in face array in program memory
#define F_VID_SEP 255

// Mesh:
// Vertex and face count limits are the limits of
// types of vertex_cnt and face_cnt variables
// Same for polygon struct
typedef struct {

#ifndef COLOUR_MONOCHROME
    colour_t edge_colour;
#ifdef USE_FILLED_MESHES
    colour_t fill_colour;
#endif
#endif

    // Position vector
    vec3d_t pos;

    // Rotation angles (in radians)
    rtnl_t yaw;
	rtnl_t pitch;
	rtnl_t roll;

    // vector<polygon> faces;    // std::vector<...>, contains IDs of vertices
    polygon_t* faces;
    // vector<vec3d> vertices;
    vec3d_t* vertices;
    size_t face_cnt;//=0;
    size_t vertex_cnt;//=0;

    mat4x4_t matRotZ, matRotX, matRotY;
    mat4x4_t matTrans;
    mat4x4_t matWorld;

    // vector<vec3d> transformedVertices;
    vec3d_t* transformedVertices;
    // vector<int> visFaceIDs;
    size_t* visFaceIDs;
    // map<int, vec3d> vert2DSpaceMap; // vertex ID, vertex object
    vmap_t vert2DSpaceMap;
    
    // vis_edge_vec:
    // Each entry is 4x int
    // 1: start_vert_ID
    // 2: end_vert_ID
    // 3: num_of_faces_which_the_edge_belongs_to (1-2)
    // 4: ID_of_1st_face (from visFaceIDs)
    // vector<int> vis_edge_vec;
    size_t* vis_edge_vec;

// #ifdef REMOVE_HIDDEN_LINES
    // map<int, bool> visVertexMap; // vertex ID, vertex visibility flag
    // vmap_t* visVertexMap;
    // Functionality of the visVertexMap replaced by
    // storing "visible" flag in vmap_t type
// #endif
} mesh3d_t;



void mesh_makeEmpty( mesh3d_t* mesh );
void mesh_free( mesh3d_t* mesh );
bool mesh_loadFromProgmem( mesh3d_t* mesh, const rtnl_t vert_arr[], const size_t face_arr[], const size_t vert_cnt, const size_t face_cnt, const size_t fixed_face_size );
#ifdef USE_LOADING_FROM_OBJ
bool mesh_loadFromObjFile( mesh3d_t* mesh, char* filename );
#endif

void mesh_setPos( mesh3d_t* mesh, rtnl_t x, rtnl_t y, rtnl_t z );
void mesh_setPosByVec( mesh3d_t* mesh, vec3d_t* pos );
void mesh_setRot( mesh3d_t* mesh, rtnl_t yaw, rtnl_t pitch, rtnl_t roll );
void mesh_setRotByVec( mesh3d_t* mesh, vec3d_t* rot );

void mesh_move( mesh3d_t* mesh, rtnl_t x, rtnl_t y, rtnl_t z );
void mesh_moveByVec( mesh3d_t* mesh, vec3d_t* pos );
void mesh_rotate( mesh3d_t* mesh, rtnl_t yaw, rtnl_t pitch, rtnl_t roll );
void mesh_rotateByVec( mesh3d_t* mesh, vec3d_t* rot );

void mesh_printVisFaceIDs( mesh3d_t* mesh );
void mesh_printVisEdgeVec( mesh3d_t* mesh );

#ifndef COLOUR_MONOCHROME
    void mesh_setEdgeColour( mesh3d_t* mesh, colour_t colour );
    #ifdef USE_FILLED_MESHES
        void mesh_setFillColour( mesh3d_t* mesh, colour_t colour );
    #endif
    #ifdef COLOUR_SINGLE_BYTE
        void mesh_setEdgeColourByValue( mesh3d_t* mesh, uint8_t colour );
        #ifdef USE_FILLED_MESHES
            void mesh_setFillColourByValue( mesh3d_t* mesh, uint8_t colour );
        #endif
    #else
        void mesh_setEdgeColourByValue( mesh3d_t* mesh, uint8_t r, uint8_t g, uint8_t b );
        #ifdef USE_FILLED_MESHES
            void mesh_setFillColourByValue( mesh3d_t* mesh, uint8_t r, uint8_t g, uint8_t b );
        #endif
    #endif
#endif


// void calcFaceBoundaries( mesh3d_t* mesh, int face_id, int* min_x, int* max_x, int* min_y, int* max_y );

#endif /* _MESH3D_H_ */