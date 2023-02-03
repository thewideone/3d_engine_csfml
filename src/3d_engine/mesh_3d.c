#include "mesh_3d.h"

#include "stb_ds.h" // for dynmic arrays

#include <stdlib.h>      // for reading files
#include <stdio.h>

void mesh_makeEmpty( mesh_t* mesh ){
    vec3d_makeEmpty( &mesh->pos );
    mesh->face_cnt = 0;
    mesh->faces = NULL;
    // mesh->matRotX = matrixMakeRotX();
    mesh->transformedVertices = NULL;
    mesh->vert2DSpaceMap = NULL;
    mesh->vertex_cnt = 0;
    mesh->vertices = NULL;
    mesh->vis_edge_vec = NULL;
    mesh->visFaceIDs = NULL;
}

void mesh_free( mesh_t* mesh ){
    for( int i=0; i < mesh->face_cnt; i++ )
        polygon_free( &mesh->faces[i] );
    arrfree( mesh->faces );
    arrfree( mesh->transformedVertices );
    vmap_free( mesh->vert2DSpaceMap );
    arrfree( mesh->vertices );
    arrfree( mesh->vis_edge_vec );
    arrfree( mesh->visFaceIDs );
}

bool mesh_loadFromObjFile( mesh_t* mesh, char* filename ){
    
    FILE* file_ptr;
    file_ptr = fopen( filename, "r" );

    if( file_ptr == NULL ){
        printf( "Error: in mesh_loadFromObjFile(): could not open the file\n" );
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
                printf( "Error: in mesh_loadFromObjFile() could not read 4 values from line\n" );
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
                printf( "Error: in mesh_loadFromObjFile() could not read first letter from line\n" );
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

// 
// Load mesh from program memory.
// mesh                 - pointer to a mesh object
// vert_array       - array of vertex values
// face_array       - array of vertex IDs for each face
// vert_cnt         - number of vertices
// faxe_cnt         - number of faces
// fixed_face_size  - number of vertices if each face of the mesh
//                    has got the same
// 
bool mesh_loadFromProgmem( mesh_t* mesh, const rtnl_t vert_array[], const size_t face_array[], const size_t vert_cnt, const size_t face_cnt, const size_t fixed_face_size ){
    // Load vertices
    // printf( "Loading vertices..." );
    for( size_t i=0; i<vert_cnt; i++ ){
        vec3d_t v;
        v.x = vert_array[ i%3 ];
        v.y = vert_array[ i%3+1 ];
        v.z = vert_array[ i%3+2 ];
        #ifdef USE_FIXED_POINT_ARITHMETIC
        v.w = floatingToFixed( (flp_t)(1) );
        #else
        v.w = 1;    // default value for 'w' member
        #endif
        arrput( mesh->vertices, v );
        // mesh->vertex_cnt++;
    }
    mesh->vertex_cnt = vert_cnt;

    // printf( "done. mesh->vertex_cnt = %d\n\n", mesh->vertex_cnt );

    // Load faces
    // printf( "\nLoading faces...\n" );
    if( fixed_face_size ){
        for( size_t f_itr=0; f_itr < face_cnt; f_itr++ ){
            polygon_t face = polygonMakeEmpty();

            for( size_t v_itr=0; v_itr < fixed_face_size; v_itr++ )
                arrput( face.p, face_array[ f_itr % fixed_face_size + v_itr ] );
        }
    }
    else {
        size_t v_itr=0;

        for( size_t f_itr=0; f_itr < face_cnt; f_itr++ ){
            // printf( "Face %d:\n", f_itr );

            polygon_t face = polygonMakeEmpty();
            
            // printf( "\tPushing vertices: " );
            while( face_array[ v_itr ] != F_VID_SEP ){
                // printf( "%d (v%d), ", v_itr, face_array[ v_itr ] );

                arrput( face.p, face_array[ v_itr ] );
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

void mesh_printVisFaceIDs( mesh_t* mesh ){
    // cout << "IDs of visible faces (" << visFaceIDs.size() << " in total):" << endl;
    // for( int i=0; i < visFaceIDs.size(); i++ )
    //     cout << visFaceIDs[i] << ", ";
    // cout << endl;

    printf( "IDs of visible faces (%lld in total):\n", arrlen(mesh->visFaceIDs) );
    for( int i=0; i<arrlen(mesh->visFaceIDs); i++ )
        printf( "%d, ", mesh->visFaceIDs[i] );
    printf( "\n" );
}

void mesh_printVisEdgeVec( mesh_t* mesh ){
    // cout << vis_edge_vec.size()/4 << " visible edges:" << endl;
    // cout << "E1 E2 C S" << endl;
    // for( int i=0; i < vis_edge_vec.size(); i+=4 ){
    //     cout << vis_edge_vec[i] << ", " << vis_edge_vec[i+1] << ", " << vis_edge_vec[i+2] << ", " << vis_edge_vec[i+3] << endl;
    // }
    printf( "%lld visible edges:\n", arrlen(mesh->vis_edge_vec)/4 );
    printf( "E1 E2 C S\n" );
    for( int i=0; i<arrlen(mesh->vis_edge_vec); i+=4 )
        printf( "%d, %d, %d, %d\n", mesh->vis_edge_vec[i], mesh->vis_edge_vec[i+1], mesh->vis_edge_vec[2], mesh->vis_edge_vec[i+3] );
}