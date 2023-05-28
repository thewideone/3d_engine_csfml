#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <inttypes.h>   // for int32_t etc.

// 
// Info
// 
// See "Hardware dependend stuff" section
// STB_DS_IMPLEMENTATION defined in math_3d.c



// 
// Hardware dependend stuff
// 
// For application-specific functions, see external_dependencies.h

// For CSFML:
#define FRAMERATE 30



// 
// Engine features:
// 

// Camera control defined in external_dependencies.h
#define USE_CAMERA

// #define USE_FILLED_MESHES

// #define USE_LOADING_FROM_OBJ

// Render only visible edges / faces
#define RENDER_VISIBLE_ONLY

// #define REMOVE_HIDDEN_LINES

// Draw only outlines of meshes
// #define DRAW_CONTOUR_ONLY



// 
// Display:
// 
#define SCREEN_WIDTH 240//390//256
#define SCREEN_HEIGHT 240//390//240
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
            // |-|R|-|G|-|B|-|-|
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
// Math:
// 
#define USE_FIXED_POINT_ARITHMETIC

#define true 1
#define false 0
#define bool int

// Improper typedef (like unsigned int) causes a dramatic crash
// #ifndef _SIZE_T_DEFINED 
// #define _SIZE_T_DEFINED
// typedef unsigned long long size_t;
// #endif

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
// For debug:
#define STDO_RTNL STDO_INT32
#else
// Rational number type (flp_t for floating point representation)
#define rtnl_t flp_t
// For debug:
#define STDO_RTNL STDO_FLT
#endif



// 
// Mesh queue (mesh_3d_queue.*):
// 
#define MESH_QUEUE_FIXED_SIZE
#define MESH_QUEUE_CAPACITY 10



// 
// Debug:
// 

// #define VERTEX_ID_DEBUG
// #define VERTEX_DOT_DEBUG

// Simple default text output function (for string, char, int, float, etc)
// For places where not-debug text output is the purpose of a function
// Usage:
//  STDO_STR( "yo mama obama" );
//  STDO_CHR( 'F' );
//  STDO_INT( 123 );
//  STDO_FLT( 12.34 );
#define STDO_STR(x) printf(x)
#define STDO_CHR(x) printf("%c", x)
#define STDO_INT(x) printf("%d", x)
#define STDO_FLT(x) printf("%f", x)

#define STDO_INT8   STDO_INT
#define STDO_UINT8  STDO_INT
#define STDO_UINT16 STDO_INT
#define STDO_INT16  STDO_INT
#define STDO_UINT32 STDO_INT
#define STDO_INT32  STDO_INT
// For size_t:
#define STDO_UINT64(x) printf("%lld", x)
#define STDO_INT64  STDO_INT

#define STDO_SIZET STDO_UINT64

#define DEBUG 3
// Usage:
//  DEBUG_PRINT( "Setup complete %d\n", (int) 123 );

#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#endif /* _CONFIG_H_ */