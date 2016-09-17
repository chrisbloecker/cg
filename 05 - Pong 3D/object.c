/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */

#include <float.h>
#include <assert.h>
#include <GL/gl.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */

#include "object.h"
#include "types.h"
#include "drawing.h"
#include "logic.h"

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
static Boolean grEl(Object * i, Object * j)
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
static Boolean objectsInv(Objects * os, int i)
{
  return (i == os->last - 1)
      || (!grEl(os->objects[i], os->objects[i+1])
          && objectsInv(os, i+1)
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
static Objects * calcSizes(Objects * os, Vector3d eye, Vector3d n)
{
  int i = 0;
  
  /* Nur mit normierten Vektoren arbeiten */
  n = vectorNorm(n);
  
  /* Jedem Objekt eine Größe zuordnen -> Hesse'sche Normalform */
  for (i = 0; i < os->last; ++i)
    os->objects[i]->size = vectorMult(vectorSub(os->objects[i]->t, eye), n);
  
  return os;
}

/**
 * Erzeugt ein Objekt und gibt dieses zurück.
 *
 * @param[in] f    Figurtyp.
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] d    Zeichenfunktion.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
static Object objectMake(Figure f, Vector3d t, Vector3d r, Vector3d s, double a, DrawFunction d, TextureName tex, Material m, char * name)
{
  Object o;
  
  o.f = f;
  
  o.t = t;
  o.r = r;
  o.s = s;
  
  o.d = d;
  
  o.a = a;
  
  o.tex = tex;
  o.m   = m;
  
  o.name = name;
  
  o.z = 0;
  
  return o;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt ein Objekt vom Typ Cube und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeCube(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name)
{
  return objectMake( FIGURE_CUBE
                   , t
                   , r
                   , s
                   , a
                   , logicGetDrawFunction(DF_CUBE)
                   , tex
                   , m
                   , name
                   );
}

/**
 * Erzeugt ein Objekt vom Typ Sphere und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeSphere(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name)
{
  return objectMake( FIGURE_SPHERE
                   , t
                   , r
                   , s
                   , a
                   , logicGetDrawFunction(DF_SPHERE)
                   , tex
                   , m
                   , name
                   );
}

/**
 * Erzeugt ein Objekt vom Typ Cylinder und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeCylinder(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name)
{
  return objectMake( FIGURE_CYLINDER
                   , t
                   , r
                   , s
                   , a
                   , logicGetDrawFunction(DF_CYLINDER)
                   , tex
                   , m
                   , name
                   );
}

/**
 * Erzeugt ein Objekt vom Typ Kegel und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeKegel(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name)
{
  return objectMake( FIGURE_KEGEL
                   , t
                   , r
                   , s
                   , a
                   , logicGetDrawFunction(DF_KEGEL)
                   , tex
                   , m
                   , name
                   );
}

/**
 * Zeichnet das Objekt o.
 *
 * @param[in] o Zu zeichnendes Objekt.
 */
extern void objectDraw(Object * o)
{
  glPushMatrix();
    /* Affinieren */
    glTranslatef(o->t.x, o->t.y, o->t.z);
    glRotatef(o->a, o->r.x, o->r.y, o->r.z);
    glScalef(o->s.x, o->s.y, o->s.z);
    
    /* Material setzen */
    materialSet(o->m);
    
    /* Zeichnen */
    o->d(o->tex);
  glPopMatrix();
}

/**
 * Erzeugt ein Objects und gibt es initialisiert zurück.
 *
 * @return initialisiertes Objects.
 */
extern Objects objectsInit(void)
{
  Objects os;
  
  os.last = 0;
  os.zMax = 0;
  
  return os;
}

/**
 * Fügt das Objekt o in den Level os ein.
 *
 * @param[in] l   Level.
 * @param[in] i   Einzufügendes Objekt.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return Level, in den i eingefügt wurde.
 */
extern Objects * objectsInsert(Objects * os, Object * o, Vector3d eye, Vector3d n)
{
  /* Es muss noch Platz sein im Level */
  assert(os->last <= OBJECT_COUNT);
  
  /* Nur mit normierten Vektoren arbeiten */
  n = vectorNorm(n);
  
  /* Einfügen von i */
  os->objects[os->last++] = o;
  
  /* z-Ebenen */
  if (o->z > os->zMax)
    os->zMax = o->z;
  
  /* Sortieren */
  os = objectsSort(os, eye, n);
  
  return os;
}

/**
 * Gibt einen Zeiger auf das i-te Object von os zurück.
 *
 * @param[in] os Objekte.
 * @param[in] i  Index des auszuwählenden Elementes.
 *
 * @return Zeiger auf das i-te Element von l.
 */
extern Object * objectsAt(Objects * os, int i)
{
  assert(i < os->last);
  
  return os->objects[i];
}

/**
 * Sortiert die Objekte os.
 *
 * @param[in] os  Objekte.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return sortierter Level.
 */
extern Objects * objectsSort(Objects * os, Vector3d eye, Vector3d n)
{
  Boolean sorted = FALSE;
  
  int i = 0;
  
  Object * tmp;
  
  /* Den Items Gewichte zuordnen */
  os = calcSizes(os, eye, n);
  
  /* Bubble-Sort */
  while (!sorted)
  {
    sorted = TRUE;
    
    for (i = 0; i < os->last - 1; ++i)
      if (grEl(os->objects[i], os->objects[i+1]))
      {
        sorted = FALSE;
        
        tmp              = os->objects[i];
        os->objects[i]   = os->objects[i+1];
        os->objects[i+1] = tmp;
      }
  }
  
  /* Der Level muss sortiert sein */
  assert(objectsInv(os, 0));
  
  return os;
}

#ifdef DEBUG
/**
 * Gibt die Objekte os auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] os     Objekte.
 */
extern void objectsPrint(FILE * stream, Objects * os)
{
  int i = 0;
  
  fprintf(stream, "DEBUG :: ObjectsInfo :\n");  
  
  for (i = 0; i < os->last; ++i)
  {
    fprintf(stream, "Object %i :\n", i);
    fprintf(stream, "  t    : (%f, %f, %f)\n", os->objects[i]->t.x, os->objects[i]->t.y, os->objects[i]->t.z);
    fprintf(stream, "  r    : (%f, %f, %f)\n", os->objects[i]->r.x, os->objects[i]->r.y, os->objects[i]->r.z);
    fprintf(stream, "  s    : (%f, %f, %f)\n", os->objects[i]->s.x, os->objects[i]->s.y, os->objects[i]->s.z);
    fprintf(stream, "  size : %f\n", os->objects[i]->size);
  }
}

/**
 * Gibt die Zeichenreihenfolge der Objekte von os aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] os     Objekte.
 */
extern void objectsPrintDrawingOrder(FILE * stream, Objects * os)
{
  /* Back -> Front */
  int i = os->last - 1;
  
  fprintf(stream, "DEBUG :: Drawing Order :");
  
  while (i >= 0)
  {
    fprintf(stream, " %s >", os->objects[i]->name);
    
    --i;
  }
  
  fprintf(stream, "\n");
}
#endif
