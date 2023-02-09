#include "mesh_3d_queue.h"

#include <stdio.h>  // for debug printf

void meshQueue_makeEmpty( mesh_queue_t* mq ){
    for( size_t i = 0; i < MESH_QUEUE_CAPACITY; i++ )
        mq->array[i] = NULL;
    mq->size = 0;
    mq->cursor = 0;
}

#ifndef MESH_QUEUE_FIXED_SIZE
// 
// Remove all elements of a queue, but
// preserving meshes.
// 
void meshQueue_freeQueue( mesh_queue_t* mq ){
}
#endif

void meshQueue_freeAllMeshes( mesh_queue_t* mq ){
    for( size_t i = 0; i < MESH_QUEUE_CAPACITY; i++ ){
        if( mq->array[i] == NULL )
            continue;
            
        mesh_free( mq->array[i] );
        mq->array[i] = NULL;
        mq->size--;
    }

    if( mq->size != 0 )
        printf( "Error: after meshQueue_freeMeshes() size not zero!\n" );
    // mq->size = 0;
    mq->cursor = 0;
}

bool meshQueue_isEmpty( mesh_queue_t* mq ){
    return ( mq->size == 0 );
}
bool meshQueue_isFull( mesh_queue_t* mq ){
    return ( mq->size == MESH_QUEUE_CAPACITY );
}

mesh_t* meshQueue_getCurrent( mesh_queue_t* mq ){
    if( mq->size == 0 )
        return NULL;
    return mq->array[ mq->cursor ];
}

void meshQueue_goToNext( mesh_queue_t* mq ){
    // If the queue is empty
    if( mq->size == 0 )
        return;
    
    do {
        mq->cursor++;

        // Wrap(?) the cursor
        if( mq->cursor == MESH_QUEUE_CAPACITY )
            mq->cursor = 0;

    } while( mq->array[ mq->cursor ] == NULL );
}

bool meshQueue_push( mesh_queue_t* mq, mesh_t* mesh ){

    // If the queue is full
    if( mq->size == MESH_QUEUE_CAPACITY ){
        printf( "Error: in meshQueue_push(): queue at its capacity.\n" );
        return false;
    }

    // Check for duplicates
    for( size_t i=0; i < MESH_QUEUE_CAPACITY; i++ )
        if( mq->array[i] == mesh )
            return false;

    // There is at least one free place
    size_t temp_cursor = 0;

    while( mq->array[ temp_cursor ] != NULL ){
        temp_cursor++;
        // Wrap(?) the cursor
        if( temp_cursor == MESH_QUEUE_CAPACITY )
            temp_cursor = 0;
    }

    mq->array[temp_cursor] = mesh;
    mq->size++;

    return true;
}

bool meshQueue_remove( mesh_queue_t* mq, mesh_t* mesh ){
    if( mq->size == 0 )
        return false;
    
    for( size_t i=0; i<MESH_QUEUE_CAPACITY; i++ )
        if( mq->array[i] == mesh ){
            mq->array[i] = NULL;
            mq->size--;

            if( mq->size )
                while( mq->array[mq->cursor] == NULL )
                    mq->cursor++;
            // Add shifting of remaining elements here if needed
            return true;
        }
    // Mesh not found
    return false;
}
bool meshQueue_removeAt( mesh_queue_t* mq, size_t idx ){
    if( idx > MESH_QUEUE_CAPACITY || idx < 0 )
        return false;
    mq->array[idx] = NULL;
    mq->size--;

    if( mq->size )
        while( mq->array[mq->cursor] == NULL )
            mq->cursor++;
    
    return true;
}

bool meshQueue_freeMesh( mesh_queue_t* mq, mesh_t* mesh ){
    if( mq->size == 0 )
        return false;
    
    for( size_t i=0; i<MESH_QUEUE_CAPACITY; i++ )
        if( mq->array[i] == mesh ){
            mesh_free( mq->array[i] );
            mq->array[i] = NULL;
            mq->size--;

            if( mq->size )
                while( mq->array[mq->cursor] == NULL )
                    mq->cursor++;
            // Add shifting of remaining elements here if needed
            return true;
        }
    // Mesh not found
    return false;
}
bool meshQueue_freeMeshAt( mesh_queue_t* mq, size_t idx ){
    if( idx > MESH_QUEUE_CAPACITY || idx < 0 || mq->array[idx] == NULL )
        return false;

    mesh_free( mq->array[idx] );
    mq->array[idx] = NULL;
    mq->size--;

    if( mq->size )
        while( mq->array[mq->cursor] == NULL )
            mq->cursor++;

    return true;
}