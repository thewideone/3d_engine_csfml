#include "mesh3d.h"

#include "stb_ds.h" // for dynamic arrays

void mesh_makeEmpty( mesh3d_t* mesh ){
#ifndef COLOUR_MONOCHROME
    // Default full white
#ifdef COLOUR_SINGLE_BYTE
    mesh->edge_colour.rgb = COLOUR_WHITE;
#ifdef USE_FILLED_MESHES
    mesh->fill_colour.rgb = COLOUR_WHITE;
#endif
#else
    mesh->edge_colour.r = 0xff;
    mesh->edge_colour.g = 0xff;
    mesh->edge_colour.b = 0xff;
#ifdef USE_FILLED_MESHES
    mesh->fill_colour.r = 0xff;
    mesh->fill_colour.g = 0xff;
    mesh->fill_colour.b = 0xff;
#endif
#endif
#endif

    vec3d_makeEmpty( &mesh->pos );

#ifdef USE_FIXED_POINT_ARITHMETIC
    mesh->yaw = floatingToFixed( 0.0f );
    mesh->pitch = floatingToFixed( 0.0f );
	mesh->roll = floatingToFixed( 0.0f );
#else
    mesh->yaw = 0.0f;
    mesh->pitch = 0.0f;
    mesh->roll = 0.0f;
#endif

    mesh->face_cnt = 0;
    mesh->faces = NULL;
    mesh->transformedVertices = NULL;
    // mesh->vert2DSpaceMap = NULL;
    // mesh->vert2DSpaceMap = 
    // vmap_makeEmpty( &(mesh->vert2DSpaceMap) );
    mesh->vert2DSpaceMap.root = NULL;
    mesh->vert2DSpaceMap.size = 0;

    mesh->vertex_cnt = 0;
    mesh->vertices = NULL;
    mesh->vis_edge_vec = NULL;
    mesh->visFaceIDs = NULL;
}

void mesh_free( mesh3d_t* mesh ){
    // printf( "Freeing polygons...\n" );
    for( size_t i=0; i < mesh->face_cnt; i++ )
        polygon_free( &mesh->faces[i] );
    // printf( "Freeing face array...\n" );
    arrfree( mesh->faces );
    // printf( "Freeing transformedVertices...\n" );
    arrfree( mesh->transformedVertices );
    // printf( "Freeing vert2DSpaceMap...\n" );
    vmap_free( &(mesh->vert2DSpaceMap) );
    // printf( "Freeing vertices...\n" );
    arrfree( mesh->vertices );
    // printf( "Freeing vis_edge_vec...\n" );
    arrfree( mesh->vis_edge_vec );
    // printf( "Freeing visFaceIDs...\n" );
    arrfree( mesh->visFaceIDs );
    // printf( "Done.\n" );
}

