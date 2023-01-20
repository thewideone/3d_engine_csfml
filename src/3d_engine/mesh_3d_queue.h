#ifndef _MESH_3D_QUEUE_H_
#define _MESH_3D_QUEUE_H_

#include "mesh_3d.h"
#include "math_3d.h"    // for bool type

// 
// For now, the queue is implemented as
// FIFO array containing pointers to meshes.
// 

#define MESH_QUEUE_FIXED_SIZE
#define MESH_QUEUE_CAPACITY 10

typedef struct {
    // Array of pointers to meshes
    mesh_t* array[MESH_QUEUE_CAPACITY];
    // Actual size
    size_t size;
    // Points on the first element
    // which has not been processed yet
    size_t cursor;
} mesh_queue_t;

void meshQueue_makeEmpty( mesh_queue_t* mq );

#ifndef MESH_QUEUE_FIXED_SIZE
// 
// Remove all elements of a queue, but
// preserving meshes.
// 
void meshQueue_freeQueue( mesh_queue_t* mq );
#endif

// 
// Free all meshes within the queue.
// 
void meshQueue_freeMeshes( mesh_queue_t* mq );

bool meshQueue_isEmpty( mesh_queue_t* mq );
bool meshQueue_isFull( mesh_queue_t* mq );

// 
// Get a pointer to the first not yet processed mesh.
// Return a pointer to a mesh pointed by the cursor.
// 
mesh_t* meshQueue_getCurrent( mesh_queue_t* mq );

// 
// Move the cursor forward one step
// (proceed to the next mesh).
// 
void meshQueue_goToNext( mesh_queue_t* mq );

// 
// Return true if succeeded,
// false otherwise (e.g. the queue is full).
// 
bool meshQueue_push( mesh_queue_t* mq, mesh_t* mesh );

// 
// Remove a mesh from a queue.
// mesh - pointer to mesh to be removed from the queue
// idx  - index of mesh to be removed from the queue
// Return true if mesh found and removed
// from the queue, false otherwise.
// 
bool meshQueue_remove( mesh_queue_t* mq, mesh_t* mesh );
bool meshQueue_removeAt( mesh_queue_t* mq, size_t idx );

// 
// Remove a mesh from a queue and free the mesh.
// mesh - pointer to mesh to be removed from queue
// idx  - index of mesh to be removed from queue
// Return true if mesh found and removed
// from queue, false otherwise.
// 
bool meshQueue_freeMesh( mesh_queue_t* mq, mesh_t* mesh );
bool meshQueue_freeMeshAt( mesh_queue_t* mq, size_t idx );

#endif // _MESH_3D_QUEUE_H_