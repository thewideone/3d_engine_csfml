#ifndef _CONFIG_H_
#define _CONFIG_H_

// 
// Display:
// 
#define SCREEN_WIDTH 390//256
#define SCREEN_HEIGHT 390//240
#define COLOUR_DEPTH 24 // in bits

// 
// Engine features:
// 
#define USE_CAMERA
#define RENDER_VISIBLE_ONLY     // render only visible edges / faces
// #define REMOVE_HIDDEN_LINES

// 
// Debug:
// 
// #define VERTEX_ID_DEBUG
// #define VERTEX_DOT_DEBUG

// 
// Math:
// 
#define USE_FIXED_POINT_ARITHMETIC

#define true 1
#define false 0
#define bool int

// Fixed point arithmetic:
// Functions taken from javidx9's
// "Back To Basics! Fixed Point Numbers in C++" video

// Floating point number type
#define flp_t float
// Fixed point number type
#define fxp_t int32_t
// Twice-wide fixed point number type
#define fxp2_t int64_t
// Number of binary digits after the decimal place
#define FP_DP 16

#ifdef USE_FIXED_POINT_ARITHMETIC
// Rational number type (fxp_t for fixed point representation)
#define rtnl_t fxp_t
#else
// Rational number type (flp_t for floating point representation)
#define rtnl_t flp_t
#endif

// 
// Mesh queue (mesh_3d_queue.*):
// 
#define MESH_QUEUE_FIXED_SIZE
#define MESH_QUEUE_CAPACITY 10

#endif /* _CONFIG_H_ */