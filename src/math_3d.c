#include <stdio.h>
#include "pc_routines.h" // for reading lines of files

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "math_3d.h"
#include "graphics.h"

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
    return (vec3d_t){ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, 1 };
}
// Subtract:
vec3d_t vectorSub( vec3d_t* v1, vec3d_t* v2 ){
    // vec3d_t v;
    // v.x = v1->x - v2->x;
    // v.y = v1->y - v2->y;
    // v.z = v1->z - v2->z;
    // v.w = 0;
    // return v;
    return (vec3d_t){ v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 1 };
}
// Multiply:
vec3d_t vectorMul( vec3d_t* v, float k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x * k;
    // v_ret.y = v->y * k;
    // v_ret.z = v->z * k;
    // v_ret.w = 0;
    // return v_ret;
    return (vec3d_t){ v->x * k, v->y * k, v->z * k, v->w };
}
// Divide:
vec3d_t vectorDiv( vec3d_t* v, float k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x / k;
    // v_ret.y = v->y / k;
    // v_ret.z = v->z / k;
    // v_ret.w = 0;
    // return v_ret;
    if( k == 0.0 ){
        printf( "Error: in vectorDiv() division by 0. Aborting." );
        exit(0);    // maybe replace with something better
    }
    return (vec3d_t){ v->x / k, v->y / k, v->z / k, v->w };
}
// Dot product:
float vectorDotProduct( vec3d_t* v1, vec3d_t* v2 ){
    return (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z) * (v2->z);
}
// Cross product:
vec3d_t vectorCrossProduct( vec3d_t* v1, vec3d_t* v2 ){
    static vec3d_t v;
	v.x = (v1->y) * (v2->z) - (v1->z) * (v2->y);
	v.y = (v1->z) * (v2->x) - (v1->x) * (v2->z);
	v.z = (v1->x) * (v2->y) - (v1->y) * (v2->x);
    v.w = 1;
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
    return (vec3d_t){ v->x / l, v->y / l, v->z / l, v->w };
}

mat4x4_t matrix_makeEmpty(){
    static mat4x4_t matrix;

    // int j=0;
    // for( int i=0; i<4; i++ )
    //     for( j=0; j<4; j++ )
    //         matrix.m[i][j] = 0;
    memset( matrix.m, 0, sizeof( matrix.m ) );

    return matrix;
}

mat4x4_t matrix_makeIdentity(){
    mat4x4_t matrix = matrix_makeEmpty();
    
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;

    return matrix;
}

mat4x4_t matrix_makeRotZ( float fAngleRad ){
    static mat4x4_t matrix;
    matrix.m[0][0] = cosf( fAngleRad );
    matrix.m[0][1] = sinf( fAngleRad );
    matrix.m[1][0] = -sinf( fAngleRad );
    matrix.m[1][1] = cosf( fAngleRad );
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4_t matrix_makeRotX( float fAngleRad ){
    static mat4x4_t matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf( fAngleRad );
    matrix.m[1][2] = sinf( fAngleRad );
    matrix.m[2][1] = -sinf( fAngleRad );
    matrix.m[2][2] = cosf( fAngleRad );
    matrix.m[3][3] = 1.0f;
    return matrix;
}

#ifdef USE_CAMERA
mat4x4_t matrix_makeRotY(float fAngleRad){
	static mat4x4_t matrix;
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][2] = sinf(fAngleRad);
	matrix.m[2][0] = -sinf(fAngleRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
#endif

// Matrix operator functions:
// Multiply matrix by vector ( m - input matrix, i - input vector ):
vec3d_t matrix_mulVector( mat4x4_t* m, vec3d_t* i ){
    vec3d_t v;
	v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
	v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
	v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
    v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
	
	return v;
}

mat4x4_t matrix_makeTranslation( float x, float y, float z ){
    static mat4x4_t matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

mat4x4_t matrix_makeProjection( float fFovDegrees, float fAspectRatio, float fNear, float fFar ){
    // Fov coefficient in radians
    float fFovRad = 1.0f / tanf( fFovDegrees * 0.5f / 180.0f * 3.14159f );

    static mat4x4_t matrix;
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / ( fFar - fNear );
    matrix.m[3][2] = ( -fFar * fNear ) / ( fFar - fNear );
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

mat4x4_t matrix_mulMatrix( mat4x4_t* m1, mat4x4_t* m2 ){
    static mat4x4_t matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
	return matrix;
}

#ifdef USE_CAMERA
//  pos - where the object should be
//  target - "forward" vector for that object
//  up - "up" vector
mat4x4_t matrix_pointAt( vec3d_t* pos, vec3d_t* target, vec3d_t* up ){
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
	static mat4x4_t matrix;
	matrix.m[0][0] = newRight.x;	    matrix.m[0][1] = newRight.y;	    matrix.m[0][2] = newRight.z;	    matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		    matrix.m[1][1] = newUp.y;		    matrix.m[1][2] = newUp.z;		    matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	    matrix.m[2][1] = newForward.y;	    matrix.m[2][2] = newForward.z;	    matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos->x;			matrix.m[3][1] = pos->y;			matrix.m[3][2] = pos->z;			matrix.m[3][3] = 1.0f;
	
    return matrix;
}

// Works only for Rotation/Translation Matrices
mat4x4_t matrix_quickInverse(mat4x4_t* m){
	static mat4x4_t matrix;
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

void vec3d_print( vec3d_t* v, int new_line_flag ){
    printf( "\t%f %f %f %f", v->x, v->y, v->z, v->w );
    if( new_line_flag )
        printf( "\n" );
}

polygon_t polygonMakeEmpty( void ){
    polygon_t poly;
    poly.p = NULL;
    poly.p_count = 0;
    
    return poly;
}

void polygon_free( polygon_t* poly ){
    arrfree( poly->p );
    poly->p = NULL;
    poly->p_count = 0;
}

void polygon_print( polygon_t* poly ){
    if( poly == NULL || poly->p_count == 0 )
        printf( "Empty\n" );
    else {
        for( int i=0; i < poly->p_count; i++ ){
            printf( "%d ", poly->p[i] );
        }
        printf( "\n" );
    }
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

/*
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
*/