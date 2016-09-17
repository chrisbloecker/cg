/****************************************************************************
 * edge_detection.c
 * Filtermodul eines Plug-ins fuer GIMP 2.2 and hoeher
 *
 * Kantenfilter
 ****************************************************************************/

/* Definitionen fuer Plug-in-Konstanten etc. */
#include "plugin.h"

#include <assert.h>
#include <stdio.h>

/****************************************************************************
 * Typen
 ****************************************************************************/

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
typedef guchar (*GetPixel)(guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch);


/**
 * Signatur eines Funktionszeigers für Filterfunktionen
 */
typedef guchar (*Filter)(GetPixel gp, guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch);

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

/****************************************************************************
 * Constants
 ***************************************************************************/

#define CONSTBACKGROUND (0)

#define IMIN (0)
#define IMAX (255)
#define ADD  (128)

#define SOBELXSIZE        (3)
#define SOBELYSIZE        (3)
#define SOBELCOMBINEDSIZE (3)
#define MEXICANHATSIZE    (5)

/****************************************************************************
 * Functions
 ****************************************************************************
 * Auxiliary
 ***************************************************************************/

/**
 * Clipping zum Wahren des Wertebereichs
 */
#define clip(v,min,max) ((v) < (min) ? (min) : (v) > (max) ? (max) : (v))

/**
 * Berechnet, ob eine Koordinate innerhalb der Grenzen eines Bildes liegt.
 */
#define isIn(x,y,bounds) (((x) >= 0) && ((y) >= 0) && ((x) < (bounds.w)) && ((y) < (bounds.h)))

/**
 * Initialisiert Quell- und Ziel-Pixelregion.
 * @param[in]     drawable  Quellbild
 * @param[in/out] srcPR     Pixelregion für das Quellbild
 * @param[in/out] dstPR     Pixelregion für das Zielbild
 * @param[in]     bounds    Gibt den Bereich des Bildes an, der bearbeitet werden soll
 */
void initPR(GimpDrawable * drawable, GimpPixelRgn * srcPR, GimpPixelRgn * dstPR
          , GIntRect bounds)
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

/****************************************************************************
 * Pixel Access
 ***************************************************************************/

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
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     Auszulesender Kanal
 * @return           Wert des Pixels x/y des Kanals ch von buf
 */
guchar getPixelNormal(guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return *getPixel(buf, bpp, bounds.w, x, y, ch);
}

/**
 * Gibt den Wert des Pixels x/y des Kanals ch in buf zurück.
 * Befindet sich x/y außerhalb des Bildes, so wird der durch CONSTBACKGROUND
 * gegebene konstante Hintergrundswert zurückgegeben, sonst der jeweilige Pixelwert.
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     Auszulesender Kanal
 * @return           Wert des Pixels x/y des Kanals ch von buf    wenn x/y im gültigen
 *                                                                Bereich liegt
 *                   CONSTBACKGROUND                              sonst
 */
guchar getPixelConstBack(guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return isIn(x, y, bounds)
       ? *getPixel(buf, bpp, bounds.w, x, y, ch)
       : CONSTBACKGROUND;
}

/**
 * Gibt den Wert des Pixels x/y des Kanals ch in buf zurück.
 * Befindet sich x/y außerhalb des Bildes, so wird konstante Fortsetzung 
 * am Rand des Bildes angenommen.
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      Auszulesende x-Koordinate
 * @param[in] y      Auszulesende y-Koordinate
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     Auszulesender Kanal
 * @return           Wert des Pixels x/y des Kanals ch von buf    wenn x/y im gültigen
 *                                                                Bereich liegt
 *                   konstante Fortsetzung des Randes             sonst
 */
guchar getPixelConstCont(guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return *getPixel( buf
                  , bpp
                  , bounds.w
                  , clip(x, 0, bounds.w - 1) /* Clipping-Funktion verwenden zum */
                  , clip(y, 0, bounds.h - 1) /* Abbilden der Koordinaten in den */
                  , ch);                     /* gültigen Bereich des Bildes     */
}

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
guchar getPixelPeriodCont(guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  while (x < 0)
    x += bounds.w;
  
  while (y < 0)
    y += bounds.h;
  
  return *getPixel(buf, bpp, bounds.w, x % bounds.w, y % bounds.h, ch);
}

/****************************************************************************
 * Filters
 ***************************************************************************/

