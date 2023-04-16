#include <stdio.h>
#include <time.h> // for 3d_engine/3d_main.h/update3DFrame()

// #include "../include/SFML/Graphics.h"
#include <SFML/Graphics.h>

#include "3d_engine/config.h"

#include "3d_engine/math_3d.h"
#include "3d_engine/vmap/vmap.h"
#include "3d_engine/mesh_3d.h"
#include "3d_engine/mesh_3d_queue.h"

#include "3d_engine/stb_ds.h"

#include "3d_engine/graphics.h"

#include "3d_engine/3d_main.h"

#include "meshes/cube.h"
#include "meshes/sphere.h"
#include "meshes/dodecahedron.h"

// 
// TODO:
// 	- make the engine available to use as a submodule in AVR project:
// 		- separate the core of the engine from main.c on github
// 	- left-right movement too slow but only sometimes??
// 	- don't draw meshes behind camera
// 	- add screen clipping
// 	- filled meshes
// 	- inverted line colour when in front of filled object?
// 	- X upgrade vmap to AVL
// 	- X add a mesh queue
// 	- X loading meshes from program memory
// 	- X dynamic meshes
// 	- X add "destructors" to free for example meshes' dynamic arrays
// 	- X move global variables and mesh processing functions to a separate file
// 	- X coloured meshes:
// 	- X (fill and edge colours separated)
// 

void mathTest( void ){
	printf( "===== MATH TEST =====\n" );
#ifdef USE_FIXED_POINT_ARITHMETIC
	fxp_t a = floatingToFixed( 5.6 );
	fxp_t b = floatingToFixed( 2.7 );
	fxp_t c = fixedMul( a, b );
	fxp_t d = fixedDiv( a, b );
	printf( "a=%f, b=%f, c=%f, d=%f\n", fixedToFloating(a), fixedToFloating(b), fixedToFloating(c), fixedToFloating(d) );
	printf( "3.5 = %d = %f\n", a, fixedToFloating( a ) );

	vec3d_t v1 = { floatingToFixed(1.0), floatingToFixed(1.0), floatingToFixed(1.0), floatingToFixed(0) },
			v2 = { floatingToFixed(2.5), floatingToFixed(3.5), floatingToFixed(4.5), floatingToFixed(0) };
#else
	vec3d_t v1 = { 1.0, 1.0, 1.0, 0 },
			v2 = { 2.5, 3.5, 4.5, 0 };
#endif

	// printf( "v1: %f, %f, %f, %f\n", v1.x, v1.y, v1.z, v1.w );
	// printf( "v2: %f, %f, %f, %f\n", v2.x, v2.y, v2.z, v2.w );
	printf( "v1: " );
	vec3d_print( &v1, true );
	printf( "v2: " );
	vec3d_print( &v2, true );

	vec3d_t v3 = vectorAdd( &v1, &v2 );
	// printf( "vectorAdd(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );
	printf( "vectorAdd(): " );
	vec3d_print( &v3, true );

	v3 = vectorSub( &v1, &v2 );
	// printf( "vectorSub(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );
	printf( "vectorSub(): " );
	vec3d_print( &v3, true );

#ifdef USE_FIXED_POINT_ARITHMETIC
	v3 = vectorMul( &v2, floatingToFixed(4.5) );
#else
	v3 = vectorMul( &v2, 4.5 );
#endif
	// printf( "vectorMul(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );
	printf( "vectorMul(): " );
	vec3d_print( &v3, true );

#ifdef USE_FIXED_POINT_ARITHMETIC
	v3 = vectorDiv( &v2, floatingToFixed(3.0) );
#else
	v3 = vectorDiv( &v2, 3.0 );
#endif
	// printf( "vectorDiv(): %f, %f, %f, %f\n", v3.x, v3.y, v3.z, v3.w );
	printf( "vectorDiv(): " );
	vec3d_print( &v3, true );

	// printVec3D( &v3, "v3: " );

	mat4x4_t mat;
	matrix_makeIdentity( &mat );
	printMatrix( &mat );
	// for( int i=0; i<16; i+=4 ){
	// 	printf( "%f %f %f %f\n", mat.m[i%4][i/4], mat.m[i%4][i/4+1], mat.m[i/4%4][i/4+2], mat.m[i%4][i/4+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[1][i], mat.m[1][i+1], mat.m[1][i+2], mat.m[1][i+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[2][i], mat.m[2][i+1], mat.m[2][i+2], mat.m[2][i+3] );
	// 	// printf( "%f %f %f %f\n", mat.m[3][i], mat.m[3][i+1], mat.m[3][i+2], mat.m[3][i+3] );
	// }
}

