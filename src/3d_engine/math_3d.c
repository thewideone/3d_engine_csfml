#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "math_3d.h"
#include "graphics.h"

#ifdef USE_FIXED_POINT_ARITHMETIC
fxp_t floatingToFixed( flp_t num ){
    return (fxp_t)( num * (flp_t)( 1 << FP_DP ) + ( num >= 0 ? 0.5 : -0.5 ) );
}
flp_t fixedToFloating( fxp_t num ){
    return (flp_t)(num) / (flp_t)( 1 << FP_DP );
}
fxp_t fixedMul( fxp_t a, fxp_t b ){
    return ( (fxp2_t)(a) * (fxp2_t)(b) ) >> FP_DP;
}
fxp_t fixedDiv( fxp_t a, fxp_t b ){
    return ( (fxp2_t)(a) << FP_DP ) / (fxp2_t)(b);
}
#endif

void vec3d_makeEmpty( vec3d_t* v ){
#ifdef USE_FIXED_POINT_ARITHMETIC
    v->x = floatingToFixed( 0.0f );
    v->y = floatingToFixed( 0.0f );
    v->z = floatingToFixed( 0.0f );
    v->w = floatingToFixed( 1.0f );
#else
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
    v->w = 1.0f;
#endif
}

void printMatrix( mat4x4_t* mat ){
    for( int i=0; i<4; i++ ){
        for( int j=0; j<4; j++ ){
#ifdef USE_FIXED_POINT_ARITHMETIC
            STDO_FLT( fixedToFloating(mat->m[i][j]) );
            STDO_CHR( ' ' );
#else
            STDO_FLT( mat->m[i][j] );
            STDO_CHR( ' ' );
#endif
        }

        STDO_CHR( '\n' );
	}
}

rtnl_t minF( rtnl_t a, rtnl_t b ){
    return !( b < a ) ? a : b;
}

rtnl_t maxF( rtnl_t a, rtnl_t b ){
    return ( a < b ) ? b : a;
}

