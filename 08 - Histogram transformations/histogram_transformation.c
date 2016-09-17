/**
 * @file histogram_transformation.c
 * Filtermodul eines Plug-ins fuer GIMP 2.2 and hoeher
 *
 * Einfache Histogrammtransformationen
 */

/* Definitionen fuer Plug-in-Konstanten etc. */
#include <glib.h>
#include "plugin.h"

/****************************************************************************
 * Macros
 ****************************************************************************/

#define IMAX 255
#define IMIN 0

/* Pixel an Koordinate x/y von d auslesen (Kanal ch) */
#define getPixel(d,bpp,w,x,y,ch) ((d) + ((bpp) * ((w) * (y) + (x))) + (ch))

/* Pixel an Koordinae x/y von d setzen (Kanal ch) */
#define setPixel(d,bpp,w,x,y,ch,v) ((*((d) + ((bpp) * ((w) * (y) + (x))) + (ch))) = (guchar)(v))

/* Clipping für die lineare Anpassung zum Wahren des Wertebereichs */
#define clip(v,min,max) ((v) = (v) > (max) ? (max) : (v) < (min) ? (min) : (v))

/**
 * Konvertieren der Intensitätswerte für die exponentielle Anpassung auf den
 * Bereich [0, 1]
 */
#define convert(v) (((v) - (IMIN)) / ((IMAX) - (IMIN)))

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

/****************************************************************************
 * Auxiliary Functions
 ****************************************************************************/

/**
 * Bestimmt Minimal- sowie Maximalwert. Ist p größer bzw. kleiner als der
 * bisherige Maximal- bzw. Minimalwert, so werden diese aktualisiert.
 * @param[in]     p    Neuer Vergleichswert
 * @param[in/out] min  Bisheriges Minimum
 * @param[in/out] max  Bisheriges Maximum
 */
void minMax(guchar p, gint * min, gint * max)
{
  *min = p < *min
         ? p
         : *min;
    
  *max = p > *max
         ? p
         : *max;
}

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

/**
 * Führt eine lineare Anpassung am durch drawable gegebenen Bild durch.
 * @param[in/out] drawable  Zu bearbeitendes Bild
 * @param[in]     k         Steigungsfaktor für die lineare Anpassung
 * @param[in]     h         Helligkeitsanhebung
 * @return        True      Wenn kein fehler aufgetreten ist
 *                False     Sonst
 */
gboolean linAdjust(GimpDrawable * drawable, gfloat k, gfloat h)
{
  gboolean error = FALSE;
  
  guint pixels  = 0;  /* Anzahl der Pixel im Bild (*Kanalanzahl) */
  
  gint x        = 0   /* x-Koordinate */
     , y        = 0   /* y-Koordinate */
     , v        = 0
     , hasAlpha = 0
     , bpp      = 0
     , ch       = 0;  /* Kanal */
  
  GIntRect bounds;    /* Ausmaße der Auswahl */
  
  guchar * buf;       /* Buffer für Bildinformationen */
       
  GimpPixelRgn srcPR  /* Quell- und */
             , dstPR; /* Ziel-Pixelregionen */
  
  gimp_progress_init("Lineare Anpassung"); /* Progressbar initialisieren */
  
  /* Ausmaße bestimmen */
  error = !gimp_drawable_mask_bounds(drawable->drawable_id
                                   , &bounds.x, &bounds.y
                                   , &bounds.w, &bounds.h);
  
  if (!error)
  {
    /* Höhe und Breite bestimmen */
    bounds.w -= bounds.x;
    bounds.h -= bounds.y;
    
    /* Bytes pro Pixel bestimmen */
    bpp = drawable->bpp;
    
    /* Wieviele Informationen werden im Bild betrachtet werden? */
    pixels = bounds.w * bounds.h * drawable->bpp;
    
    /* Ist ein Alpha-Kanal vorhanden? */
    hasAlpha = gimp_drawable_has_alpha (drawable->drawable_id);
    
    g_debug("Selection Mask (Bounding-Box): (x=%i, y=%i, w=%i, h=%i)",
            bounds.x, bounds.y, bounds.w, bounds.h);
    
    /* Pixelregionen initialisieren */
    initPR(drawable, &srcPR, &dstPR, bounds);
    
    /* Speicher für den Buffer holen */
    buf = g_new(guchar, pixels);

    /* Ausgewählten Bildbereich in den Buffer kopieren */
    gimp_pixel_rgn_get_rect(&srcPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);

  #ifdef DEBUG
    GTimer * timer = g_timer_new();
  #endif

    /* Lineare Anpassung mit Clipping durchführen und Progressbar aktualisieren */
    for (y = 0; y < bounds.h; ++y)
    {
      for (x = 0; x < bounds.w; ++x)
      {
        for (ch = 0; ch < (bpp - (hasAlpha ? 1 : 0)) ; ++ch)
        {
          v = k * (*getPixel(buf, bpp, bounds.w, x, y, ch)) + h;
          
          setPixel(buf, bpp, bounds.w, x, y, ch, clip(v, IMIN, IMAX));
        }
      }
      
      if (y % 10)
        gimp_progress_update((double)y / (2 * bounds.h) + 0.5);
    }
    
    /* Bearbeitetes Bild zurückschreiben */
    gimp_pixel_rgn_set_rect(&dstPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);

    gimp_progress_update((double)100);

  #ifdef DEBUG
    gulong ms = 0;
    g_timer_stop (timer);
    g_debug ("Dauer %f Sekunden.\n", g_timer_elapsed (timer, &ms));
    g_timer_destroy (timer);
  #endif
    
    /* Aufräumen */  
    g_free(buf);
    
    gimp_drawable_flush(drawable);
    
    gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
    
    error = !gimp_drawable_update(drawable->drawable_id
                                , bounds.x, bounds.y
                                , bounds.w, bounds.h);
    if (error)
      g_debug("Error writing Image back!");
  }
  else
    g_debug("No Selection!");
   
  return !error;
}