void dynamicArrayTest( void ){
	printf( "===== DYNAMIC ARRAY TEST =====\n" );
	vec3d_t* vertices = NULL;

	printf( "Putting vertices.\n" );
	vec3d_t loop1_vec;

#ifdef USE_FIXED_POINT_ARITHMETIC
	loop1_vec.x = floatingToFixed(0);
	loop1_vec.y = floatingToFixed(1);
	loop1_vec.z = floatingToFixed(2);
	loop1_vec.w = floatingToFixed(3);
#else
	loop1_vec.x = 0;
	loop1_vec.y = 1;
	loop1_vec.z = 2;
	loop1_vec.w = 3;
#endif
	for( int i=0; i<4; i++ ){
		
#ifdef USE_FIXED_POINT_ARITHMETIC
		loop1_vec.x += floatingToFixed(0.12);
		loop1_vec.y += floatingToFixed(0.12);
		loop1_vec.z += floatingToFixed(0.12);
		loop1_vec.w += floatingToFixed(0.12);
#else
		loop1_vec.x += 0.12;
		loop1_vec.y += 0.12;
		loop1_vec.z += 0.12;
		loop1_vec.w += 0.12;
#endif
		vec3d_t v_ret = arrput( vertices, loop1_vec );
		// printf( "v_ret: %f, %f, %f, %f\n", v_ret.x, v_ret.y, v_ret.z, v_ret.w );
		printf( "v_ret: " );
		vec3d_print( &v_ret, true );
	}

	printf( "vertices: cap = %lld, len = %lld\n", arrcap(vertices), arrlen(vertices) );

	printf( "vertices:\n" );
	for( int i=0; i<arrlen( vertices ); i++ ){
		vec3d_t loop_vec = vertices[i];
		// printf( " -> v%d: %f, %f, %f, %f\n", i, loop_vec.x, loop_vec.y, loop_vec.z, loop_vec.w );
		printf( " -> v%d: ", i );
		vec3d_print( &loop_vec, true );
	}

	arrfree( vertices );
}

void binarySearchTreeMapTest( void ){
	printf( "===== VMAP TEST =====\n" );
	vec3d_t v1;

#ifdef USE_FIXED_POINT_ARITHMETIC
	v1.x = floatingToFixed(0);
	v1.y = floatingToFixed(1);
	v1.z = floatingToFixed(2);
	v1.w = floatingToFixed(3);
#else
	v1.x = 0;
	v1.y = 1;
	v1.z = 2;
	v1.w = 3;
#endif;
	vec3d_t v2 = v1;
	vec3d_t v3;
#ifdef USE_FIXED_POINT_ARITHMETIC
	v3.x = floatingToFixed(0.3);
	v3.y = floatingToFixed(1.3);
	v3.z = floatingToFixed(2.3);
	v3.w = floatingToFixed(3.3);
#else
	v3.x = 0.3;
	v3.y = 1.3;
	v3.z = 2.3;
	v3.w = 3.3;
#endif

	vmap_t map;

	printf( "Making empty map...\n" );
	vmap_makeEmpty( &map );

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
	printf( "Inserting 1st element...\n" );
	vmap_insert( &map, 0, &v1, 0 );
	printf( "Inserting 2nd element...\n" );
	vmap_insert( &map, 1, &v2, 1 );
	printf( "Inserting 3rd element...\n" );
	vmap_insert( &map, 2, &v3, 0 );
#else
	vmap_insert( &map, 0, &v1 );
	vmap_insert( &map, 1, &v2 );
	vmap_insert( &map, 2, &v3 );
#endif

	if( vmap_isEmpty( &map ) )
		printf( "Map empty\n" );

	for( int i=0; i<3; i++ ){
		vec3d_t found_vert;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
		bool found_vis_flag;
		bool ret = vmap_find( &map, i, &found_vert, &found_vis_flag );
#else
		bool ret = vmap_find( &map, i, &found_vert );
#endif
		if( ret ){
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
			// printf( "Found node of key %d: %f, %f, %f, %f, %d\n", i, found_node->v.x, found_node->v.y, found_node->v.z, found_node->v.w, found_node->visible );
			printf( "Found node of key %d and vis_flg %d: ", i, found_vis_flag );
			vec3d_print( &found_vert, true );
#else
			// printf( "Found node of key %d: %f, %f, %f, %f\n", i, found_node->v.x, found_node->v.y, found_node->v.z, found_node->v.w );
			printf( "Found node of key %d: ", i );
			vec3d_print( &found_vert, true );
#endif
		}
	}

	printf( "Plotting graph...\n" );
	vmap_graph( &map );
	printf( "Printing...\n" );
	vmap_printInorder( &map );

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
	bool ret = vmap_updateNode( &map, 0, &v3, 1 );
#else
	bool ret = vmap_updateNode( &map, 0, &v3 );
#endif

	if( ret )
		printf( "Successufully updated v0.\n" );
	else
		printf( "Failed to update v0.\n" );

	printf( "Printing again...\n" );
	vmap_printInorder( &map );

	printf( "Freeing...\n" );
	vmap_free( &map );
	printf( "Done.\n" );
	// map = NULL;	// keep this in mind
}

