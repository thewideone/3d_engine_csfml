#ifndef _CUBE_MESH_H_
#define _CUBE_MESH_H_

#include "../3d_engine/mesh_3d.h"   // for F_VID_SEP
#include "../3d_engine/math_3d.h"   // for fxp_t and flp_t
#include "../3d_engine/config.h"    // for fixed point arithmetic and
                                    // optional size_t definitions

#define CUBE_MESH_V_CNT 8
#define CUBE_MESH_F_CNT 6

#ifdef USE_FIXED_POINT_ARITHMETIC
extern const fxp_t cube_mesh_verts[];
#else
extern const flp_t cube_mesh_verts[];
#endif

extern const size_t cube_mesh_faces[];

#endif // _CUBE_MESH_H_