#ifndef _CONFIG_H_
#define _CONFIG_H_

// 
// Debug:
// 
// #define VERTEX_ID_DEBUG
// #define VERTEX_DOT_DEBUG
// Draw only outlines of meshes
// #define DRAW_CONTOUR_ONLY

// 
// Display:
// 
#define SCREEN_WIDTH 390//256
#define SCREEN_HEIGHT 390//240
// #define COLOUR_MONOCHROME

#ifndef COLOUR_MONOCHROME
    // Number of bits per R/G/B channel
    #define COLOUR_DEPTH 1
    #if COLOUR_DEPTH <= 2
        #define COLOUR_SINGLE_BYTE
        #define COLOUR_WHITE   0xff
        #define COLOUR_BLACK   0x00
        #if COLOUR_DEPTH == 1
            //  _______________
            // |0|R|0|G|0|B|-|-|
            // 
            #define COLOUR_RED     0b01000000
            #define COLOUR_GREEN   0b00010000
            #define COLOUR_BLUE    0b00000100
        #else
            //  _______________
            // |R|R|G|G|B|B|-|-|
            // 
            #define COLOUR_RED     0b11000000
            #define COLOUR_GREEN   0b00110000
            #define COLOUR_BLUE    0b00001100
        #endif

        #define COLOUR_CYAN    (COLOUR_GREEN | COLOUR_BLUE)
        #define COLOUR_MAGENTA (COLOUR_RED   | COLOUR_BLUE)
        #define COLOUR_YELLOW  (COLOUR_RED   | COLOUR_GREEN)
    #endif

#endif  // COLOUR_MONOCHROME

// 
// Engine features:
// 
#define USE_CAMERA
#define USE_FILLED_MESHES
// #define USE_LOADING_FROM_OBJ
// Render only visible edges / faces
#define RENDER_VISIBLE_ONLY
// #define REMOVE_HIDDEN_LINES

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