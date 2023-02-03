#ifndef _SPHERE_MESH_H_
#define _SPHERE_MESH_H_

#include "../3d_engine/mesh_3d.h"   // for F_VID_SEP
#include "../3d_engine/math_3d.h"   // for fxp_t

#define SPHERE_MESH_V_CNT 20
#define SPHERE_MESH_F_CNT 24

extern const fxp_t sphere_mesh_verts[];
extern const size_t sphere_mesh_faces[];

#endif // _SPHERE_MESH_H_