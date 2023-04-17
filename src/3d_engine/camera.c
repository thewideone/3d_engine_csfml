#include "camera.h"

camera_t* cam_active_ptr = NULL;

// 
// Set camera members to default values, that is:
// camera position  = { 0, 0, 0, 1 },
// look direction   = { 0, 0, 0, 1 },
// camera yaw       = 0
// 
void camera_makeDefault( camera_t* cam ){
	#ifdef USE_FIXED_POINT_ARITHMETIC
	#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
		cam->pos.x = floatingToFixed(0);
		cam->pos.y = floatingToFixed(0);
		cam->pos.z = floatingToFixed(0);
		cam->pos.w = floatingToFixed(1);
	#endif
	#ifdef USE_CAMERA
		cam->look_dir.x = floatingToFixed(0);
		cam->look_dir.y = floatingToFixed(0);
		cam->look_dir.z = floatingToFixed(0);
		cam->look_dir.w = floatingToFixed(1);
		cam->yaw = floatingToFixed(0);
	#endif
	
	#else
	#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
		cam->pos.x = 0;
		cam->pos.y = 0;
		cam->pos.z = 0;
		cam->pos.w = 1;
	#endif
	#ifdef USE_CAMERA
		cam->look_dir.x = 0;
		cam->look_dir.y = 0;
		cam->look_dir.z = 0;
		cam->look_dir.w = 1;
		cam->yaw = 0;
	#endif
	
	#endif
}

// 
// Return a pointer to the active camera
// or NULL if there is none.
// 
camera_t* camera_getActive( void ){
    return cam_active_ptr;
}

// 
// Set active camera pointer.
// 
void camera_setActive( camera_t* cam ){
    cam_active_ptr = cam;
}