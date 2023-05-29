#ifndef _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_
#define _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_

// 
// To make this engine universal,
// this and *.c files contain all dependencies
// on external features like drawing a line on a screen
// or camera control, so that the engine can be used on
// any hardware.
// 

#include "config.h"     // for rtnl_t
#include "math_3d.h"    // for colour_t

#ifdef USE_CAMERA
// Camera control functions.
// Sensitive only to constant key press.
// sf* - functions of SFML library
#define CAMERA_MOVE_PRESSED_UP sfKeyboard_isKeyPressed(sfKeySpace)
#define CAMERA_MOVE_PRESSED_DOWN sfKeyboard_isKeyPressed(sfKeyLShift) || sfKeyboard_isKeyPressed(sfKeyRShift)
#define CAMERA_MOVE_PRESSED_LEFT sfKeyboard_isKeyPressed(sfKeyA)
#define CAMERA_MOVE_PRESSED_RIGHT sfKeyboard_isKeyPressed(sfKeyD)
#define CAMERA_MOVE_PRESSED_FORWARD sfKeyboard_isKeyPressed(sfKeyW)
#define CAMERA_MOVE_PRESSED_BACKWARD sfKeyboard_isKeyPressed(sfKeyS)

#define CAMERA_LOOK_PRESSED_UP sfKeyboard_isKeyPressed(sfKeyUp)
#define CAMERA_LOOK_PRESSED_DOWN sfKeyboard_isKeyPressed(sfKeyDown)
#define CAMERA_LOOK_PRESSED_LEFT sfKeyboard_isKeyPressed(sfKeyLeft)
#define CAMERA_LOOK_PRESSED_RIGHT sfKeyboard_isKeyPressed(sfKeyRight)
#define CAMERA_LOOK_PRESSED_ROLL_LEFT sfKeyboard_isKeyPressed(sfKeyComma)
#define CAMERA_LOOK_PRESSED_ROLL_RIGHT sfKeyboard_isKeyPressed(sfKeyPeriod)

#endif // USE_CAMERA

void engine3D_register_drawLine( void *function_ptr );
void engine3D_register_putText( void *function_ptr );

// 
// Just for debug.
// Should not be used as regular text-drawing function,
// because that one should be provided by a display library used.
// 
void engine3D_putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
                      , colour_t* colour
#endif
                      );

// 
// Draw a line on the screen.
// Used by engine3D_drawMesh().
// 
void engine3D_drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
                       , colour_t* colour
#endif
                      );


#endif // _3D_ENGINE_EXTERNAL_DEPENDENCIES_H_