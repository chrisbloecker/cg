#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include "types.h"
#include "level.h"

#define LOGIC_COUNT_LIGHTS (1)

/** Richtungen, in die die Kamera bewegt werden kann */
typedef enum {
  CAM_UP
, CAM_LEFT
, CAM_DOWN
, CAM_RIGHT
, CAM_FORWARD
, CAM_BACKWARD
, CAM_DUMMY
} LogicCamDirection;

/** Ecken, auf die die Kamera per Tastendruck gesetzt werden kann */
typedef enum {
  CORNER_1
, CORNER_2
, CORNER_3
, CORNER_4
} Corner;

/** Animationsnamen */
typedef enum {
  ANIMATION_GIRAFFE
, ANIMATION_PIG
, ANIMATION_FISH
, ANIMATION_DUMMY
} AnimationName;

/** Animationsinformationen */
typedef struct {
  Boolean isAnimated     /* Animation durchführen         */
        , stop           /* Gestoppt wegen Kollision o.ä. */
        ;
  double * animProperty  /* Animationsmerkmal             */
       ,   animMin
       ,   animMax
       ;
} Animation;

/**
 * Initialisiert das Spiel und sagt "1".
 *
 * @return 1.
 */
extern int logicInit(void);

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval);

/**
 * Gibt den Level zurück.
 *
 * @return Level.
 */

extern Level logicGetLevel(void);

/**
 * Gibt die zum Objekttypen t zugeordnete Zeichenfunktion zurück.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t gehörende Zeichenfunktion.
 */
extern DrawFunction logicGetDrawFunction(DrawFunctionType t);

/**
 * Setzt den Bewegungsstatus der Kamera in Richtung cd auf b.
 *
 * @param[in] cd Bewegungsrichtung für die Kamera.
 * @param[in] b  Status der Bewegung in Richtung cd.
 */
extern void logicSetCamMovement(LogicCamDirection cd, Boolean b);

/**
 * Gibt die Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß
 * zurück.
 *
 * @return Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß.
 */
extern int logicGetCamRotation(void);

/**
 * Gibt die Höhe der Kamera bezogen auf die Grundposition zurück.
 *
 * @return Höhe der Kamera bezogen auf die Grundposition.
 */
extern double logicGetCamHeight(void);

/**
 * Schaltet das Licht mit der Nummer i ein bzw. aus.
 */
extern void logicSetLight(unsigned i);

/**
 * @return Gibt zurück, ob Licht i eingeschaltet ist.
 */
extern Boolean logicGetLight(unsigned i);

/**
 * @return Gibt den Augpunkt zurück.
 */

extern Vector3d logicGetEye(void);

/**
 * @return Gibt den Mittelpunkt der Szene zurück.
 */

extern Vector3d logicGetCenter(void);

/**
 * Schaltet zwischen Gitter, Glas und Nichts um.
 */
extern void logicSetGlass(void);

/**
 * Setzt die Kamera auf Kante c.
 *
 * @param[in] c Kante, auf die die Kamera gesetzt werden soll.
 */

extern void logicSetToCorner(Corner c);

/**
 * Prüft, ob n ein Element bezeichnet, welches auswählbar sein soll.
 *
 * @param[in] n Objektname.
 *
 * @return true  wenn n auswählbar sein soll
 *         false sonst.
 */

extern Boolean logicIsPickable(PickingName n);

/**
 * Wählt ein auswählbares Objekt aus oder lässt es los.
 *
 * @param[in] n Name des Objektes.
 * @param[in] b Auswahlstatus.
 */

extern void logicSelect(PickingName n, Boolean b);

/**
 * Bewegt das ausgewählte Objekt.
 * Die Bewegung wird anhand von v berechnet.
 *
 * @param[in] v Gibt die auszuführende Verschiebung an.
 */

extern void logicMove(Vector3d v);

/**
 * Animation für das mit n bezeichnete Objekt einschalten.
 *
 * @param[in] n Name des zu animierenden Objektes.
 */

extern void logicAnimate(AnimationName n);

#endif
