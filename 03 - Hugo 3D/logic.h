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

/** Datenstruktur, die die anzuzeigenden Texte enthält */
typedef struct {
  int points
     , level
     , lives;
} Text;

/** Richtungen, in die die Kamera bewegt werden kann */
typedef enum {
  CAM_UP
, CAM_LEFT
, CAM_DOWN
, CAM_RIGHT
} LogicCamDirection;

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
 * Gibt zurück, wie weit Hugo vorangeschritten ist.
 *
 * @return Hugos y-Position.
 */
extern double logicGetShift(void);

/**
 * Gibt Hugos x-Position zurück.
 *
 * @return Hugos x-Position;
 */
extern double logicGetHugoShift(void);

/**
 * Fügt eine Bewegung zum Bewegungspuffer hinzu.
 * Wenn der Puffer bereits 2 Einträge enthält, so wird nichts gemacht.
 *
 * @param[in] d Richtung, die hinzugefügt werden soll.
 */
extern void logicAddMovement(Direction d);

/**
 * Gibt die zum aktuellen Level zugeordn zurück, die den
 * Objekttypen t zeichnet.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t und zum aktuellen Level gehörende Zeichenfunktion.
 */
extern DrawFunction logicGetDrawFunction(DrawFunctionType t);

/**
 * Gibt die zum aktuellen Level zugeordnete Animationsfunktion zurück.
 *
 * @return zum aktuellen Level gehörende Animationsfunktion.
 */
extern AnimationFunction logicGetAnimationFunction(void);

/**
 * Initialisiert den Iterator für die Items.
 */
extern void logicItemIterInit(void);

/**
 * Initialisiert den Iterator für die Hindernisse.
 */
extern void logicObstacleIterInit(void);

/**
 * Prüft, ob es noch weitere Gegenstände innerhalb des Sichtbereiches gibt.
 *
 * @return true  wenn es noch Gegenstände im Sichtbereich gibt
 *         false sonst.
 */
extern Boolean logicItemHasNext(void);

/**
 * Prüft, ob es noch weitere Hindernisse innerhalb des Sichtbereiches gibt.
 *
 * @return true  wenn es noch Hindernisse im Sichtbereich gibt
 *         false sonst.
 */
extern Boolean logicObstacleHasNext(void);

/**
 * Gibt die Position des nächsten Gegenstandes zurück und setzt den Zähler des
 * aktuellen Gegenstandes hoch.
 *
 * @return Position des nächsten Gegenstandes.
 */
extern Vector3d logicItemNext(void);

/**
 * Gibt die Position des nächsten Hindernisses zurück und setzt den Zähler des
 * aktuellen Hindernisses hoch.
 *
 * @return Position des nächsten Hindernisses.
 */
extern Vector3d logicObstacleNext(void);

/**
 * Stellt Textinformationen anhand des aktuellen Spielstandes zusammen.
 *
 * @return Textinformationen.
 */
extern Text logicGetText(void);

/**
 * Gibt den von den Textinformationen t belegten Speicher wieder frei.
 *
 * @param[in] t Textinformationen, deren belegter Speicher wieder freigegeben
 *              werden soll.
 */
extern void logicCleanText(Text t);

/**
 * Gibt an, ob die Spielhilfe ausgegeben werden soll.
 *
 * @return true  wenn die Hilfe ausgegeben werden soll
 *         false sonst.
 */
extern Boolean logicShowHelp(void);

/**
 * Legt fest, ob die Spielhilfe ausgegeben werden soll.
 * showHelp <- !showHelp
 */
extern void logicSetHelp(void);

extern void logicSetPause(void);

/**
 * Gibt dem Spieler ein Leben dazu.
 */
extern void logicAddLife(void);

/**
 * Wechselt zu Level i.
 *
 * @param[in] i Level.
 */
extern void logicChangeLevel(unsigned i);

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
 * Gibt die Auslenkung der Lichtquelle des Leuchtturms bezogen auf die
 * Grundposition im Gradmaß zurück.
 *
 * @return Auslenkung der Lichtquelle des Leuchtturms bezogen auf die
 *         Grundposition im Gradmaß.
 */
extern double logicGetLighthouseRotation(void);

/**
 * Schaltet das Licht mit der Nummer i ein bzw. aus.
 */
extern void logicSetLight(unsigned i);

/**
 * @return Gibt zurück, ob Licht i eingeschaltet ist.
 */
extern Boolean logicGetLight(unsigned i);

#endif
