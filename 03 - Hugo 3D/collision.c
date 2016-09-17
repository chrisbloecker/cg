/**
 * @file
 *
 * Das Modul stellt Funktionen zur Kollisionserkennung zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "collision.h"
#include "types.h"
#include "vector.h"
#include "logic.h"

#define COLLISION
#include "constants.h"
#undef COLLISION

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * ----------------------------------------------------------------------------
 * Kollision mit Items
 * -------------------------------------------------------------------------- */

/**
 * Prüft, ob hugo mit coin kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] coin Münze.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCoin(Vector3d hugo, Vector3d coin)
{
  return vectorLength(vectorSub(hugo, coin)) < COLLISION_DISTANCE_COIN;
}

/**
 * Prüft, ob hugo mit starfish kollidiert ist.
 *
 * @param[in] hugo     Hugo.
 * @param[in] starfish Seestern.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionStarfish(Vector3d hugo, Vector3d starfish)
{
  return vectorLength(vectorSub(hugo, starfish)) < COLLISION_DISTANCE_STARFISH;
}

/**
 * Prüft, ob hugo mit shell kollidiert ist.
 *
 * @param[in] hugo  Hugo.
 * @param[in] shell Muschel.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionShell(Vector3d hugo, Vector3d shell)
{
  return vectorLength(vectorSub(hugo, shell)) < COLLISION_DISTANCE_SHELL;
}

/* ----------------------------------------------------------------------------
 * Kollision mit Hindernissen
 * -------------------------------------------------------------------------- */

/**
 * Prüft, ob hugo mit car kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] car  Auto.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCar(Vector3d hugo, Vector3d car)
{
  /* Anzahl der "Eckpunkte", die betrachtet werden */
  #define VERTICES_CAR (6)
  
  Boolean res = FALSE;
  
  unsigned i = 0;
  
  Vector3d vertices[VERTICES_CAR];
  
  /* "Eckpunkte" */
  vertices[0] = vectorAdd(vectorScale(vectorMake(  0.6,   0.0,   1.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  vertices[1] = vectorAdd(vectorScale(vectorMake(  0.6,   0.0, - 1.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  vertices[2] = vectorAdd(vectorScale(vectorMake(- 0.6,   0.0, - 1.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  vertices[3] = vectorAdd(vectorScale(vectorMake(- 0.6,   0.0,   1.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  vertices[4] = vectorAdd(vectorScale(vectorMake(  0.6,   0.0,   0.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  vertices[5] = vectorAdd(vectorScale(vectorMake(- 0.6,   0.0,   0.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), car);
  
  /* Hugo kollidiert, wenn er mit einem der Eckpunkte in Berührung kommt */
  while (!res && i < VERTICES_CAR)
  {
    /* Bäm */
    res = vectorLength(vectorSub(hugo, vertices[i])) < COLLISION_DISTANCE_CAR;
    
    ++i;
  }
  
  return res;
  
  /* ja, weg damit, wird nicht mehr gebraucht */
  #undef VERTICES_CAR
}

/**
 * Prüft, ob hugo mit ship kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] ship Schiff.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionShip(Vector3d hugo, Vector3d ship)
{
  /* Anzahl der "Eckpunkte", die betrachtet werden */
  #define VERTICES_SHIP (5)
  
  Boolean res = FALSE;
  
  unsigned i = 0;
  
  Vector3d vertices[VERTICES_SHIP];
  
  /* "Eckpunkte" */
  vertices[0] = vectorAdd(vectorScale(vectorMake(  0.0,   0.0, - 1.0), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), ship);
  vertices[1] = vectorAdd(vectorScale(vectorMake(- 0.6,   0.0,   0.2), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), ship);
  vertices[2] = vectorAdd(vectorScale(vectorMake(- 0.2,   0.0,   0.8), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), ship);
  vertices[3] = vectorAdd(vectorScale(vectorMake(  0.2,   0.0,   0.8), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), ship);
  vertices[4] = vectorAdd(vectorScale(vectorMake(  0.6,   0.0,   0.2), SCALE_X_OBSTACLE, SCALE_X_OBSTACLE, SCALE_X_OBSTACLE), ship);
  
  /* Hugo kollidiert, wenn er mit einem der Eckpunkte in Berührung kommt */
  while (!res && i < VERTICES_SHIP)
  {
    /* ... */
    res = vectorLength(vectorSub(hugo, vertices[i])) < COLLISION_DISTANCE_SHIP;
    ++i;
  }
  
  return res;
  
  /* und weg ... */
  #undef VERTICES_SHIP
}

/**
 * Prüft, ob hugo mit crab kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] crab Krabbe oder Krebs, wenn man will ...
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCrab(Vector3d hugo, Vector3d crab)
{
  return vectorLength(vectorSub(hugo, crab)) < COLLISION_DISTANCE_CRAB;
}
