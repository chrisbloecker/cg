/**
 * @file
 *
 *
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "vertex.h"

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
 * Kopiert die Vertexdaten von s nach d.
 *
 * @param[in] s Zeiger auf Source-Vertex.
 * @param[in] d Zeiger auf Destination-Vertex.
 */
extern void vertexCopy(Vertex * s, Vertex * d)
{
  int i;
  
  for (i = 0; i < VERTEX_ATTRIBUTE_DUMMY; ++i)
    (*d)[i] = (*s)[i];
}

/**
 * Gibt den Vertex v auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] v      Zeiger auf Vertex.
 */
extern void vertexPrint(FILE * stream, Vertex * v)
{
  unsigned i;
  
  fprintf(stream, "Vertex : (%f", (*v)[0]);
  for (i = 1; i < VERTEX_ATTRIBUTE_DUMMY; ++i)
    fprintf(stream, ", %f", (*v)[i]);
  fprintf(stream, ")\n");
}