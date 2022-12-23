#include <stdio.h>
#include <time.h> // for update3DFrame()

// #include "../include/SFML/Graphics.h"
#include <SFML/Graphics.h>

#include "config.h"

#include "math_3d.h"
#include "vmap/vmap.h"
#include "mesh_3d.h"

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

	mat4x4_t mat = matrix_makeIdentity();
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
	v3.x = 0.3;
	v3.y = 1.3;
	v3.z = 2.3;
	v3.w = 3.3;

	vmap_t* map = NULL;

#ifdef REMOVE_HIDDEN_LINES
	vmap_insertNode( &map, 0, &v1, 0 );
	vmap_insertNode( &map, 1, &v2, 1 );
	vmap_insertNode( &map, 2, &v3, 0 );
#else
	vmap_insertNode( &map, 0, &v1 );
	vmap_insertNode( &map, 1, &v2 );
	vmap_insertNode( &map, 2, &v3 );
#endif

	if( map == NULL )
		printf( "map == NULL!\n" );

	for( int i=0; i<3; i++ ){
		vmap_t* found_node = vmap_search( map, i );
		if( found_node != NULL )
#ifdef REMOVE_HIDDEN_LINES
			printf( "Found node of key %d: %f, %f, %f, %f, %d\n", i, found_node->v.x, found_node->v.y, found_node->v.z, found_node->v.w, found_node->visible );
#else
			printf( "Found node of key %d: %f, %f, %f, %f\n", i, found_node->v.x, found_node->v.y, found_node->v.z, found_node->v.w );
#endif
	}

	vmap_print( map );
	vmap_free( map );
	map = NULL;	// keep this in mind
}

