#ifndef _3D_CAMERA_H_
#define _3D_CAMERA_H_

#include "config.h"

#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)

#include "math_3d.h"

typedef struct {
	vec3d_t pos;        //v_camera;  // only a placeholder now
#ifdef USE_CAMERA
	vec3d_t look_dir;   //v_look_dir;
	rtnl_t yaw;         //f_yaw;
#endif
} camera_t;

void camera_makeDefault( camera_t* cam );
camera_t* camera_getActive( void );
void camera_setActive( camera_t* cam );

#endif

#endif // _3D_CAMERA_H_