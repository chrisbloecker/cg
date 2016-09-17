/**
 * @file
 *
 * Stellt Funktionen für die Arbeit mit der Level-Datenstruktur zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */
 
 /* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <assert.h>
#include <float.h>
#include <math.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "level.h"

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Ordnungsrelation.
 * Vergleicht die Items i und j.
 * Dabei gilt i > j wenn die z-Ebene von i kleiner ist als die von j
 *       oder       wenn die Entfernung von der Kamera von i größer ist als die von j.
 *
 * @param[in] i Item 1.
 * @param[in] j Item 2.
 *
 * @return true  wenn i > j
 *         false sonst.
 */
static Boolean grEl(LevelItem * i, LevelItem * j)
{
  return (i->z < j->z)
      || (i->z == j->z && i->size - j->size > DBL_EPSILON)
      ;
}

/**
 * Invariante.
 * Die Invariante hält, wenn l nur ein Element hat oder wenn gilt, dass für alle
 * i das i-te Element kleiner ist als das (i+1)-te.
 *
 * @param[in] l Level.
 * @param[in] i Index des "ersten" Elements.
 *
 * @return true  wenn die Invariante hält
 *         false sonst.
 */
static Boolean levelInv(Level l, unsigned i)
{
  return (i == l.last - 1)
      || (!grEl(l.items[i], l.items[i+1])
          && levelInv(l, i+1)
         );
}

/**
 * Ordnet den Elementen des Levels Größen zu.
 * Ein Element ist größer, je größer die Entfernung zum Augpunkt ist.
 *
 * @param[in] l   Level.
 * @param[in] eve Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return Level, wobei den Elementen Größen zugeordnet wurden.
 */
