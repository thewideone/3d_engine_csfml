#include "csfml_graphics.h"

#include <assert.h>
#include <stdio.h>

#include "3d_engine/stb_ds.h"

#include "3d_engine/math_3d.h"
#include "3d_engine/vmap/vmap.h"

sfFont* font;
sfRenderWindow* rend_win_global;

int SFML_initGraphics( sfRenderWindow* render_window ){
    rend_win_global = render_window;

    font = sfFont_createFromFile("Minecraft.ttf");
    if ( !font ){
		DEBUG_PRINT( "Error: Could not load a font.\n" );
        return 1;
	}
    return 0;
}

void SFML_freeGraphics( void ){
    sfFont_destroy(font);
}

void CSFML_putText( char* str, rtnl_t x, rtnl_t y, int size
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    sfColor txt_colour;

#ifdef COLOUR_MONOCHROME
    txt_colour = sfWhite;
#else
#ifdef COLOUR_SINGLE_BYTE
#if COLOUR_DEPTH == 1
    uint8_t r = ((colour->rgb) >> 6) & 0b0000001;
    uint8_t g = ((colour->rgb) >> 4) & 0b0000001;
    uint8_t b = ((colour->rgb) >> 2) & 0b0000001;
    txt_colour = sfColor_fromRGB( r*255, g*255, b*255 );
#else   // COLOUR_DEPTH != 1
    uint8_t r = ((colour->rgb) >> 6) & 0b0000011;
    uint8_t g = ((colour->rgb) >> 4) & 0b0000011;
    uint8_t b = ((colour->rgb) >> 2) & 0b0000011;

    txt_colour = sfColor_fromRGB( r*(255/3), g*(255/3), b*(255/3) );
#endif  // COLOUR_DEPTH
#else   // colour not single byte
    txt_colour = sfColor_fromRGB( colour->r, colour->g, colour->b );
#endif
#endif

    sfText* text = sfText_create();
    sfText_setString( text, str );
    sfText_setFont( text, font );
    sfText_setCharacterSize( text, size );

#ifdef USE_FIXED_POINT_ARITHMETIC
    sfVector2f pos = { fixedToFloating(x), fixedToFloating(y) };
#else
    sfVector2f pos = { x, y };
#endif
    sfText_setPosition( text, pos );

    sfText_setColor( text, txt_colour );

    sfRenderWindow_drawText( rend_win_global, text, NULL );

    sfText_destroy(text);
}

void CSFML_drawLine( rtnl_t x0, rtnl_t y0, rtnl_t x1, rtnl_t y1
#ifndef COLOUR_MONOCHROME
            , colour_t* colour
#endif
            ){
    sfVertexArray* vert_array;
    vert_array = sfVertexArray_create();

    sfVertexArray_setPrimitiveType( vert_array, sfLines );

    sfVertex v1, v2;
#ifdef USE_FIXED_POINT_ARITHMETIC
    v1.position.x = fixedToFloating(x0);
    v1.position.y = fixedToFloating(y0);
    v2.position.x = fixedToFloating(x1);
    v2.position.y = fixedToFloating(y1);
#else
    v1.position.x = x0;
    v1.position.y = y0;
    v2.position.x = x1;
    v2.position.y = y1;
#endif

#ifdef COLOUR_MONOCHROME
    v1.color = sfWhite;
    v2.color = sfWhite;
#else
#ifdef COLOUR_SINGLE_BYTE
#if COLOUR_DEPTH == 1
    uint8_t r = ((colour->rgb) >> 6) & 0b0000001;
    uint8_t g = ((colour->rgb) >> 4) & 0b0000001;
    uint8_t b = ((colour->rgb) >> 2) & 0b0000001;
    v1.color = sfColor_fromRGB( r*255, g*255, b*255 );
    v2.color = sfColor_fromRGB( r*255, g*255, b*255 );
#else   // COLOUR_DEPTH != 1
    uint8_t r = ((colour->rgb) >> 6) & 0b0000011;
    uint8_t g = ((colour->rgb) >> 4) & 0b0000011;
    uint8_t b = ((colour->rgb) >> 2) & 0b0000011;

    v1.color = sfColor_fromRGB( r*(255/3), g*(255/3), b*(255/3) );
    v2.color = sfColor_fromRGB( r*(255/3), g*(255/3), b*(255/3) );
#endif  // COLOUR_DEPTH
#else   // colour not single byte
    v1.color = sfColor_fromRGB( colour->r, colour->g, colour->b );
    v2.color = sfColor_fromRGB( colour->r, colour->g, colour->b );
#endif
#endif
    // v1.color = colour;
    // v2.color = colour;
    sfVertexArray_append( vert_array, v1 );
    sfVertexArray_append( vert_array, v2 );

    sfRenderWindow_drawVertexArray( rend_win_global, vert_array, NULL );
}