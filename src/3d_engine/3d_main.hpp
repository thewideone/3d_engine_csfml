#ifndef _3D_ENGINE_MAIN_H_
#define _3D_ENGINE_MAIN_H_

#include <stdio.h>

#include <SFML/Graphics.h>

#include "config.h"

#include "math_3d.h"
#include "vmap/vmap.h"
#include "mesh_3d.h"

#include "stb_ds.h"

#include "graphics.h"

bool getAnimateFlag( void );
void setAnimateFlag( bool value );

void setup3D( void );
void free3D( void );
void draw_mesh( mesh_t* mesh, sfRenderWindow* render_window );
void processMesh( mesh_t* mesh, mat4x4_t* matView, flp_t rot_angle_x, flp_t rot_angle_z );
void update3DFrame( sfRenderWindow* renderWindow, flp_t f_elapsed_time, flp_t* f_theta );

#endif // _3D_ENGINE_MAIN_H_