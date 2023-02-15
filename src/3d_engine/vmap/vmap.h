#ifndef _VMAP_H_
#define _VMAP_H_

#include "../config.h"
#include "../math_3d.h"

// TODO:
//  - change implementation to some autobalanced tree like AVL or Red-Black one

// 
// Binary tree map for storing 3D vectors and their visibility flags
// vmap is short for "vector 3D map"
// 

typedef struct vmap_t {
    int key;                // used as vector's ID
    vec3d_t v;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    bool visible;           // set if vector v is visible
#endif
    int8_t bf;              // balance factor
    struct vmap_t* parent;  // for remove I guess
    struct vmap_t* left;
    struct vmap_t* right;
} vmap_t;

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
vmap_t* vmap_createNode( int key, vec3d_t* v, bool vis_flag );  // private method I guess
vmap_t* vmap_insertNode( vmap_t** root, int key, vec3d_t* v, bool vis_flag );
#else
vmap_t* vmap_createNode( int key, vec3d_t* v );
vmap_t* vmap_insertNode( vmap_t** root, int key, vec3d_t* v );
#endif
vmap_t* vmap_search( vmap_t* root, int key );
void vmap_print( vmap_t* root );
void vmap_free( vmap_t* root );

#endif /* _VMAP_H_ */