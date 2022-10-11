#ifndef _MATH_H_
#define _MATH_H_

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

/*
 TODO:
    - add type-initiators like polygon_t poly = createPolygon(); ?

 */

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
    // ( faster access than vector.size() I guess )
	int p_count;
} polygon_t;

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
    // #warning "Add map type"
    
    // vis_edge_vec:
    // Each entry is 4x int
    // 1: start_vert_ID
    // 2: end_vert_ID
    // 3: num_of_faces_which_the_edge_belongs_to (1-2)
    // 4: ID_of_1st_face (from visFaceIDs)
    // vector<int> vis_edge_vec;
    int* vis_edge_vec;

#ifdef REMOVE_HIDDEN_LINES
    map<int, bool> visVertexMap; // vertex ID, vertex visibility flag
#endif
} mesh_t;

#endif /* _MATH_H_ */