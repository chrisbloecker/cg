#ifndef __VERTEX_H__
#define __VERTEX_H__
/**
 * @file
 * Schnittstelle des Moduls für Vertices.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "vector6d.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */
 
/** Attribute eines Vertex */
typedef enum {
  VERTEX_ATTRIBUTE_X
, VERTEX_ATTRIBUTE_Y
, VERTEX_ATTRIBUTE_Z
, VERTEX_ATTRIBUTE_R
, VERTEX_ATTRIBUTE_G
, VERTEX_ATTRIBUTE_B
, VERTEX_ATTRIBUTE_DUMMY
} VertexAttribute;

/** Ein Vertex */
typedef GLdouble Vertex[VERTEX_ATTRIBUTE_DUMMY];

/* ----------------------------------------------------------------------------
 * Konstanten
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
extern Vector6d vertexMakeVector6dFromVertex(Vertex * s);

/**
 * Kopiert die Vertexdaten von s nach d.
 *
 * @param[in] s Zeiger auf Source-Vertex.
 * @param[in] d Zeiger auf Destination-Vertex.
 */
extern void vertexCopy(Vertex * s, Vertex * d);

/**
 * Gibt den Vertex v auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] v      Zeiger auf Vertex.
 */
extern void vertexPrint(FILE * stream, Vertex * v);

#endif
