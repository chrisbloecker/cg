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

typedef struct {
  char * points
     , * level
     , * lives;
} Text;

/* Skalierung der Objekte */
#define SCALE_GROUND      (0.6 )
#define SCALE_HUGO        (0.15)
#define SCALE_OBSTACLE    (0.2 )
#define SCALE_ENVIRONMENT (0.2 )
#define SCALE_ITEM        (0.1 )

/* Translation der Objekte */
#define TRANSLATION_Y_HUGO        (- 0.8 )
#define TRANSLATION_X_OBSTACLE    (  0.4 )
#define TRANSLATION_X_ENVIRONMENT (  0.78)
#define TRANSLATION_X_ITEM        (  0.4 )

/* Speed für Objekte */
#define SPEED_ENVIRONMENT (0.75)

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
 * Gibt die zum aktuellen Level zugeordnete Zeichenfunktion zurück, die den
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
extern Vector2d logicItemNext(void);

/**
 * Gibt die Position des nächsten Hindernisses zurück und setzt den Zähler des
 * aktuellen Hindernisses hoch.
 *
 * @return Position des nächsten Hindernisses.
 */
extern Vector2d logicObstacleNext(void);

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

#endif
