#include "external_dependencies.h"

static void (*drawLine_ptr) ( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

static void (*putText_ptr) ( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            );

void engine3D_register_drawLine( void (*function_ptr) ){
    drawLine_ptr = function_ptr;
}

void engine3D_register_putText( void (*function_ptr) ){
    putText_ptr = function_ptr;
}

void putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    putText_ptr( str, x, y, size
#ifndef COLOUR_MONOCHROME
            , colour
#endif
               );
}

void drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    drawLine_ptr( x0, y0, x1, y1
#ifndef COLOUR_MONOCHROME
            , colour
#endif
                );
}