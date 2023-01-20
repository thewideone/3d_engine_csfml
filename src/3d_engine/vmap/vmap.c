#include "vmap.h"

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
vmap_t* vmap_createNode( int key, vec3d_t* v, bool vis_flag ){
#else
vmap_t* vmap_createNode( int key, vec3d_t* v ){
#endif
    vmap_t* new_node_ptr = (vmap_t*) malloc( sizeof( vmap_t ) );

    new_node_ptr->key = key;
    new_node_ptr->v.x = v->x;    // shallow copy is enough for this type
    new_node_ptr->v.y = v->y;
    new_node_ptr->v.z = v->z;
    new_node_ptr->v.w = v->w;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    new_node_ptr->visible = vis_flag;
#endif
    new_node_ptr->left = NULL;
    new_node_ptr->right = NULL;

    return new_node_ptr;
}

// 
// Search the map
// 
// root - tree to be searched
// key  - key of the node of interest
// 
// Return address of the found node, or NULL if not found.
// 
vmap_t* vmap_search( vmap_t* root, int key ){

    if( root == NULL )
        return NULL;
    
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

// 
// Insert node to map
// root     - pointer to pointer of the root of the tree
// key      - key of
// v        - pointer to vector to be added
// vis_flag - visibility flag of the vector
// 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
vmap_t* vmap_insertNode( vmap_t** root, int key, vec3d_t* v, bool vis_flag ){
#else
vmap_t* vmap_insertNode( vmap_t** root, int key, vec3d_t* v ){
#endif

    // If key already exists, return
    if( vmap_search( *root, key ) != NULL )
        return NULL;

    // Create a new node
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    vmap_t* new_node_ptr = vmap_createNode( key, v, vis_flag );
#else
    vmap_t* new_node_ptr = vmap_createNode( key, v );
#endif

    vmap_t* search_head_ptr = *root;
    vmap_t* parent_ptr = NULL;

    while( search_head_ptr != NULL ){
        parent_ptr = search_head_ptr;
        if( key < search_head_ptr->key )
            search_head_ptr = search_head_ptr->left;
        else if( key > search_head_ptr->key )
            search_head_ptr = search_head_ptr->right;
        else {
            // Given key has been found,
            // we don't want duplicates
            free(new_node_ptr);
            return NULL;
        }
    }

    if( *root == NULL )
        *root = new_node_ptr;
    // If the root is NULL (the tree is empty)
    // The new node is the root node
    if( parent_ptr == NULL ){
        parent_ptr = new_node_ptr;
        // *root = new_node_ptr;
    }

    // If the new key is less than the leaf node key
    // Assign the new node to be its left child
    else if( key < parent_ptr->key )
        parent_ptr->left = new_node_ptr;
    
    // Else, assign the new node to be its right child
    else
        parent_ptr->right = new_node_ptr;
    
    // Return a pointer to where the new node is
    // inserted
    return parent_ptr;
}

// 
// Print binary tree map in order of ascending keys
// 
void vmap_print( vmap_t* root ){
    if (root == NULL)
        return;
    else {
        vmap_print(root->left);
        printf( "Key: %d, v: ", root->key );
        vec3d_print( &(root->v), 1 );
        vmap_print(root->right);
    }
}

// 
// Delete whole map
// 
void vmap_free( vmap_t* root ){
    if (root == NULL)
        return;
    else{
        vmap_free(root->left);
        vmap_free(root->right);
        // printf("Freeing node of key %d\n", root->key );
        free( root );
        root = NULL;
    }
}