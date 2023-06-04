#ifndef _DODECAHEDRON_MESH_H_
#define _DODECAHEDRON_MESH_H_

#include "../3d_engine/mesh3d.h"   // for F_VID_SEP
#include "../3d_engine/math_3d.h"   // for fxp_t

#define DODECAHEDRON_MESH_V_CNT 20
#define DODECAHEDRON_MESH_F_CNT 12

extern const fxp_t dodecahedron_mesh_verts[];
extern const size_t dodecahedron_mesh_faces[];

#endif // _DODECAHEDRON_MESH_H_