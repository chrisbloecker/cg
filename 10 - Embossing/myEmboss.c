/****************************************************************************
 * emboss.c
 * Filtermodul eines Plug-ins fuer GIMP 2.2 and hoeher
 *
 * Embossfilter
 ****************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Definitionen fuer Plug-in-Konstanten etc. */
#include "plugin.h"

/*****************************************************************************
 * Types
 *****************************************************************************/

/**
 * Datenstruktur für einen Vektor im R³
 */
typedef struct {
  gfloat x
       , y
       , z;
} Vector;

/**
 * Bildinformationen : Höhe, Breite
 */
typedef struct
{
  gint x
     , y
     , w
     , h;
} GIntRect;

/**
 * Signatur eines Funktionszeigers für Funktionen zum Zugriff auf Pixel
 */
typedef guchar (*GetPixel)(guchar * buf, guchar bpp, GIntRect bounds, gint x, gint y, guchar ch);


/**
 * Signatur eines Funktionszeigers für Filterfunktionen
 */
typedef guchar * (*Filter)(GetPixel gp, guchar * srcBuf, guchar * dstBuf
                           , guchar bpp, gint x, gint y, GIntRect bounds);

/**
 * Informationen zum Filtern eines Bildes
 */
typedef struct
{
  Filter   filter;     /* Filterfunktion */
  GetPixel getPixel;   /* Pixelzugriffsfunktion */
  gint     filterSize; /* Ausmaße des Filterkernels */
  gchar *  filterName; /* Name des Filters */
} FilterInfo;

/*****************************************************************************
 * Constants
 *****************************************************************************/

/* die Zahl Pi */
#ifndef M_PI
#define M_PI (3.14159265)
#endif

/* Wertebereich Pixelintensitat */
#define I_MIN (0)
#define I_MAX (UCHAR_MAX)

/* Wert fuer Pixel außerhalb des Bildes */
#define I_OUT (0)

/* Größen der Filter */
#define GAUSSIANSIZE  (3)
#define EMBOSSSIZE    (3)
#define LUMINANCESIZE (1)
#define INVERTSIZE    (1)

/*****************************************************************************
 * Global Variables
 ****************************************************************************/
 
/* Lichtvektor für den Emboss-Filter */
Vector light;

/*****************************************************************************
 * Auxiliary Functions
 *****************************************************************************
 * Pixel Functions
 ****************************************************************************/

/**
 * Clipping zum Wahren des Wertebereichs
 */
#define clip(v,min,max) ((v) < (min) ? (min) : (v) > (max) ? (max) : (v))

/**
 * Berechnet, ob eine Koordinate innerhalb der Grenzen eines Bildes liegt.
 */
#define isIn(x,y,bounds) (((x) >= 0) && ((y) >= 0) && ((x) < (bounds.w)) && ((y) < (bounds.h)))

/**
 * Gibt einen Zeiger auf den Wert zurück, der in d an Stelle x/y ist.
 */
#define getPixel(d,bpp,w,x,y,ch) ((d) + ((bpp) * ((w) * (y) + (x))) + (ch))

/**
 * Pixel an Koordinate x/y von d setzen (Kanal ch)
 */
#define setPixel(d,bpp,w,x,y,ch,v) ((*((d) + ((bpp) * ((w) * (y) + (x))) + (ch))) = (guchar)(v))

/**
 * Gibt den Wert des Pixels x/y des Kanals ch in buf zurück.
 *
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     Auszulesender Kanal
 *
 * @return           Wert des Pixels x/y des Kanals ch von buf
 */
guchar getPixelNormal(guchar * buf, guchar bpp, GIntRect bounds, gint x, gint y, guchar ch)
{
  return *getPixel(buf, bpp, bounds.w, x, y, ch);
}

/**
 * Gibt den Wert des Pixels x/y des Kanals ch in buf zurück.
 * Befindet sich x/y außerhalb des Bildes, so wird der durch CONSTBACKGROUND
 * gegebene konstante Hintergrundswert zurückgegeben, sonst der jeweilige Pixelwert.
 *
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] ch     Auszulesender Kanal
 *
 * @return           Wert des Pixels x/y des Kanals ch von buf    wenn x/y im gültigen
 *                                                                Bereich liegt
 *                   CONSTBACKGROUND                              sonst
 */
guchar getPixelConstBack(guchar * buf, guchar bpp, GIntRect bounds, gint x, gint y, guchar ch)
{
  return isIn(x, y, bounds)
       ? *getPixel(buf, bpp, bounds.w, x, y, ch)
       : I_OUT;
}

