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
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "list.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
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
extern List listMakeEmpty(void)
{  
  return (List) 0;
}

/**
 * Prüft, ob die Liste l leer ist.
 *
 * @param[in] l Liste, die geprüft werden soll.
 *
 * @return 1, wenn die Liste l leer ist, sonst 0.
 */
extern Boolean listIsEmpty(List l)
{  
  return l == (List) 0;
}

/**
 * Liefert das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geliefert werden soll.
 *
 * @return erstes Element der Liste l.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern Element listHead(List l)
{
  assert(!listIsEmpty(l));
  
  return l->value;
}

/**
 * Liefert die Liste l ohne das erste Element.
 *
 * @param[in] l Liste, die ohne das erste Element geliefert werden soll.
 *
 * @return Liste l ohne das erste Element.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List listTail(List l)
{
  assert(!listIsEmpty(l));
  
  return l->next;
}

/**
 * Loescht das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geloescht werden soll.
 *
 * @return Liste, die durch Loeschen entsteht.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List listRemoveHead(List l)
{
  List res = NULL;
  
  assert(!listIsEmpty(l));
  
  res = listTail(l);
  
  free(l);

  return res;
}

/**
 * Fuegt am Kopf der Liste l einen Knoten mit dem Wert e hinzu.
 *
 * @param[in] e Wert des Knotens, der hinzugefuegt werden soll.
 * @param[in] l Liste, an deren Kopf der Knoten hinzugefuegt werden soll.
 *
 * @return Liste, die durch Hinzufuegen entsteht.
 */
extern List listCons(Element e, List l)
{
  List res = NULL;
  
  res = calloc(1, sizeof(*l));
  
  if (!res) exit(1);

  res->value = e;
  res->next = l;
  
  return res;
}

/**
 * Liefert die Laenge der Liste l.
 *
 * @param[in] l Liste, deren Laenge geliefert werden soll.
 *
 * @return Laenge der Liste l.
 */
extern unsigned long listLength(List l)
{
  return (listIsEmpty(l))
       ? 0
       : 1 + listLength(listTail(l));
}

/**
 * Liefert das Element an der Position i in der Liste l.
 *
 * @param[in] l Liste, deren Element an der Position i geliefert werden soll.
 * @param[in] i Position des Elements, das geliefert werden soll.
 *
 * @return Element, das an der Position i in der Liste l steht.
 */
extern Element listAt(List l, unsigned long i)
{
  return (i == 0)
       ? listHead(l)
       : listAt(listTail(l), i - 1);
}

/**
 * Prueft, ob das Element e in der Liste l enthalten ist.
 *
 * @param[in] e Element, dessen Vorkommen in der Liste l geprueft werden soll.
 * @param[in] l Liste, deren Elemente geprueft werden sollen.
 *
 * @return 1, wenn das Element e in der Liste l enthalten ist, sonst 0.
 */
extern Boolean listIsIn(List l, Element e)
{
  return (listIsEmpty(l))
       ? 0
       : (listEqElement(listHead(l), e))
         ? 1
         : listIsIn(listTail(l), e);
}

/**
 * Fuegt das Element e in die Liste l ein.
 *
 * @param[in] e Element, das in die  Liste l eingefuegt werden soll.
 * @param[in] l Liste, in die das Element e eingefuegt werden soll..
 *
 * @return Liste, die das Element e enthaelt.
 */
extern List listInsert(List l, Element e)
{
  if (listIsEmpty(l)) return listCons(e, l);
  if (listEqElement(listHead(l), e)) return l;
  if (listGeElement(listHead(l), e)) return listCons(e, l);
  l->next = listInsert(listTail(l), e);
  return l;
}

/**
 * Entfernt das Element e aus der Liste l.
 *
 * @param[in] e Element, das aus der Liste l entfernt werden soll.
 * @param[in] l Liste, aus der das Element e entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List listRemove(List l, Element e)
{
  if (listIsEmpty(l)) return l;
  if (listGeElement(listHead(l), e) && !listEqElement(listHead(l), e)) return l;
  if (listEqElement(listHead(l), e)) return listRemoveHead(l);
  l->next = listRemove(listTail(l), e);
  return l;
}

/**
 * Entfernt alle Elemente aus der Liste l.
 *
 * @param[in] l Liste, aus der alle Elemente entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List listRemoveAll(List l)
{
  if (listIsEmpty(l)) return l;
  return listRemoveAll(listRemoveHead(l));
}

