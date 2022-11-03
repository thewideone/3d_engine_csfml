#include <stdio.h>

// #include "../include/SFML/Graphics.h"
#include <SFML/Graphics.h>

#include "config.h"

#include "math_3d.h"

#include "stb_ds.h"

#include "graphics.h"

void mathTest( void ){
	vec3d_t v1 = { 1.0, 1.0, 1.0, 0 },
			v2 = { 2.5, 3.5, 4.5, 0 };

	printf( "v1: %f, %f, %f, %f\n", v1.x, v1.y, v1.z, v1.w );
	printf( "v2: %f, %f, %f, %f\n", v2.x, v2.y, v2.z, v2.w );

	vec3d_t v3 = vectorAdd( &v1, &v2 );
	printf( "vectorAdd(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );

	v3 = vectorSub( &v1, &v2 );
	printf( "vectorSub(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );

	v3 = vectorMul( &v2, 4.5 );
	printf( "vectorMul(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );

	v3 = vectorDiv( &v2, 3.0 );
	printf( "vectorDiv(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );

	// printVec3D( &v3, "v3: " );

	mat4x4_t mat = matrixMakeIdentity();
	printMatrix( &mat );
	// for( int i=0; i<16; i+=4 ){
	// 	printf( "%f %f %f %f\n", mat.m[i%4][i/4], mat.m[i%4][i/4+1], mat.m[i/4%4][i/4+2], mat.m[i%4][i/4+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[1][i], mat.m[1][i+1], mat.m[1][i+2], mat.m[1][i+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[2][i], mat.m[2][i+1], mat.m[2][i+2], mat.m[2][i+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[3][i], mat.m[3][i+1], mat.m[3][i+2], mat.m[3][i+3] );
	// }
}

void dynamicArrayTest( void ){
	vec3d_t* vertices = NULL;

	printf( "Putting vertices.\n" );
	vec3d_t loop1_vec;
	loop1_vec.x = 0;
	loop1_vec.y = 1;
	loop1_vec.z = 2;
	loop1_vec.w = 3;
	for( int i=0; i<4; i++ ){
		
		loop1_vec.x += 0.12;
		loop1_vec.y += 0.12;
		loop1_vec.z += 0.12;
		loop1_vec.w += 0.12;

		vec3d_t v_ret = arrput( vertices, loop1_vec );
		printf( "v_ret: %f, %f, %f, %f\n", v_ret.x, v_ret.y, v_ret.z, v_ret.w );
	}

	printf( "vertices: cap = %lld, len = %lld\n", arrcap(vertices), arrlen(vertices) );

	printf( "vertices:\n" );
	for( int i=0; i<arrlen( vertices ); i++ ){
		vec3d_t loop_vec = vertices[i];
		printf( " -> v%d: %f, %f, %f, %f\n", i, loop_vec.x, loop_vec.y, loop_vec.z, loop_vec.w );
	}

	arrfree( vertices );
}

void binaryTreeMapTest( void ){
	vec3d_t v1;
	v1.x = 0;
	v1.y = 1;
	v1.z = 2;
	v1.w = 3;
	vec3d_t v2 = v1;
	vec3d_t v3;

	vmap_t* map = NULL;

	map = vmap_insertNode( map, 0, v1, 0 );
	vmap_insertNode( map, 1, v2, 1 );
	vmap_insertNode( map, 2, v3, 0 );

	for( int i=0; i<3; i++ ){
		vmap_t* found_node = vmap_search( map, i );
		if( found_node != NULL )
			printf( "Found node of key %d: %f, %f, %f, %f, %d\n", i, found_node->v.x, found_node->v.y, found_node->v.z, found_node->v.w, found_node->visible );
	}

	vmap_print( map );
	vmap_free( map );
}

void meshTest( void ){
	mesh_t mesh = mesh_makeEmpty();
	mesh_loadFromObjFile( &mesh, "obj_models/cube.obj" );

	printf( "mesh.vertices: cap = %lld, len = %lld\n", arrcap(mesh.vertices), arrlen(mesh.vertices) );

	printf( "mesh.vertices (%d):\n", mesh.vertex_cnt );
	for( int i=0; i < mesh.vertex_cnt; i++ ){
		vec3d_t loop_vec = mesh.vertices[i];
		printf( " -> v%d: %f, %f, %f, %f\n", i, loop_vec.x, loop_vec.y, loop_vec.z, loop_vec.w );
	}

	printf( "mesh.faces (%d):\n", mesh.face_cnt );
	for( int i=0; i < mesh.face_cnt; i++ ){
		polygon_t poly = mesh.faces[i];
		printf( "Face %d: ", i );
		polygon_print( &poly );
	}

	mesh_free( &mesh );
}

void graphicsTest( sfRenderWindow* renderWindow ){
	drawLine( 50, 50, 100, 100, sfWhite, renderWindow );

	char str[20];
	sprintf( str, "Hello putText %d", 123 );
	putText( str, 150, 150, 50, sfMagenta, renderWindow );
}

int main()
{
	// Create a window:
	sfVideoMode videoMode = {800, 600, 24};
    sfRenderWindow* window;
	sfFont* font;
	sfText* text;
	sfEvent event;

	window = sfRenderWindow_create(videoMode, "SFML window", sfResize | sfClose, NULL);
			//( sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "3D test" );
    
	if (!window){
		printf( "Error: Could not create a window.\n" );
        return 1;
	}

	initGraphics();

	/* Create a graphical text to display */
    font = sfFont_createFromFile("Minecraft.ttf");
    if (!font){
		printf( "Error: Could not load a font.\n" );
        return 1;
	}
    text = sfText_create();
    sfText_setString(text, "Hello SFML");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 50);
	// window.setFramerateLimit( 144 );
	printf("Hello World!\n");

	// sf::CircleShape circle(2.0);

	// TODO: see math_3d.h

	// mathTest();
	// meshTest();

	while (sfRenderWindow_isOpen(window)){
		/* Process events */
        while (sfRenderWindow_pollEvent(window, &event)){
            /* Close window : exit */
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

		/* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

		/* Draw the text */
        sfRenderWindow_drawText(window, text, NULL);

		graphicsTest( window );

		/* Update the window */
        sfRenderWindow_display(window);
	}

	sfRenderWindow_destroy(window);
	sfText_destroy(text);
    sfFont_destroy(font);
	
	freeGraphics();

	return (0);
}