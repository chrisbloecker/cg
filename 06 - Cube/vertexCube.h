#ifndef __VERTEXCUBE_H__
#define __VERTEXCUBE_H__
/**
 * @file
 * Schnittstelle des Moduls für Vertex-Würfel.
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
#include "vector3d.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Struktur eines Würfels */
typedef struct {
  Vertex * v; /* Zeiger auf die Vertex-Informationen */
  int s;      /* Feinheit                            */
  GLuint * i; /* Zeiger auf die Indizes zum Zeichnen */
} VertexCube;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Berechnet die Anzahl der Knoten des Würfels.
 *
 * @param[in] s Unterteilungstiefe.
 *
 * @return Anzahl der Knoten des Würfels.
 */
extern unsigned vertexCubeNumOfNodes(unsigned s);

/**
 * Initialisiert den Vertex-Cube c.
 *
 * @param[in] c Zu initialisierender Vertex-Cube.
 */
extern VertexCube vertexCubeInit(VertexCube c);

/**
 * Gibt den vom Vertex-Cube c belegten Speicher frei.
 *
 * @param[in] c Zu löschender Vertex-Cube.
 */
extern VertexCube vertexCubeClear(VertexCube c);

/**
 * Erzeugt einen Würfel der "Größe" s und übernimmt dabei die vorhandenen
 * Informationen aus c.
 *
 * @param[in] c Würfel, der bereits Informationen enthält.
 * @param[in] s "Größe" des neuen Würfels.
 */
extern VertexCube vertexCubeMake(VertexCube c, unsigned s);

/**
 * Zeichnet die Knoten des Würfels c.
 *
 * @param[in] c        Würfel.
 * @param[in] selectes Index des ausgewählten Knotens.
 */
extern void vertexCubeDrawNodes(VertexCube c, unsigned selected);

/**
 * Zeichnet die Seiten des Würfels anhand der zuvor berechneten 
 * Zeichenreihenfolge.
 *
 * @param[in] c Zu zeichnender Würfel.
 */
extern void vertexCubeDrawPlanes(VertexCube c);

/**
 * Weist dem ausgewählten Vertex eine zufällige Farbe zu.
 *
 * @param[in] c Würfel.
 * @param[in] i Index des ausgewählten Vertex.
 *
 * @return Würfel, bei dem der i-te Vertex eine neue Farbe erhalten hat.
 */
extern VertexCube vertexCubeSwitchColor(VertexCube c, unsigned i);

#endif