// Vector operator functions:
// Add:
vec3d_t vectorAdd( vec3d_t* v1, vec3d_t* v2 ){
    // vec3d_t v;
    // v.x = v1->x + v2->x;
    // v.y = v1->y + v2->y;
    // v.z = v1->z + v2->z;
    // v.w = 0;    // idk what to put in here <- 1! put 1!
    // return v;
#ifdef USE_FIXED_POINT_ARITHMETIC
    return (vec3d_t){ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, floatingToFixed(1) };
#else
    return (vec3d_t){ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, 1 };
#endif
}
// Subtract:
vec3d_t vectorSub( vec3d_t* v1, vec3d_t* v2 ){
    // vec3d_t v;
    // v.x = v1->x - v2->x;
    // v.y = v1->y - v2->y;
    // v.z = v1->z - v2->z;
    // v.w = 0;
    // return v;
#ifdef USE_FIXED_POINT_ARITHMETIC
    return (vec3d_t){ v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, floatingToFixed(1) };
#else
    return (vec3d_t){ v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 1 };
#endif
}
// Multiply:
vec3d_t vectorMul( vec3d_t* v, rtnl_t k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x * k;
    // v_ret.y = v->y * k;
    // v_ret.z = v->z * k;
    // v_ret.w = 0;
    // return v_ret;
#ifdef USE_FIXED_POINT_ARITHMETIC
    return (vec3d_t){ fixedMul(v->x, k), fixedMul(v->y, k), fixedMul(v->z, k), v->w };
#else
    return (vec3d_t){ v->x * k, v->y * k, v->z * k, v->w };
#endif
}
// Divide:
vec3d_t vectorDiv( vec3d_t* v, rtnl_t k ){
    // vec3d_t v_ret;
    // v_ret.x = v->x / k;
    // v_ret.y = v->y / k;
    // v_ret.z = v->z / k;
    // v_ret.w = 0;
    // return v_ret;
    if( k == 0.0 ){
        DEBUG_PRINT( "Error: in vectorDiv() division by 0. Aborting." );
        exit(0);    // maybe replace with something better
    }
#ifdef USE_FIXED_POINT_ARITHMETIC
    return (vec3d_t){ fixedDiv(v->x, k), fixedDiv(v->y, k), fixedDiv(v->z, k), v->w };
#else
    return (vec3d_t){ v->x / k, v->y / k, v->z / k, v->w };
#endif
}
// Dot product:
rtnl_t vectorDotProduct( vec3d_t* v1, vec3d_t* v2 ){
#ifdef USE_FIXED_POINT_ARITHMETIC
    return fixedMul(v1->x, v2->x) + fixedMul( v1->y, v2->y ) + fixedMul( v1->z, v2->z );
#else
    return (v1->x)*(v2->x) + (v1->y)*(v2->y) + (v1->z) * (v2->z);
#endif
}
// Cross product:
vec3d_t vectorCrossProduct( vec3d_t* v1, vec3d_t* v2 ){
    static vec3d_t v;
#ifdef USE_FIXED_POINT_ARITHMETIC
    v.x = fixedMul( v1->y, v2->z ) - fixedMul( v1->z, v2->y );
	v.y = fixedMul( v1->z, v2->x ) - fixedMul( v1->x, v2->z );
	v.z = fixedMul( v1->x, v2->y ) - fixedMul( v1->y, v2->x );
    v.w = floatingToFixed(1);
#else
	v.x = (v1->y) * (v2->z) - (v1->z) * (v2->y);
	v.y = (v1->z) * (v2->x) - (v1->x) * (v2->z);
	v.z = (v1->x) * (v2->y) - (v1->y) * (v2->x);
    v.w = 1;
#endif
	return v;
}
// Length:
rtnl_t vectorLength( vec3d_t* v ){
#ifdef USE_FIXED_POINT_ARITHMETIC
    return floatingToFixed( sqrt( fixedToFloating( vectorDotProduct(v, v) ) ) );
#else
    return sqrtf( vectorDotProduct(v, v) );
#endif
}
// Normalise:
vec3d_t vectorNormalise( vec3d_t* v ){
    rtnl_t l = vectorLength( v );
#ifdef USE_FIXED_POINT_ARITHMETIC 
    if( l == floatingToFixed( 0.0 ) )
        l = floatingToFixed( 0.0001 );
    // DEBUG_PRINT( "vectorNormalise() l = %f\n", fixedToFloating(l) );
// #else
//     DEBUG_PRINT( "vectorNormalise() l = %f\n", l );
#endif
    // vec3d_t v_ret;
    // v_ret.x = v->x / l;
    // v_ret.y = v->y / l;
    // v_ret.z = v->z / l;
    // v_ret.w = 0;
    // return v_ret;
#ifdef USE_FIXED_POINT_ARITHMETIC
    return (vec3d_t){ fixedDiv( v->x, l ), fixedDiv( v->y, l), fixedDiv( v->z, l ), v->w };
#else
    return (vec3d_t){ v->x / l, v->y / l, v->z / l, v->w };
#endif
}

void matrix_makeEmpty( mat4x4_t* matrix ){
    // int j=0;
    // for( int i=0; i<4; i++ )
    //     for( j=0; j<4; j++ )
    //         matrix.m[i][j] = 0
#ifdef USE_FIXED_POINT_ARITHMETIC
    memset( matrix->m, floatingToFixed(0), sizeof( matrix->m ) );
#else
    memset( matrix->m, 0, sizeof( matrix->m ) );
#endif
}

void matrix_makeIdentity( mat4x4_t* matrix ){
    matrix_makeEmpty( matrix );

#ifdef USE_FIXED_POINT_ARITHMETIC
    matrix->m[0][0] = floatingToFixed(1.0f);
    matrix->m[1][1] = floatingToFixed(1.0f);
    matrix->m[2][2] = floatingToFixed(1.0f);
    matrix->m[3][3] = floatingToFixed(1.0f);
#else
    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
#endif
}

