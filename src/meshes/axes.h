#ifndef _MESH_AXES_H_
#define _MESH_AXES_H_
// 
// Generated by obj file to C array converter by Szymon Kajda.
// Mesh name: "axes_2"
// Settings:
// 	Vertex coordinates representation: fixed point
// 	Fixed point type: "int32_t"
// 	Number of binary digits after the decimal place: 16
// 	Face array type: size_t
// 	Fixed number of vertices in faces: no
// 	Vertex ID separator in face array: F_VID_SEP
// 

#include "../3d_engine/mesh3d.h"    // for F_VID_SEP
#include "../3d_engine/math_3d.h"   // for fxp_t and flp_t
#include "../3d_engine/config.h"    // for fixed point arithmetic and
                                    // optional size_t definitions

#define AXES_MESH_V_CNT 102
#define AXES_MESH_F_CNT 121

extern const rtnl_t axes_mesh_verts[];
extern const size_t axes_mesh_faces[];

#endif // _MESH_AXES_H_