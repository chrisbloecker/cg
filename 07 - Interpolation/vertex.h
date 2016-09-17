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
#include <stdio.h>
#include <GL/gl.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */
 
/** Attribute eines Vertex */
typedef enum {
  VERTEX_ATTRIBUTE_X
, VERTEX_ATTRIBUTE_Y
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
