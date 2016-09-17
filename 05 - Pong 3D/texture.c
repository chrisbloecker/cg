/**
 * @file
 * Texturen-Modul.
 * Das Modul kapselt die Textur-Funktionalitaet (insbesondere das Laden und
 * Binden) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "texture.h"
#include "cgimage.h"
#include "types.h"
#include "vector.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* Textur */
typedef struct {
  unsigned id;
  char * filename;
} Texture;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/** Anzahl der Texturen. */
#define TEXTURE_COUNT (TEXTURE_EMPTY)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

static Boolean texturing = TRUE;

static Texture textures[TEXTURE_COUNT]; /* Texturen */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert das Texturen-Array und setzt die Dateinamen der zu ladenden
 * Texturen.
 */
static int initTextureArray(void)
{
  int i;
  
  unsigned texIDs[TEXTURE_COUNT];
  glGenTextures(TEXTURE_COUNT, texIDs);
  
  for (i = 0; i < TEXTURE_COUNT; ++i)
  {
    textures[i].id = texIDs[i];
  }
  
  textures[TEXTURE_METAL].filename = "textures/metal.png";
  textures[TEXTURE_STONE].filename = "textures/stone.png";
  textures[TEXTURE_WATER].filename = "textures/water.png";
  textures[TEXTURE_SUN].filename   = "textures/sun.png";
  
  return 1;
}

/**
 * Bestimmt den BitmapMode der angegebenen Textur.
 *
 * @param[in] self CGImage, dessen BitmapMode bestimmt werden soll.
 *
 * @return BitmapMode der angegebenen Textur.
 */
unsigned calculateGLBitmapMode(CGImage * self)
{
  switch (self->bpp)
  {
    case 1:
      return GL_LUMINANCE;
    case 2:
      return GL_LUMINANCE_ALPHA;
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
    default:
      return GL_RGB;
  }
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Bindet die Textur texture, so dass sie fuer alle nachfolgende gezeichneten
 * Primitiven verwendet wird.
 *
 * @param texture Bezeichner der Textur, die gebunden werden soll.
 */
extern void bindTexture(TextureName t)
{
  texturing
    ? glBindTexture(GL_TEXTURE_2D, textures[t].id)
    : glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_EMPTY].id)
    ;
}

/**
 * Laed Texturen und initialisiert das Texturmapping.
 *
 * @return 1 wenn Laden und Initialisieren erfolgreich war
 *         0 sonst.
 */
extern int loadTextures(void)
{
  int i;
  
  CGImage * image = NULL;

  if (initTextureArray())
  {
    /* Alle Texturen nacheinander laden. */
    for (i = 0; i < TEXTURE_EMPTY; ++i)
    {
      #ifdef DEBUG
      fprintf(stderr, "DEBUG :: Loading %s.\n", textures[i].filename);
      #endif
      
      image = CGImage_load(textures[i].filename);

      if (image != NULL)
      {
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        
        gluBuild2DMipmaps( GL_TEXTURE_2D
                         , image->bpp
                         , image->width
                         , image->height
                         , calculateGLBitmapMode(image)
                         , GL_UNSIGNED_BYTE
                         , image->data);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        CGImage_free(image);
      }
      else
        return 0;
    }
    
    return 1;
  }
  else
    return 0;
}

/**
 * Laed Texturen und initialisiert das Texturmapping.
 *
 * @return 1 wenn Laden und Initialisieren erfolgreich war
 *         0 sonst.
 */
extern int initTextures(void)
{
  /* Ebenen-Koeffizienten für Generierung von Textur-Koordinaten */
  /* Parallele zur y-z-Ebene */
  GLfloat map_s[4] = { 2.0f, 0.0f, 0.0f, 0.0f };
  /* Parallele zur x-z-Ebene */
  GLfloat map_t[4] = { 0.0f, 2.0f, 0.0f, 0.0f };

  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  /* Einstellungen für die automatische Generierung von Texturkoordinaten
   * nur gültig im Falle von impliziter Texturierung */
  glTexGenfv(GL_S, GL_OBJECT_PLANE, map_s);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, map_t);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  
  /* Texturen laden */
  return loadTextures();
}

/**
 * Schaltet die Giraffen-Textur um.
 */
extern void textureSwitch(void)
{
  texturing = !texturing;
  
  texturing
    ? glEnable(GL_TEXTURE_2D)
    : glDisable(GL_TEXTURE_2D)
    ;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: texturing %s.\n", texturing ? "on" : "off");
  #endif
}
