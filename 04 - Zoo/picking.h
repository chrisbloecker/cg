#ifndef __PICKING_H__
#define __PICKING_H__

/** Mögliche Picking-Aktionen */
typedef enum {
  ACTION_SELECT
, ACTION_RELEASE
, ACTION_ANIMATE
} PickingAction;

/**
 * Picking. Auswahl von Szenenobjekten durch Klicken mit der Maus.
 *
 * @param[in] x x-Koordinate.
 * @param[in] y y-Koordinate.
 * @param[in] a Auszuführende Aktion.
 */
extern void pick(int x, int y, PickingAction a);

#endif