void meshTest( void ){
	printf( "===== MESH TEST =====\n" );
#ifdef USE_LOADING_FROM_OBJ
	mesh_t mesh;
	mesh_makeEmpty( &mesh );
	mesh_loadFromObjFile( &mesh, "obj_models/cube.obj" );

	printf( "mesh.vertices: cap = %lld, len = %lld\n", arrcap(mesh.vertices), arrlen(mesh.vertices) );

	printf( "mesh.vertices (%lld):\n", mesh.vertex_cnt );
	for( size_t i=0; i < mesh.vertex_cnt; i++ ){
		vec3d_t loop_vec = mesh.vertices[i];
		printf( " -> v%lld: ", i );
		vec3d_print( &loop_vec, true );
	}

	printf( "mesh.faces (%lld):\n", mesh.face_cnt );
	for( size_t i=0; i < mesh.face_cnt; i++ ){
		polygon_t poly = mesh.faces[i];
		printf( "Face %lld: ", i );
		polygon_print( &poly );
	}

	mesh_free( &mesh );
#endif

	
	mesh_t mesh_prgm;
	mesh_makeEmpty( &mesh_prgm );
	mesh_loadFromProgmem( &mesh_prgm, cube_mesh_verts, cube_mesh_faces, CUBE_MESH_V_CNT, CUBE_MESH_F_CNT, false );

	printf( "mesh_prgm.vertices (%lld):\n", mesh_prgm.vertex_cnt );
	for( size_t i=0; i < mesh_prgm.vertex_cnt; i++ ){
		vec3d_t loop_vec = mesh_prgm.vertices[i];
		printf( " -> v%lld: ", i );
		vec3d_print( &loop_vec, true );
	}	
	printf( "mesh_prgm.faces (%lld):\n", mesh_prgm.face_cnt );
	for( size_t i=0; i < mesh_prgm.face_cnt; i++ ){
		polygon_t poly = mesh_prgm.faces[i];
		printf( "Face %lld: ", i );
		polygon_print( &poly );
	}

	mesh_free( &mesh_prgm );
	

	
	// Dynamic mesh
	mesh_t* mesh_prgm_dyn = (mesh_t*)malloc( sizeof(mesh_t) );
	mesh_makeEmpty( mesh_prgm_dyn );
	mesh_loadFromProgmem( mesh_prgm_dyn, cube_mesh_verts, cube_mesh_faces, CUBE_MESH_V_CNT, CUBE_MESH_F_CNT, false );

	printf( "mesh_prgm_dyn->vertices (%lld):\n", mesh_prgm_dyn->vertex_cnt );
	for( size_t i=0; i < mesh_prgm_dyn->vertex_cnt; i++ ){
		vec3d_t loop_vec = mesh_prgm_dyn->vertices[i];
		printf( " -> v%lld: ", i );
		vec3d_print( &loop_vec, true );
	}	
	printf( "mesh_prgm_dyn->faces (%lld):\n", mesh_prgm_dyn->face_cnt );
	for( size_t i=0; i < mesh_prgm_dyn->face_cnt; i++ ){
		polygon_t poly = mesh_prgm_dyn->faces[i];
		printf( "Face %lld: ", i );
		polygon_print( &poly );
	}

	mesh_free( mesh_prgm_dyn );
	
}

