#ifndef _VMAP_H_
#define _VMAP_H_

#include "../config.h"
#include "../math_3d.h"

// Binary tree map for storing 3D vectors and their visibility flags
// vmap is short for "vector 3D map"
typedef struct vmap_t {
    int key;    // used as vector's ID
    vec3d_t v;
#ifdef REMOVE_HIDDEN_LINES
    bool visible;    // set if vector v is visible
#endif
    struct vmap_t* left;
    struct vmap_t* right;
} vmap_t;

vmap_t* vmap_createNode( int key, vec3d_t* v, int vis_flag );
vmap_t* vmap_search( vmap_t* root, int key );
vmap_t* vmap_insertNode( vmap_t** root, int key, vec3d_t* v, int vis_flag );
void vmap_print( vmap_t* root );
void vmap_free( vmap_t* root );

#endif /* _VMAP_H_ */