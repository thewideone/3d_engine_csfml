#include "3d_main.h"

mesh_t mesh;
mat4x4_t mat_proj;
bool animate = 0;

#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
vec3d_t v_camera;  // only a placeholder now
#endif
#ifdef USE_CAMERA
vec3d_t v_look_dir;
rtnl_t f_yaw;
#endif

bool getAnimateFlag( void ){
    return animate;
}
void setAnimateFlag( bool value ){
    animate = value;
}

void setup3D( void ){
	mesh_makeEmpty( &mesh );
	// mesh_setEdgeColourByValue( &mesh, COLOUR_GREEN );
#ifdef USE_LOADING_FROM_OBJ
	bool ret = mesh_loadFromObjFile( &mesh, "obj_models/cube.obj" );
#else
	bool ret = mesh_loadFromProgmem( &mesh, cube_mesh_verts, cube_mesh_faces, CUBE_MESH_V_CNT, CUBE_MESH_F_CNT, false );
	// bool ret = mesh_loadFromProgmem( &mesh, sphere_mesh_verts, sphere_mesh_faces, SPHERE_MESH_V_CNT, SPHERE_MESH_F_CNT, false );
	// bool ret = mesh_loadFromProgmem( &mesh, dodecahedron_mesh_verts, dodecahedron_mesh_faces, DODECAHEDRON_MESH_V_CNT, DODECAHEDRON_MESH_F_CNT, false );
#endif
	if( !ret ){
		DEBUG_PRINT( "Error: in setup3D() loading mesh from file failed\n" );
	}

#ifdef USE_FIXED_POINT_ARITHMETIC
#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
	v_camera.x = floatingToFixed(0);
	v_camera.y = floatingToFixed(0);
	v_camera.z = floatingToFixed(0);
	v_camera.w = floatingToFixed(1);
#endif
#ifdef USE_CAMERA
	v_look_dir.x = floatingToFixed(0);
	v_look_dir.y = floatingToFixed(0);
	v_look_dir.z = floatingToFixed(0);
	v_look_dir.w = floatingToFixed(1);
	f_yaw = floatingToFixed(0);
#endif
	matrix_makeProjection( &mat_proj,
		floatingToFixed(90.0), floatingToFixed( (flp_t)SCREEN_HEIGHT / (flp_t)SCREEN_WIDTH ),
        floatingToFixed( 0.1f ), floatingToFixed( 1000.0f ) );
#else
#if defined(RENDER_VISIBLE_ONLY) || defined(USE_CAMERA)
	v_camera.x = 0;
	v_camera.y = 0;
	v_camera.z = 0;
	v_camera.w = 1;
#endif
#ifdef USE_CAMERA
	v_look_dir.x = 0;
	v_look_dir.y = 0;
	v_look_dir.z = 0;
	v_look_dir.w = 1;
	f_yaw = 0;
#endif
	matrix_makeProjection( &mat_proj,
		90.0,
		(flp_t)SCREEN_HEIGHT / (flp_t)SCREEN_WIDTH,
        0.1f, 1000.0f );
#endif
}

void free3D( void ){
	mesh_free( &mesh );
}