void matrix_makeRotZ( mat4x4_t* matrix, rtnl_t fAngleRad ){
    matrix_makeEmpty( matrix );
#ifdef USE_FIXED_POINT_ARITHMETIC
    matrix->m[0][0] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
    matrix->m[0][1] = floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
    matrix->m[1][0] = -floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
    matrix->m[1][1] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
    matrix->m[2][2] = floatingToFixed( 1.0f );
    matrix->m[3][3] = floatingToFixed( 1.0f );
#else
    matrix->m[0][0] = cosf( fAngleRad );
    matrix->m[0][1] = sinf( fAngleRad );
    matrix->m[1][0] = -sinf( fAngleRad );
    matrix->m[1][1] = cosf( fAngleRad );
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
#endif
}

void matrix_makeRotX( mat4x4_t* matrix, rtnl_t fAngleRad ){
    matrix_makeEmpty( matrix );
#ifdef USE_FIXED_POINT_ARITHMETIC
    matrix->m[0][0] = floatingToFixed( 1.0f );
    matrix->m[1][1] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
    matrix->m[1][2] = floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
    matrix->m[2][1] = -floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
    matrix->m[2][2] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
    matrix->m[3][3] = floatingToFixed( 1.0f );
#else
    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = cosf( fAngleRad );
    matrix->m[1][2] = sinf( fAngleRad );
    matrix->m[2][1] = -sinf( fAngleRad );
    matrix->m[2][2] = cosf( fAngleRad );
    matrix->m[3][3] = 1.0f;
#endif
}

#ifdef USE_CAMERA
void matrix_makeRotY( mat4x4_t* matrix, rtnl_t fAngleRad ){
    matrix_makeEmpty( matrix );
#ifdef USE_FIXED_POINT_ARITHMETIC
    matrix->m[0][0] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
	matrix->m[0][2] = floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
	matrix->m[2][0] = -floatingToFixed( sinf( fixedToFloating( fAngleRad ) ) );
	matrix->m[1][1] = floatingToFixed( 1.0f );
	matrix->m[2][2] = floatingToFixed( cosf( fixedToFloating( fAngleRad ) ) );
	matrix->m[3][3] = floatingToFixed( 1.0f );
#else
	matrix->m[0][0] = cosf(fAngleRad);
	matrix->m[0][2] = sinf(fAngleRad);
	matrix->m[2][0] = -sinf(fAngleRad);
	matrix->m[1][1] = 1.0f;
	matrix->m[2][2] = cosf(fAngleRad);
	matrix->m[3][3] = 1.0f;
#endif
}
#endif

// Matrix operator functions:
// Multiply matrix by vector ( m - input matrix, i - input vector ):
vec3d_t matrix_mulVector( mat4x4_t* m, vec3d_t* i ){
    vec3d_t v;
#ifdef USE_FIXED_POINT_ARITHMETIC
    v.x = fixedMul( i->x, m->m[0][0] ) + fixedMul( i->y, m->m[1][0] ) + fixedMul( i->z, m->m[2][0] ) + fixedMul( i->w, m->m[3][0] );
    v.y = fixedMul( i->x, m->m[0][1] ) + fixedMul( i->y, m->m[1][1] ) + fixedMul( i->z, m->m[2][1] ) + fixedMul( i->w, m->m[3][1] );
    v.z = fixedMul( i->x, m->m[0][2] ) + fixedMul( i->y, m->m[1][2] ) + fixedMul( i->z, m->m[2][2] ) + fixedMul( i->w, m->m[3][2] );
    v.w = fixedMul( i->x, m->m[0][3] ) + fixedMul( i->y, m->m[1][3] ) + fixedMul( i->z, m->m[2][3] ) + fixedMul( i->w, m->m[3][3] );
#else
	v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
	v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
	v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
    v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
#endif
	return v;
}

void matrix_makeTranslation( mat4x4_t* matrix, rtnl_t x, rtnl_t y, rtnl_t z ){
    matrix_makeEmpty( matrix );
#ifdef USE_FIXED_POINT_ARITHMETIC
    matrix->m[0][0] = floatingToFixed( 1.0f );
    matrix->m[1][1] = floatingToFixed( 1.0f );
    matrix->m[2][2] = floatingToFixed( 1.0f );
    matrix->m[3][3] = floatingToFixed( 1.0f );
#else
    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
#endif
    matrix->m[3][0] = x;
    matrix->m[3][1] = y;
    matrix->m[3][2] = z;
}