/**
 * Führt eine exponentielle Anpassung am durch drawable gegebenen Bild durch.
 * @param[in/out] drawable  Zu bearbeitendes Bild
 * @param[in]     alpha     Parameter für die exponentielle Anpassung
 * @return        True      Wenn kein Fehler aufgetreten ist
 *                False     Sonst
 */
gboolean expAdjust(GimpDrawable * drawable, gfloat alpha)
{
  gboolean error = FALSE;
  
  guint pixels = 0;   /* Anzahl der zu bearbeitenden Pixel (*Kanalanzahl) */

  gint x        = 0   /* Laufvariablen */
     , y        = 0
     , ch       = 0   /* Kanalanzahl */
     , hasAlpha = 0
     , bpp      = 0;  /* Bytes pro Pixel */
  
  GIntRect bounds;    /* Ausgewählter Bereich für die Bearbeitung */
  
  guchar v = 0;
  
  guchar * buf;       /* Speicher für die zu bearbeitenden Pixel */
       
  GimpPixelRgn srcPR  /* Quell- und */
             , dstPR; /* Ziel-Pixelregionen */
  
  gimp_progress_init("Exponentielle Anpassung"); /* Progressbar initialisieren */
  
  /* Auswahl bestimmen */
  error = !gimp_drawable_mask_bounds(drawable->drawable_id
                                   , &bounds.x, &bounds.y
                                   , &bounds.w, &bounds.h);

  if (!error)
  {
    /* Höhe und Breite bestimmen */
    bounds.w -= bounds.x;
    bounds.h -= bounds.y;
    
    /* Bytes pro Pixel bestimmen */
    bpp = drawable->bpp;
    
    /* Anzahl der zu betrachtenden Pixel pro Bilddurchlauf */
    pixels    = bounds.w * bounds.h * bpp;
    
    /* Ist ein Alpha-Kanal vorhanden? */
    hasAlpha = gimp_drawable_has_alpha(drawable->drawable_id);
    
    g_debug("Selection Mask (Bounding-Box): (x=%i, y=%i, w=%i, h=%i)",
            bounds.x, bounds.y, bounds.w, bounds.h);
    
    /* Pixelregionen initialisieren */
    initPR(drawable, &srcPR, &dstPR, bounds);
    
    /* Speicher holen */
    buf = g_new(guchar, pixels);

  #ifdef DEBUG
    GTimer * timer = g_timer_new();
  #endif

    /* Ausgewählten Bildbereich in den Buffer kopieren */
    gimp_pixel_rgn_get_rect(&srcPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);
    
    /**
     * Exponentielle Anpassung durchführen. 
     * Hierfür zunächst Intensitäten auf [0, 1] abbilden, 
     * dann Anpassung durchführen und Rucktransformation auf [IMIN, IMAX].
     */
    for (y = 0; y < bounds.h; ++y)
    {
      for (x = 0; x < bounds.w; ++x)
      {
        for (ch = 0; ch < (bpp - (hasAlpha ? 1 : 0)); ++ch)
        {
          v = *getPixel(buf, bpp, bounds.w, x, y, ch);
          
          v = (guchar)((IMAX - IMIN) * pow(convert((gdouble)v), alpha) + IMIN + 0.5);
          
          setPixel(buf, bpp, bounds.w, x, y, ch, v);
        }
      }
      
      if (y % 10)
        gimp_progress_update((double)y / bounds.h);
    }
     
    /* Bearbeitetes Bild zurückschreiben */
    gimp_pixel_rgn_set_rect(&dstPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);
    
    gimp_progress_update((double)100);
      
  #ifdef DEBUG
    gulong ms = 0;
    g_timer_stop (timer);
    g_debug ("Dauer %f Sekunden.\n", g_timer_elapsed (timer, &ms));
    g_timer_destroy (timer);
  #endif  
    
    /* Aufräumen */
    g_free(buf);
    
    gimp_drawable_flush(drawable);
    
    gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
    
    error = !gimp_drawable_update(drawable->drawable_id
                                , bounds.x, bounds.y
                                , bounds.w, bounds.h);
    if (error)
      g_debug("Error writing Image back!");
  }
  else
    g_debug("No Selection!");
  
  return !error;
}