/**
 * Initialisiert Quell- und Ziel-Pixelregion.
 *
 * @param[in]     drawable  Quellbild
 * @param[in/out] srcPR     Pixelregion für das Quellbild
 * @param[in/out] dstPR     Pixelregion für das Zielbild
 * @param[in]     bounds    Gibt den Bereich des Bildes an, der bearbeitet werden soll
 */
void initPR(GimpDrawable * drawable, GimpPixelRgn * srcPR, GimpPixelRgn * dstPR, GIntRect bounds)
{
  gimp_pixel_rgn_init (srcPR,            /* Quell-Pixelregion */
                       drawable,         /* Zeichenbereich */
                       bounds.x,         /* x-Startpunkt */
                       bounds.y,         /* y-Startpunkt */
                       bounds.w,         /* Breite */
                       bounds.h,         /* Hoehe */
                       FALSE, FALSE);    /* Pixelregion wird nicht beschrieben */

  gimp_pixel_rgn_init (dstPR,            /* Ziel-Pixelregion */
                       drawable,         /* Zeichenbereich */
                       bounds.x,         /* x-Startpunkt */
                       bounds.y,         /* y-Startpunkt */
                       bounds.w,         /* Breite */
                       bounds.h,         /* Hoehe */
                       TRUE, TRUE);      /* Pixelregion wird beschrieben */
}

/*****************************************************************************
 * Vector Functions
 *****************************************************************************/

/**
 * Berechnet den durch azimuth und elevation gegeebenen Lichtvektor
 * 
 * @param[in] azimuth   Winkel zur x-y-Ebene
 * @param[in] elevation Winkel in der x-y-Ebene
 *
 * @return Durch azimuth und elevation gegebener Vektor
 */
Vector calcLight(gfloat azimuth, gfloat elevation)
{
  elevation = (90 - elevation) / 180.0 * M_PI;
  azimuth = azimuth / 180.0 * M_PI;

  light.x = sin(elevation) * cos(azimuth);
  light.y = sin(elevation) * sin(azimuth);
  light.z = cos(elevation);
  
  return light;
}

/**
 * Berechnet die Länge des Vektors v
 *
 * @param[in] v Vektor, von dem die Länge zu berechnen ist
 *
 * @return    Länge von v
 */
gdouble vectorLength(Vector v)
{
  return sqrt( pow(v.x, 2)
             + pow(v.y, 2)
             + pow(v.z, 2));
}

/**
 * Berechnet den dem Vektor v entsprechenden normierten Vektor
 *
 * @param[in] v Vektor, der normiert werden soll
 *
 * @return      Normierter Vektor v0
 */
Vector vectorNorm(Vector v)
{
  gfloat div;
  
  div = vectorLength(v);
  
  v.x /= div;
  v.y /= div;
  v.z /= div;
  
  return v;
}

/**
 * Berechnet das Skalarprodukt der Vektoren v1 und v2
 *
 * @param[in] v1 Erster Vektor
 * @param[in] v2 Zweiter Vektor
 *
 * @return Das Skalarprodukt der Vektoren v1 und v2
 */
gfloat vectorScalarProduct(Vector v1, Vector v2)
{
  return v1.x * v2.x
       + v1.y * v2.y
       + v1.z * v2.z;
}

/** 
 * Berechnet den Winkel zwischen den Vektoren v1 und v2 im Gradmaß.
 *
 * @param[in] v1 Erster Vektor
 * @param[in] v2 Zweiter Vektor
 *
 * @return Winkel zwischen den Vektoren v1 und v2 im Gradmaß
 */
gdouble vectorAngle(Vector v1, Vector v2)
{
  return 180 * acos(vectorScalarProduct(v1, v2) / (vectorLength(v1) * vectorLength(v2))) / M_PI;
}

/*****************************************************************************
 * Filters
 *****************************************************************************/

/**
 * Filterung eines Pixels derart, dass die Luminanz berechnet wird.
 *
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 *
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar * luminance(GetPixel gp, guchar * srcBuf, guchar * dstBuf, guchar bpp, gint x, gint y, GIntRect bounds)
{
  guchar v  = 0
       , ch = 0;
  
  /**
   * Zuerst den Wert bestimmen, denn srcBuf und dstBuf können identisch sein und
   * zum Auslesen und Schreiben werden Macros verwendet
   */
  v = (guchar) ( 0.2989 * *getPixel(srcBuf, bpp, bounds.w, x, y, 0)
               + 0.5870 * *getPixel(srcBuf, bpp, bounds.w, x, y, 1)
               + 0.1140 * *getPixel(srcBuf, bpp, bounds.w, x, y, 2));
  
  /* Alle Kanäle erhalten denselben Luminanzwert */
  for (ch = 0; ch < bpp; ++ch)
    setPixel(dstBuf, bpp, bounds.w, x, y, ch, v);
                 
  return dstBuf;
}

