#ifndef _3D_ENGINE_MAIN_H_
#define _3D_ENGINE_MAIN_H_

#include <stdio.h>

#include <SFML/Graphics.h>

#include "config.h"

#include "math_3d.h"
#include "vmap/vmap.h"
#include "mesh3d.h"

#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
#include "camera.h"
#endif

#include "stb_ds.h"

// #include "graphics.h"
#include "external_dependencies.h"

#include "../meshes/cube.h"
#include "../meshes/sphere.h"
#include "../meshes/dodecahedron.h"
#include "mesh3d_queue.h"

void engine3D_setupProjectionMatrix( mat4x4_t* mat );
void engine3D_computeViewMatrix( camera_t* cam, mat4x4_t* mat_view, flp_t f_elapsed_time );

#ifdef USE_CAMERA
void engine3D_processMesh( mesh3d_t* mesh, mat4x4_t* mat_proj, mat4x4_t* mat_view );
#else
void engine3D_processMesh( mesh3d_t* mesh, mat4x4_t* mat_proj );
#endif

void engine3D_drawMesh( mesh3d_t* mesh );

#endif // _3D_ENGINE_MAIN_H_