// 
// Load mesh from program memory.
// mesh                 - pointer to a mesh object
// vert_arr       - array of vertex values
// face_arr       - array of vertex IDs for each face
// vert_cnt         - number of vertices
// faxe_cnt         - number of faces
// fixed_face_size  - number of vertices if each face of the mesh
//                    has got the same
// 
bool mesh_loadFromProgmem( mesh3d_t* mesh, const rtnl_t vert_arr[], const size_t face_arr[], const size_t vert_cnt, const size_t face_cnt, const size_t fixed_face_size ){
    // Load vertices
    // printf( "Loading vertices..." );

    size_t idx=0;
    for( size_t v_itr=0; v_itr < vert_cnt; v_itr++ ){
        
        vec3d_t v;
        v.x = vert_arr[ idx ];
        v.y = vert_arr[ idx+1 ];
        v.z = vert_arr[ idx+2 ];
        #ifdef USE_FIXED_POINT_ARITHMETIC
        v.w = floatingToFixed( (flp_t)(1) );
        #else
        v.w = 1;    // default value for 'w' member
        #endif

        // printf( " -> v%lld: ", v_itr );
        // printf( "(%d, %d, %d)", idx, idx+1, idx+2 );
        // vec3d_printAsFixed( &v, true );
        
        arrput( mesh->vertices, v );
        idx+=3;
    }
    mesh->vertex_cnt = vert_cnt;

    // printf( "done. mesh->vertex_cnt = %d\n\n", mesh->vertex_cnt );

    // Load faces
    // printf( "\nLoading faces...\n" );
    if( fixed_face_size ){
        for( size_t f_itr=0; f_itr < face_cnt; f_itr++ ){
            polygon_t face = polygonMakeEmpty();

            for( size_t v_itr=0; v_itr < fixed_face_size; v_itr++ )
                arrput( face.p, face_arr[ f_itr % fixed_face_size + v_itr ] );
        }
    }
    else {
        size_t v_itr=0;

        for( size_t f_itr=0; f_itr < face_cnt; f_itr++ ){
            // printf( "Face %d:\n", f_itr );

            polygon_t face = polygonMakeEmpty();
            
            // printf( "\tPushing vertices: " );
            while( face_arr[ v_itr ] != F_VID_SEP ){
                // printf( "%d (v%d), ", v_itr, face_arr[ v_itr ] );

                arrput( face.p, face_arr[ v_itr ] );
                face.p_count++;
                
                v_itr++;
            }
            // printf( "\n" );
            
            arrput( mesh->faces, face );
            v_itr++;
        }
    }
    mesh->face_cnt = face_cnt;

    // printf( "Loading faces done. mesh->face_cnt = %d\n", mesh->face_cnt );

    return true;
}

#ifdef USE_LOADING_FROM_OBJ
#include "external_dependencies.h"
// 
// Load mesh from .obj file.
// mesh     - mesh structure to be loaded into
// filename - name of the .obj file
// 
bool mesh_loadFromObjFile( mesh3d_t* mesh, char* filename ){
    
    FILE* file_ptr;
    file_ptr = fopen( filename, "r" );

    if( file_ptr == NULL ){
        DEBUG_PRINT( "Error: in mesh_loadFromObjFile(): could not open the file\n" );
        return false;
    }

    // Create a line buffer anc read a line from file
    // we assume that no line exceeds 128 characters

    char* line_buf = NULL;
    size_t line_len;
    size_t buf_size = 128;

    // printf( "Starting to read the file...\n" );
    while( ( line_len = getline( &line_buf, &buf_size, file_ptr ) ) != -1 ){

        // Each line of .obj starts with
        // 'v' for vertex or 'f' for face
        char letter;

        // We assume we're not generating normals, textures etc.:
        // Add a vertex to the local cache
        if( line_buf[0] == 'v' ){
            vec3d_t v;

            #ifdef USE_FIXED_POINT_ARITHMETIC
            flp_t x, y, z;
            int values_read = sscanf( line_buf, "%c %f %f %f", &letter, &x, &y, &z );
            v.x = floatingToFixed( x );
            v.y = floatingToFixed( y );
            v.z = floatingToFixed( z );
            v.w = floatingToFixed( (flp_t)(1) );    // default value for 'w' member
            #else
            int values_read = sscanf( line_buf, "%c %f %f %f", &letter, &v.x, &v.y, &v.z );
            v.w = 1;    // default value for 'w' member
            #endif

            if( values_read != 4 ){
                DEBUG_PRINT( "Error: in mesh_loadFromObjFile() could not read 4 values from line\n" );
                return false;
            }
            // printf( "Putting vertex...\n" );
            arrput( mesh->vertices, v );
            // printf( "Incrementing vertex count...\n" );
            mesh->vertex_cnt++;
        }
        // Add a face to the mesh
        if( line_buf[0] == 'f' ){
            
            if( sscanf( line_buf, "%c ", &letter ) != 1 ){
                DEBUG_PRINT( "Error: in mesh_loadFromObjFile() could not read first letter from line\n" );
                return false;
            }

            // Read what points make up our face (n-gon):
            polygon_t new_face = polygonMakeEmpty();
            // int point_count=0;

            // For all numbers in this line, we read IDs of vertices
            int point_ID;
            int chars_read_total = 0;   // total of chars read from the buffer
            int chars_read; // records the last position read in the buffer

            // printf( "Face: " );
            // printf( "Scanning from face-string...\n" );
            while( 1 == sscanf( line_buf + chars_read_total,
                   "%*[^0123456789]%d%n", &point_ID, &chars_read ) ){
                
                chars_read_total += chars_read;

                // Indexes of .obj start from 1 and ours from 0,
                // so subtract 1
                point_ID -= 1;

                // printf( "%d ", point_ID );

                arrput( new_face.p, point_ID );
                // point_count++;
                new_face.p_count++;
            }
            // printf( "\ttotal: %d\n", chars_read_total );
            
            // new_face.p_count = point_count;

            // Apply a random colour:
            //new_face.colour = sf::Color( 255, 255, 255 );
            // int random_r =  100+( std::rand() % ( 255-100+1 ) );
            // int random_g =  100+( std::rand() % ( 255-100+1 ) );
            // int random_b =  100+( std::rand() % ( 255-100+1 ) );
            // new_face.colour = sf::Color( random_r, random_g, random_b );

            // Add the face to our mesh
            arrput( mesh->faces, new_face );
            mesh->face_cnt++;

            // Destroy the face, copy of which has just been added
            // to the mesh
            // polygon_free( &new_face );
            // free( &new_face.p );
            // arrfree( new_face.p );
        }
    }
    // transformedVertices.reserve( vertex_cnt );
    //visFaceIDs.reserve( face_cnt );

    return true;
}
#endif