void matrix_makeProjection( mat4x4_t* matrix, rtnl_t fFovDegrees, rtnl_t fAspectRatio, rtnl_t fNear, rtnl_t fFar ){
    matrix_makeEmpty( matrix );
#ifdef USE_FIXED_POINT_ARITHMETIC
    // Fov coefficient in radians
    rtnl_t fFovRad = fixedDiv( floatingToFixed( 1.0f ), floatingToFixed( tanf( fixedToFloating( fFovDegrees ) * 0.5f / 180.0f * 3.14159f ) ) );
    
    matrix->m[0][0] = fixedMul( fAspectRatio, fFovRad );
    matrix->m[1][1] = fFovRad;
    matrix->m[2][2] = fixedDiv( fFar, ( fFar - fNear ) );
    matrix->m[3][2] = fixedDiv( fixedMul( -fFar, fNear ), ( fFar - fNear ) );
    matrix->m[2][3] = floatingToFixed( 1.0f );
    matrix->m[3][3] = floatingToFixed( 0.0f );
#else
    // Fov coefficient in radians
    rtnl_t fFovRad = 1.0f / tanf( fFovDegrees * 0.5f / 180.0f * 3.14159f );
    
    matrix->m[0][0] = fAspectRatio * fFovRad;
    matrix->m[1][1] = fFovRad;
    matrix->m[2][2] = fFar / ( fFar - fNear );
    matrix->m[3][2] = ( -fFar * fNear ) / ( fFar - fNear );
    matrix->m[2][3] = 1.0f;
    matrix->m[3][3] = 0.0f;
#endif
}

void matrix_mulMatrix( mat4x4_t* out_m, mat4x4_t* m1, mat4x4_t* m2 ){
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
#ifdef USE_FIXED_POINT_ARITHMETIC
            out_m->m[r][c] = fixedMul( m1->m[r][0], m2->m[0][c] ) + fixedMul( m1->m[r][1], m2->m[1][c] ) + fixedMul( m1->m[r][2], m2->m[2][c] ) + fixedMul( m1->m[r][3], m2->m[3][c] );
#else
			out_m->m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
#endif
}

#ifdef USE_CAMERA
//  pos - where the object should be
//  target - "forward" vector for that object
//  up - "up" vector
void matrix_pointAt( mat4x4_t* out_m, vec3d_t* pos, vec3d_t* target, vec3d_t* up ){
    // Calculate new forward direction:
    vec3d_t newForward = vectorSub( target, pos );
    // DEBUG_PRINT( "newForward init:\n" );
	// vec3d_print( &newForward, 1 );
    newForward = vectorNormalise( &newForward );
    // DEBUG_PRINT( "newForward after norm:\n" );
	// vec3d_print( &newForward, 1 );

    // Calculate new up direction:
    vec3d_t a = vectorMul( &newForward, vectorDotProduct( up, &newForward ) );
    // DEBUG_PRINT( "a:\n" );
	// vec3d_print( &a, 1 );
    vec3d_t newUp = vectorSub( up, &a );
    // DEBUG_PRINT( "newUp:\n" );
	// vec3d_print( &newUp, 1 );
    newUp = vectorNormalise( &newUp );
    // DEBUG_PRINT( "newUp after norm:\n" );
	// vec3d_print( &newUp, 1 );

    // Calculate new right direction:
    vec3d_t newRight = vectorCrossProduct( &newUp, &newForward );
    // DEBUG_PRINT( "newRight:\n" );
	// vec3d_print( &newRight, 1 );

    // Construct Dimensioning and Translation Matrix	
#ifdef USE_FIXED_POINT_ARITHMETIC
    out_m->m[0][0] = newRight.x;	    out_m->m[0][1] = newRight.y;	    out_m->m[0][2] = newRight.z;	    out_m->m[0][3] = floatingToFixed(0.0f);
	out_m->m[1][0] = newUp.x;		    out_m->m[1][1] = newUp.y;		    out_m->m[1][2] = newUp.z;		    out_m->m[1][3] = floatingToFixed(0.0f);
	out_m->m[2][0] = newForward.x;	    out_m->m[2][1] = newForward.y;	    out_m->m[2][2] = newForward.z;	    out_m->m[2][3] = floatingToFixed(0.0f);
	out_m->m[3][0] = pos->x;			out_m->m[3][1] = pos->y;			out_m->m[3][2] = pos->z;			out_m->m[3][3] = floatingToFixed(1.0f);
#else
	out_m->m[0][0] = newRight.x;	    out_m->m[0][1] = newRight.y;	    out_m->m[0][2] = newRight.z;	    out_m->m[0][3] = 0.0f;
	out_m->m[1][0] = newUp.x;		    out_m->m[1][1] = newUp.y;		    out_m->m[1][2] = newUp.z;		    out_m->m[1][3] = 0.0f;
	out_m->m[2][0] = newForward.x;	    out_m->m[2][1] = newForward.y;	    out_m->m[2][2] = newForward.z;	    out_m->m[2][3] = 0.0f;
	out_m->m[3][0] = pos->x;			out_m->m[3][1] = pos->y;			out_m->m[3][2] = pos->z;			out_m->m[3][3] = 1.0f;
#endif
}