/**
 * Führt eine Äquilibrierung auf dem durch drawable gegebenen Bild durch.
 * @param[in/out] drawable  Das zu bearbeitende Bild
 * @return        True      Wenn kein Fehler aufgetreten ist
 *                False     Sonst
 */
gboolean aequi(GimpDrawable * drawable)
{
  gboolean error = FALSE;
  
  guint i       = 0
      , pixels  = 0 /* Anzal der zu betrachtenden Pixel (*Kanalanzahl) */
      , sumR    = 0
      , sumG    = 0
      , sumB    = 0;
      
  gint x        = 0
     , y        = 0
     , hasAlpha = 0
     , bpp      = 0;          /* Bytes pro Pixel */
  
  guint * histoR = NULL
      , * histoG = NULL
      , * histoB = NULL;      /* Histogramm */
  
  gdouble * sumHistoR = NULL
        , * sumHistoG = NULL
        , * sumHistoB = NULL; /* Relatives Summenhistogramm */
  
  GIntRect bounds;            /* Auswahlbereich */
  
  guchar * buf;               /* Zu bearbeitendes Bild */
       
  GimpPixelRgn srcPR          /* Quell- und */
             , dstPR;         /* Ziel-Pixelregionen */
  
  gimp_progress_init("Aequilibrierung"); /* Progressbar initialisieren */
  
  /* Auswahlbereich bestimmen */
  error = !gimp_drawable_mask_bounds(drawable->drawable_id
                                   , &bounds.x, &bounds.y
                                   , &bounds.w, &bounds.h);
  
  if (!error)
  {
    /* Höhe und Breite bestimmen */
    bounds.w -= bounds.x;
    bounds.h -= bounds.y;
    
    /* Bytes pro Pixel bestimmen */
    bpp = drawable->bpp;
    
    /* Anzahl der zu betrachtenden Informationen pro Bilddurchlauf */
    pixels    = bounds.w * bounds.h;
    
    /* Ist ein Alpha-Kanal vorhanden? */
    hasAlpha = gimp_drawable_has_alpha(drawable->drawable_id);
    
    g_debug("Selection Mask (Bounding-Box): (x=%i, y=%i, w=%i, h=%i)",
            bounds.x, bounds.y, bounds.w, bounds.h);
    
    /* Pixelregionen initialisieren */
    initPR(drawable, &srcPR, &dstPR, bounds);
    
    /* Speicher holen */
    buf = g_new(guchar, pixels * bpp);
    histoR = g_new(guint, IMAX - IMIN + 1);
    histoG = g_new(guint, IMAX - IMIN + 1);
    histoB = g_new(guint, IMAX - IMIN + 1);
    sumHistoR = g_new(gdouble, IMAX - IMIN + 1);
    sumHistoG = g_new(gdouble, IMAX - IMIN + 1);
    sumHistoB = g_new(gdouble, IMAX - IMIN + 1);

    /* Histogramm initialisieren */
    for (i = 0; i <= IMAX - IMIN; ++i)
    {
      histoR[i] = 0;
      histoG[i] = 0;
      histoB[i] = 0;
    }

    /* Ausgewählten Bildbereich in den Buffer kopieren */
    gimp_pixel_rgn_get_rect(&srcPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);
    
  #ifdef DEBUG
    GTimer * timer = g_timer_new();
  #endif
    
    /* Histogramm berechnen und Progressbar aktualisieren */
    for (y = 0; y < bounds.h; ++y)
    {
      for (x = 0; x < bounds.w; ++x)
      {
        ++histoR[*getPixel(buf, bpp, bounds.w, x, y, 0)];
        ++histoG[*getPixel(buf, bpp, bounds.w, x, y, 1)];
        ++histoB[*getPixel(buf, bpp, bounds.w, x, y, 2)];
      }
      
      if (y % 10)
        gimp_progress_update((double) y / (2 * bounds.h));
    }
    
    /* Relatives Summenhistogramm berechnen */
    sumR = 0;
    sumG = 0;
    sumB = 0;
    for (i = 0; i <= IMAX - IMIN; ++i)
    {
      sumR += histoR[i];
      sumG += histoG[i];
      sumB += histoB[i];
      
      sumHistoR[i] = (double) sumR / pixels;
      sumHistoG[i] = (double) sumG / pixels;
      sumHistoB[i] = (double) sumB / pixels;
    }
    
    /* Äquilibrierung durchführen und Progressbar aktualisieren. */
    for (y = 0; y < bounds.h; ++y)
    {
      for (x = 0; x < bounds.w; ++x)
      {
        setPixel(buf, bpp, bounds.w, x, y, 0,
                (gchar)((IMAX - IMIN) * sumHistoR[*getPixel(buf, bpp, bounds.w, x, y, 0)] + IMIN));
        setPixel(buf, bpp, bounds.w, x, y, 1,
                (gchar)((IMAX - IMIN) * sumHistoG[*getPixel(buf, bpp, bounds.w, x, y, 1)] + IMIN));
        setPixel(buf, bpp, bounds.w, x, y, 2,
                (gchar)((IMAX - IMIN) * sumHistoB[*getPixel(buf, bpp, bounds.w, x, y, 2)] + IMIN));
      }
      
      if (y % 10)
        gimp_progress_update((double)y / (2 * bounds.h) + 0.5);
    }  
    
    /* Bearbeitetes Bild zurückschreiben */
    gimp_pixel_rgn_set_rect(&dstPR, buf,
                            bounds.x, bounds.y,
                            bounds.w, bounds.h);
    
    gimp_progress_update((double)100);

  #ifdef DEBUG
    gulong ms = 0;
    g_timer_stop (timer);
    g_debug ("Dauer %f Sekunden.\n", g_timer_elapsed (timer, &ms));
    g_timer_destroy (timer);
  #endif
    
    /* Aufräumen */
    g_free(buf);
    g_free(histoR);
    g_free(histoG);
    g_free(histoB);
    g_free(sumHistoR);
    g_free(sumHistoG);
    g_free(sumHistoB);
    
    gimp_drawable_flush(drawable);
    
    gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
    
    error = !gimp_drawable_update(drawable->drawable_id
                                , bounds.x, bounds.y
                                , bounds.w, bounds.h);

    if (error)
      g_debug("Error writing Image back!");
    }
    else
      g_debug("No Selection!");
  
  return !error;
}

