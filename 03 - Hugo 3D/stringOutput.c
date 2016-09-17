/**
 * @file
 * Einfache Funktion zum Zeichnen von Text fuer GLUT-Programme.
 */

/* ---- System Header einbinden ---- */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "stringOutput.h"

/**
 * Zeichnen einer Zeichfolge in den Vordergrund. Gezeichnet wird mit Hilfe von
 * <code>glutBitmapCharacter(...)</code>. Kann wie <code>printf genutzt werden.</code>
 * @param x x-Position des ersten Zeichens 0 bis 1 (In).
 * @param y y-Position des ersten Zeichens 0 bis 1 (In).
 * @param color Textfarbe (In).
 * @param format Formatstring fuer die weiteren Parameter (In).
 */
void
drawString (GLfloat x, GLfloat y, GLfloat * color, char *format, ...)
{

  GLint matrixMode;             /* Zwischenspeicher akt. Matrixmode */
  va_list args;                 /* variabler Teil der Argumente */
  char buffer[255];             /* der formatierte String */
  char *s;                      /* Zeiger/Laufvariable */
  va_start (args, format);
  vsprintf (buffer, format, args);
  va_end (args);

  /* aktuelle Zeichenfarbe (u.a. Werte) sichern */
  glPushAttrib (GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);

  /* aktuellen Matrixmode speichern */
  glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
  glMatrixMode (GL_PROJECTION);

  /* aktuelle Projektionsmatrix sichern */
  glPushMatrix ();

  /* neue orthogonale 2D-Projektionsmatrix erzeugen */
  glLoadIdentity ();
  gluOrtho2D (0.0, 1.0, 1.0, 0.0);

  glMatrixMode (GL_MODELVIEW);

  /* aktuelle ModelView-Matrix sichern */
  glPushMatrix ();

  /* neue ModelView-Matrix zuruecksetzen */
  glLoadIdentity ();

  /* Tiefentest ausschalten */
  glDisable (GL_DEPTH_TEST);

  /* Licht ausschalten */
  glDisable (GL_LIGHTING);

  /* Nebel ausschalten */
  glDisable (GL_FOG);

  /* Blending ausschalten */
  glDisable (GL_BLEND);

  /* Texturierung ausschalten */
  glDisable (GL_TEXTURE_1D);
  glDisable (GL_TEXTURE_2D);
  /* glDisable (GL_TEXTURE_3D); */

  /* neue Zeichenfarbe einstellen */
  glColor4fv (color);

  /* an uebergebenene Stelle springen */
  glRasterPos2f (x, y);

  /* Zeichenfolge zeichenweise zeichnen */
  for (s = buffer; *s; s++)

    {
      glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, *s);
    }

  /* alte ModelView-Matrix laden */
  glPopMatrix ();
  glMatrixMode (GL_PROJECTION);

  /* alte Projektionsmatrix laden */
  glPopMatrix ();

  /* alten Matrixmode laden */
  glMatrixMode (matrixMode);

  /* alte Zeichenfarbe und Co. laden */
  glPopAttrib ();
}

/**
 * Zeichnet ein Rechteck mit den Hilfetexten.
 */
extern void drawHelp(void)
{
  int matrixMode; /* Zwischenspeicher akt. Matrixmode */
  float textColor[3] = {0.0, 0.0, 0.0};
  
  /* aktuelle Zeichenfarbe (u.a. Werte) sichern */
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
  
  /* aktuellen Matrixmode speichern */
  glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
  glMatrixMode(GL_PROJECTION);
  
  /* aktuelle Projektionsmatrix sichern */
  glPushMatrix();
  
  /* neue orthogonale 2D-Projektionsmatrix erzeugen */
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 1.0, 0.0);
  
  glMatrixMode(GL_MODELVIEW);

  /* aktuelle ModelView-Matrix sichern */
  glPushMatrix();

  /* neue ModelView-Matrix zuruecksetzen */
  glLoadIdentity();

  /* Tiefentest ausschalten */
  glDisable(GL_DEPTH_TEST);

  /* Licht ausschalten */
  glDisable(GL_LIGHTING);
  
  glColor3f(0.6, 0.6, 0.6);
  
  /* Kästchen */
  glBegin(GL_QUADS);
    glVertex2f(0.75, 0.75);
    glVertex2f(0.75, 0.25);
    glVertex2f(0.25, 0.25);
    glVertex2f(0.25, 0.75);
  glEnd();
  
  /* alte ModelView-Matrix laden */
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);

  /* alte Projektionsmatrix laden */
  glPopMatrix();

  /* alten Matrixmode laden */
  glMatrixMode(matrixMode);

  /* alte Zeichenfarbe und Co. laden */
  glPopAttrib();
  
  /* Texte */
  drawString(0.3, 0.30, textColor, "Hilfe : ");
  drawString(0.3, 0.35, textColor, "Beenden : ");
  drawString(0.3, 0.40, textColor, "Steuerung : ");
  drawString(0.3, 0.45, textColor, "Kamerabewegung : ");
  drawString(0.3, 0.50, textColor, "Lichtquellen : ");
  drawString(0.3, 0.55, textColor, "Levelsprung : ");
  
  drawString(0.5, 0.30, textColor, "h | H");
  drawString(0.5, 0.35, textColor, "ESC | q | Q");
  drawString(0.5, 0.40, textColor, "<- | ->");
  drawString(0.5, 0.45, textColor, "w | a | s | d");
  drawString(0.5, 0.50, textColor, "F1 | F2");
  drawString(0.5, 0.55, textColor, "1 | 2 | 3");
}