// 
// Set position of mesh.
// mesh     - mesh pointer
// x, y, z  - 3D space coordinates
// 
void mesh_setPos( mesh3d_t* mesh, rtnl_t x, rtnl_t y, rtnl_t z ){
    mesh->pos.x = x;
    mesh->pos.y = y;
    mesh->pos.z = z;
}
// 
// Set position of mesh.
// mesh     - mesh pointer
// pos      - 3D space vector of coordinates
// 
void mesh_setPosByVec( mesh3d_t* mesh, vec3d_t* pos ){
    mesh->pos.x = pos->x;
    mesh->pos.y = pos->y;
    mesh->pos.z = pos->z;
}
// 
// Set rotation of mesh.
// mesh              - mesh pointer
// yaw, pitch, roll  - angles in radians
// 
void mesh_setRot( mesh3d_t* mesh, rtnl_t yaw, rtnl_t pitch, rtnl_t roll ){
    mesh->yaw = yaw;
    mesh->pitch = pitch;
    mesh->roll = roll;
}
// 
// Set rotation of mesh.
// mesh - mesh pointer
// rot  - 3D vector of angles in radians
// 
void mesh_setRotByVec( mesh3d_t* mesh, vec3d_t* rot ){
    mesh->yaw = rot->x;
    mesh->pitch = rot->y;
    mesh->roll = rot->z;
}

// 
// Move mesh.
// mesh     - mesh pointer
// x, y, z  - 3D space coordinates
// 
void mesh_move( mesh3d_t* mesh, rtnl_t x, rtnl_t y, rtnl_t z ){
    mesh->pos.x += x;
    mesh->pos.y += y;
    mesh->pos.z += z;
}
// 
// Move mesh by vector.
// mesh     - mesh pointer
// pos      - 3D space vector of coordinates
// 
void mesh_moveByVec( mesh3d_t* mesh, vec3d_t* pos ){
    mesh->pos.x += pos->x;
    mesh->pos.y += pos->y;
    mesh->pos.z += pos->z;
}
// 
// Rotate mesh.
// mesh              - mesh pointer
// yaw, pitch, roll  - angles in radians
// 
void mesh_rotate( mesh3d_t* mesh, rtnl_t yaw, rtnl_t pitch, rtnl_t roll ){
    mesh->yaw += yaw;
    mesh->pitch += pitch;
    mesh->roll += roll;
}
// 
// Rotate mesh by vector of angles.
// mesh - mesh pointer
// rot  - 3D vector of angles in radians
// 
void mesh_rotateByVec( mesh3d_t* mesh, vec3d_t* rot ){
    mesh->yaw += rot->x;
    mesh->pitch += rot->y;
    mesh->roll += rot->z;
}

