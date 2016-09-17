/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalität (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * System
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <GL/glut.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * Eigene
 * -------------------------------------------------------------------------- */
#include "scene.h"
#include "logic.h"
#include "vector.h"
#include "displaylist.h"
#include "picking.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
 
/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

static float 
/* Positionen der Lichtquellen */
  lightPos[LOGIC_COUNT_LIGHTS][4] = { {0.0, 5.0, 3.0, 1.0}
                                    }
                            
/* Farben der Lichtquellen */
, lightColor[LOGIC_COUNT_LIGHTS][3][4] = { { {0.1, 0.1, 0.1, 1.0}
                                           , {1.0, 1.0, 1.0, 1.0}
                                           , {1.5, 1.5, 1.5, 1.0}
                                           }
                                         }
;

static Boolean useDisplayLists = TRUE  /*   DisplayListen benutzen ? */
             #ifdef DEBUG
             , zInfo           = FALSE /* z-Informationen anzeigen ? */
             #endif
             ;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Ruft die Zeichenfunktion bzw. Displayliste zum Zeichnen der durch f gegebenen
 * Zeichenfunktion auf.
 *
 * @param[in] i Zeiger auf zu zeichnendes LevelItem.
 */
static void drawMyType(LevelItem * i)
{
  #ifdef DEBUG
  if (zInfo)
    fprintf(stderr, "DEBUG :: Drawing %s\n", levelPickingNameToString(i->n));
  #endif
  
  glPushName(i->n);
  
  glPushMatrix();
    glTranslatef(i->t.x, i->t.y, i->t.z);
    glRotatef(i->a, i->r.x, i->r.y, i->r.z);
    
    useDisplayLists
      ? displaylistCall(i->f)
      : logicGetDrawFunction(i->f)();
  glPopMatrix();
  
  glPopName();
}

/**
 * Lichtquellen setzen und ein- bzw. ausschalten.
 */
static void setLights(void)
{
  /* Sonne */
  if (logicGetLight(0))
  {
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor[0][1]);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightColor[0][0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor[0][2]);
  }
  
  logicGetLight(0)
    ? glEnable(GL_LIGHT0)
    : glDisable(GL_LIGHT0);
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Beruecksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 *
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
extern void setProjection(double aspect)
{
  /* perspektivische Projektion */
  gluPerspective( 70.0   /* Oeffnungswinkel      */
                , aspect /* Seitenverhaeltnis    */
                , 0.1    /* nahe Clipping-Ebene  */
                , 30.0   /* ferne Clipping-Ebene */
                );
}

/**
 * Setzen der Kameraposition.
 */
extern void setCamera(void)
{
  /* Kameraposition */
  Vector3d eye    = logicGetEye()
         , center = logicGetCenter()
         ;
  
  /* Kameraposition */
  gluLookAt( eye.x,    eye.y,    eye.z     /* Augpunkt  */
           , center.x, center.y, center.z  /* Zentrum   */
           , 0.0, 1.0, 0.0                 /* Up-Vektor */
           );
}

/**
 * Zeichnet die Szene
 */
extern void sceneDraw(Boolean names)
{
  int i = 0;
  
  unsigned z = 0;
  
  Level l = logicGetLevel();
  LevelItem * item;
  
  setLights();
  
  /* Blenden */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  i = l.last - 1;
  
  /* Back -> Front */
  while (z < l.zMax && i >= 0)
  {
    item = levelItemAt(l, i);
    
    /* z-BUffer bei neuer z-Ebene löschen */
    if (z != item->z)
    {
      #ifdef DEBUG
      if (zInfo)
        fprintf(stderr, "DEBUG :: Clearing Z-Buffer.\n");
      #endif
      
      glClear(GL_DEPTH_BUFFER_BIT);
      
      ++z;
    }
    
    if (names)
    {
      /* Zeichnen fürs Picking, nur die Elemente zeichnen, die pickable sind */
      if (logicIsPickable(item->n))
        drawMyType(item);
    }
    else
      drawMyType(item);
    
    --i;
  }
  
  /* letzte z-Ebene */
  while (i >= 0)
  {
    item = levelItemAt(l, i);
    
    if (names)
    {
      if (logicIsPickable(item->n))
        drawMyType(item);
    }
    else
      drawMyType(item);
    
    --i;
  }
  
  #ifdef DEBUG
  if (zInfo)
    fprintf(stderr, "\n");
  #endif
}

/**
 * Initialisierung der Szene.
 */
extern int sceneInit(void)
{
  /* Hintergrundfarbe */
  glClearColor( 0.0f
              , 0.0f
              , 0.0f
              , 0.0f
              );
  
  /* z-Buffer */
  glEnable(GL_DEPTH_TEST);
  
  /* Licht */
  glEnable(GL_LIGHTING);
  
  /* Culling */
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  
  /* Normalen normalisieren */
  glEnable(GL_NORMALIZE);
  
  /* Blending */
  glEnable(GL_BLEND);
  
  /* Texturierung */
  glEnable(GL_TEXTURE_2D);
  
  return 1;
}

/**
 * Legt fest, ob Displaylisten zum Zeichnen verwendet werden sollen oder nicht.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetDisplayListUsage(void)
{
  useDisplayLists = !useDisplayLists;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Display Lists %s.\n", useDisplayLists ? "on" : "off");
  #endif
}

#ifdef DEBUG
/**
 * Schaltet das Anzeigen von z-Informationen ein bzw. aus.
 */
extern void sceneSetZInfo(void)
{
  zInfo = !zInfo;
  
  fprintf(stderr, "DEBUG :: Z-Info %s.\n", zInfo ? "on" : "off");
}
#endif
