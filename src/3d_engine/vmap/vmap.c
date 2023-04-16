#include "vmap.h"

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

// 
// Private functions
// 

// 
// Get height of a subtree.
// subroot - subtree to compute height of
// Return height of a subtree,
// 0 if subroot is NULL.
// 
size_t vmap_getHeight( vmap_node_t* subroot ){
    if( subroot == NULL )
        return 0;
    
    size_t height_left = vmap_getHeight( subroot->left );
    size_t height_right = vmap_getHeight( subroot->right );

    if( height_left > height_right )
        return ( height_left + 1 );
    return ( height_right + 1);
}

// 
// Get balance factor of a subtree.
// subroot - subtree to compute b.f. of
// Return b.f. of a subtree
// (height of right subtree - height of
// the left one).
// 
int8_t vmap_getBalanceFactor( vmap_node_t* subroot ){
    return ( vmap_getHeight( subroot->right ) - vmap_getHeight( subroot->left ) );
}

// 
// Rotate subtree to the left or right.
// subroot - node of balance factor +-2
//           (root of subtree to be rotated)
// This methods change the subroot pointer.
// 
void vmap_rotateLeft( vmap_node_t** subroot ){
    vmap_node_t* temp = (*subroot)->right;

    // vmap_node_t* T1 = *subroot->left;
    // Subtree no. 2:
    vmap_node_t* T2 = temp->left;
    // vmap_node_t* T3 = temp->right;

    (*subroot)->right = T2;
    temp->left = *subroot;

    // Update parents
    if( T2 != NULL )
        T2->parent = *subroot;

    vmap_node_t* tmp_parent = (*subroot)->parent;
    (*subroot)->parent = temp;
    temp->parent = tmp_parent;

    // Update balance factors
    (*subroot)->bf = vmap_getBalanceFactor( *subroot );
    *subroot = temp;
    (*subroot)->bf = vmap_getBalanceFactor( *subroot );

    // std::cout << "-----Rot_left-----" << std::endl;
    // graph();
}
void vmap_rotateRight( vmap_node_t** subroot ){
    vmap_node_t* temp = (*subroot)->left;

    // vmap_node_t* T1 = temp->left;
    // Subtree no. 2:
    vmap_node_t* T2 = temp->right;
    // vmap_node_t* T3 = *subroot->right;

    (*subroot)->left = T2;
    temp->right = *subroot;

    // Update parents
    if( T2 != NULL )
        T2->parent = *subroot;

    vmap_node_t* tmp_parent = (*subroot)->parent;
    (*subroot)->parent = temp;
    temp->parent = tmp_parent;

    // Update balance factors
    (*subroot)->bf = vmap_getBalanceFactor( *subroot );
    *subroot = temp;
    (*subroot)->bf = vmap_getBalanceFactor( *subroot );

    // std::cout << "-----Rot_right-----" << std::endl;
    // graph();
}

// 
// Rotate subtree first to the left and then
// to the right or vice-versa.
// subroot - node of balance factor +-2
//           (root of subtree to be rotated)
// This methods change the subroot pointer.
// 
void vmap_rotateLeftRight( vmap_node_t** subroot ){
    vmap_rotateLeft( &((*subroot)->left) );
    vmap_rotateRight( subroot );
}
void vmap_rotateRightLeft( vmap_node_t** subroot ){
    vmap_rotateRight( &((*subroot)->right) );
    vmap_rotateLeft( subroot );
}

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
// Auxillary find function.
// subroot - tree to be searched
// key  - key of the node of interest
// Return pointer to the found node, or NULL if not found.
// 
vmap_node_t* vmap_findAux( vmap_node_t* subroot, vmap_key_t key ){

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
    // DEBUG_PRINT( "In vmap_insertAux(): " );
    // DEBUG_PRINT( "subroot = %d\n", (*subroot) );
    if( (*subroot) == NULL ){
        // Create a new node
        // DEBUG_PRINT( "Creating a new node... " );
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        vmap_node_t* new_node_ptr = vmap_createNode( key, v, vis_flag );
#else
        vmap_node_t* new_node_ptr = vmap_createNode( key, v );
#endif
        if( new_node_ptr == NULL ){
            DEBUG_PRINT( "Error: in vmap_insertAux(): memory allocation failed.\n" );
            return NULL;
        }

        // DEBUG_PRINT( "done.\n" );

        return new_node_ptr;
    }

    // Insert the new node
    if( key < (*subroot)->key ){
        // DEBUG_PRINT( "Inserting to the left...\n" );
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        (*subroot)->left = vmap_insertAux( &((*subroot)->left), key, v, vis_flag );
#else
        (*subroot)->left = vmap_insertAux( &((*subroot)->left), key, v );
#endif
        (*subroot)->left->parent = *subroot;
    }
    else if( key > (*subroot)->key ){
        // DEBUG_PRINT( "Inserting to the right...\n" );
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        (*subroot)->right = vmap_insertAux( &((*subroot)->right), key, v, vis_flag );
#else
        (*subroot)->right = vmap_insertAux( &((*subroot)->right), key, v );
#endif
        (*subroot)->right->parent = *subroot;
    }
    else {
        // Key is already existing (duplicate found)
        // DEBUG_PRINT( "Key duplicate.\n" );
        return *subroot;
    }

    // DEBUG_PRINT("Computing bf...\n ");
    (*subroot)->bf = vmap_getBalanceFactor( *subroot );

    // Balance tree
    if( (*subroot)->bf > 1 ){
        if( (*subroot)->right->bf > 0 )
            vmap_rotateLeft( subroot );
        else
            vmap_rotateRightLeft( subroot );
    }
    else if( (*subroot)->bf < -1 ){
        if( (*subroot)->left->bf > 0 )
            vmap_rotateLeftRight( subroot );
        else
            vmap_rotateRight( subroot );
    }

    return *subroot;
}

