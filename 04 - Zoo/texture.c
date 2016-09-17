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
#include "displaylist.h"
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

/**
 * Bildinformationen : Höhe, Breite
 */
typedef struct
{
  int x
    , y
    , w
    , h;
} IntRect;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/** Anzahl der Texturen. */
#define TEXTURE_COUNT (TEXTURE_EMPTY)

/** Ausmaße einer selbst berechneten Textur */
#define TEXTURE_SIZE (256)

/** BPP einer selbst berechneten Textur */
#define TEXTURE_BPP (1)

/** Kanalinformationen */
#define I_MIN (  0)
#define I_MAX (255)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/**
 * Pixel an Koordinate x/y von d setzen (Kanal ch)
 */
#define setPixel(d,bpp,w,x,y,ch,v) ((*((d) + ((bpp) * ((w) * (y) + (x))) + (ch))) = (unsigned char)(v))

/**
 * Gibt einen Zeiger auf den Wert zurück, der in d an Stelle x/y ist.
 */
#define getPixel(d,bpp,w,x,y,ch) ((d) + ((bpp) * ((w) * (y) + (x))) + (ch))

/**
 * Berechnet, ob eine Koordinate innerhalb der Grenzen eines Bildes liegt.
 */
#define isIn(x,y,bounds) (((x) >= 0) && ((y) >= 0) && ((x) < (bounds.w)) && ((y) < (bounds.h)))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

static int useTexture      = 0  /* Selbst berechnete Textur verwenden             */
         , textureSize     = 3  /* Größe des Musters der selbst berechneten Texur */
         ;

static Texture textures[TEXTURE_COUNT]; /* Texturen */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Gibt den Wert des Pixels x/y des Kanals ch in buf zurück.
 * Befindet sich x/y außerhalb des Bildes, so wird periodische Fortsetzung 
 * am Rand des Bildes angenommen. (Thorus-Faltung)
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     Auszulesender Kanal
 * @return           Wert des Pixels x/y des Kanals ch von buf    wenn x/y im gültigen
 *                                                                Bereich liegt
 *                   periodische Fortsetzung des Randes           sonst
 */
static unsigned char getPixelPeriodCont(unsigned char * buf, unsigned char bpp, int x, int y, IntRect bounds, unsigned char ch)
{
  while (x < 0)
    x += bounds.w;
  
  while (y < 0)
    y += bounds.h;
  
  return *getPixel(buf, bpp, bounds.w, x % bounds.w, y % bounds.h, ch);
}

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
  
  textures[TEXTURE_GRASS].filename   = "textures/grass.png";
  textures[TEXTURE_METAL].filename   = "textures/metal.png";
  textures[TEXTURE_STONE].filename   = "textures/stone.png";
  textures[TEXTURE_SOCKET].filename  = "textures/socket.png";
  textures[TEXTURE_WATER].filename   = "textures/water.png";
  textures[TEXTURE_PAPER].filename   = "textures/paper.png";
  textures[TEXTURE_GIRAFFE].filename = "textures/giraffe.png";
  textures[TEXTURE_FISH].filename    = "textures/fish.png";
  textures[TEXTURE_SUN].filename     = "textures/sun.png";
  
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

/**
 * Berechnet eine Textur, welche aus schwarzen und weißen Quadraten besteht.
 *
 * @return Zeiger auf eine Textur, welche aus Quadraten besteht.
 */
static CGImage * calcSquares(void)
{
  int x   /* Pixel -     */
    , y   /* Koordinaten */
    , ch  /* Kanal       */
    , v   /* Pixelwert   */
    , s = (int) (TEXTURE_SIZE / pow(2, textureSize))
    ;
  
  CGImage * squares;
  
  /* Initialisieren */
  squares = CGImage_create(TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_BPP);
  
  for (y = 0; y < TEXTURE_SIZE; ++y)
  {
    for (x = 0; x < TEXTURE_SIZE; ++x)
    {
      for (ch = 0; ch < TEXTURE_BPP; ++ch)
      {
        /* Farbe bestimmen */
        v = ((x % (2 * s) > s) == (y % (2 * s) > s))
          ? I_MAX
          : I_MIN
          ;
        
        /* setzen */
        setPixel(squares->data, TEXTURE_BPP, TEXTURE_SIZE, x, y, ch, v);
      }
    }
  }
  
  return squares;
}

/**
 * Berechnet eine Textur, welche aus vertikalen schwarzen und weißen Streifen
 * besteht.
 *
 * @return Zeiger auf eine Textur, welche aus Streifen besteht.
 */