/**
 * Führt eine Filterung mit dem Gauß-Filter durch.
 * Danach wird ADD zum erhaltenen Wert hinzuaddiert, um negative Ergebnisse
 * nicht komplett abzuschneiden. Zum Schluss wird Clipping durchgeführt.
 *        | 1  2  1 |
 * 1/16 * | 2  4  2 |
 *        | 1  2  1 |
 *
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 *
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar * gaussian(GetPixel gp, guchar * srcBuf, guchar * dstBuf, guchar bpp, gint x, gint y, GIntRect bounds)
{
  guchar v  = 0
       , ch = 0;
  
  for (ch = 0; ch < bpp; ++ch)
  {
    v = clip(
              (
                 gp(srcBuf, bpp, bounds, x - 1, y - 1, ch)
              + (gp(srcBuf, bpp, bounds, x    , y - 1, ch) << 1)
              +  gp(srcBuf, bpp, bounds, x + 1, y - 1, ch)
              + (gp(srcBuf, bpp, bounds, x - 1, y    , ch) << 1)
              + (gp(srcBuf, bpp, bounds, x    , y    , ch) << 2)
              + (gp(srcBuf, bpp, bounds, x + 1, y    , ch) << 1)
              +  gp(srcBuf, bpp, bounds, x - 1, y + 1, ch)
              + (gp(srcBuf, bpp, bounds, x    , y + 1, ch) << 1)
              +  gp(srcBuf, bpp, bounds, x + 1, y + 1, ch)
              ) >> 4
            , I_MIN
            , I_MAX);
      
    setPixel(dstBuf, bpp, bounds.w, x, y, ch, v);
  }
  
  return dstBuf;
}

/**
 * Filterung eines Pixels derart, dass der Emboss-Filter angewendet wird.
 *
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 *
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar * calcEmboss(GetPixel gp, guchar * srcBuf, guchar * dstBuf, guchar bpp, gint x, gint y, GIntRect bounds)
{
  gint dx = 0
     , dy = 0;
  
  guchar v  = 0
       , ch = 0;
  
  Vector n
       , n0;
  
  /* Diskrete Ableitung in x-Richtung */
  dx = ( gp(srcBuf, bpp, bounds, x + 1, y, 0)
       - gp(srcBuf, bpp, bounds, x - 1, y, 0)
       ) >> 1;
  
  /* Diskrete Ableitung in y-Richtung */     
  dy = ( gp(srcBuf, bpp, bounds, x, y + 1, 0)
       - gp(srcBuf, bpp, bounds, x, y - 1, 0)
       ) >> 1;
  
  /* Normale am Punkt (x/y) */
  n.x = -dx;
  n.y = -dy;
  n.z = 1;
  
  /* Normierter Normalenvektor am Punkt (x/y) */
  n0 = vectorNorm(n);
  
if (x == 5 && y == 5)
printf ("Light %.2f, %.2f, %.2f\n", light.x, light.y, light.z);

  v = fabs(n0.x) < DBL_EPSILON && fabs(n0.y) < DBL_EPSILON
    ? (I_MAX - I_MIN) * light.z + I_MIN
    : vectorAngle(n0, light) <= 90
    ? (I_MAX - I_MIN) * vectorScalarProduct(n0, light) + I_MIN
    : 0;
  
  for(ch = 0; ch < bpp; ++ch)
   setPixel(dstBuf, bpp, bounds.w, x, y, ch, v);
  
  return dstBuf;
}

/**
 * Filterung eines Pixels derart, dass die Intensitäten invertiert werden.
 *
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 *
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar * invert(GetPixel gp, guchar * srcBuf, guchar * dstBuf, guchar bpp, gint x, gint y, GIntRect bounds)
{
  guchar v  = 0
       , ch = 0;
  
  for (ch = 0; ch < bpp; ++ch)
  {
    v = I_MAX - *getPixel(srcBuf, bpp, bounds.w, x, y, ch) + I_MIN;
    
    setPixel(dstBuf, bpp, bounds.w, x, y, ch, v);
  }
  
  return dstBuf;
}

/**
 * Kopiert einen Speicherbereich von srcBuf nach dstBuf.
 *
 * @param[in] srcBuf Quellbereich
 * @param[in] dstBuf Zielbereich
 * @param[in] size   Gibt an, wieviele Byte zu kopieren sind
 *
 * @return    Zeiger auf den Zielbereich
 */