/**
 * Führt eine Filterung mit dem Sobel-Filter in x-Richtung durch.
 * Danach wird ADD zum erhaltenen Wert hinzuaddiert, um negative Ergebnisse
 * nicht komplett abzuschneiden. Zum Schluss wird Clipping durchgeführt.
 * | 1  0 -1 |
 * | 2  0 -2 |
 * | 1  0 -1 |
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     zu filternder Kanal
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar sobelX(GetPixel gp, guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return 
       clip
       (  ADD
       +  gp(buf, bpp, x - 1, y - 1, bounds, ch)
       -  gp(buf, bpp, x + 1, y - 1, bounds, ch)
       + (gp(buf, bpp, x - 1, y    , bounds, ch) << 1)
       - (gp(buf, bpp, x + 1, y    , bounds, ch) << 1)
       +  gp(buf, bpp, x - 1, y + 1, bounds, ch)
       -  gp(buf, bpp, x + 1, y + 1, bounds, ch)
       , IMIN
       , IMAX
       );
}

/**
 * Führt eine Filterung mit dem Sobel-Filter in y-Richtung durch.
 * Danach wird ADD zum erhaltenen Wert hinzuaddiert, um negative Ergebnisse
 * nicht komplett abzuschneiden. Zum Schluss wird Clipping durchgeführt.
 * | 1  2  1 |
 * | 0  0  0 |
 * |-1 -2 -1 |
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     zu filternder Kanal
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar sobelY(GetPixel gp, guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return
       clip
       (  ADD
       +  gp(buf, bpp, x - 1, y - 1, bounds, ch)
       + (gp(buf, bpp, x    , y - 1, bounds, ch) << 1)
       +  gp(buf, bpp, x + 1, y - 1, bounds, ch)
       -  gp(buf, bpp, x - 1, y + 1, bounds, ch)
       - (gp(buf, bpp, x    , y + 1, bounds, ch) << 1)
       -  gp(buf, bpp, x + 1, y + 1, bounds, ch)
       , IMIN
       , IMAX
       );
}

/**
 * Führt eine Filterung mit dem kombinierten Sobel-Filter in durch.
 * Danach wird ADD zum erhaltenen Wert hinzuaddiert, um negative Ergebnisse
 * nicht komplett abzuschneiden. Zum Schluss wird Clipping durchgeführt.
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     zu filternder Kanal
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar sobelCombined(GetPixel gp, guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  /**
   * Wie kann hier ein negativer Wert auftreten, für den man 128 addieren muss?
   */
  return /* (Dx[A]² + Dy[A])^½ */
       clip
       (  ADD
       + sqrt( pow(  gp(buf, bpp, x - 1, y - 1, bounds, ch)
                  -  gp(buf, bpp, x + 1, y - 1, bounds, ch)
                  + (gp(buf, bpp, x - 1, y    , bounds, ch) << 1)
                  - (gp(buf, bpp, x + 1, y    , bounds, ch) << 1)
                  +  gp(buf, bpp, x - 1, y + 1, bounds, ch)
                  -  gp(buf, bpp, x + 1, y + 1, bounds, ch)
                  , 2
                  )
             + pow(  gp(buf, bpp, x - 1, y - 1, bounds, ch)
                  + (gp(buf, bpp, x    , y - 1, bounds, ch) << 1)
                  +  gp(buf, bpp, x + 1, y - 1, bounds, ch)
                  -  gp(buf, bpp, x - 1, y + 1, bounds, ch)
                  - (gp(buf, bpp, x    , y + 1, bounds, ch) << 1)
                  -  gp(buf, bpp, x + 1, y + 1, bounds, ch)
                  , 2
                  )
             )
       , IMIN
       , IMAX
       );
}

/**
 * Führt eine Filterung mit dem Mexican-Hat-Filter durch.
 * Danach wird ADD zum erhaltenen Wert hinzuaddiert, um negative Ergebnisse
 * nicht komplett abzuschneiden. Zum Schluss wird Clipping durchgeführt.
 *        | 0  1  2  1  0 |
 *        | 1  0 -2  0  1 |
 * 1/16 * | 2 -2 -8 -2  2 |
 *        | 1  0 -2  0  1 |
 *        | 0  1  2  1  0 |
 * @param[in] gp     Funktion zum Auslesen der Pixelwerte aus buf
 * @param[in] buf    Speicherbereich, der die Pixelwerte eines Bildes enthält
 * @param[in] bpp    Gibt an, wieviele Bytes pro Pixel verwendet werden
 * @param[in] x      zu filternde x-Koordinate im Quellbild
 * @param[in] y      zu filternde y-Koordinate im Quellbild
 * @param[in] bounds Ausmaße des Bildes
 * @param[in] ch     zu filternder Kanal
 * @return           Gefilterter Wert für Position x/y im Zielbild
 */