// Works only for Rotation/Translation Matrices
void matrix_quickInverse( mat4x4_t* out_mat, mat4x4_t* m ){
	matrix_makeEmpty( out_mat );
#ifdef USE_FIXED_POINT_ARITHMETIC
	out_mat->m[0][0] = m->m[0][0]; out_mat->m[0][1] = m->m[1][0]; out_mat->m[0][2] = m->m[2][0]; out_mat->m[0][3] = floatingToFixed(0.0f);
	out_mat->m[1][0] = m->m[0][1]; out_mat->m[1][1] = m->m[1][1]; out_mat->m[1][2] = m->m[2][1]; out_mat->m[1][3] = floatingToFixed(0.0f);
	out_mat->m[2][0] = m->m[0][2]; out_mat->m[2][1] = m->m[1][2]; out_mat->m[2][2] = m->m[2][2]; out_mat->m[2][3] = floatingToFixed(0.0f);
    out_mat->m[3][0] = -( fixedMul( m->m[3][0], out_mat->m[0][0] ) + fixedMul( m->m[3][1], out_mat->m[1][0] ) + fixedMul( m->m[3][2], out_mat->m[2][0] ) );
    out_mat->m[3][1] = -( fixedMul( m->m[3][0], out_mat->m[0][1] ) + fixedMul( m->m[3][1], out_mat->m[1][1] ) + fixedMul( m->m[3][2], out_mat->m[2][1] ) );
    out_mat->m[3][2] = -( fixedMul( m->m[3][0], out_mat->m[0][2] ) + fixedMul( m->m[3][1], out_mat->m[1][2] ) + fixedMul( m->m[3][2], out_mat->m[2][2] ) );
    out_mat->m[3][3] = floatingToFixed( 1.0f );
#else
    out_mat->m[0][0] = m->m[0][0]; out_mat->m[0][1] = m->m[1][0]; out_mat->m[0][2] = m->m[2][0]; out_mat->m[0][3] = 0.0f;
	out_mat->m[1][0] = m->m[0][1]; out_mat->m[1][1] = m->m[1][1]; out_mat->m[1][2] = m->m[2][1]; out_mat->m[1][3] = 0.0f;
	out_mat->m[2][0] = m->m[0][2]; out_mat->m[2][1] = m->m[1][2]; out_mat->m[2][2] = m->m[2][2]; out_mat->m[2][3] = 0.0f;
	out_mat->m[3][0] = -(m->m[3][0] * out_mat->m[0][0] + m->m[3][1] * out_mat->m[1][0] + m->m[3][2] * out_mat->m[2][0]);
	out_mat->m[3][1] = -(m->m[3][0] * out_mat->m[0][1] + m->m[3][1] * out_mat->m[1][1] + m->m[3][2] * out_mat->m[2][1]);
	out_mat->m[3][2] = -(m->m[3][0] * out_mat->m[0][2] + m->m[3][1] * out_mat->m[1][2] + m->m[3][2] * out_mat->m[2][2]);
	out_mat->m[3][3] = 1.0f;
#endif
    // return matrix;
}
#endif