guchar * copyBuf(guchar * srcBuf, guchar * dstBuf, unsigned size)
{
  unsigned i = 0;
  
  guchar * res = dstBuf;
  
  while (i < size)
  {
    *dstBuf = *srcBuf;
    
    ++i;
    ++srcBuf;
    ++dstBuf;
  }
  
  return res;
}

/**
 * Filtert das mit drawable übergebene Bild mit dem Filter f.
 *
 * @param[in] drawable    Das zu filternde Bild
 * @param[in] f           Filterinfo mit folgenden Informationen:
 *            filter      Filter, der zum Filtern verwendet werden soll
 *            getPixel    Funktion für den Zugriff auf die Pixel
 *            filterSize  Gibt die Ausmaße des Filters an (-> quadratisch)
 *            filterName  Name des Filters
 *
 * @return    True        wenn kein Fehler aufgetreten ist
 *            False       sonst
 */
gboolean filter (GimpDrawable * drawable, FilterInfo f)
{
  gboolean error = FALSE;
  
  guint pixels    = 0  /* Anzahl der Pixel im Bild (*Kanalanzahl) */
      , access    = 0  /* Gibt an, wieviele Pixelzugriffe bisher gemacht wurden */
      , update    = 0;

  gint x          = 0    /* x-Koordinate */
     , y          = 0    /* y-Koordinate */
     , hasAlpha   = 0    /* Alphakanal vorhanden? */
     , bpp        = 0    /* Bytes pro Pixel */
     , border     = 0;   /* Rand, der aufgrund des Filterskernels nicht mit der 
                            "normalen" Pixelzugriffsfunktion gefiltert werden darf */
       
  GIntRect bounds;       /* Ausmaße der Auswahl */
  
  guchar * srcBuf        /* Buffer für Bildinformationen */
       , * dstBuf; 
       
  GimpPixelRgn srcPR     /* Quell- und */
             , dstPR;    /* Ziel-Pixelregionen */
  
  /* Das komplette Bild filtern */
  bounds.x = 0;
  bounds.y = 0;
  bounds.w = drawable->width;
  bounds.h = drawable->height;
  
  /* Bytes pro Pixel bestimmen */
  bpp = drawable->bpp;
  
  update = bounds.w * bounds.h;
  
  /* Wieviele Pixel*Kanäle hat das Bild? */
  pixels = update * bpp;
  
  /* Ist ein Alpha-Kanal vorhanden? */
  hasAlpha = gimp_drawable_has_alpha (drawable->drawable_id);
  
  border = (f.filterSize - 1) >> 1;
  
  /* Progressbar initialisieren */
  gimp_progress_init(f.filterName);
  
  /* Pixelregionen initialisieren */
  initPR(drawable, &srcPR, &dstPR, bounds);
  
  /* Speicher für die Buffer holen */
  srcBuf = g_new(guchar, pixels);
  dstBuf = g_new(guchar, pixels);    

  /* Ausgewählten Bildbereich in den Buffer kopieren */
  gimp_pixel_rgn_get_rect(&srcPR, srcBuf,
                          bounds.x, bounds.y,
                          bounds.w, bounds.h);

#ifdef DEBUG
  GTimer * timer = g_timer_new();
#endif

  /**
   * Unbedenklichen Bereich in der Mitte des Bildes filtern, hier kann die
   * normale Funktion zum Zugriff auf die Pixel benutzt werden, da kein Zugriff
   * außerhalb des gültigen Bereichs stattfinden wird.
   */
  for (y = border; y < (bounds.h - border); ++y)
  {
    for (x = border; x < (bounds.w - border); ++x)
    {
      dstBuf = f.filter(getPixelNormal, srcBuf, dstBuf, bpp, x, y, bounds);
        
      ++access;
    }

    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / update);
  }
  
  /**
   * Streifen an der oberen und unteren Kante des Bildes filtern, für die
   * die mit g übergebene Funktion zum Zugriff auf die Pixel benutzt werden muss.
   */
  for (y = 0; y < border; ++y)
  {
    for (x = 0; x < bounds.w; ++x)
    {
      dstBuf = f.filter(f.getPixel, srcBuf, dstBuf, bpp, x, y, bounds);
      
      dstBuf = f.filter(f.getPixel, srcBuf, dstBuf, bpp, bounds.w - x - 1, bounds.h - y - 1, bounds);
      
      access += 2;
    }
    
    /* Aktualisieren der Progress-Bar */    
    gimp_progress_update((double)access / update);
  }
  
  /**
   * Streifen an der linken und rechten Kante des Bildes filtern, für die
   * die mit g übergebene Funktion zum Zugriff auf die Pixel benutzt werden muss.
   */
  for (y = border; y < (bounds.h - border); ++y)
  {
    for (x = 0; x < border; ++x)
    {
      dstBuf = f.filter(f.getPixel, srcBuf, dstBuf, bpp, x, y, bounds);
      
      dstBuf = f.filter(f.getPixel, srcBuf, dstBuf, bpp, bounds.w - x - 1, bounds.h - y - 1, bounds);
    
      access += 2;
    }

    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / update);
  }
  
  /* Aktualisieren der Progress-Bar, Fertig */
  gimp_progress_update((double)100);
  
  /* Bearbeitetes Bild zurückschreiben */
  gimp_pixel_rgn_set_rect(&dstPR, dstBuf,
                          bounds.x, bounds.y,
                          bounds.w, bounds.h);

