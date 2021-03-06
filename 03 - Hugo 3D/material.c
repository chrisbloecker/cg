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
    /* Car */
    { {0.1, 0.1, 0.6, 1.0} , {0.1, 0.1, 0.3, 1.0} , {0.2, 0.2, 0.8, 1.0} }
  , /* Car Cabin */
    { {0.1, 0.1, 0.6, 1.0} , {0.1, 0.1, 0.3, 1.0} , {0.2, 0.2, 0.8, 1.0} }
  , /* Car Tyre */
    { {0.02, 0.02, 0.02, 1.0} , {0.01, 0.01, 0.01, 1.0} , {0.4, 0.4, 0.4, 1.0} }
  , /* Frontscheinwerfer */
    { {1.0, 1.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} , {1.0, 1.0, 0.0, 1.0} }
  , /* Rücklicht */
    { {1.0, 0.0, 0.0, 1.0} , {1.0, 0.0, 0.0, 1.0} , {1.0, 0.0, 0.0, 1.0} }
  , /* Ship */
    { {0.25, 0.4, 0.0, 1.0} , {0.1, 0.03, 0.0, 1.0} , {0.3, 0.2, 0.1, 1.0} }
  , /* Ship Sail */
    { {0.6, 0.6, 0.6, 1.0} , {0.0, 0.0, 0.0, 1.0} , {1.0, 1.0, 1.0, 1.0} }
  , /* Sunshade Stick */
    { {1.0, 1.0, 1.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.9, 0.9, 0.9, 1.0} }
  , /* Sunshade Top Inner */
    { {0.5, 0.5, 0.0, 1.0} , {0.2, 0.2, 0.0, 1.0} , {0.3, 0.3, 0.0, 1.0} }
  , /* Sunshade Top Outer */
    { {0.5, 0.0, 0.0, 1.0} , {0.5, 0.0, 0.0, 1.0} , {0.5, 0.0, 0.0, 1.0} }
  , /* Tree Trunk */
    { {0.3, 0.3, 0.3, 1.0} , {0.43, 0.12, 0.01, 1.0} , {0.33, 0.06, 0.01, 1.0} }
  , /* Tree Crown */
    { {0.1, 0.4, 0.1, 1.0} , {0.0, 0.5, 0.0, 1.0} , {0.0, 0.2, 0.0, 1.0} }
  , /* Buoy */
    { {0.8, 0.2, 0.1, 1.0} , {0.4, 0.1, 0.1, 1.0} , {0.3, 0.0, 0.0, 1.0} }
  , /* Palm Trunk Dark */
    { {0.4, 0.2, 0.02, 1.0} , {0.2, 0.1, 0.01, 1.0} , {0.2, 0.1, 0.01, 1.0} }
  , /* Palm Trunk Light */
    { {0.6, 0.2, 0.02, 1.0} , {0.2, 0.1, 0.01, 1.0} , {0.3, 0.1, 0.01, 1.0} }
  , /* Palm Leaf */
    { {0.1, 0.5, 0.1, 1.0} , {0.0, 0.2, 0.0, 1.0} , {0.0, 0.3, 0.0, 1.0} }
  , /* Coconut */
    { {0.2, 0.1, 0.0, 1.0} , {0.2, 0.05, 0.0, 1.0} , {0.43, 0.12, 0.01, 1.0} }
  , /* Coin */
    { {0.7, 0.7, 0.0, 1.0} , {0.3, 0.3, 0.3, 1.0} , {0.4, 0.4, 0.0, 1.0} }
  , /* Starfish */
    { {0.8, 0.1, 0.1, 1.0} , {0.2, 0.01, 0.01, 1.0} , {0.2, 0.0, 0.0, 1.0} }
  , /* Ice Waffle */
    { {0.4, 0.3, 0.2, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.4, 0.3, 0.2, 1.0} }
  , /* Ice Banana */
    { {0.9, 0.9, 0.6, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.5, 0.25, 1.0} }
  , /* Ice Strawberry */
    { {0.9, 0.6, 0.6, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.3, 0.3, 1.0} }
  , /* Ice Chocolate */
    { {0.6, 0.4, 0.2, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.3, 0.2, 0.1, 1.0} }
  , /* Street */
    { {0.4, 0.4, 0.4, 1.0} , {0.2, 0.2, 0.2, 1.0} , {0.2, 0.2, 0.2, 1.0} }
  , /* Street Surface */
    { {0.7, 0.7, 0.7, 1.0} , {0.5, 0.5, 0.5, 1.0} , {0.8, 0.8, 0.8, 1.0} }
  , /* Street Background */
    { {0.3, 0.3, 0.3, 1.0} , {0.5, 0.5, 0.5, 1.0} , {0.5, 0.4, 0.2, 1.0} }
  , /* Beach */
    { {0.3, 0.3, 0.3, 1.0} , {0.3, 0.3, 0.3, 1.0} , {1.0, 0.9, 0.3, 1.0} }
  , /* Beach Surface */
    { {0.4, 0.4, 0.4, 1.0} , {0.3, 0.3, 0.3, 1.0} , {0.7, 0.5, 0.2, 1.0} }
  , /* Beach Background */
    { {0.5, 0.5, 0.5, 1.0} , {0.3, 0.3, 0.3, 1.0} , {0.8, 0.7, 0.1, 1.0} }
  , /* River */
    { {0.3, 0.3, 0.8, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.2, 0.2, 0.5, 1.0} }
  , /* River Surface */
    { {0.0, 0.0, 0.8, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.0, 0.0, 0.5, 1.0} }
  , /* River Background */
    { {0.2, 0.3, 0.5, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.7, 1.0, 1.0} }
  , /* Lighthouse Red */
    { {1.0, 0.0, 0.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.0, 0.0, 1.0} }
  , /* Lighthouse White */
    { {1.0, 1.0, 1.0, 1.0} , {0.0, 0.0, 0.0, 1.0} , {0.5, 0.5, 0.5, 1.0} }
  , /* Hugo Body */
    { {0.6, 0.4, 0.2, 1.0} , {0.3, 0.2, 0.1, 1.0} , {0.4, 0.0, 0.0, 1.0} }
  , /* Hugo Legs */
    { {0.8, 0.4, 0.2, 1.0} , {0.7, 0.4, 0.0, 1.0} , {0.4, 0.1, 0.0, 1.0} }
  , /* Hugo Feet */
    { {0.04, 0.04, 0.04, 1.0} , {0.02, 0.02, 0.02, 1.0} , {0.4, 0.4, 0.4, 1.0} }
  , /* Hugo Arms */
    { {0.8, 0.4, 0.2, 1.0} , {0.7, 0.4, 0.0, 1.0} , {0.4, 0.1, 0.0, 1.0} }
  , /* Hugo Head */
    { {0.9, 0.6, 0.4, 1.0} , {0.2, 0.3, 0.0, 1.0} , {0.4, 0.3, 0.0, 1.0} }
};

