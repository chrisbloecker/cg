#ifndef __COLLISION_H__
#define __COLLISION_H__
/**
 * @file
 *
 * Das Modul stellt Funktionen zur Kollisionserkennung zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"

/**
 * Prüft, ob hugo mit coin kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] coin Münze.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCoin(Vector3d hugo, Vector3d coin);

/**
 * Prüft, ob hugo mit starfish kollidiert ist.
 *
 * @param[in] hugo     Hugo.
 * @param[in] starfish Seestern.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionStarfish(Vector3d hugo, Vector3d starfish);

/**
 * Prüft, ob hugo mit shell kollidiert ist.
 *
 * @param[in] hugo  Hugo.
 * @param[in] shell Muschel.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionShell(Vector3d hugo, Vector3d shell);

/**
 * Prüft, ob hugo mit car kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] car  Auto.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCar(Vector3d hugo, Vector3d car);

/**
 * Prüft, ob hugo mit ship kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] ship Schiff.
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionShip(Vector3d p, Vector3d ship);

/**
 * Prüft, ob hugo mit crab kollidiert ist.
 *
 * @param[in] hugo Hugo.
 * @param[in] crab Krabbe oder Krebs, wenn man will ...
 *
 * @return true  wenn Hugo kollidiert ist
 *         false sonst.
 */
extern Boolean collisionCrab(Vector3d p, Vector3d crab);

#endif
