#ifndef _VMAP_H_
#define _VMAP_H_

#include "../config.h"
#include "../math_3d.h"

// Number of spaces before each level of graph
#define VMAP_GRAPH_INDENT 6
#define VMAP_SHOW_PARENT_KEY

#define vmap_key_t int

// TODO:
//  - change implementation to some autobalanced tree like AVL or Red-Black one

// 
// Binary tree map for storing 3D vectors and their visibility flags
// vmap is short for "vector 3D map"
// 

typedef struct vmap_node_t {
    vmap_key_t key;                 // used as vector's ID
    vec3d_t v;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    bool visible;                   // set if vector v is visible
#endif
    int8_t bf;                      // balance factor
    struct vmap_node_t* parent;     // for remove I guess
    struct vmap_node_t* left;
    struct vmap_node_t* right;
} vmap_node_t;

typedef struct {
    vmap_node_t* root;
    size_t size;
} vmap_t;

// 
// Private functions
// 
// vmap_node_t* vmap_createNode( vmap_key_t key, vec3d_t* v
// #if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
//                             , bool vis_flag 
// #endif
// );
// vmap_node_t* vmap_insertAux( vmap_node_t** root, vmap_key_t key, vec3d_t* v
// #if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
//                             , bool vis_flag 
// #endif
// );

// vmap_node_t* vmap_findAux( vmap_node_t* root, vmap_key_t key );

// void vmap_printInorderAux( vmap_node_t* subroot );
// void vmap_graphAux( vmap_node_t* root, uint8_t indent );
// void vmap_freeAux( vmap_node_t* root );

// 
// Public functions
// 
void vmap_makeEmpty( vmap_t* vmap );

bool vmap_isEmpty( vmap_t* vmap );

bool vmap_find( vmap_t* vmap, vmap_key_t key, vec3d_t* v 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool* vis_flag 
#endif
);

bool vmap_updateNode( vmap_t* vmap, vmap_key_t key, vec3d_t* v 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool vis_flag 
#endif
);

void vmap_insert( vmap_t* vmap, vmap_key_t key, vec3d_t* v
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool vis_flag 
#endif
);

void vmap_printInorder( vmap_t* vmap );
void vmap_graph( vmap_t* vmap );
void vmap_free( vmap_t* vmap );

#endif /* _VMAP_H_ */