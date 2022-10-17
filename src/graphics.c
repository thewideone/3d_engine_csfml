#include "graphics.h"

#include <assert.h>
#include <stdio.h>

#include "stb_ds.h"

int putText( char* str, float x, float y, int size, sfColor colour, sfRenderWindow* windowToDrawOnto ){
    // sf::Font font;
    // if (!font.loadFromFile("Minecraft.ttf"))
    //     cout<<"Failed to load Minecraft.ttf\n";
    // //font.setSmooth(false);
    // sf::Text text;
    // text.setFont(font);
    // text.setString(ss.str());
    // // set the character size
    // text.setCharacterSize(size); // in pixels, not points!
    // // set the color
    // text.setFillColor( colour );
    // text.setPosition( x, y );
    // windowToDrawOnto.draw(text);

    sfFont* font = sfFont_createFromFile("Minecraft.ttf");
    if ( !font ){
		printf( "Error: Could not load a font.\n" );
        return 1;
	}

    sfText* text = sfText_create();
    sfText_setString( text, str );
    sfText_setFont( text, font );
    sfText_setCharacterSize( text, size );

    sfVector2f pos = { x, y };
    sfText_setPosition( text, pos );

    sfText_setColor( text, colour );

    sfRenderWindow_drawText( windowToDrawOnto, text, NULL );

    return 0;
}

void drawLine( float x0, float y0, float x1, float y1, sfColor colour, sfRenderWindow* renderWindow ){
    // sfVertexArray line[] = {
    //     sfVertex(sfVector2f(x0, y0), colour ),
    //     sfVertex(sfVector2f(x1, y1), colour )
    // };
    // sfRenderWindow_drawLine();
    // windowToDrawOnto.draw(line, 2, sfLines);
    sfRenderStates renderer;

    sfVertexArray* vert_array;
    vert_array = sfVertexArray_create();

    sfVertex v1, v2;
    v1.position.x = x0;
    v1.position.y = y0;
    v2.position.x = x1;
    v2.position.y = y1;
    sfVertexArray_append( vert_array, v1 );
    sfVertexArray_append( vert_array, v2 );

    sfRenderWindow_drawVertexArray( renderWindow, vert_array, &renderer);
}

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