#ifndef _CUBE_MESH_H_
#define _CUBE_MESH_H_

#include "../3d_engine/mesh_3d.h"   // for F_VID_SEP
#include "../3d_engine/math_3d.h"   // for fxp_t and flp_t

#define CUBE_MESH_V_CNT 8
#define CUBE_MESH_F_CNT 6

extern const fxp_t cube_mesh_verts[];
// extern const flp_t cube_mesh_verts[];
extern const size_t cube_mesh_faces[];

#endif // _CUBE_MESH_H_