#ifdef DEBUG
  gulong ms = 0;
  g_timer_stop (timer);
  g_debug ("Dauer %f Sekunden.\n", g_timer_elapsed (timer, &ms));
  g_timer_destroy (timer);
#endif
  
  /* Aufräumen */  
  g_free(srcBuf);
  g_free(dstBuf);
  
  gimp_drawable_flush(drawable);
  
  gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
  
  error = !gimp_drawable_update(drawable->drawable_id
                              , bounds.x, bounds.y
                              , bounds.w, bounds.h);
  if (error)
    g_debug("Error writing Image back!");
   
  return !error;
}

/**
 * Filtert ein Bild mit dem Pencil-Sketch Filter.
 *
 * @param[in] drawable Das zu Filternde Bild
 * @param[in] alpha    Gewichtung des weichgezeichneten invertierten Bildes bei
 *                     der Addition mit dem Original
 *
 * @return    True        wenn kein Fehler aufgetreten ist
 *            False       sonst
 */
gboolean pencilSketch(GimpDrawable * drawable, gfloat alpha)
{
  gboolean error = FALSE;
  
  guint pixels    = 0  /* Anzahl der Pixel im Bild (*Kanalanzahl) */
      , access    = 0  /* Gibt an, wieviele Pixelzugriffe bisher gemacht wurden */
      , update    = 0;

  gint x          = 0    /* x-Koordinate */
     , y          = 0    /* y-Koordinate */
     , hasAlpha   = 0    /* Alphakanal vorhanden? */
     , bpp        = 0    /* Bytes pro Pixel */
     , ch         = 0    /* Kanal */
     , v          = 0;
       
  GIntRect bounds;     /* Ausmaße der Auswahl */
  
  guchar * srcBuf      /* Buffer für Bildinformationen */
       , * gssBuf
       , * dstBuf; 
       
  GimpPixelRgn srcPR   /* Quell- und */
             , dstPR;  /* Ziel-Pixelregionen */
  
  /* Das komplette Bild filtern */
  bounds.x = 0;
  bounds.y = 0;
  bounds.w = drawable->width;
  bounds.h = drawable->height;
  
  /* Bytes pro Pixel bestimmen */
  bpp = drawable->bpp;
  
  /* Anzahl der Pixelzugriffe */
  update = 5 * bounds.w * bounds.h;
  
  /* Wieviele Pixel*Kanäle hat das Bild? */
  pixels = bounds.w * bounds.h * bpp;
  
  /* Ist ein Alpha-Kanal vorhanden? */
  hasAlpha = gimp_drawable_has_alpha (drawable->drawable_id);
  
  /* Progressbar initialisieren */
  gimp_progress_init("Pencil Sketch");
  
  /* Pixelregionen initialisieren */
  initPR(drawable, &srcPR, &dstPR, bounds);
  
  /* Speicher für die Buffer holen */
  srcBuf = g_new(guchar, pixels);
  gssBuf = g_new(guchar, pixels);
  dstBuf = g_new(guchar, pixels);

  /* Ausgewählten Bildbereich in den Buffer kopieren */
  gimp_pixel_rgn_get_rect(&srcPR, srcBuf,
                          bounds.x, bounds.y,
                          bounds.w, bounds.h);
  
  /* Kopie des Quellbildes erstellen */
  dstBuf = copyBuf(srcBuf, dstBuf, bounds.h * bounds.w * bpp);

#ifdef DEBUG
  GTimer * timer = g_timer_new();
#endif

  /* Quellbild invertieren */
  for (y = 0; y < bounds.h; ++y)
  {
    for (x = 0; x < bounds.w; ++x)
    {
      srcBuf = invert(getPixelNormal, srcBuf, srcBuf, bpp, x, y, bounds);
      
      ++access;
    }
    
    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / update);
  }
  
  
  /**
   * Invertiertes Bild weichzeichnen und gewichtet mit alpha mit dem Quellbild
   * addieren.
   */
  for (y = 0; y < bounds.h; ++y)
  {
    for (x = 0; x < bounds.w; ++x)
    {
      /* invertiertes Bild weichzeichnen */
      gssBuf = gaussian(getPixelConstBack, srcBuf, gssBuf, bpp, x, y, bounds);
      
      for (ch = 0; ch < bpp; ++ch)
      {
        v =         (*getPixel(dstBuf, bpp, bounds.w, x, y, ch))
          + alpha * (*getPixel(gssBuf, bpp, bounds.w, x, y, ch));
        
        setPixel(dstBuf, bpp, bounds.w, x, y, ch, clip(v, I_MIN, I_MAX));
      }
        
      access += 4;
    }

    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / update);
  }
  
  /* Aktualisieren der Progress-Bar, Fertig */
  gimp_progress_update((double)100);
  
  /* Bearbeitetes Bild zurückschreiben */
  gimp_pixel_rgn_set_rect(&dstPR, dstBuf,
                          bounds.x, bounds.y,
                          bounds.w, bounds.h);

