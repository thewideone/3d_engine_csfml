#ifndef _3D_ENGINE_MAIN_H_
#define _3D_ENGINE_MAIN_H_

#include <stdio.h>

#include <SFML/Graphics.h>

#include "config.h"

#include "math_3d.h"
#include "vmap/vmap.h"
#include "mesh_3d.h"

#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
#include "camera.h"
#endif

#include "stb_ds.h"

// #include "graphics.h"
#include "external_dependencies.h"

#include "../meshes/cube.h"
#include "../meshes/sphere.h"
#include "../meshes/dodecahedron.h"

bool getAnimateFlag( void );
void setAnimateFlag( bool value );

void setup3D( void );
void free3D( void );
void draw_mesh( mesh_t* mesh );

#ifdef USE_CAMERA
void processMesh( mesh_t* mesh, mat4x4_t* mat_proj, mat4x4_t* mat_view, flp_t rot_angle_x, flp_t rot_angle_z );
#else
void processMesh( mesh_t* mesh, mat4x4_t* mat_proj, flp_t rot_angle_x, flp_t rot_angle_z );
#endif

void update3DFrame( flp_t f_elapsed_time, flp_t* f_theta );

#endif // _3D_ENGINE_MAIN_H_