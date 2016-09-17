/**
 * @file
 *
 * Das Modul definiert Materialeigenschaften.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "material.h"

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */
 
/* Materialeigenschaften */
static MaterialProps material = {
    /* Grass */
    { {0.0, 0.2, 0.0, 1.0} , {0.0, 0.3, 0.0, 1.0} , {0.2, 0.8, 0.1, 1.0} }
    /* Metal */
  , { {0.1, 0.1, 0.1, 1.0} , {0.1, 0.1, 0.1, 1.0} , {0.8, 0.8, 0.8, 1.0} }
    /* Glass */
  , { {0.1, 0.1, 0.1, 0.2} , {0.1, 0.1, 0.1, 0.2} , {0.9, 0.9, 0.9, 0.2} }
    /* Stone */
  , { {0.1, 0.1, 0.1, 1.0} , {0.1, 0.1, 0.1, 1.0} , {0.6, 0.6, 0.6, 1.0} }
    /* Water */
  , { {0.0, 0.0, 0.1, 0.5} , {0.0, 0.0, 0.1, 0.5} , {0.2, 0.5, 0.8, 0.5} }
    /* Sun */
  , { {5.0, 5.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} }
    /* Ball0 */
  , { {5.0, 0.0, 0.0, 1.0} , {1.0, 0.0, 0.0, 1.0} , {1.0, 0.0, 0.0, 1.0} }
    /* Ball1 */
  , { {0.0, 5.0, 0.0, 1.0} , {0.0, 1.0, 0.0, 1.0} , {0.0, 1.0, 0.0, 1.0} }
    /* Ball2 */
  , { {0.0, 0.0, 5.0, 1.0} , {0.0, 0.0, 1.0, 1.0} , {0.0, 0.0, 1.0, 1.0} }
    /* Ball3 */
  , { {5.0, 5.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} }
    /* Ball4 */
  , { {5.0, 0.0, 5.0, 1.0} , {1.0, 0.0, 1.0, 1.0} , {1.0, 0.0, 1.0, 1.0} }
    /* Ball5 */
  , { {0.0, 5.0, 5.0, 1.0} , {0.0, 1.0, 1.0, 1.0} , {0.0, 1.0, 1.0, 1.0} }
};

/* Spiegeligkeiten */
static float shininess[MAT_DUMMY] = { /* Spiegeligkeit */ 
                                       1.0 /* Grass    */
                                    ,  0.1 /* Metal    */
                                    ,  0.2 /* Glass    */
                                    ,  1.0 /* Stone    */
                                    ,  0.2 /* Water    */
                                    ,  1.0 /* Sun      */
                                    ,  1.0 /* Ball0    */
                                    ,  1.0 /* Ball1    */
                                    ,  1.0 /* Ball2    */
                                    ,  1.0 /* Ball3    */
                                    ,  1.0 /* Ball4    */
                                    ,  1.0 /* Ball5    */
                                    };

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Setzt den aktuellen Materialtyp auf m.
 *
 * @param[in] m Zu verwendender Materialtyp.
 */
extern void materialSet(Material m)
{
  /* Materialeigenschaften inklusive Spiegeligkeit setzen */
  glMaterialfv(GL_FRONT, GL_AMBIENT,   material[m][LIGHT_AMBIENT] );
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   material[m][LIGHT_DIFFUSE] );
  glMaterialfv(GL_FRONT, GL_SPECULAR,  material[m][LIGHT_SPECULAR]);
  glMaterialfv(GL_FRONT, GL_SHININESS, &shininess[m]);
  
  /* Farbe des Materials auf den Spekularen Anteil setzen */
  glColor4fv(material[m][LIGHT_SPECULAR]);
}