void meshQueueTest( void ){
	printf( "===== MESH QUEUE TEST =====\n" );

	mesh_queue_t mq;
	meshQueue_makeEmpty( &mq );

	mesh_t mesh1, mesh2;
	mesh_t* mesh3 = (mesh_t*)malloc( sizeof(mesh_t) );

	mesh_makeEmpty( &mesh1 );
	mesh_makeEmpty( &mesh2 );
	mesh_makeEmpty( mesh3 );
#ifdef USE_LOADING_FROM_OBJ
	mesh_loadFromObjFile( &mesh1, "obj_models/cube.obj" );
	mesh_loadFromObjFile( &mesh2, "obj_models/dodecahedron.obj" );
	mesh_loadFromObjFile( &mesh3, "obj_models/sphere.obj" );
#else
	mesh_loadFromProgmem( &mesh1, cube_mesh_verts, cube_mesh_faces, CUBE_MESH_V_CNT, CUBE_MESH_F_CNT, false );
	mesh_loadFromProgmem( &mesh2, sphere_mesh_verts, sphere_mesh_faces, SPHERE_MESH_V_CNT, SPHERE_MESH_F_CNT, false );
	mesh_loadFromProgmem( mesh3, dodecahedron_mesh_verts, dodecahedron_mesh_faces, DODECAHEDRON_MESH_V_CNT, DODECAHEDRON_MESH_F_CNT, false );

#endif

	bool ret = meshQueue_push( &mq, &mesh1 );
	if( !ret )
		printf( "Error: in meshQueueTest(): failed to push mesh1 into mq.\n" );
	ret = meshQueue_push( &mq, &mesh2 );
	if( !ret )
		printf( "Error: in meshQueueTest(): failed to push mesh2 into mq.\n" );
	ret = meshQueue_push( &mq, mesh3 );
	if( !ret )
		printf( "Error: in meshQueueTest(): failed to push mesh3 into mq.\n" );

	for( size_t mi=0; mi < mq.size; mi++ ){
		mesh_t* mptr = meshQueue_getCurrent( &mq );

		if( mptr == NULL ){
			printf( "null\n" );
			continue;
		}

		printf( "mesh%lld.vertices (%lld):\n", mi+1, mptr->vertex_cnt );

		printf( "mesh%lld.faces (%lld):\n", mi+1, mptr->face_cnt );

		meshQueue_goToNext( &mq );
	}

	meshQueue_removeAt( &mq, 1 );

	printf( "Removed 2nd mesh, size = %lld.\n", mq.size );

	for( size_t mi=0; mi < mq.size; mi++ ){
		mesh_t* mptr = meshQueue_getCurrent( &mq );

		if( mptr == NULL ){
			printf( "null\n" );
			continue;
		}

		printf( "mesh%lld.vertices (%lld):\n", mi+1, mptr->vertex_cnt );

		printf( "mesh%lld.faces (%lld):\n", mi+1, mptr->face_cnt );

		meshQueue_goToNext( &mq );
	}

	meshQueue_push( &mq, &mesh2 );

	for( size_t mi=0; mi < mq.size; mi++ ){
		mesh_t* mptr = meshQueue_getCurrent( &mq );

		if( mptr == NULL ){
			printf( "null\n" );
			continue;
		}

		printf( "mesh%lld.vertices (%lld):\n", mi+1, mptr->vertex_cnt );

		printf( "mesh%lld.faces (%lld):\n", mi+1, mptr->face_cnt );

		meshQueue_goToNext( &mq );
	}

	meshQueue_freeAllMeshes( &mq );
}

void graphicsTest( sfRenderWindow* renderWindow ){
	printf( "===== GRAPHICS TEST =====\n" );

	drawLine( 50, 50, 100, 100, sfWhite, renderWindow );

	char str[20];
	sprintf( str, "Hello putText %d", 123 );
#ifdef USE_FIXED_POINT_ARITHMETIC
	putText( str, floatingToFixed(150), floatingToFixed(150), 50, sfMagenta, renderWindow );
#else
	putText( str, 150, 150, 50, sfMagenta, renderWindow );
#endif
}

int main(){

	// Create a window:
#ifdef COLOUR_MONOCHROME
	// Just for SFML window, the engine will work in monochrome mode
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, 24 };
#else
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, COLOUR_DEPTH };
#endif
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

	// sf::CircleShape circle(2.0);

	setup3D();

	flp_t f_theta = 0;
	clock_t t1 = clock();
	clock_t t2 = clock();

	DEBUG_PRINT( "Setup complete %d\n", (int) 123 );

	// printf( "Setup complete.\n" );

	mathTest();
	// dynamicArrayTest();
	binarySearchTreeMapTest();
	// meshTest();
	// meshQueueTest();

	while (sfRenderWindow_isOpen(window)){
		/* Process events */
        while (sfRenderWindow_pollEvent(window, &event)){
            /* Close window : exit */
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
			if (event.type == sfEvtMouseButtonPressed){
                // Toggle object rotation animation
                if (event.mouseButton.button == sfMouseRight){
                    if( getAnimateFlag() ){
                        setAnimateFlag(0);
                        // mesh_printVisEdgeVec( &mesh );
                    }
                    else
                        setAnimateFlag(1);
                }
                // Reset rotation angle
                if (event.mouseButton.button == sfMouseLeft){
                    f_theta = 0.0f;
                }
            }
            if (event.type == sfEvtMouseWheelMoved){
                f_theta += event.mouseWheel.delta*3.14/180.0;
            }
        }

		/* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

		/* Draw the text */
        // sfRenderWindow_drawText(window, text, NULL);

		// graphicsTest( window );

		t2 = clock();
		flp_t elapsed_time = (flp_t)(t2-t1) / CLOCKS_PER_SEC;
		t1 = t2;

		update3DFrame( window, elapsed_time, &f_theta );

		/* Update the window */
        sfRenderWindow_display(window);
	}

	sfRenderWindow_destroy(window);
	sfText_destroy(text);
    sfFont_destroy(font);

	free3D();
	freeGraphics();

	return (0);
}