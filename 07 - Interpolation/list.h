#ifndef __LIST_H__
#define __LIST_H__
/**
 * @file list.c Schnittstelle einer Bibliothek für Listenoperationen.
 *
 * Listenelemente sind ganzzahlige Werte grösser gleich Null.
 * Listen können jeden Wert nur einmal enthalten.
 * Listen sind aufsteigend sortiert.
 * Listen sind als rekursive Struktur implementiert (einfach verkette Listen).
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Datentyp der Listenelemente */
typedef unsigned long Element;

/** Datentyp der Liste. Hier verkette Liste. */
typedef struct Node *List;

/** Struktur eines Knotens der Liste. */
struct Node
{
  /** Zeiger auf nachfolgenden Knoten. */
  List next;
  /** Nutzlast des Knotens (Wert des Listenelementes), */
  Element value;
};

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/**
 * Prueft zwei Listenelemente auf Gleichheit.
 * Definiert zusammen mit list_geElement die Ordnungsrelation der Elemente
 *
 * @param[in] x erstes Element.
 * @param[in] y zweites Element.
 *
 * @return 1 wenn Elemente gleich sind, sonst 0.
 */
#define listEqElement(x,y) ((x)==(y))

/**
 * Prueft, ob ein Listenelement groesser oder gleich einem zweiten ist.
 * Definiert zusammen mit list_eqElement die Ordnungsrelation der Elemente
 *
 * @param[in] x erstes Element.
 * @param[in] y zweites Element.
 *
 * @return 1 wenn x groesser oder gleich y ist, sonst 0.
 */
#define listGeElement(x,y) ((x)>=(y))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine leere Liste.
 *
 * @return leere Liste.
 */
extern List listMakeEmpty(void);

/**
 * Prueft, ob die Liste l leer ist.
 *
 * @param[in] l Liste, die geprüft werden soll.
 *
 * @return 1, wenn die Liste l leer ist, sonst 0.
 */
extern Boolean listIsEmpty(List l);

/**
 * Liefert das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geliefert werden soll.
 *
 * @return erstes Element der Liste l.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern Element listHead(List l);

/**
 * Liefert die Liste l ohne das erste Element.
 *
 * @param[in] l Liste, die ohne das erste Element geliefert werden soll.
 *
 * @return Liste l ohne das erste Element.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List listTail(List l);

/**
 * Loescht das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geloescht werden soll.
 *
 * @return Liste, die durch Loeschen entsteht.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List listRemoveHead(List l);

/**
 * Fuegt am Kopf der Liste l einen Knoten mit dem Wert e hinzu.
 *
 * @param[in] e Wert des Knotens, der hinzugefuegt werden soll.
 * @param[in] l Liste, an deren Kopf der Knoten hinzugefuegt werden soll.
 *
 * @return Liste, die durch Hinzufuegen entsteht.
 */
extern List listCons(Element e, List l);

/**
 * Liefert die Laenge der Liste l.
 *
 * @param[in] l Liste, deren Laenge geliefert werden soll.
 *
 * @return Laenge der Liste l.
 */
extern unsigned long listLength(List l);

/**
 * Liefert das Element an der Position i in der Liste l.
 *
 * @param[in] l Liste, deren Element an der Position i geliefert werden soll.
 * @param[in] i Position des Elements, das geliefert werden soll.
 *
 * @return Element, das an der Position i in der Liste l steht.
 */
extern Element listAt(List l, unsigned long i);

/**
 * Prueft, ob das Element e in der Liste l enthalten ist.
 *
 * @param[in] e Element, dessen Vorkommen in der Liste l geprueft werden soll.
 * @param[in] l Liste, deren Elemente geprueft werden sollen.
 *
 * @return 1, wenn das Element e in der Liste l enthalten ist, sonst 0.
 */
extern Boolean listIsIn(List l, Element e);

/**
 * Fuegt das Element e in die Liste l ein.
 *
 * @param[in] e Element, das in die  Liste l eingefuegt werden soll.
 * @param[in] l Liste, in die das Element e eingefuegt werden soll..
 *
 * @return Liste, die das Element e enthaelt.
 */
extern List listInsert(List l, Element e);

/**
 * Entfernt das Element e aus der Liste l.
 *
 * @param[in] e Element, das aus der Liste l entfernt werden soll.
 * @param[in] l Liste, aus der das Element e entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List listRemove(List l, Element e);

/**
 * Entfernt alle Elemente aus der Liste l.
 *
 * @param[in] l Liste, aus der alle Elemente entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List listRemoveAll(List l);

#endif