/**
 * DIE Funktion zum Filtern des Drawables
 *
 * @param[in] drawable das Drawable, das aktiv war, als der Filter aufgerufen
 *                     wurde.
 * @param[in] transformType Transformations-Typ.
 * @param[in] alpha Alpha-Wert der exponentiellen Anpassung.
 * @param[in] k Steigung der linearen Anpassung.
 * @param[in] h y-Achsenabschnitt der linearen Anpassung.
 *
 * @return TRUE = alles ok, FALSE = ein Fehler ist aufgetreten
 */
gboolean filterDrawable (GimpDrawable * drawable,
                         gint transformType, gfloat alpha, gfloat k, gfloat h)
{
  gboolean error = FALSE;

  /* allgemeine Informationen zum Bild */
  gint bpp = 1;        /* Bytes pro Pixel */
  gint hasAlpha = 0;   /* Alpha-Informationen dabei ? */

  /* Bildgroesse und Pixelformat anzeigen */
  bpp = drawable->bpp;
  hasAlpha = gimp_drawable_has_alpha (drawable->drawable_id);

  g_debug ("Width %i, Height %i, BPP %i, Alpha %i",
           drawable->width, drawable->height, bpp, hasAlpha);

  switch (transformType)
    {
    case 0:
      g_debug ("transformType: Lineare Anpassung\n");
      error = !linAdjust(drawable, k, h);
      break;
    case 1:
      g_debug ("transformType: Aequilibrierung\n");
      error = !aequi(drawable);
      break;
    case 2:
      g_debug ("transformType: Exponentielle Anpassung: alpha: %f\n", alpha);
      error = !expAdjust(drawable, alpha);
      break;
    default:
      g_debug ("transformType: Unbekannt\n");
      break;
    }

  return !error;
}
