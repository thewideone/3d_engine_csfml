#include "mesh_3d.h"

#include "stb_ds.h" // for dynmic arrays

#include <stdlib.h>      // for reading files
#include <stdio.h>

mesh_t mesh_makeEmpty(){
    static mesh_t mesh;
    mesh.face_cnt = 0;
    mesh.faces = NULL;
    // mesh.matRotX = matrixMakeRotX();
    mesh.transformedVertices = NULL;
    mesh.vert2DSpaceMap = NULL;
    mesh.vertex_cnt = 0;
    mesh.vertices = NULL;
    mesh.vis_edge_vec = NULL;
    mesh.visFaceIDs = NULL;

    return mesh;
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
            int values_read = sscanf( line_buf, "%c %f %f %f", &letter, &v.x, &v.y, &v.z );
            v.w = 1;    // default value for 'w' member

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