/* Spiegeligkeiten */
static float shininess[MATERIALS] = { /* Spiegeligkeit */ 
                                       1.0 /* Car                */
                                    ,  1.0 /* Car Cabin          */
                                    ,  1.0 /* Car Tyre           */
                                    ,  1.0 /* Car Frontlight     */
                                    ,  1.0 /* Car Backlight      */
                                    ,  1.0 /* Ship               */
                                    ,  1.0 /* Ship Sail          */
                                    ,  1.0 /* Sunshade Stick     */
                                    ,  1.0 /* Sunshade Top Inner */
                                    ,  1.0 /* Sunshade Top Outer */
                                    ,  1.0 /* Tree Trunk         */
                                    ,  2.0 /* Tree Crown         */
                                    ,  1.0 /* Buoy               */
                                    ,  1.0 /* Palm Trunk Dark    */
                                    ,  1.0 /* Palm Trunk Light   */
                                    ,  1.0 /* Palm Leaf          */
                                    ,  1.0 /* Coconut            */
                                    ,  1.0 /* Coin               */
                                    ,  1.0 /* Starfish           */
                                    ,  1.0 /* Ice Waffle         */
                                    ,  1.0 /* Ice Banana         */
                                    ,  1.0 /* Ice Strawberry     */
                                    ,  1.0 /* Ice Chocolate      */
                                    ,  1.0 /* Street             */
                                    ,  1.0 /* Street Surface     */
                                    ,  1.0 /* Street Background  */
                                    ,  1.0 /* Beach              */
                                    ,  1.0 /* Beach Surface      */
                                    ,  1.0 /* Beach Background   */
                                    ,  1.0 /* River              */
                                    ,  1.0 /* River Surface      */
                                    ,  1.0 /* River Background   */
                                    ,  0.5 /* Lighthouse Red     */
                                    ,  0.5 /* Lighthouse White   */
                                    ,  1.0 /* Hugo Body          */
                                    ,  1.0 /* Hugo Legs          */
                                    ,  1.0 /* Hugo Feet          */
                                    ,  1.0 /* Hugo Arms          */
                                    ,  1.0 /* Hugo Head          */
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