void meshTest( void ){
	mesh_t mesh = mesh_makeEmpty();
	mesh_loadFromObjFile( &mesh, "obj_models/cube.obj" );

	printf( "mesh.vertices: cap = %lld, len = %lld\n", arrcap(mesh.vertices), arrlen(mesh.vertices) );

	printf( "mesh.vertices (%lld):\n", mesh.vertex_cnt );
	for( size_t i=0; i < mesh.vertex_cnt; i++ ){
		vec3d_t loop_vec = mesh.vertices[i];
		printf( " -> v%lld: %f, %f, %f, %f\n", i, loop_vec.x, loop_vec.y, loop_vec.z, loop_vec.w );
	}

	printf( "mesh.faces (%lld):\n", mesh.face_cnt );
	for( size_t i=0; i < mesh.face_cnt; i++ ){
		polygon_t poly = mesh.faces[i];
		printf( "Face %lld: ", i );
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

mesh_t mesh;
mat4x4_t mat_proj;
int animate = 1;

#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
vec3d_t v_camera;  // only a placeholder now
#endif
#ifdef USE_CAMERA
vec3d_t v_look_dir;
float f_yaw;
#endif

void setup3D( void ){
	mesh = mesh_makeEmpty();
	mesh_loadFromObjFile( &mesh, "obj_models/cube.obj" );

	mat_proj = matrix_makeProjection(
		90.0,
		(float)SCREEN_HEIGHT / (float)SCREEN_WIDTH,
        0.1f, 1000.0f );
}

void free3D( void ){
	mesh_free( &mesh );
}

void draw_mesh( mesh_t* mesh, sfRenderWindow* render_window );
void processMesh( mesh_t* mesh, vec3d_t* pos, mat4x4_t* matView, float rot_angle_x, float rot_angle_z );

void update3DFrame( sfRenderWindow* renderWindow, float f_elapsed_time, float* f_theta ){
#ifdef USE_CAMERA
    // Camera movement (WSAD hehe) and looking around (arrows)
    if (sfKeyboard_isKeyPressed(sfKeyUp))
        v_camera.y -= 2.0f * f_elapsed_time;
    if (sfKeyboard_isKeyPressed(sfKeyDown))
        v_camera.y += 2.0f * f_elapsed_time;
    if (sfKeyboard_isKeyPressed(sfKeyRight)){
        f_yaw -= 2.0f * f_elapsed_time;
        //v_camera.x += 8.0f * f_elapsed_time;// * sinf(f_yaw);
        //v_camera.z += 8.0f * f_elapsed_time * cosf(f_yaw);
    }
    if (sfKeyboard_isKeyPressed(sfKeyLeft)){
        f_yaw += 2.0f * f_elapsed_time;
        //v_camera.x -= 8.0f * f_elapsed_time;// * sinf(f_yaw);
        //v_camera.z -= 8.0f * f_elapsed_time * cosf(f_yaw);
    }
    
    // We've integrated time into this, so it's a velocity vector:
    vec3d_t v_forward = vectorMul( &v_look_dir, 4.0f * f_elapsed_time );

    // My trial of implementing left and right strafing
    // I've changd some control bindings
    // pls at least change the name of this vector:
    vec3d_t temp_vUp = { 0, 1, 0, 1 };
    // Calculate the right direction:
    vec3d_t v_right_raw = vectorCrossProduct( &temp_vUp, &v_forward );
    //					  v seems 2 big
    vec3d_t v_right = vectorMul( &v_right_raw, 64.0f * f_elapsed_time );
    
    if (sfKeyboard_isKeyPressed(sfKeyW))
        v_camera = vectorAdd( &v_camera, &v_forward );
    if (sfKeyboard_isKeyPressed(sfKeyS))
        v_camera = vectorSub( &v_camera, &v_forward );
    if (sfKeyboard_isKeyPressed(sfKeyA))
        v_camera = vectorSub( &v_camera, &v_right );
        //f_yaw += 2.0f * f_elapsed_time;
    if (sfKeyboard_isKeyPressed(sfKeyD))
        v_camera = vectorAdd( &v_camera, &v_right );
        //f_yaw -= 2.0f * f_elapsed_time;
    
    //v_look_dir = { 0, 0, 1 };
    vec3d_t v_up = { 0, 1, 0, 1 };
    //vec3d_t v_target = vectorAdd( v_camera, v_look_dir );
    vec3d_t v_target = { 0, 0, 1, 1 };
    mat4x4_t mat_camera_rot = matrix_makeRotY( f_yaw );
    v_look_dir = matrix_mulVector( &mat_camera_rot, &v_target );
    v_target = vectorAdd( &v_camera, &v_look_dir );

    mat4x4_t mat_camera = matrix_pointAt( &v_camera, &v_target, &v_up );

    // Make view matrix from camera:
    mat4x4_t mat_view = matrix_quickInverse( &mat_camera );

	// printf( "MatView:\n" );
	// printMatrix( &mat_view );	
#endif

	// mat4x4 matRotZ, matRotX;
    // Current angle:
    if( animate )
        *f_theta += 1.0 * f_elapsed_time;

    vec3d_t pos1 = { 0.0f, 0.0f, 2.0f, 0.0f };
    vec3d_t pos2 = { 0.0f, 0.0f, 4.0f, 0.0f };
	
#ifdef USE_CAMERA
    processMesh( &mesh, &pos1, &mat_view, *f_theta, (*f_theta)*0.5 );
#else
	mat4x4_t dummy;
	processMesh( &mesh, &pos1, &dummy, *f_theta, (*f_theta)*0.5 );
#endif
	draw_mesh( &mesh, renderWindow );

}

void processMesh( mesh_t* mesh, vec3d_t* pos, mat4x4_t* matView, float rot_angle_x, float rot_angle_z ){
	// Apply rotation in Z axis:
    mat4x4_t matRotZ = matrix_makeRotZ( rot_angle_z );
    // Apply rotation in X axis:
    mat4x4_t matRotX = matrix_makeRotX( rot_angle_x );

    // Translation matrix
    // mat4x4 matTrans;
    // mesh.matTrans = matrixMakeTranslation( 0.0f, 0.0f, 2.0f );
    mat4x4_t matTrans = matrix_makeTranslation( pos->x, pos->y, pos->z );

    // World matrix:
    // mat4x4 matWorld;
    mat4x4_t matWorld = matrix_makeIdentity();
    matWorld = matrix_mulMatrix( &matRotZ, &matRotX );     // order important
    matWorld = matrix_mulMatrix( &matWorld, &matTrans );   // second

	// printf( "matRotZ:\n" );
	// printMatrix( &matRotZ );
	// printf( "matRotX:\n" );
	// printMatrix( &matRotX );
	// printf( "matTrans:\n" );
	// printMatrix( &matTrans );
	// printf( "MatWorld:\n" );
	// printMatrix( &matWorld );

	arrfree( mesh->transformedVertices );
	// mesh->transformedVertices = NULL;
	// Not really needed, just to reserve amount
    // of RAM space exactly equal to our needs
	// arrsetcap( mesh->transformedVertices, mesh->vertex_cnt );

	// Transform every vertex of the mesh
    for( size_t i=0; i < mesh->vertex_cnt; i++ ){
		vec3d_t vertex = mesh->vertices[i];
        // Convert to world space
        vec3d_t v_transformed = matrix_mulVector( &matWorld, &vertex );
		arrput( mesh->transformedVertices, v_transformed );
    }

	// Vector storing only IDs of visible faces
	arrfree( mesh->visFaceIDs );
	// mesh->visFaceIDs = NULL;
	// arrsetcap( mesh->visFaceIDs, mesh->face_cnt );	// is it needed?

	/* // Debug print:

	printf( "mesh->vertices:\n" );
	for( size_t i=0; i < mesh->vertex_cnt; i++ )
		printf( "%f %f %f\n", mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z );
	
	printf( "mesh.vertices (%lld):\n", mesh->vertex_cnt );
	for( size_t i=0; i < mesh->vertex_cnt; i++ ){
		vec3d_t v = mesh->vertices[i];
		printf( "v %lld: ", i );
		vec3d_print( &v, 1 );
	}

	printf( "mesh.faces (%lld):\n", mesh->face_cnt );
	for( size_t i=0; i < mesh->face_cnt; i++ ){
		polygon_t poly = mesh->faces[i];
		printf( "Face %lld: ", i );
		polygon_print( &poly );
	}

	printf( "mesh.transformedVertices (%lld):\n", arrlen(mesh->transformedVertices) );
	for( size_t i=0; i < arrlen(mesh->transformedVertices); i++ ){
		vec3d_t v = mesh->transformedVertices[i];
		vec3d_print( &v, 1 );
	}
	*/

	// Add each visible face's ID to the visFaceIDs vector
    int face_id = 0;
    for( size_t i=0; i < mesh->face_cnt; i++ ){ 
		polygon_t face = mesh->faces[i];	// idk how but it works
#ifdef RENDER_VISIBLE_ONLY
        // Use cross-product to get surface normal:
        vec3d_t normal, edge1, edge2;
        edge1 = vectorSub( &mesh->transformedVertices[face.p[2]],
                           &mesh->transformedVertices[face.p[1]] );
        edge2 = vectorSub( &mesh->transformedVertices[face.p[0]],
                           &mesh->transformedVertices[face.p[1]] );
		// The normal vector is a cross product of two edges of the face:
        normal = vectorCrossProduct( &edge1, &edge2 );
        // Normalise the normal (normalised length = 1.0 unit):
        normal = vectorNormalise( &normal );

        // Get ray from the face to the camera:
        vec3d_t v_camera_ray = vectorSub( &mesh->transformedVertices[face.p[0]],
                                      &v_camera );
        // If the ray is aligned with normal, then face is visible:
        // Use '>' to render in inverse (like a view from inside):
		if( vectorDotProduct( &normal, &v_camera_ray ) < 0.0f ){
            arrput( mesh->visFaceIDs, face_id );
		}
#else
		arrput( mesh->visFaceIDs, face_id );
#endif
		face_id++;
    }

	// Map of projected vertices
    // (only the visible ones are projected)
    // int: ID of vertex, vec3d: vertex data
	vmap_free( mesh->vert2DSpaceMap );
	mesh->vert2DSpaceMap = NULL;

	// printf( "Freed vert2DSpaceMap. Transforming vertices (%lld)...\n", mesh->vertex_cnt );

#ifdef RENDER_VISIBLE_ONLY
	// Transform only visible vertices
    for( int curr_vert_id = 0; curr_vert_id < mesh->vertex_cnt; curr_vert_id++ ){
        // For all visible faces	
        for( int i=0; i < arrlen(mesh->visFaceIDs); i++ ){
            // For all vertices in a face
            for( int j=0; j < mesh->faces[ mesh->visFaceIDs[i] ].p_count; j++ ){
                // If an ID of a vertex in face == ID of the current vertex
                if( mesh->faces[ mesh->visFaceIDs[i] ].p[j] == curr_vert_id ){
                    // We're using a label here as it's the easiest
                    // way to get out of multiple loops
                    // cout<<"Vertex "<<curr_vert_id<<" visible. Going to LABEL_VERTEX_VISIBLE\n";
                    goto LABEL_VERTEX_VISIBLE;
                }
			}
		}
        // Oh you're here? Didn't jump to the label?
        // Seems like the vertex is not visible then,
        // so skip it
        continue;

        LABEL_VERTEX_VISIBLE:
#else
	// Transform all vertices
    for( int curr_vert_id = 0; curr_vert_id < mesh->vertex_cnt; curr_vert_id++ ){
#endif
            // Transform current vertex
#ifdef USE_CAMERA
            // Convert world space to view space
            vec3d_t vertViewed = matrix_mulVector( matView, &mesh->transformedVertices[curr_vert_id] );
            vec3d_t vertProjected = matrix_mulVector( &mat_proj, &vertViewed );
#else   // NOT USING CAMERA
            vec3d_t vertProjected = matrix_mulVector( &mat_proj, &mesh->transformedVertices[curr_vert_id] );
#endif
            // Scale into view, we moved the normalising into cartesian space
            // out of the matrix.vector function from the previous versions, so
            // do this manually:
            vertProjected = vectorDiv( &vertProjected, vertProjected.w );
	
            // But since the result is in range of -1 to 1,
            // we have to scale it into view:
            vec3d_t vOffsetView = { 1, 1, 0, 0 };
            vertProjected = vectorAdd( &vertProjected, &vOffsetView );
            vertProjected.x *= 0.5f * (float)SCREEN_WIDTH;
            vertProjected.y *= 0.5f * (float)SCREEN_HEIGHT;
	
            // Add this vertex and its ID into the map
#ifdef REMOVE_HIDDEN_LINES
			vmap_insertNode( &mesh->vert2DSpaceMap,
							 curr_vert_id,
							 &vertProjected, 1 );
#else
			vmap_insertNode( &mesh->vert2DSpaceMap,
							 curr_vert_id,
							 &vertProjected );
#endif
	}

	// printf( "mesh->vert2DSpaceMap:\n" );
	// vmap_print( mesh->vert2DSpaceMap );

	// Each entry is 4x int:
    // 1: start_vert_ID
    // 2: end_vert_ID
    // 3: num_of_faces_which_the_edge_belongs_to (1-2)
    // 4: ID_of_1st_face
    arrfree(mesh->vis_edge_vec);
    // Reserve max number of visible edges? (9 for cube)
    //vis_edge_vec.reserve( 9*sizeof(int) );

	// Fill the visible edge vector
    // For each visible face
    for( int i=0; i < arrlen(mesh->visFaceIDs); i++ ){

        int vert_cnt = mesh->faces[ mesh->visFaceIDs[i] ].p_count;
        
        // For each point
        for( int vert_id = 0; vert_id < vert_cnt; vert_id++ ){

			int edge_start_v_id, edge_end_v_id;
			bool presence_flag = 0;

			edge_start_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[vert_id];
			
			if( vert_id == vert_cnt - 1 )
				// For the last edge
				edge_end_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[0];
			else 
				edge_end_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[vert_id + 1];

            // For the whole visible edge array
            for( int j=0; j < arrlen(mesh->vis_edge_vec); j+=4 ){                
                // If the edge exists in the visible edge array
                if( (mesh->vis_edge_vec[j]   == edge_start_v_id &&
					 mesh->vis_edge_vec[j+1] == edge_end_v_id   ) || 
                    (mesh->vis_edge_vec[j]   == edge_end_v_id   &&
					 mesh->vis_edge_vec[j+1] == edge_start_v_id ) ){
                    presence_flag = 1;
                    // (j+2) - occurence count
                    mesh->vis_edge_vec[j+2]++;
                    break;
                }
            }
            // If the edge doesn't exist in the visible edge array, add it
            if( presence_flag == 0 ){
                arrput( mesh->vis_edge_vec, edge_start_v_id );
				arrput( mesh->vis_edge_vec, edge_end_v_id );
				arrput( mesh->vis_edge_vec, 1 );
				arrput( mesh->vis_edge_vec, mesh->visFaceIDs[i] );
            }
        }
    }

	// mesh_printVisEdgeVec( mesh );
}

void draw_mesh( mesh_t* mesh, sfRenderWindow* render_window ){
    // Draw every visible edge
    // Debug (commented): print the visible edge array
    for( int i=0; i<arrlen(mesh->vis_edge_vec); i+=4 ){
        // Draw only outline of the mesh
		// Useful for debugging, TRY IT!!!
        // if( mesh->vis_edge_vec[ i + 2 ] > 1 )
        //     continue;

		// printf( "Looking for verts %d and %d...\n", mesh->vis_edge_vec[i], mesh->vis_edge_vec[i+1] );
        
		vmap_t* v_proj_el = NULL;
		v_proj_el = vmap_search( mesh->vert2DSpaceMap, mesh->vis_edge_vec[i] );
		if( v_proj_el == NULL ){
			printf( "Error: in mesh->vert2DSpaceMap could not find vertex of ID=%d\n",
					mesh->vis_edge_vec[i] );
			return;
		}
		vec3d_t vertProjected1 = v_proj_el->v;
		int vert1_ID = v_proj_el->key;

		// vec3d_print( &v_proj_el->v, 0 );

		v_proj_el = NULL;
		v_proj_el = vmap_search( mesh->vert2DSpaceMap, mesh->vis_edge_vec[i+1] );
		if( v_proj_el == NULL ){
			printf( "Error: in mesh->vert2DSpaceMap could not find vertex of ID=%d\n",
					mesh->vis_edge_vec[i+1] );
			return;
		}
		vec3d_t vertProjected2 = v_proj_el->v;
		int vert2_ID = v_proj_el->key;

		// vec3d_print( &v_proj_el->v, 1 );

		// vec3d_print( &vertProjected1, 0 );
		// vec3d_print( &vertProjected2, 1 );
        // vec3d_t vertProjected1 = mesh.vert2DSpaceMap.find(mesh.vis_edge_vec[i])->second;
        // vec3d_t vertProjected2 = mesh.vert2DSpaceMap.find(mesh.vis_edge_vec[i+1])->second;

        // int vert1_ID = mesh.vert2DSpaceMap.find(mesh.vis_edge_vec[i])->first;
        // int vert2_ID = mesh.vert2DSpaceMap.find(mesh.vis_edge_vec[i+1])->first;

#ifdef VERTEX_DOT_DEBUG
        if( mesh.visVertexMap.size() > 0 ){
            sf::CircleShape dot(3);
            if( mesh.visVertexMap.find( vert1_ID )->second )
                dot.setFillColor(sf::Color(50,255,50));
            else
                dot.setFillColor(sf::Color(255,50,50));
            
            dot.setPosition( vertProjected1.x, vertProjected1.y );
            render_window.draw(dot);
        }
#endif /* VERTEX_DOT_DEBUG */

#ifdef VERTEX_ID_DEBUG
        stringstream s1;

        s1 << vert1_ID;
        // putText( s1, vertProjected1.x, vertProjected1.y, 8, sf::Color::Yellow,  render_window );
        // s2 << vert2_ID;  // one is enough, because the printed values are the same and overlap on the screen
        // putText( s2, vertProjected2.x, vertProjected2.y, 8, sf::Color::Green,  render_window );

        // cout<<"\nEdge "<<i/4<<": "<<mesh.vis_edge_vec[i]<<" "<<mesh.vis_edge_vec[i+1];//<<" "<<mesh.vis_edge_vec[i+2]<<" "<<mesh.vis_edge_vec[i+3];
        // cout<<": ("<<vertProjected1.x<<", "<<vertProjected1.y<<")-("<<vertProjected2.x<<", "<<vertProjected2.y<<")";
#endif /* VERTEX_ID_DEBUG */

        drawLine( vertProjected1.x, vertProjected1.y,
                  vertProjected2.x, vertProjected2.y,
                  sfWhite, render_window );
    }
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

	// binaryTreeMapTest();

	setup3D();

	float f_theta = 0;
	clock_t t1 = clock();
	clock_t t2 = clock();


	while (sfRenderWindow_isOpen(window)){
		/* Process events */
        while (sfRenderWindow_pollEvent(window, &event)){
            /* Close window : exit */
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
			if (event.type == sfEvtMouseButtonPressed){
                // Toggle object rotation animation
                if (event.mouseButton.button == sfMouseRight){
                    if( animate ){
                        animate = 0;
                        mesh_printVisEdgeVec( &mesh );
                    }
                    else
                        animate = 1;
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
		float elapsed_time = (float)(t2-t1) / CLOCKS_PER_SEC;
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