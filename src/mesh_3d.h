#ifndef _MESH_3D_H_
#define _MESH_3D_H_

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
//  - add loading from program memory

// Mesh:
// Vertex and face count limits are the limits of
// types of vertex_cnt and face_cnt variables
// Same for polygon struct
typedef struct {
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


// Idk if it makes sense because a mesh should be loaded
// with info just after its declaration
mesh_t mesh_makeEmpty();
void mesh_free( mesh_t* mesh );
bool mesh_loadFromObjFile( mesh_t* mesh, char* filename );
void mesh_printVisFaceIDs( mesh_t* mesh );
void mesh_printVisEdgeVec( mesh_t* mesh );

void calcFaceBoundaries( mesh_t* mesh, int face_id, int* min_x, int* max_x, int* min_y, int* max_y );

#endif /* _MESH_3D_H_ */