static Level calcSizes(Level l, Vector3d eye, Vector3d n)
{
  unsigned i = 0;
  
  /* Nur mit normierten Vektoren arbeiten */
  n = vectorNorm(n);
  
  /* Jedem Objekt eine Größe zuordnen -> Hesse'sche Normalform */
  for (i = 0; i < l.last; ++i)
    l.items[i]->size = vectorMult(vectorSub(l.items[i]->p, eye), n);
  
  return l;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert einen Level und gibt ihn zurück.
 *
 * @return Level.
 */
extern Level levelInit()
{
  Level l;
  
  l.last = 0;
  l.zMax = 0;
  
  return l;
}

/**
 * Fügt das Item i in den Level l ein.
 *
 * @param[in] l   Level.
 * @param[in] i   Einzufügendes Objekt.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return Level, in den i eingefügt wurde.
 */
extern Level levelInsert(Level l, LevelItem * i, Vector3d eye, Vector3d n)
{
  /* Es muss noch Platz sein im Level */
  assert(l.last <= LEVEL_ITEMCOUNT);
  
  /* Nur mit normierten Vektoren arbeiten */
  n = vectorNorm(n);
  
  /* Einfügen von i */
  l.items[l.last++] = i;
  
  /* z-Ebenen */
  if (i->z > l.zMax)
    l.zMax = i->z;
  
  /* Sortieren */
  levelSort(l, eye, n);
  
  return l;
}

/**
 * Erzeugt ein LevelItem und gibt es zurück.
 *
 * @param[in] p Position.
 * @param[in] r Rotationsache.
 * @param[in] t Translation.
 * @param[in] f Zeichenfunktion.
 * @param[in] a Drehwinkel.
 * @param[in] n Name.
 * @param[in] z z-Ebene.
 *
 * @return LevelItem mit den oben genannten Eigenschaften.
 */
extern LevelItem levelItemMake(Vector3d p, Vector3d r, Vector3d t, DrawFunctionType f, double a, PickingName n, unsigned z)
{
  LevelItem i;
  
  i.p = p; /* Position       */
  i.r = r; /* Rotation Axis  */
  i.t = t; /* Translation    */
  
  i.a = a; /* Rotation Angle */
  
  i.f = f; /* Draw Function  */
  
  i.n = n; /* Picking Name   */
  i.z = z; /* z-Ebene        */
  
  return i;
}

/**
 * Gibt einen Zeiger auf das i-te LevelItem von l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index des auszuwählenden Elementes.
 *
 * @return Zeiger auf das i-te Element von l.
 */
extern LevelItem * levelItemAt(Level l, unsigned i)
{
  assert(i < l.last);
  
  return l.items[i];
}

/**
 * Sortiert den Level l.
 *
 * @param[in] l   Level.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return sortierter Level.
 */
extern Level levelSort(Level l, Vector3d eye, Vector3d n)
{
  Boolean sorted = FALSE;
  
  unsigned i = 0;
  
  LevelItem * tmp;
  
  /* Den Items Gewichte zuordnen */
  l = calcSizes(l, eye, n);
  
  /* Dumb-Sort */
  while (!sorted)
  {
    sorted = TRUE;
    
    for (i = 0; i < l.last - 1; ++i)
      if (grEl(l.items[i], l.items[i+1]))
      /*if (l.items[i].size > l.items[i+1].size)*/
      {
        sorted = FALSE;
        
        tmp          = l.items[i];
        l.items[i]   = l.items[i+1];
        l.items[i+1] = tmp;
      }
  }
  
  /* Der Level muss sortiert sein */
  assert(levelInv(l, 0));
  
  return l;
}

/**
 * Gibt die String-Repräsentation des PickinName n zurück.
 *
 * @param[in] n PickinName.
 *
 * @return String-Repräsentation von n.
 */
extern char * levelPickingNameToString(PickingName n)
{
  char * res;
  
  switch (n)
  {
    case NAME_UNPICKABLE:
      res = "Unpickable";
      break;
    case NAME_GROUND:
      res = "Ground";
      break;
    case NAME_CEILING:
      res = "Ceiling";
      break;
    case NAME_SOCKET:
      res = "Socket";
      break;
    case NAME_FENCE_FRONT:
      res = "Fence Front";
      break;
    case NAME_FENCE_BACK:
      res = "Fence Back";
      break;
    case NAME_FENCE_LEFT:
      res = "Fence Left";
      break;
    case NAME_FENCE_RIGHT:
      res = "Fence Right";
      break;
    case NAME_POOL:
      res = "Pool";
      break;
    case NAME_WATER:
      res = "Water";
      break;
    case NAME_PIG:
      res = "Pig";
      break;
    case NAME_PIG_HEAD:
      res = "Pig Head";
      break;
    case NAME_PIG_BODY:
      res = "Pig Body";
      break;
    case NAME_GIRAFFE:
      res = "Giraffe";
      break;
    case NAME_GIRAFFE_HEAD:
      res = "Giraffe Head";
      break;
    case NAME_GIRAFFE_BODY:
      res = "Giraffe Body";
      break;
    case NAME_FISH:
      res = "Fish";
      break;
    case NAME_FISH_HEAD:
      res = "Fish Head";
      break;
    case NAME_FISH_BODY:
      res = "Fish Body";
      break;
    case NAME_SUN:
      res = "Sun";
      break;
    case NAME_GIRAFFE_CUBE:
      res = "Giraffe Cube";
      break;
  }
  
  return res;
}

#ifdef DEBUG
/**
 * Gibt den Level l aus.
 *
 * @param[in] l Level.
 */
extern void levelPrint(Level l)
{
  unsigned i = 0;
  
  fprintf(stderr, "DEBUG :: LevelInfo :\n");  
  
  for (i = 0; i < l.last; ++i)
  {
    fprintf(stderr, "Item %i :\n", i);
    fprintf(stderr, "  p    : (%f, %f, %f)\n", l.items[i]->p.x, l.items[i]->p.y, l.items[i]->p.z);
    fprintf(stderr, "  r    : (%f, %f, %f)\n", l.items[i]->r.x, l.items[i]->r.y, l.items[i]->r.z);
    fprintf(stderr, "  t    : (%f, %f, %f)\n", l.items[i]->t.x, l.items[i]->t.y, l.items[i]->t.z);
    fprintf(stderr, "  size : %f\n", l.items[i]->size);
    fprintf(stderr, "  n    : %s\n", levelPickingNameToString(l.items[i]->n));
  }
}

/**
 * Gibt die Zeichenreihenfolge der Objekte des Levels l aus.
 *
 * @param[in] l Level.
 */
extern void levelPrintDrawingOrder(Level l)
{
  /* Back -> Front */
  int i = l.last - 1;
  
  fprintf(stderr, "DEBUG :: Drawing Order :");
  
  while (i >= 0)
  {
    fprintf(stderr, " %s >", levelPickingNameToString(l.items[i]->n));
    
    --i;
  }
  
  fprintf(stderr, "\n");
}
#endif
