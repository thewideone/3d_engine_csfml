#include <stdio.h>
#include <stdlib.h>  // for reading files

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "math_3d.h"

#include "graphics.h"

vmap_t* vmap_createNode( int key, vec3d_t v, int vis_flag ){
    vmap_t* new_node_ptr = (vmap_t*) malloc( sizeof( vmap_t ) );

    new_node_ptr->key = key;
    new_node_ptr->v = v;    // shallow copy is enough for this type
    new_node_ptr->visible = vis_flag;
    new_node_ptr->left = NULL;
    new_node_ptr->right = NULL;

    return new_node_ptr;
}

// Search the map
// root - a tree to be searched, if the function
//        returns 1
// key  - key of the node of interest
vmap_t* vmap_search( vmap_t* root, int key ){
    vmap_t* found_node_ptr = NULL;

    while( root != NULL ){
        if( key > root->key )
            root = root->right;
        else if( key < root->key )
            root = root->left;
        // If the key is found, break
        else{
            found_node_ptr = root;
            break;
        }
    }
    return found_node_ptr;
}

// Insert node to map
// node     - to what tree to insert
// key      - key of
// v        - vector to be added
// vis_flag - visibility flag of the vector
vmap_t* vmap_insertNode( vmap_t* root, int key, vec3d_t v, int vis_flag ){
    vmap_t* new_node_ptr = vmap_createNode( key, v, vis_flag );

    vmap_t* search_head_ptr = root;
    vmap_t* search_tail_ptr = NULL;

    while( search_head_ptr != NULL ){
        search_tail_ptr = search_head_ptr;
        if( key < search_head_ptr->key )
            search_head_ptr = search_head_ptr->left;
        else
            search_head_ptr = search_head_ptr->right;
    }

    // If the root is NULL (the tree is empty)
    // The new node is the root node
    if( search_tail_ptr == NULL ){
        search_tail_ptr = new_node_ptr;
    }

    // If the new key is less than the leaf node key
    // Assign the new node to be its left child
    else if( key < search_tail_ptr->key )
        search_tail_ptr->left = new_node_ptr;
    
    // Else
    // Assign the new node to be its right child
    else
        search_tail_ptr->right = new_node_ptr;
    
    // Return the pointer to where the new node is
    // inserted
    return search_tail_ptr;
}

// Print binary tree map in order of ascending keys
void vmap_print( vmap_t* root ){
    if (root == NULL)
        return;
    else {
        vmap_print(root->left);
        printf( "%d ", root->key);
        vmap_print(root->right);
    }
}

// Delete whole map
void vmap_free( vmap_t* root ){
    if (root == NULL)
        return;
    else{
        vmap_free(root->left);
        vmap_free(root->right);
        printf("Freeing node of key %d\n", root->key );
        free( root );
        root = NULL;
    }
}

// void printVec3D( vec3d_t* v, char* name ){
//     char vec_val[30];
//     sprintf( vec_val, "{%f, %f, %f, %f}", v->x, v->y, v->z, v->w );
//     strcat( name, vec_val );
//     strcat( name, "\n" );
//     printf( name );
// }

void printMatrix( mat4x4_t* mat ){
    for( int i=0; i<4; i++ ){
        for( int j=0; j<4; j++ )
			printf( "%f ", mat->m[i][j] );
		printf( "\n" );
	}
}

float minF( float a, float b ){
    return !( b < a ) ? a : b;
}

float maxF( float a, float b ){
    return ( a < b ) ? b : a;
}

// Vector operator functions:
// Add:
vec3d_t vectorAdd( vec3d_t* v1, vec3d_t* v2 ){
    // vec3d_t v;
    // v.x = v1->x + v2->x;
    // v.y = v1->y + v2->y;
    // v.z = v1->z + v2->z;
    // v.w = 0;    // idk what to put in here
    // return v;
    return (vec3d_t){ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, 0 };
}
// Subtract:
vec3d_t vectorSub( vec3d_t* v1, vec3d_t* v2 ){
    // vec3d_t v;
    // v.x = v1->x - v2->x;
    // v.y = v1->y - v2->y;
    // v.z = v1->z - v2->z;
    // v.w = 0;
    // return v;
    return (vec3d_t){ v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 0 };
}
// Multiply:
vec3d_t vectorMul( vec3d_t* v, float k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x * k;
    // v_ret.y = v->y * k;
    // v_ret.z = v->z * k;
    // v_ret.w = 0;
    // return v_ret;
    return (vec3d_t){ v->x * k, v->y * k, v->z * k, 0 };
}
// Divide:
vec3d_t vectorDiv( vec3d_t* v, float k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x / k;
    // v_ret.y = v->y / k;
    // v_ret.z = v->z / k;
    // v_ret.w = 0;
    // return v_ret;
    return (vec3d_t){ v->x / k, v->y / k, v->z / k, 0 };
}
// Dot product:
float vectorDotProduct( vec3d_t* v1, vec3d_t* v2 ){
    return (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z) * (v2->z);
}
// Cross product:
vec3d_t vectorCrossProduct( vec3d_t* v1, vec3d_t* v2 ){
    vec3d_t v;
	v.x = (v1->y) * (v2->z) - (v1->z) * (v2->y);
	v.y = (v1->z) * (v2->x) - (v1->x) * (v2->z);
	v.z = (v1->x) * (v2->y) - (v1->y) * (v2->x);
	return v;
}
// Length:
float vectorLength( vec3d_t* v ){
    return sqrtf( vectorDotProduct(v, v) );
}
// Normalise:
vec3d_t vectorNormalise( vec3d_t* v ){
    float l = vectorLength( v );
    // vec3d_t v_ret;
    // v_ret.x = v->x / l;
    // v_ret.y = v->y / l;
    // v_ret.z = v->z / l;
    // v_ret.w = 0;
    // return v_ret;
    return (vec3d_t){ v->x / l, v->y / l, v->z / l, 0 };
}