void mesh_printVisFaceIDs( mesh3d_t* mesh ){
    // cout << "IDs of visible faces (" << visFaceIDs.size() << " in total):" << endl;
    // for( int i=0; i < visFaceIDs.size(); i++ )
    //     cout << visFaceIDs[i] << ", ";
    // cout << endl;

    // printf( "IDs of visible faces (%lld in total):\n", arrlen(mesh->visFaceIDs) );
    STDO_STR( "IDs of visible faces (" );
    STDO_SIZET( arrlen(mesh->visFaceIDs) );
    STDO_STR( " in total):\n" );

    for( uint16_t i=0; i<arrlen(mesh->visFaceIDs); i++ ){
        // printf( "%d, ", mesh->visFaceIDs[i] );
        STDO_SIZET( mesh->visFaceIDs[i] );
        STDO_STR( ", " );
    }
    STDO_CHR( '\n' );
}

void mesh_printVisEdgeVec( mesh3d_t* mesh ){
    // cout << vis_edge_vec.size()/4 << " visible edges:" << endl;
    // cout << "E1 E2 C S" << endl;
    // for( int i=0; i < vis_edge_vec.size(); i+=4 ){
    //     cout << vis_edge_vec[i] << ", " << vis_edge_vec[i+1] << ", " << vis_edge_vec[i+2] << ", " << vis_edge_vec[i+3] << endl;
    // }
    // printf( "%lld visible edges:\n", arrlen(mesh->vis_edge_vec)/4 );
    STDO_UINT16( (uint16_t) arrlen(mesh->vis_edge_vec)/4 );
    STDO_STR( " visible edges:\n" );
    STDO_STR( "E1 E2 C S\n" );
    for( uint16_t i=0; i<arrlen(mesh->vis_edge_vec); i+=4 ){
        // printf( "%d, %d, %d, %d\n", mesh->vis_edge_vec[i], mesh->vis_edge_vec[i+1], mesh->vis_edge_vec[2], mesh->vis_edge_vec[i+3] );
        STDO_SIZET( mesh->vis_edge_vec[i] );
        STDO_STR( ", " );
        STDO_SIZET( mesh->vis_edge_vec[i+1] );
        STDO_STR( ", " );
        STDO_SIZET( mesh->vis_edge_vec[i+2] );
        STDO_STR( ", " );
        STDO_SIZET( mesh->vis_edge_vec[i+3] );
        STDO_CHR( '\n' );
    }
}

#ifndef COLOUR_MONOCHROME
void mesh_setEdgeColour( mesh3d_t* mesh, colour_t colour ){
    mesh->edge_colour = colour;
}
#ifdef USE_FILLED_MESHES
void mesh_setFillColour( mesh3d_t* mesh, colour_t colour ){
    mesh->fill_colour = colour;
}
#endif
#ifdef COLOUR_SINGLE_BYTE
void mesh_setEdgeColourByValue( mesh3d_t* mesh, uint8_t colour ){
    mesh->edge_colour.rgb = colour;
}
#ifdef USE_FILLED_MESHES
void mesh_setFillColourByValue( mesh3d_t* mesh, uint8_t colour ){
    mesh->fill_colour.rgb = colour;
}
#endif
#else
void mesh_setEdgeColourByValue( mesh3d_t* mesh, uint8_t r, uint8_t g, uint8_t b ){
    mesh->edge_colour.r = r;
    mesh->edge_colour.g = g;
    mesh->edge_colour.b = b;
}
#ifdef USE_FILLED_MESHES
void mesh_setFillColourByValue( mesh3d_t* mesh, uint8_t r, uint8_t g, uint8_t b ){
    mesh->fill_colour.r = r;
    mesh->fill_colour.g = g;
    mesh->fill_colour.b = b;
}
#endif
#endif
#endif