#ifdef USE_CAMERA
// 
// Compute camera and view matrices,
// the latter of which is used for mesh transformation,
// if using camera
// mat_view			- view matrix
// f_elapsed_time	- elapsed time since the last frame,
// 					  don't ask me about unit XD
// Input parameters not listed in the function declaration:
// v_camera
// f_yaw
// v_look_dir
// 
void computeViewMatrix( mat4x4_t* mat_view, flp_t f_elapsed_time ){
    // Camera movement (WSAD hehe) and looking around (arrows)
#ifdef USE_FIXED_POINT_ARITHMETIC
	if (sfKeyboard_isKeyPressed(sfKeyUp))
        v_camera.y -= floatingToFixed( 2.0f * f_elapsed_time );
    if (sfKeyboard_isKeyPressed(sfKeyDown))
        v_camera.y += floatingToFixed( 2.0f * f_elapsed_time );
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        f_yaw -= floatingToFixed( 2.0f * f_elapsed_time );
    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        f_yaw += floatingToFixed( 2.0f * f_elapsed_time );
    
    // We've integrated time into this, so it's a velocity vector:
	// DEBUG_PRINT( "v_look_dir: " );
	// vec3d_print( &v_look_dir, 1 );

    vec3d_t v_forward = vectorMul( &v_look_dir, floatingToFixed( 4.0f * f_elapsed_time ) );

	// DEBUG_PRINT( "f_yaw: %f\n", (float) fixedToFloating(f_yaw) );
#else
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
	// DEBUG_PRINT( "v_look_dir: " );
	// vec3d_print( &v_look_dir, 1 );

    vec3d_t v_forward = vectorMul( &v_look_dir, 4.0f * f_elapsed_time );

	// DEBUG_PRINT( "f_yaw: %f\n", (float) f_yaw );
#endif

	// DEBUG_PRINT( "v_camera: " );
	// vec3d_print( &v_camera, 1 );
	// DEBUG_PRINT( "v_forward: " );
	// vec3d_print( &v_forward, 1 );

    // My trial of implementing left and right strafing
    // I've changd some control bindings
    // pls at least change the name of this vector:
#ifdef USE_FIXED_POINT_ARITHMETIC
	vec3d_t temp_vUp = { floatingToFixed(0), floatingToFixed(1), floatingToFixed(0), floatingToFixed(1) };
#else
    vec3d_t temp_vUp = { 0, 1, 0, 1 };
#endif
    // Calculate the right direction:
    vec3d_t v_right_raw = vectorCrossProduct( &temp_vUp, &v_forward );
    //					  v seems 2 big
#ifdef USE_FIXED_POINT_ARITHMETIC
	vec3d_t v_right = vectorMul( &v_right_raw, floatingToFixed( 64.0f * f_elapsed_time ) );
#else
    vec3d_t v_right = vectorMul( &v_right_raw, 64.0f * f_elapsed_time );
#endif

	// DEBUG_PRINT( "temp_vUp: " );
	// vec3d_print( &temp_vUp, 1 );
	// DEBUG_PRINT( "v_right_raw: " );
	// vec3d_print( &v_right_raw, 1 );
	// DEBUG_PRINT( "v_right: " );
	// vec3d_print( &v_right, 1 );
    
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
#ifdef USE_FIXED_POINT_ARITHMETIC
	vec3d_t v_up = { floatingToFixed(0), floatingToFixed(1), floatingToFixed(0), floatingToFixed(1) };
	vec3d_t v_target = { floatingToFixed(0), floatingToFixed(0), floatingToFixed(1), floatingToFixed(1) };
#else
    vec3d_t v_up = { 0, 1, 0, 1 };
    //vec3d_t v_target = vectorAdd( v_camera, v_look_dir );
    vec3d_t v_target = { 0, 0, 1, 1 };
#endif

	// DEBUG_PRINT( "v_camera: " );
	// vec3d_print( &v_camera, 1 );
	// DEBUG_PRINT( "v_up: " );
	// vec3d_print( &v_up, 1 );
	// DEBUG_PRINT( "v_target: " );
	// vec3d_print( &v_target, 1 );
	
    mat4x4_t mat_camera_rot;
	matrix_makeRotY( &mat_camera_rot, f_yaw );
    v_look_dir = matrix_mulVector( &mat_camera_rot, &v_target );
    v_target = vectorAdd( &v_camera, &v_look_dir );

    mat4x4_t mat_camera;
	matrix_pointAt( &mat_camera, &v_camera, &v_target, &v_up );

    // Make view matrix from camera:
    matrix_quickInverse( mat_view, &mat_camera );

	
	// DEBUG_PRINT( "mat_camera_rot:\n" );
	// printMatrix( &mat_camera_rot );	
	// DEBUG_PRINT( "v_look_dir: " );
	// vec3d_print( &v_look_dir, 1 );
	// DEBUG_PRINT( "v_target: " );
	// vec3d_print( &v_target, 1 );

	// DEBUG_PRINT( "mat_camera:\n" );
	// printMatrix( &mat_camera );	
	// DEBUG_PRINT( "mat_view:\n" );
	// printMatrix( &mat_view );	
}
#endif