static CGImage * calcStripes(void)
{
  int x   /* Pixel -     */
    , y   /* Koordinaten */
    , ch  /* Kanal       */
    , v   /* Pixelwert   */
    , s = (int) (TEXTURE_SIZE / pow(2, textureSize))
    ;
  
  CGImage * stripes;
  
  /* Initialisieren */
  stripes = CGImage_create(TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_BPP);
  
  /* Streifen */
  for (y = 0; y < TEXTURE_SIZE; ++y)
  {
    for (x = 0; x < TEXTURE_SIZE; ++x)
    {
      for (ch = 0; ch < TEXTURE_BPP; ++ch)
      {
        /* Farbe bestimmen */
        v = (y % (2 * s) <= s)
          ? I_MAX
          : I_MIN
          ;
        
        /* setzen */
        setPixel(stripes->data, TEXTURE_BPP, TEXTURE_SIZE, x, y, ch, v);
      }
    }
  }
  
  return stripes;
}

/**
 * Berechnet eine Textur, welche aus der Drehung um angle aus in entsteht.
 * Hierbei wird Inverse-Mapping mit Nearest Neighbor und 
 * periodischer Bildfortsetzung verwendet.
 *
 * @param[in] in    Zeiger auf Textur, die um angle gedreht werden soll.
 * @param[in] angle Drehwinkel.
 *
 * @return Zeiger auf die Textur, die durch die Drehung um angle aus in entsteht.
 */
static CGImage * textureRotate(CGImage * in, unsigned angle)
{
  int x   /* Pixel -     */
    , y   /* Koordinaten */
    , ch  /* Kanal       */
    , v   /* Pixelwert   */
    ;
  
  /* Bildgrenzen */
  IntRect bounds = {0, 0, TEXTURE_SIZE, TEXTURE_SIZE};
  
  Vector3d u;
  
  /* Initialisieren */
  CGImage * out = CGImage_create(TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_BPP);
  
  for (y = 0; y < TEXTURE_SIZE; ++y)
  {
    for (x = 0; x < TEXTURE_SIZE; ++x)
    {
      for (ch = 0; ch < TEXTURE_BPP; ++ch)
      {
        /* Bildkoordinate im Quellbild berechnen */
        u = vectorRotateZ(vectorMake(x, y, 0), 360 - angle);
        
        /* Aus dem Quellbild mittels NN und periodischer Fortsetzung auslesen */
        v = getPixelPeriodCont(in->data, TEXTURE_BPP, (int) (u.x + 0.5), (int) (u.y + 0.5), bounds, ch);
        
        /* Im Zielbild setzen */
        setPixel(out->data, TEXTURE_BPP, TEXTURE_SIZE, x, y, ch, v);
      }
    }
  }
  
  return out;
}

/**
 * Berechnet Texturen für die Giraffe.
 */
static void calcTextures(void)
{
  CGImage * squares = calcSquares() /* Quadratmuster  */
        , * stripes = calcStripes() /* Streifenmuster */
        , * tex
        ;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Calculating Textures.\n");
  #endif
  
  /* Karos */
  tex = textureRotate(squares, 45);
  
  glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_OWN_1].id);
  gluBuild2DMipmaps( GL_TEXTURE_2D
                   , tex->bpp
                   , tex->width
                   , tex->height
                   , calculateGLBitmapMode(tex)
                   , GL_UNSIGNED_BYTE
                   , tex->data);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  /* Rotierte löschen */
  CGImage_free(tex);
  
  
  /* Streifen */
  tex = textureRotate(stripes, 45);
  
  glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_OWN_2].id);  
  gluBuild2DMipmaps( GL_TEXTURE_2D
                   , tex->bpp
                   , tex->width
                   , tex->height
                   , calculateGLBitmapMode(tex)
                   , GL_UNSIGNED_BYTE
                   , tex->data);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  /* Rotierte löschen */
  CGImage_free(tex);
  
  /* Texturen löschen */
  CGImage_free(squares);
  CGImage_free(stripes);
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
  /* Der Giraffe evtl. die selbst berechneten Texturen verpassen */
  if (t == TEXTURE_GIRAFFE && useTexture != 0)
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_OWN_1 + useTexture - 1].id);
  
  /* Allen anderen die normale Textur */
  else
    glBindTexture(GL_TEXTURE_2D, textures[t].id);
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
    for (i = 0; i <= TEXTURE_SUN; ++i)
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
    
    calcTextures();
    
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
  useTexture = (useTexture + 1) % 3;
  
  displaylistSet();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: useTexture : %i\n", useTexture);
  #endif
}

/**
 * Inkrementiert die Größe der selbst berechneten Muster um i.
 *
 * @param[in] i Inkrementor.
 */
extern void textureSetSize(int i)
{
  textureSize += i;
  
  /* toBounds */
  if (textureSize < 0)
    textureSize = 0;
  else if (pow(2, textureSize) > TEXTURE_SIZE)
    textureSize = (int) (log(TEXTURE_SIZE) / log(2));
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: textureSize : %i\n", textureSize);
  #endif
  
  /* Texturen neu berechnen */
  calcTextures();
}