#ifdef DEBUG
  gulong ms = 0;
  g_timer_stop (timer);
  g_debug ("Dauer %f Sekunden.\n", g_timer_elapsed (timer, &ms));
  g_timer_destroy (timer);
#endif
  
  /* Aufräumen */
  g_free(srcBuf);
  g_free(gssBuf);
  g_free(dstBuf);
  
  gimp_drawable_flush(drawable);
  
  gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
  
  error = !gimp_drawable_update(drawable->drawable_id
                              , bounds.x, bounds.y
                              , bounds.w, bounds.h);
  if (error)
    g_debug("Error writing Image back!");
   
  return !error;
}

/**
 * DIE Funktion zum Filtern des Drawables
 *
 * @param[in] drawable das Drawable, das aktiv war, als der Filter aufgerufen
 *                     wurde.
 * @param[in] azimuth Azimuth fuer Emboss.
 * @param[in] elevation Elevation fuer Emboss.
 * @param[in] alphaPencilSketch Blendfaktors fuer Pencil-Sketch.
 * @param[in] emboss 1, wenn Emboss, 0, wenn Pencil-Sketch.
 *
 * @return TRUE = alles ok, FALSE = ein Fehler ist aufgetreten
 */
gboolean
filterDrawable(GimpDrawable * drawable, gfloat azimuth, gfloat elevation,
    gfloat alphaPencilSketch, gint emboss)
{
  gint32 imageID = gimp_drawable_get_image(drawable->drawable_id);

  /* eventuelle Selektion aufheben */
  gimp_selection_none(imageID);
  
  gboolean error = FALSE;
  
  if (drawable->bpp > 2)
  {
    FilterInfo fLuminance;
    
    fLuminance.filter     = luminance;
    fLuminance.filterSize = LUMINANCESIZE;
    fLuminance.filterName = "Luminance";
    fLuminance.getPixel   = getPixelNormal;
    
    error = !filter(drawable, fLuminance);
  }

  if (!error)
  {
  /* fuer Filter: Emboss */
    if (emboss)
    {
      FilterInfo fEmboss;
      
      fEmboss.filter     = calcEmboss;
      fEmboss.filterSize = EMBOSSSIZE;
      fEmboss.filterName = "Emboss";
      fEmboss.getPixel   = getPixelConstBack;
    
      light = vectorNorm(calcLight(azimuth, elevation));
    
      error = !filter(drawable, fEmboss);
    }
    else /* fuer Pencil Sketch */
    {
      g_debug ("Pencil-Sketch: alpha %.2f\n", alphaPencilSketch);
    
      error = !pencilSketch(drawable, alphaPencilSketch);
    }
  }
  else
    g_debug("An Error Occured!");

  return !error;
}
