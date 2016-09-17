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
 * Erzeugt einen Vektor aus einem Vertex.
 *
 * @param[in] s Zeiger auf den Vertex.
 *
 * @return Vektor, der durch den Vertex s entstanden ist.
 */
extern Vector6d vertexMakeVector6dFromVertex(Vertex * s)
{
  return vector6dMake( (*s)[VERTEX_ATTRIBUTE_X]
                     , (*s)[VERTEX_ATTRIBUTE_Y]
                     , (*s)[VERTEX_ATTRIBUTE_Z]
                     , (*s)[VERTEX_ATTRIBUTE_R]
                     , (*s)[VERTEX_ATTRIBUTE_G]
                     , (*s)[VERTEX_ATTRIBUTE_B]
                     );
}

/**
 * Kopiert die Vertexdaten von s nach d.
 *
 * @param[in] s Zeiger auf Source-Vertex.
 * @param[in] d Zeiger auf Destination-Vertex.
 */
extern void vertexCopy(Vertex * s, Vertex * d)
{
  int i;
  
  for (i = 0; i < 6; ++i)
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
  fprintf(stream, "Vertex : (%f, %f, %f, %f, %f, %f)\n", (*v)[0], (*v)[1], (*v)[2], (*v)[3], (*v)[4], (*v)[5]);
}