guchar mexicanHat(GetPixel gp, guchar * buf, guchar bpp, gint x, gint y, GIntRect bounds, guchar ch)
{
  return
       clip
       (
         ADD
       +
        ((
         +  gp(buf, bpp, x - 1, y - 2, bounds, ch)
         + (gp(buf, bpp, x    , y - 2, bounds, ch) << 1)
         +  gp(buf, bpp, x + 1, y - 2, bounds, ch)
         +  gp(buf, bpp, x - 2, y - 1, bounds, ch)
         - (gp(buf, bpp, x    , y - 1, bounds, ch) << 1)
         +  gp(buf, bpp, x + 2, y - 1, bounds, ch)
         + (gp(buf, bpp, x - 2, y    , bounds, ch) << 1)
         - (gp(buf, bpp, x - 1, y    , bounds, ch) << 1)
         - (gp(buf, bpp, x    , y    , bounds, ch) << 3)
         - (gp(buf, bpp, x + 1, y    , bounds, ch) << 1)
         + (gp(buf, bpp, x + 2, y    , bounds, ch) << 1)
         +  gp(buf, bpp, x - 2, y + 1, bounds, ch)
         - (gp(buf, bpp, x    , y + 1, bounds, ch) << 1)
         +  gp(buf, bpp, x + 2, y + 1, bounds, ch)
         +  gp(buf, bpp, x - 1, y + 2, bounds, ch)
         + (gp(buf, bpp, x    , y + 2, bounds, ch) << 1)
         +  gp(buf, bpp, x + 1, y + 2, bounds, ch)
         ) >> 4)
       , IMIN
       , IMAX
       );
}

/**
 * Filtert das mit drawable übergebene Bild mit dem Filter f.
 * @param[in] drawable    Das zu filternde Bild
 * @param[in] f           Filterinfo mit folgenden Informationen:
 *            filter      Filter, der zum Filtern verwendet werden soll
 *            getPixel    Funktion für den Zugriff auf die Pixel
 *            filterSize  Gibt die Ausmaße des Filters an (-> quadratisch)
 *            filterName  Name des Filters
 * @return    True        wenn kein Fehler aufgetreten ist
 *            False       sonst
 */