// Matrix operator functions:
// Multiply matrix by vector ( m - input matrix, i - input vector ):
vec3d_t matrixMulVector( mat4x4_t* m, vec3d_t* i ){
    vec3d_t v;
	v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
	v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
	v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
    v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
	
	return v;
}

mat4x4_t matrix_makeEmpty(){
    mat4x4_t matrix;

    // int j=0;
    // for( int i=0; i<4; i++ )
    //     for( j=0; j<4; j++ )
    //         matrix.m[i][j] = 0;
    memset( matrix.m, 0, sizeof( matrix.m ) );

    return matrix;
}

mat4x4_t matrixMakeIdentity(){
    mat4x4_t matrix = matrix_makeEmpty();
    
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;

    return matrix;
}

mat4x4_t matrixMakeRotZ( float fAngleRad ){
    mat4x4_t matrix;
    matrix.m[0][0] = cosf( fAngleRad );
    matrix.m[0][1] = sinf( fAngleRad );
    matrix.m[1][0] = -sinf( fAngleRad );
    matrix.m[1][1] = cosf( fAngleRad );
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrixMakeRotX( float fAngleRad ){
    mat4x4_t matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf( fAngleRad );
    matrix.m[1][2] = sinf( fAngleRad );
    matrix.m[2][1] = -sinf( fAngleRad );
    matrix.m[2][2] = cosf( fAngleRad );
    matrix.m[3][3] = 1.0f;
    return matrix;
}

#ifdef USE_CAMERA
mat4x4_t matrixMakeRotY(float fAngleRad){
	mat4x4_t matrix;
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][2] = sinf(fAngleRad);
	matrix.m[2][0] = -sinf(fAngleRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
#endif

mat4x4_t matrixMakeTranslation( float x, float y, float z ){
    mat4x4_t matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4_t matrixMakeProjection( float fFovDegrees, float fAspectRatio, float fNear, float fFar ){
    // Fov coefficient in radians
    float fFovRad = 1.0f / tanf( fFovDegrees * 0.5f / 180.0f * 3.14159f );

    mat4x4_t matrix;
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / ( fFar - fNear );
    matrix.m[3][2] = ( -fFar * fNear ) / ( fFar - fNear );
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

mat4x4_t matrixMulMatrix( mat4x4_t* m1, mat4x4_t* m2 ){
    mat4x4_t matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
	return matrix;
}

#ifdef USE_CAMERA
//  pos - where the object should be
//  target - "forward" vector for that object
//  up - "up" vector
mat4x4_t matrixPointAt( vec3d_t* pos, vec3d_t* target, vec3d_t* up ){
    // Calculate new forward direction:
    vec3d_t newForward = vectorSub( target, pos );
    newForward = vectorNormalise( &newForward );

    // Calculate new up direction:
    vec3d_t a = vectorMul( &newForward, vectorDotProduct( up, &newForward ) );
    vec3d_t newUp = vectorSub( up, &a );
    newUp = vectorNormalise( &newUp );

    // Calculate new right direction:
    vec3d_t newRight = vectorCrossProduct( &newUp, &newForward );

    // Construct Dimensioning and Translation Matrix	
	mat4x4_t matrix;
	matrix.m[0][0] = newRight.x;	    matrix.m[0][1] = newRight.y;	    matrix.m[0][2] = newRight.z;	    matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		    matrix.m[1][1] = newUp.y;		    matrix.m[1][2] = newUp.z;		    matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	    matrix.m[2][1] = newForward.y;	    matrix.m[2][2] = newForward.z;	    matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos->x;			matrix.m[3][1] = pos->y;			matrix.m[3][2] = pos->z;			matrix.m[3][3] = 1.0f;
	
    return matrix;
}

// Works only for Rotation/Translation Matrices
mat4x4_t matrixQuickInverse(mat4x4_t* m){
	mat4x4_t matrix;
	matrix.m[0][0] = m->m[0][0]; matrix.m[0][1] = m->m[1][0]; matrix.m[0][2] = m->m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m->m[0][1]; matrix.m[1][1] = m->m[1][1]; matrix.m[1][2] = m->m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m->m[0][2]; matrix.m[2][1] = m->m[1][2]; matrix.m[2][2] = m->m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m->m[3][0] * matrix.m[0][0] + m->m[3][1] * matrix.m[1][0] + m->m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m->m[3][0] * matrix.m[0][1] + m->m[3][1] * matrix.m[1][1] + m->m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m->m[3][0] * matrix.m[0][2] + m->m[3][1] * matrix.m[1][2] + m->m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
#endif

polygon_t polygonMakeEmpty( void ){
    polygon_t poly;
    poly.p = NULL;
    poly.p_count = 0;
    
    return poly;
}

void printPolygon( polygon_t* poly ){
    for( int i=0; i < poly->p_count; i++ ){
        printf( "%d ", poly->p[i] );
    }
    printf( "\n" );
}

mesh_t mesh_makeEmpty(){
    mesh_t mesh;
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

// Line equation calculated from v1 and v2,
// v3 tested if is lying on it
// The collinearity test has a threshold of 1 pixel (usage of floor())
bool areCollinear( vec3d_t* v1, vec3d_t* v2, vec3d_t* v3, bool object_num, sfRenderWindow* windowToDrawOnto ){
    float m, b; // line equation coefficients
    float y_expected;

    m = (v2->y - v1->y) / (v2->x - v1->x);

    // v1.y = m*v1.x + b
    b = v1->y - m*v1->x;

    y_expected = m*v3->x + b;

    // stringstream ss;
    // ss << v3.y << endl << y_expected;
    char str[16];
    sprintf( str, "%f\n%f", v3->y, y_expected );
    if( object_num )
        putText( str, 50, 180, 10, sfMagenta, windowToDrawOnto );
    else
        putText( str, 10, 180, 10, sfMagenta, windowToDrawOnto );

    if( floor(v3->y) == floor(y_expected) || floor(v3->y) == floor(y_expected) + 1.0 || floor(v3->y) == floor(y_expected) - 1.0 )
        return true;
    return false;
}

void computePlaneCoefficients( float* A, float* B, float* C, float* D, vec3d_t* vert1, vec3d_t* vert2, vec3d_t* vert3 ){
    *A = vert1->y*(vert2->z - vert3->z) + vert2->y*(vert3->z - vert1->z) + vert3->y*(vert1->z - vert2->z);
    *B = vert1->z*(vert2->x - vert3->x) + vert2->z*(vert3->x - vert1->x) + vert3->z*(vert1->x - vert2->x);
    *C = vert1->x*(vert2->y - vert3->y) + vert2->x*(vert3->y - vert1->y) + vert3->x*(vert1->y - vert2->y);
    *D = -1*(*A)*vert1->x - (*B)*vert1->y - (*C)*vert1->z;
}

void calcFaceBoundaries( mesh_t* mesh, int face_id, int* min_x, int* max_x, int* min_y, int* max_y ){
    // min_x = SCREEN_WIDTH;  max_x = 0;
    // min_y = SCREEN_HEIGHT; max_y = 0;

    // For each vertex of this face
    for( int vert_ctnr = 0;
             vert_ctnr <  mesh->faces[ mesh->visFaceIDs[face_id] ].p_count;
             vert_ctnr++ ){
            
        int vert_id = mesh->faces[ mesh->visFaceIDs[face_id] ].p[ vert_ctnr ];

        // vec3d_t curr_vert = mesh.vert2DSpaceMap.find( vert_id )->second;
        vmap_t* found_node = vmap_search( mesh->vert2DSpaceMap, vert_id );

        if( found_node != NULL ){
            vec3d_t curr_vert = found_node->v;

            if( curr_vert.x < *min_x )   *min_x = curr_vert.x;
            if( curr_vert.y < *min_y )   *min_y = curr_vert.y;
            if( curr_vert.x > *max_x )   *max_x = curr_vert.x;
            if( curr_vert.y > *max_y )   *max_y = curr_vert.y;
        }
        else{
            printf( "Error: in math.c: calcFaceBoundaries(): could not find vertex of id %d\n", vert_id );
            return;
        }
    }
}