void vec3d_print( vec3d_t* v, int new_line_flag ){
#ifdef USE_FIXED_POINT_ARITHMETIC
    // printf( "\t%f %f %f %f", fixedToFloating(v->x), fixedToFloating(v->y), fixedToFloating(v->z), fixedToFloating(v->w) );
    STDO_CHR( '\t' );
    STDO_FLT( fixedToFloating(v->x) );
    STDO_CHR( ' ' );
    STDO_FLT( fixedToFloating(v->y) );
    STDO_CHR( ' ' );
    STDO_FLT( fixedToFloating(v->z) );
    STDO_CHR( ' ' );
    STDO_FLT( fixedToFloating(v->w) );
#else
    // printf( "\t%f %f %f %f", v->x, v->y, v->z, v->w );
    STDO_CHR( '\t' );
    STDO_FLT( v->x );
    STDO_CHR( ' ' );
    STDO_FLT( v->y );
    STDO_CHR( ' ' );
    STDO_FLT( v->z );
    STDO_CHR( ' ' );
    STDO_FLT( v->w );
#endif
    if( new_line_flag )
        STDO_CHR( '\n' );
}

#ifdef USE_FIXED_POINT_ARITHMETIC
void vec3d_printAsFixed( vec3d_t* v, int new_line_flag ){
    // printf( "\t%d %d %d %d", v->x, v->y, v->z, v->w );
    STDO_CHR( '\t' );
    STDO_RTNL( v->x );
    STDO_CHR( ' ' );
    STDO_RTNL( v->y );
    STDO_CHR( ' ' );
    STDO_RTNL( v->z );
    STDO_CHR( ' ' );
    STDO_RTNL( v->w );
    if( new_line_flag )
        STDO_CHR( '\n' );
}
#endif

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
        STDO_STR( "Empty\n" );
    else {
        for( int i=0; i < poly->p_count; i++ ){
            // printf( "%d ", poly->p[i] );
            STDO_UINT16( poly->p[i] );
            STDO_CHR( ' ' );
        }
        STDO_CHR( '\n' );
    }
}

// Line equation calculated from v1 and v2,
// v3 tested if is lying on it
// The collinearity test has a threshold of 1 pixel (usage of floor())
#ifdef USE_FIXED_POINT_ARITHMETIC
#warning "Function areCollinear() in math_3d.c not ported to fixed point arithmetic yet!"
#endif
// bool areCollinear( vec3d_t* v1, vec3d_t* v2, vec3d_t* v3, bool object_num, sfRenderWindow* windowToDrawOnto ){
//     rtnl_t m, b; // line equation coefficients
//     rtnl_t y_expected;

//     m = (v2->y - v1->y) / (v2->x - v1->x);

//     // v1.y = m*v1.x + b
//     b = v1->y - m*v1->x;

//     y_expected = m*v3->x + b;

//     // stringstream ss;
//     // ss << v3.y << endl << y_expected;
//     char str[16];
//     sprintf( str, "%f\n%f", v3->y, y_expected );
//     if( object_num )
//         putText( str, 50, 180, 10, sfMagenta, windowToDrawOnto );
//     else
//         putText( str, 10, 180, 10, sfMagenta, windowToDrawOnto );

//     if( floor(v3->y) == floor(y_expected) || floor(v3->y) == floor(y_expected) + 1.0 || floor(v3->y) == floor(y_expected) - 1.0 )
//         return true;
//     return false;
// }

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
            DEBUG_PRINT( "Error: in math.c: calcFaceBoundaries(): could not find vertex of id %d\n", vert_id );
            return;
        }
    }
}
*/