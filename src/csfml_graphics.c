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

/*
vec3d_t findCentre( mesh_t* mesh, int face_ID ){
    // mesh.printVisFaceIDs();
    // mesh.printVisEdgeVec();
    // cout << "Trying to find centre for face of ID: " << face_ID << "... ";

    vec3d_t vec_avg;
    vec_avg.x=0;
    vec_avg.y=0;
    int vert0_id = mesh->faces[ face_ID ].p[ 0 ];
    assert( vert0_id >= 0 );
    // vec_avg.z=mesh.vert2DSpaceMap.find( vert0_id )->second.z;    // just take these values
    // vec_avg.w=mesh.vert2DSpaceMap.find( vert0_id )->second.w;    // from the 1st element
    vmap_t* found_node_ptr = vmap_search( mesh->vert2DSpaceMap, vert0_id );
    if( found_node_ptr != NULL ){
        vec_avg.x = found_node_ptr->v.x;
        vec_avg.w = found_node_ptr->v.w;
    }

    assert( face_ID >= 0 );

    int vertex_cnt = mesh->faces[face_ID].p_count;

    assert( vertex_cnt > 0 );

    for( int i=0; i < vertex_cnt; i++ ){
        int vert_id = mesh->faces[ face_ID ].p[ i ];

        assert( vert_id >= 0 );

        // vec_avg.x += mesh.vert2DSpaceMap.find( vert_id )->second.x;
        // vec_avg.y += mesh.vert2DSpaceMap.find( vert_id )->second.y;

        // float res_x = mesh.vert2DSpaceMap.find( vert_id )->second.x;
        // float res_y = mesh.vert2DSpaceMap.find( vert_id )->second.y;
        float res_x, res_y;

        found_node_ptr = vmap_search( mesh->vert2DSpaceMap, vert0_id );
        if( found_node_ptr != NULL ){
            res_x = found_node_ptr->v.x;
            res_y = found_node_ptr->v.y;
        }

        // assert( (res_x >= 0.0) && (res_y >= 0.0) );

        vec_avg.x += res_x;
        vec_avg.y += res_y;

    }
    vec_avg.x /= (float) vertex_cnt;
    vec_avg.y /= (float) vertex_cnt;

    // cout << "done." << endl;

    return vec_avg;
}
*/

int getOutlineEdgeCount( mesh_t* mesh ){
    // In vis_edge_vec:
    // Each entry is 4x int:
    // 0: start_vert_ID
    // 1: end_vert_ID
    // 2: num_of_faces_which_the_edge_belongs_to (1-2)
    // 3: ID_of_1st_face

    int count = 0;

    for( int edge_itr = 0;
            edge_itr < arrlen(mesh->vis_edge_vec);
            edge_itr += 4 ){
        if( mesh->vis_edge_vec[ edge_itr + 2 ] > 1 )
            continue;
        count++;
    }

    return count;
}

// NOT WORKING
// void paintPolygonContours( mesh& mesh, int face_ID, sf::Color colour, sf::RenderWindow &windowToDrawOnto ){
//     int vertex_cnt = mesh.faces[face_ID].p_count;
//     for( int i=0; i < vertex_cnt - 1; i++ ){
//         float x0 = mesh.vertices[mesh.faces[face_ID].p[i]].x;
//         float y0 = mesh.vertices[mesh.faces[face_ID].p[i]].y;
//         float x1 = mesh.vertices[mesh.faces[face_ID].p[i+1]].x;
//         float y1 = mesh.vertices[mesh.faces[face_ID].p[i+1]].y;
//         drawLine( x0 , y0, x1, y1, colour, windowToDrawOnto );
//     }

//     float x0 = mesh.vertices[mesh.faces[face_ID].p[vertex_cnt]].x;
//     float y0 = mesh.vertices[mesh.faces[face_ID].p[vertex_cnt]].y;
//     float x1 = mesh.vertices[mesh.faces[face_ID].p[0]].x;
//     float y1 = mesh.vertices[mesh.faces[face_ID].p[0]].y;
//     drawLine( x0 , y0, x1, y1, colour, windowToDrawOnto );
// }