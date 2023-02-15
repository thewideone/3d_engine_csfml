#include "vmap.h"

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

// 
// Private functions
// 

// 
// Create a new node.
// key          - key of the node
// v            - vec3d_t data of the node
// ( vis_flag   - visibility flag of v )
// Return pointer to the newly created node.
// 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
vmap_node_t* vmap_createNode( vmap_key_t key, vec3d_t* v, bool vis_flag ){
#else
vmap_node_t* vmap_createNode( vmap_key_t key, vec3d_t* v ){
#endif
    vmap_node_t* new_node_ptr = (vmap_node_t*) malloc( sizeof( vmap_node_t ) );

    new_node_ptr->key = key;
    new_node_ptr->v.x = v->x;    // shallow copy is enough for this type
    new_node_ptr->v.y = v->y;
    new_node_ptr->v.z = v->z;
    new_node_ptr->v.w = v->w;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    new_node_ptr->visible = vis_flag;
#endif
    new_node_ptr->bf = 0;
    new_node_ptr->parent = NULL;
    new_node_ptr->left = NULL;
    new_node_ptr->right = NULL;

    return new_node_ptr;
}

// 
// Search the map.
// subroot - tree to be searched
// key  - key of the node of interest
// Return pointer to the found node, or NULL if not found.
// 
vmap_node_t* vmap_search( vmap_node_t* subroot, vmap_key_t key ){

    if( subroot == NULL )
        return NULL;
    
    vmap_node_t* found_node_ptr = NULL;

    while( subroot != NULL ){
        if( key > subroot->key )
            subroot = subroot->right;
        else if( key < subroot->key )
            subroot = subroot->left;
        // If the key is found, break
        else{
            found_node_ptr = subroot;
            break;
        }
    }
    return found_node_ptr;
}

// 
// Insert node to map.
// subroot      - pointer to pointer of the subroot of the tree
// key          - key of
// v            - pointer to vector to be added
// ( vis_flag   - visibility flag of v )
// Return pointer to the newly created node.
// 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
vmap_node_t* vmap_insertAux( vmap_node_t** subroot, vmap_key_t key, vec3d_t* v, bool vis_flag ){
#else
vmap_node_t* vmap_insertAux( vmap_node_t** subroot, vmap_key_t key, vec3d_t* v ){
#endif

    // If key already exists, return
    if( vmap_search( *subroot, key ) != NULL )
        return NULL;

    // Create a new node
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    vmap_node_t* new_node_ptr = vmap_createNode( key, v, vis_flag );
#else
    vmap_node_t* new_node_ptr = vmap_createNode( key, v );
#endif

    vmap_node_t* search_head_ptr = *subroot;
    vmap_node_t* parent_ptr = NULL;

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

    if( *subroot == NULL )
        *subroot = new_node_ptr;
    // If the subroot is NULL (the tree is empty)
    // The new node is the subroot node
    if( parent_ptr == NULL ){
        parent_ptr = new_node_ptr;
        // *subroot = new_node_ptr;
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
// Auxillary recursive function.
// Print map in order of ascending keys.
// 
void vmap_printInorderAux( vmap_node_t* subroot ){
    if (subroot == NULL)
        return;
    else {
        vmap_print(subroot->left);
        printf( "Key: %d, v: ", subroot->key );
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        vec3d_print( &(subroot->v), 0 );
        if( !(subroot->visible) )
            printf( "not " );
        printf( "visible\n" );
#else  
        vec3d_print( &(subroot->v), 1 );
#endif
        vmap_print(subroot->right);
    }
}

// 
// Auxillary recursive function.
// Graph map.
// subroot  - subroot of a tree to be drawn
// indent   - number of spaces to insert before
//            each node
// 
void vmap_graphAux( vmap_node_t* subroot, uint8_t indent ){
    if( subroot != NULL ){
        vmap_graphAux( subroot->right, indent + VMAP_GRAPH_INDENT );

        // Make indent
        for( uint8_t i=0; i<indent; i++ )
            printf( " " );
        
        // Print key
        printf( "{%d, ", subroot->key );
        // Print vec3d data
        vec3d_print( &(subroot->v), 0 );
        
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        // Print visibility
        printf( ", " );
        if( subroot->visible )
            printf( "v" );
        else
            printf( "n" );
#endif
        // Print balance factor
        printf( "|%d", subroot->bf );

#ifdef VMAP_SHOW_PARENT_KEY
        // Print parent's key
        printf( " ]%d", subroot->parent->key );
#endif

        printf( "}\n" );

        vmap_graphAux( subroot->left, indent + VMAP_GRAPH_INDENT );
    }
}

// 
// Auxillary recursive function.
// Delete whole map.
// 
void vmap_freeAux( vmap_node_t* subroot ){
    if (subroot == NULL)
        return;
    else{
        vmap_free(subroot->left);
        vmap_free(subroot->right);
        // printf("Freeing node of key %d\n", subroot->key );
        free( subroot );
        subroot = NULL;
    }
}

// 
// Public functions
// 

void vmap_insert( vmap_t* vmap, vmap_key_t key, vec3d_t* v
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool vis_flag 
#endif
){

}

bool vmap_find( vmap_t* vmap, vmap_key_t key, vec3d_t* v
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool* vis_flag 
#endif
){

}

void vmap_printInorder( vmap_t* vmap ){
    if( vmap->size == 0 ){
        printf( "Empty\n" );
        return;
    }

    vmap_printInorderAux( vmap->root );
}

void vmap_graph( vmap_t* vmap ){
    if( vmap->size == 0 ){
        printf( "Empty\n" );
        return;
    }

    vmap_graphAux( vmap->root, 0 );
}

void vmap_free( vmap_t* vmap ){
    if( vmap->root == NULL )
        return;

    vmap_freeAux( vmap->root );
}