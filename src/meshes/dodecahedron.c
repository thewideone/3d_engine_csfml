#include "dodecahedron.h"

const fxp_t dodecahedron_mesh_verts[] = {
	-9234, -39114, -28417,
	-29880, -39114, 0,
	24173, -39114, -17563,
	24173, -39114, 17563,
	-9234, -39114, 28417,
	-48347, -9234, 0,
	-14940, -9234, -45980,
	39114, -9234, -28417,
	39114, -9234, 28417,
	-14940, -9234, 45980,
	-39114, 9234, -28417,
	14940, 9234, -45980,
	48347, 9234, 0,
	14940, 9234, 45980,
	-39114, 9234, 28417,
	-24173, 39114, -17563,
	9234, 39114, -28417,
	29880, 39114, 0,
	9234, 39114, 28417,
	-24173, 39114, 17563
};

const size_t dodecahedron_mesh_faces[] = {
	0, 2, 3, 4, 1,	F_VID_SEP,
	0, 1, 5, 10, 6,	F_VID_SEP,
	2, 0, 6, 11, 7,	F_VID_SEP,
	3, 2, 7, 12, 8,	F_VID_SEP,
	5, 1, 4, 9, 14,	F_VID_SEP,
	4, 3, 8, 13, 9,	F_VID_SEP,
	11, 6, 10, 15, 16,	F_VID_SEP,
	10, 5, 14, 19, 15,	F_VID_SEP,
	12, 7, 11, 16, 17,	F_VID_SEP,
	13, 8, 12, 17, 18,	F_VID_SEP,
	16, 15, 19, 18, 17,	F_VID_SEP,
	14, 9, 13, 18, 19,	F_VID_SEP
};