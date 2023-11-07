#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include "slong.h"
#include "entities/entity_types.h"

// entity flags
#define ENTITY_FLAG_GROUND 0
#define ENTITY_FLAG_JUMP 1
#define ENTITY_FLAG_DOWN 2
#define ENTITY_FLAG_MOVE 3
#define ENTITY_FLAG_UPDATE 4
#define ENTITY_FLAG_SLOPE 5
#define ENTITY_FLAG_MIRROR 6
#define ENTITY_FLAG_INVINCIBLE 7
#define ENTITY_FLAG_PAUSE 8
#define ENTITY_FLAG_MENU 9
#define ENTITY_FLAG_10 10
#define ENTITY_FLAG_11 11
#define ENTITY_FLAG_12 12
#define ENTITY_FLAG_13 13
#define ENTITY_FLAG_14 14
#define ENTITY_FLAG_15 15

// transition flags
#define TX_RESUME 0x02
#define TX_HANDOFF 0x04

// Camera Boundary Configurations
//
// CR_CAMERA_NONE:
//   no camera boundaries
//
// CR_CAMERA_ALL:
//   left, right, top and bottom boundaries
#define CR_CAMERA_NONE 0
#define CR_CAMERA_ALL 1

/**
 * Sets the camera boundary configuration.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the camera boundary configuration
 */
void util_set_camera(cr_app *, int);

/**
 * Verifies that an entity is actually within the visibile boundary.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the entity whose position will be verified
 *
 * Returns:
 *   int - 1 if the entity is on screen, otherwise 0
 */
int util_is_on_screen(cr_app *, cr_entity *);

/**
 * Initializes the entity array with default values.
 *
 * Params:
 *   cr_entity* - a pointer to the first element of the entity array
 */
void util_init_entities(cr_entity *);

/**
 * Initializes the entity type array with default values.
 *
 * Params:
 *   cr_entity_type* - a pointer to the first element of the entity type array
 */
void util_init_entity_types(cr_entity_type *);

#endif