// 
// Project a 3D mesh on 2D surface,
// save transformed vertices in the mesh structure
// and fill mesh's visible edge array
// with visible vertx IDs.
// mesh 		- pointer to a mesh to be processed
// mat_view		- pointer to a "view matrix"
// rot_angle_x	- angle of rotation of the mesh (in degrees) in X axis
// rot_angle_z	- angle of rotation of the mesh (in degrees) in Z axis
// 
#ifdef USE_CAMERA
void processMesh( mesh_t* mesh, mat4x4_t* mat_view, flp_t rot_angle_x, flp_t rot_angle_z ){
#else
void processMesh( mesh_t* mesh, flp_t rot_angle_x, flp_t rot_angle_z ){
#endif
	// Apply rotation in Z and X axis:
#ifdef USE_FIXED_POINT_ARITHMETIC
	mat4x4_t matRotZ, matRotX;
	matrix_makeRotZ( &matRotZ, floatingToFixed( rot_angle_z ) );
    matrix_makeRotX( &matRotX, floatingToFixed( rot_angle_x ) );
	// DEBUG_PRINT( "rot_angle_x = %f\n", (float) rot_angle_x );
	// DEBUG_PRINT( "rot_angle_z = %f\n", (float) rot_angle_z );
	// printMatrix( &matRotX );
	// printMatrix( &matRotZ );
#else
    mat4x4_t matRotZ, matRotX;
	matrix_makeRotZ( &matRotZ, rot_angle_z );
    matrix_makeRotX( &matRotX, rot_angle_x );
#endif
    // Translation matrix
    // mat4x4 matTrans;
    // mesh.matTrans = matrixMakeTranslation( 0.0f, 0.0f, 2.0f );
    mat4x4_t matTrans;
	matrix_makeTranslation( &matTrans, mesh->pos.x, mesh->pos.y, mesh->pos.z );

    // World matrix:
    // mat4x4 matWorld;
    mat4x4_t matWorld, mat_temp;
	matrix_makeIdentity( &matWorld );
	matrix_makeIdentity( &mat_temp );
    matrix_mulMatrix( &mat_temp, &matRotZ, &matRotX );     // order important
    matrix_mulMatrix( &matWorld, &mat_temp, &matTrans );   // second

	// DEBUG_PRINT( "matRotZ:\n" );
	// printMatrix( &matRotZ );
	// DEBUG_PRINT( "matRotX:\n" );
	// printMatrix( &matRotX );
	// DEBUG_PRINT( "matTrans:\n" );
	// printMatrix( &matTrans );
	// DEBUG_PRINT( "MatWorld:\n" );
	// printMatrix( &matWorld );
	// DEBUG_PRINT( "MatProj:\n" );
	// printMatrix( &mat_proj );

	arrfree( mesh->transformedVertices );
	// mesh->transformedVertices = NULL;
	// Not really needed, just to reserve amount
    // of RAM space exactly equal to our needs
	// arrsetcap( mesh->transformedVertices, mesh->vertex_cnt );

	// Transform every vertex of the mesh
	// DEBUG_PRINT( "mesh->transformedVertices:\n" );

    for( size_t i=0; i < mesh->vertex_cnt; i++ ){
		vec3d_t vertex = mesh->vertices[i];
        // Convert to world space
        vec3d_t v_transformed = matrix_mulVector( &matWorld, &vertex );
		// vec3d_print( &v_transformed, 1 );
		arrput( mesh->transformedVertices, v_transformed );
    }

	// DEBUG_PRINT( "Transformed vertices.\n" );

	// Vector storing only IDs of visible faces
	arrfree( mesh->visFaceIDs );

	// DEBUG_PRINT( "Freed mesh->visFaceIDs.\n" );
	// mesh->visFaceIDs = NULL;
	// arrsetcap( mesh->visFaceIDs, mesh->face_cnt );	// is it needed?

	/* // Debug print:

	DEBUG_PRINT( "mesh->vertices:\n" );
	for( size_t i=0; i < mesh->vertex_cnt; i++ )
		DEBUG_PRINT( "%f %f %f\n", (float) mesh->vertices[i].x, (float) mesh->vertices[i].y, (float) mesh->vertices[i].z );
	
	DEBUG_PRINT( "mesh.vertices (%lld):\n", (size_t) mesh->vertex_cnt );
	for( size_t i=0; i < mesh->vertex_cnt; i++ ){
		vec3d_t v = mesh->vertices[i];
		DEBUG_PRINT( "v %lld: ", (int) i );
		vec3d_print( &v, 1 );
	}

	DEBUG_PRINT( "mesh.faces (%lld):\n", (size_t) mesh->face_cnt );
	for( size_t i=0; i < mesh->face_cnt; i++ ){
		polygon_t poly = mesh->faces[i];
		DEBUG_PRINT( "Face %lld: ", (int) i );
		polygon_print( &poly );
	}

	DEBUG_PRINT( "mesh.transformedVertices (%lld):\n", (size_t) arrlen(mesh->transformedVertices) );
	for( size_t i=0; i < arrlen(mesh->transformedVertices); i++ ){
		vec3d_t v = mesh->transformedVertices[i];
		vec3d_print( &v, 1 );
	}
	*/

	// Add each visible face's ID to the visFaceIDs vector
    int face_id = 0;
    for( size_t i=0; i < mesh->face_cnt; i++ ){
#ifdef RENDER_VISIBLE_ONLY
		polygon_t face = mesh->faces[i];	// idk how but it works
		// DEBUG_PRINT( "Processing face %d: ", (size_t) i );
		// polygon_print( &face );

        // Use cross-product to get surface normal:
        vec3d_t normal, edge1, edge2;
        edge1 = vectorSub( &mesh->transformedVertices[face.p[2]],
                           &mesh->transformedVertices[face.p[1]] );
        edge2 = vectorSub( &mesh->transformedVertices[face.p[0]],
                           &mesh->transformedVertices[face.p[1]] );
		// DEBUG_PRINT( "Face %d edge1: ", (size_t) i );
		// vec3d_print( &edge1, true );
		// DEBUG_PRINT( "Face %d edge2: ", (size_t) i );
		// vec3d_print( &edge2, true );
		// The normal vector is a cross product of two edges of the face:
        normal = vectorCrossProduct( &edge1, &edge2 );
		// DEBUG_PRINT( "Face %d normal: ", (size_t) i );
		// vec3d_print( &normal, true );
        // Normalise the normal (normalised length = 1.0 unit):
        normal = vectorNormalise( &normal );

		// DEBUG_PRINT( "Face %d normal normalised: ", (size_t) i );
		// vec3d_print( &normal, true );

        // Get ray from the face to the camera:
        vec3d_t v_camera_ray = vectorSub( &mesh->transformedVertices[face.p[0]],
                                      &v_camera );
		// DEBUG_PRINT( "Face %d v_camera_ray: ", (size_t) i );
		// vec3d_print( &v_camera_ray, true );

        // If the ray is aligned with normal, then face is visible:
        // Use '>' to render in inverse (like a view from inside):
#ifdef USE_FIXED_POINT_ARITHMETIC
		// vectorDotProduct() is of floating point type for now
		// DEBUG_PRINT( "Face %d dot product: %f\n", i, (float) fixedToFloating(vectorDotProduct( &normal, &v_camera_ray ) ) );
		if( vectorDotProduct( &normal, &v_camera_ray ) < floatingToFixed(0.0f) ){
#else
		if( vectorDotProduct( &normal, &v_camera_ray ) < 0.0f ){
#endif
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
	vmap_free( &(mesh->vert2DSpaceMap) );
	// mesh->vert2DSpaceMap = NULL;

	// DEBUG_PRINT( "Freed vert2DSpaceMap. Transforming vertices (%lld)...\n", (size_t) mesh->vertex_cnt );

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
    for( uint16_t curr_vert_id = 0; curr_vert_id < mesh->vertex_cnt; curr_vert_id++ ){
#endif
            // Transform current vertex
#ifdef USE_CAMERA
            // Convert world space to view space
            vec3d_t vertViewed = matrix_mulVector( mat_view, &mesh->transformedVertices[curr_vert_id] );
            vec3d_t vertProjected = matrix_mulVector( &mat_proj, &vertViewed );
#else   // NOT USING CAMERA
			// DEBUG_PRINT( "vertex to use: " );
			// vec3d_print( &mesh->transformedVertices[curr_vert_id], 1 );

            vec3d_t vertProjected = matrix_mulVector( &mat_proj, &mesh->transformedVertices[curr_vert_id] );

			// DEBUG_PRINT( "vertProjected initially: " );
			// vec3d_print( &vertProjected, 1 );
#endif
            // Scale into view, we moved the normalising into cartesian space
            // out of the matrix.vector function from the previous versions, so
            // do this manually:
            vertProjected = vectorDiv( &vertProjected, vertProjected.w );

			// DEBUG_PRINT( "vertProjected after div: " );
			// vec3d_print( &vertProjected, 1 );
	
            // But since the result is in range of -1 to 1,
            // we have to scale it into view:
#ifdef USE_FIXED_POINT_ARITHMETIC
            vec3d_t vOffsetView = { floatingToFixed(1), floatingToFixed(1), floatingToFixed(0), floatingToFixed(0) };
#else
			vec3d_t vOffsetView = { 1, 1, 0, 0 };
#endif

            vertProjected = vectorAdd( &vertProjected, &vOffsetView );

			// DEBUG_PRINT( "vertProjected after add: " );
			// vec3d_print( &vertProjected, 1 );

#ifdef USE_FIXED_POINT_ARITHMETIC
			vertProjected.x = fixedMul( vertProjected.x, floatingToFixed( 0.5f * (flp_t)SCREEN_WIDTH ) );
            vertProjected.y = fixedMul( vertProjected.y, floatingToFixed( 0.5f * (flp_t)SCREEN_HEIGHT ) );
			// DEBUG_PRINT( "vertProjected after mul: " );
			// vec3d_print( &vertProjected, 1 );
#else
            vertProjected.x *= 0.5f * (flp_t)SCREEN_WIDTH;
            vertProjected.y *= 0.5f * (flp_t)SCREEN_HEIGHT;
#endif
            // Add this vertex and its ID into the map
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
			vmap_insert( &mesh->vert2DSpaceMap,
							 curr_vert_id,
							 &vertProjected, 1 );
#else
			vmap_insert( &mesh->vert2DSpaceMap,
							 curr_vert_id,
							 &vertProjected );
#endif
	}

	// DEBUG_PRINT( "mesh->vert2DSpaceMap:\n" );
	// vmap_printInorder( &(mesh->vert2DSpaceMap) );

	// DEBUG_PRINT( "Freeing vis_edge_vec...\n" );

	// Each entry is 4x int:
    // 1: start_vert_ID
    // 2: end_vert_ID
    // 3: num_of_faces_which_the_edge_belongs_to (1-2)
    // 4: ID_of_1st_face
	if( mesh->vis_edge_vec != NULL )
    	arrfree(mesh->vis_edge_vec);
    // Reserve max number of visible edges? (9 for cube)
    //vis_edge_vec.reserve( 9*sizeof(int) );

	// DEBUG_PRINT( "Filling vis_edge_vec...\n" );

	// Fill the visible edge vector
    // For each visible face
    for( size_t i=0; i < arrlen(mesh->visFaceIDs); i++ ){

        size_t vert_cnt = mesh->faces[ mesh->visFaceIDs[i] ].p_count;
        
        // For each point
        for( size_t vert_id = 0; vert_id < vert_cnt; vert_id++ ){

			size_t edge_start_v_id, edge_end_v_id;
			bool presence_flag = 0;

			edge_start_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[vert_id];
			
			if( vert_id == vert_cnt - 1 )
				// For the last edge
				edge_end_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[0];
			else 
				edge_end_v_id = mesh->faces[ mesh->visFaceIDs[i] ].p[vert_id + 1];

            // For the whole visible edge array
            for( size_t j=0; j < arrlen(mesh->vis_edge_vec); j+=4 ){                
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
}

// 
// Draw mesh on given render window.
// Replace drawLine() function at the end with any function
// drawing a straight line on your output device
// 
void drawMesh( mesh_t* mesh, sfRenderWindow* render_window ){
    // Draw every visible edge
    // Debug (commented): print the visible edge array
    for( int i=0; i<arrlen(mesh->vis_edge_vec); i+=4 ){
#ifdef DRAW_CONTOUR_ONLY
        if( mesh->vis_edge_vec[ i + 2 ] > 1 )
            continue;
#endif

		// DEBUG_PRINT( "Looking for verts %d and %d...\n", (int) mesh->vis_edge_vec[i], (int) mesh->vis_edge_vec[i+1] );
        
		int vert1_ID = mesh->vis_edge_vec[i];
		int vert2_ID = mesh->vis_edge_vec[i+1];
		vec3d_t vertProjected1, vertProjected2;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
		bool v1_vis_flg, v2_vis_flg;

		bool ret1 = vmap_find( &(mesh->vert2DSpaceMap), vert1_ID, &vertProjected1, &v1_vis_flg );
#else
		bool ret1 = vmap_find( &(mesh->vert2DSpaceMap), vert1_ID, &vertProjected1 );
#endif
		if( !ret1 ){
			DEBUG_PRINT( "Error: in mesh->vert2DSpaceMap could not find the first vertex of ID=%d\n",
					(int) vert1_ID );
			return;
		}

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
		bool ret2 = vmap_find( &(mesh->vert2DSpaceMap), vert2_ID, &vertProjected2, &v2_vis_flg );
#else
		bool ret2 = vmap_find( &(mesh->vert2DSpaceMap), vert2_ID, &vertProjected2 );
#endif
		if( !ret2 ){
			DEBUG_PRINT( "Error: in mesh->vert2DSpaceMap could not find the second vertex of ID=%d\n",
					(int) vert2_ID );
			return;
		}

		// vec3d_print( &vertProjected1, 0 );
		// vec3d_print( &vertProjected2, 1 );

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
#endif // VERTEX_DOT_DEBUG

#ifdef VERTEX_ID_DEBUG
        stringstream s1;

        s1 << vert1_ID;
        // putText( s1, vertProjected1.x, vertProjected1.y, 8, sf::Color::Yellow,  render_window );
        // s2 << vert2_ID;  // one is enough, because the printed values are the same and overlap on the screen
        // putText( s2, vertProjected2.x, vertProjected2.y, 8, sf::Color::Green,  render_window );

        // cout<<"\nEdge "<<i/4<<": "<<mesh.vis_edge_vec[i]<<" "<<mesh.vis_edge_vec[i+1];//<<" "<<mesh.vis_edge_vec[i+2]<<" "<<mesh.vis_edge_vec[i+3];
        // cout<<": ("<<vertProjected1.x<<", "<<vertProjected1.y<<")-("<<vertProjected2.x<<", "<<vertProjected2.y<<")";
#endif // VERTEX_ID_DEBUG

#ifdef COLOUR_MONOCHROME
		drawLine( vertProjected1.x, vertProjected1.y,
                  vertProjected2.x, vertProjected2.y,
                  sfWhite, render_window );
#else
#ifdef COLOUR_SINGLE_BYTE

#if COLOUR_DEPTH == 1
		uint8_t r = ((mesh->edge_colour.rgb) >> 6) & 0b0000001;
		uint8_t g = ((mesh->edge_colour.rgb) >> 4) & 0b0000001;
		uint8_t b = ((mesh->edge_colour.rgb) >> 2) & 0b0000001;
		drawLine( vertProjected1.x, vertProjected1.y,
                  vertProjected2.x, vertProjected2.y,
                  sfColor_fromRGB( r*255, g*255, b*255 ), render_window );
#ifdef USE_FILLED_MESHES
		r = ((mesh->fill_colour.rgb) >> 6) & 0b0000001;
		g = ((mesh->fill_colour.rgb) >> 4) & 0b0000001;
		b = ((mesh->fill_colour.rgb) >> 2) & 0b0000001;
		// Fill the mesh
#endif
#else
		uint8_t r = ((mesh->edge_colour.rgb) >> 6) & 0b0000011;
		uint8_t g = ((mesh->edge_colour.rgb) >> 4) & 0b0000011;
		uint8_t b = ((mesh->edge_colour.rgb) >> 2) & 0b0000011;
		drawLine( vertProjected1.x, vertProjected1.y,
                  vertProjected2.x, vertProjected2.y,
                  sfColor_fromRGB( r*(255/3), g*(255/3), b*(255/3) ), render_window );
#ifdef USE_FILLED_MESHES
		r = ((mesh->fill_colour.rgb) >> 6) & 0b0000011;
		g = ((mesh->fill_colour.rgb) >> 4) & 0b0000011;
		b = ((mesh->fill_colour.rgb) >> 2) & 0b0000011;

		// Fill the mesh
#endif
#endif
#else
        drawLine( vertProjected1.x, vertProjected1.y,
                  vertProjected2.x, vertProjected2.y,
                  sfColor_fromRGB( mesh->edge_colour.r, mesh->edge_colour.g, mesh->edge_colour.b ), render_window );
#ifdef USE_FILLED_MESHE
		// Fill the mesh
#endif
#endif
#endif
    }
}

void update3DFrame( sfRenderWindow* renderWindow, flp_t f_elapsed_time, flp_t* f_theta ){
	// mat4x4 matRotZ, matRotX;
    // Current angle:
    if( animate )
        *f_theta += 1.0 * f_elapsed_time;
	
	// DEBUG_PRINT( "f_theta = %f,\f_elapsed_time = %f\n", (float) (*f_theta), (float) f_elapsed_time );

#ifdef USE_FIXED_POINT_ARITHMETIC
	vec3d_t pos1 = { floatingToFixed(0.0f), floatingToFixed(0.0f), floatingToFixed(2.0f), floatingToFixed(0.0f) };
    // vec3d_t pos2 = { floatingToFixed(0.0f), floatingToFixed(0.0f), floatingToFixed(4.0f), floatingToFixed(0.0f) };
#else
    vec3d_t pos1 = { 0.0f, 0.0f, 2.0f, 0.0f };
    // vec3d_t pos2 = { 0.0f, 0.0f, 4.0f, 0.0f };
#endif

	mesh.pos = pos1;
	
#ifdef USE_CAMERA
	mat4x4_t mat_view;
	computeViewMatrix( &mat_view, f_elapsed_time );
    processMesh( &mesh, &mat_view, *f_theta, (*f_theta)*0.5 );
#else
	processMesh( &mesh, *f_theta, (*f_theta)*0.5 );
#endif

	drawMesh( &mesh, renderWindow );
}