// 
// Auxillary recursive function.
// Print map in order of ascending keys.
// 
void vmap_printInorderAux( vmap_node_t* subroot ){
    if (subroot == NULL)
        return;
    else {
        vmap_printInorderAux(subroot->left);
        
        STDO_STR( "Key: " );
        STDO_VMAP_KEY( subroot->key );
        STDO_STR( ", v: " );
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        vec3d_print( &(subroot->v), 0 );

        STDO_STR( ", " );
        if( !(subroot->visible) )
            STDO_STR( "not " );
        STDO_STR( "visible\n" );
#else  
        vec3d_print( &(subroot->v), 1 );
#endif
        vmap_printInorderAux(subroot->right);
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
            STDO_CHR( ' ' );
        
        // Print key
        STDO_CHR( '{' );
        STDO_VMAP_KEY( subroot->key );
        STDO_STR( ", " );
        // Print vec3d data
        vec3d_print( &(subroot->v), 0 );
        
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
        // Print visibility
        STDO_STR( ", " );
        if( subroot->visible )
            STDO_CHR( 'v' );
        else
            STDO_CHR( 'n' );
#endif
        // Print balance factor
        STDO_CHR( '|' );
        STDO_UINT8( subroot->bf );

#ifdef VMAP_SHOW_PARENT_KEY
        // Print parent's key
        if( subroot->parent != NULL ){
            STDO_STR( " ] " );
            STDO_VMAP_KEY( subroot->parent->key );
        }
#endif

        STDO_STR( "}\n" );

        vmap_graphAux( subroot->left, indent + VMAP_GRAPH_INDENT );
    }
}

// 
// Auxillary recursive function.
// Delete whole map.
// 
void vmap_freeAux( vmap_node_t** subroot ){
    if (*subroot != NULL){
		// DEBUG_PRINT("Freeing left subtree of key %d\n", (vmap_key_t) (*subroot)->key );
        vmap_freeAux( &( (*subroot)->left  ) );
		// DEBUG_PRINT("Freeing right subtree of key %d\n", (vmap_key_t) (*subroot)->key );
        vmap_freeAux( &( (*subroot)->right ) );
        // DEBUG_PRINT("Freeing node of key %d\n", (vmap_key_t) (*subroot)->key );
        free( *subroot );
        *subroot = NULL;
    }
}

// 
// Public functions
// 

void vmap_makeEmpty( vmap_t* vmap ){
    vmap->root = NULL;
    // vmap->root->left = NULL;
    // vmap->root->right = NULL;
    // vmap->root->parent = NULL;
    vmap->size = 0;
}

bool vmap_isEmpty( vmap_t* vmap ){
    return ( vmap->root == NULL ) || ( vmap->size == 0 );
}

// 
// Find a node in a map.
// vmap     - tree to be searched
// key      - key of the node of interest
// v        - if found, copied values of vertex of interest
// vis_flag - if found, copied visibility flag of the vertex
// Return true if found, false otherwise.
// 
bool vmap_find( vmap_t* vmap, vmap_key_t key, vec3d_t* v 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool* vis_flag 
#endif
){
    if( vmap->root == NULL )
        return false;
    
    vmap_node_t* node_ptr = vmap_findAux( vmap->root, key );

    if( node_ptr == NULL )
        return false;
    
    *v = node_ptr->v;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    *vis_flag = node_ptr->visible;
#endif

    return true;
}

// 
// Update a single map node with given values.
// vmap     - tree to be searched
// key      - key of the node of interest
// v        - new value for vertex of node of interest
// vis_flag - new value for visibility flag of this vertex
// Return true if found and update succeeded, false otherwise.
// 
bool vmap_updateNode( vmap_t* vmap, vmap_key_t key, vec3d_t* v 
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool vis_flag 
#endif
){
    if( vmap->root == NULL )
        return false;
    
    vmap_node_t* node_ptr = vmap_findAux( vmap->root, key );

    if( node_ptr == NULL )
        return false;
    
    node_ptr->v = *v;
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    node_ptr->visible = vis_flag;
#endif

    return true;
}

void vmap_insert( vmap_t* vmap, vmap_key_t key, vec3d_t* v
#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
                , bool vis_flag 
#endif
){

#if defined(REMOVE_HIDDEN_LINES) || defined(RENDER_VISIBLE_ONLY)
    vmap->root = vmap_insertAux( &(vmap->root), key, v, vis_flag );
#else
    vmap->root = vmap_insertAux( &(vmap->root), key, v );
#endif

    if( vmap->root == NULL ){
        DEBUG_PRINT( "Error: in vmap_insert() (key = %d): failed to insert a new node.", (vmap_key_t) key );
        return;
    }

    vmap->size++;
}

void vmap_printInorder( vmap_t* vmap ){
    if( vmap->size == 0 ){
        DEBUG_PRINT( "Empty\n" );
        return;
    }

    vmap_printInorderAux( vmap->root );
}

void vmap_graph( vmap_t* vmap ){
    if( vmap->size == 0 ){
        DEBUG_PRINT( "Empty\n" );
        return;
    }

    vmap_graphAux( vmap->root, 0 );
}

void vmap_free( vmap_t* vmap ){
	if( vmap == NULL ){
        // DEBUG_PRINT( "Warning: in vmap_free(): vmap is already NULL.\n" );
		return;
	}
    if( vmap->root == NULL ){
        // DEBUG_PRINT( "Warning: in vmap_free(): vmap is already empty.\n" );
        return;
	}

    vmap_freeAux( &(vmap->root) );

	vmap->root = NULL;
}