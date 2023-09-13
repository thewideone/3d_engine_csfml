#include "axes.h"

#ifdef USE_FIXED_POINT_ARITHMETIC
const fxp_t axes_man_mesh_verts[] = {
	52429, -13107, 0,
	52429, 13107, 0,
	78643, -13107, 0,
	78643, 13107, 0,
	13107, 52429, 0,
	-13107, 65536, 0,
	13107, 78643, 0,
	-13107, 0, 52429,
	13107, 0, 52429,
	13107, 0, 78643
};
#endif

const size_t axes_man_mesh_faces[] = {
	// 0, 2, 3, 1,	F_VID_SEP,
	// 4, 6, 5,	F_VID_SEP,
	// 8, 9, 7,	F_VID_SEP
	0, 2, 3, 1,     F_VID_SEP,
	0, 1, 3, 2,     F_VID_SEP,
	4, 6, 5,        F_VID_SEP,
	4, 5, 6,        F_VID_SEP,
	8, 9, 7,        F_VID_SEP,
	8, 7, 9,        F_VID_SEP
};