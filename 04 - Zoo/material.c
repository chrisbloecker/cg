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
    /* Fence */
  , { {0.1, 0.1, 0.1, 1.0} , {0.1, 0.1, 0.1, 1.0} , {0.2, 0.2, 0.2, 1.0} }
    /* Glass */
  , { {0.1, 0.1, 0.1, 0.2} , {0.1, 0.1, 0.1, 0.2} , {0.9, 0.9, 0.9, 0.2} }
    /* Stone */
  , { {0.1, 0.1, 0.1, 1.0} , {0.1, 0.1, 0.1, 1.0} , {0.7, 0.7, 0.7, 1.0} }
    /* Water */
  , { {0.0, 0.0, 0.1, 0.5} , {0.0, 0.0, 0.1, 0.5} , {0.2, 0.5, 0.8, 0.5} }
    /* Pig */
  , { {0.9, 0.6, 0.6, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.3, 0.3, 1.0} }
    /* Pig Eye */
  , { {0.0, 0.0, 0.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {1.0, 1.0, 1.0, 1.0} }
    /* Pig Nose */
  , { {0.0, 0.0, 0.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.1, 0.1, 0.1, 1.0} }
    /* Giraffe */
  , { {0.0, 0.0, 0.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {1.0, 0.9, 0.5, 1.0} }
    /* Fish */
  , { {0.0, 0.2, 0.0, 1.0} , {0.0, 0.3, 0.0, 1.0} , {0.2, 0.9, 0.3, 1.0} }
    /* Sun */
  , { {5.0, 5.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} }

};

/* Spiegeligkeiten */
static float shininess[MATERIALS] = { /* Spiegeligkeit */ 
                                       1.0 /* Grass    */
                                    ,  0.1 /* Fence    */
                                    ,  0.2 /* Glass    */
                                    ,  1.0 /* Stone    */
                                    ,  0.2 /* Water    */
                                    ,  1.0 /* Pig      */
                                    ,  1.0 /* Pig Eye  */
                                    ,  1.0 /* Pig Nose */
                                    ,  1.0 /* Giraffe  */
                                    ,  1.0 /* Sun      */
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