gboolean filter (GimpDrawable * drawable, FilterInfo f)
{
  gboolean error = FALSE;
  
  guint pixels    = 0  /* Anzahl der Pixel im Bild (*Kanalanzahl) */
      , access    = 0; /* Gibt an, wieviele Pixelzugriffe bisher gemacht wurden */

  
  gint x        = 0    /* x-Koordinate */
     , y        = 0    /* y-Koordinate */
     , hasAlpha = 0    /* Alphakanal vorhanden? */
     , bpp      = 0    /* Bytes pro Pixel */
     , ch       = 0    /* Kanal */
     , border   = 0;   /* Rand, der aufgrund des Filterskernels nicht mit der 
                          "normalen" Pixelzugriffsfunktion gefiltert werden darf */
       
  GIntRect bounds;     /* Ausmaße der Auswahl */
  
  guchar * srcBuf      /* Buffer für Bildinformationen */
       , * dstBuf; 
       
  GimpPixelRgn srcPR   /* Quell- und */
             , dstPR;  /* Ziel-Pixelregionen */
  
  gimp_progress_init(f.filterName); /* Progressbar initialisieren */
  
  /**
   * Wenn keine Auswahl getroffen wurde, dann komplettes Bild filtern,
   * außerdem die Ausmaße der zu filternden Region bestimmen */
  if (!gimp_drawable_mask_bounds(drawable->drawable_id
                                , &bounds.x, &bounds.y
                                , &bounds.w, &bounds.h))
  {
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = drawable->width;
    bounds.h = drawable->height;
  }
  else
  {
    /* Höhe und Breite bestimmen */
    bounds.w -= bounds.x;
    bounds.h -= bounds.y;
  }
  
  /* Bytes pro Pixel bestimmen */
  bpp = drawable->bpp;
  
  /* Wieviele Informationen werden im Bild betrachtet werden? */
  pixels = bounds.w * bounds.h * drawable->bpp;
  
  /* Ist ein Alpha-Kanal vorhanden? */
  hasAlpha = gimp_drawable_has_alpha (drawable->drawable_id);
  
  border = (f.filterSize - 1) >> 1;
  
  g_debug("Selection Mask (Bounding-Box): (x=%i, y=%i, w=%i, h=%i)",
          bounds.x, bounds.y, bounds.w, bounds.h);
  
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
      for (ch = 0; ch < (bpp - (hasAlpha ? 1 : 0)) ; ++ch)
      {
        setPixel( dstBuf
                , bpp
                , bounds.w
                , x
                , y
                , ch
                , f.filter( getPixelNormal
                          , srcBuf
                          , bpp
                          , x
                          , y
                          , bounds
                          , ch));
        
        ++access;
      }
    }

    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / pixels);
  }
  
  /**
   * Streifen an der oberen und unteren Kante des Bildes filtern, für die
   * die mit g übergebene Funktion zum Zugriff auf die Pixel benutzt werden muss.
   */
  for (y = 0; y < border; ++y)
  {
    for (x = 0; x < bounds.w; ++x)
    {
      for (ch = 0; ch < (bpp - (hasAlpha ? 1 : 0)) ; ++ch)
      {
        setPixel( dstBuf
                , bpp
                , bounds.w
                , x
                , y
                , ch
                , f.filter( f.getPixel
                          , srcBuf
                          , bpp
                          , x
                          , y
                          , bounds
                          , ch));
        
        setPixel( dstBuf
                , bpp
                , bounds.w
                , bounds.w - x - 1
                , bounds.h - y - 1
                , ch
                , f.filter( f.getPixel
                          , srcBuf
                          , bpp
                          , bounds.w - x - 1
                          , bounds.h - y - 1
                          , bounds
                          , ch));
      
        access += 2;
      }
    }
    
    /* Aktualisieren der Progress-Bar */    
    gimp_progress_update((double)access / pixels);
  }
  
  /**
   * Streifen an der linken und rechten Kante des Bildes filtern, für die
   * die mit g übergebene Funktion zum Zugriff auf die Pixel benutzt werden muss.
   */
  for (y = border; y < (bounds.h - border); ++y)
  {
    for (x = 0; x < border; ++x)
    {
      for (ch = 0; ch < (bpp - (hasAlpha ? 1 : 0)) ; ++ch)
      {
        setPixel( dstBuf
                , bpp
                , bounds.w
                , x
                , y
                , ch
                , f.filter( f.getPixel
                          , srcBuf
                          , bpp
                          , x
                          , y
                          , bounds
                          , ch));
        
        setPixel( dstBuf
                , bpp
                , bounds.w
                , bounds.w - x - 1
                , bounds.h - y - 1
                , ch
                , f.filter( f.getPixel
                          , srcBuf
                          , bpp
                          , bounds.w - x - 1
                          , bounds.h - y - 1
                          , bounds
                          , ch));
      
        access += 2;
      }
    }

    /* Aktualisieren der Progress-Bar */
    gimp_progress_update((double)access / pixels);
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
 * DIE Funktion zum Filtern des Drawables
 *
 * @param[in] drawable das Drawable, das aktiv war, als der Filter aufgerufen
 *                     wurde.
 * @param[in] filterType Kantenfilter-Typ.
 * @param[in] borderMode Modus der Randbehandlung.
 *
 * @return TRUE = alles ok, FALSE = ein Fehler ist aufgetreten
 */
gboolean
filterDrawable (GimpDrawable * drawable, gint filterType, gint borderMode)
{
  FilterInfo f;
  
  switch (filterType)
    {
    case 0:
      f.filter     = sobelX;
      f.filterSize = SOBELXSIZE;
      f.filterName = "Sobel-Filter in X-Richtung";
      g_debug ("FilterType: SobelX\n");
      break;

    case 1:
      f.filter     = sobelY;
      f.filterSize = SOBELYSIZE;
      f.filterName = "Sobel-Filter in Y-Richtung";
      g_debug ("FilterType: SobelY\n");
      break;

    case 2:
      f.filter     = sobelCombined;
      f.filterSize = SOBELCOMBINEDSIZE;
      f.filterName = "Kombinierter Sobel-Filter";
      g_debug ("FilterType: Sobel Kombiniert\n");
      break;

    case 3:
      f.filter     = mexicanHat;
      f.filterSize = MEXICANHATSIZE;
      f.filterName = "Mexican-Hat-Filter";
      g_debug ("FilterType: Mexican-Hat\n");
      break;

    default:
      g_debug ("FilterType: Unbekannt\n");
      break;
    }

  switch (borderMode)
    {
    case 0:
      f.getPixel = getPixelConstBack;
      g_debug ("EdgeMode: Konstante Hintergrundfarbe\n");
      break;
    case 1:
      f.getPixel = getPixelConstCont;
      g_debug ("EdgeMode: Konstante Fortsetzung\n");
      break;
    case 2:
      f.getPixel = getPixelPeriodCont;
      g_debug ("EdgeMode: Periodische Fortsetzung\n");
      break;
    default:
      g_debug ("EdgeMode: Unbekannt\n");
      break;
    }

  return filter(drawable, f);
}
