/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ---- System Header einbinden ---- */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

/* ---- Eigene Header einbinden ---- */
#include "io.h"
#include "types.h"
#include "logic.h"
#include "scene.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
 
/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60

/* Veränderung der Ballgeschwindigkeit */
#define BALL_SPEED_CHANGE (0.1)

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */
 
/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhältnisses
 * des Anzeigefensters, sodass das Seitenverhältnisse der Szene unverändert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 *
 * @param[in] aspect Seitenverhaeltnis des Anzeigefensters.
 */
static void setProjection(GLdouble aspect)
{
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  /* Koordinatensystem bleibt quadratisch */
  if (aspect <= 1)
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
  else
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
}

/**
 * Verarbeitung eines Tasturereignisses.
 * Pfeiltasten steuern die Position sowie Auslenkung des Schlägers.
 * ESC-Taste und q, Q beenden das Programm.
 *
 * @param[in] key          Taste, die das Ereignis ausgeloest hat. (ASCII-Wert
 *                         oder WERT des GLUT_KEY_<SPECIAL>).
 * @param[in] status       Status der Taste, GL_TRUE=gedrueckt,
 *                         GL_FALSE=losgelassen.
 * @param[in] isSpecialKey ist die Taste eine Spezialtaste?
 * @param[in] x            x-Position des Mauszeigers zum Zeitpunkt der 
 *                         Ereignisauslösung.
 * @param[in] y            y-Position des Mauszeigers zum Zeitpunkt der 
 *                         Ereignisauslösung.
 */
static void handleKeyboardEvent(int key, int status, Boolean isSpecialKey, int x, int y)
{
  Boolean pressed = status == GLUT_DOWN;

  /** Keycode der ESC-Taste */
  #define ESC 27

  switch (key)
  {
    /* Bewegung/Rotation des Schlägers in entsprechende Richtung starten */
    case GLUT_KEY_LEFT:
      setMovement(dirLeft, pressed);
      break;
    case GLUT_KEY_RIGHT:
      setMovement(dirRight, pressed);
      break;
    case GLUT_KEY_UP:
      setMovement(dirUp, pressed);
      break;
    case GLUT_KEY_DOWN:
      setMovement(dirDown, pressed);
      break;
    /* Programm beenden */
    case 'q':
    case 'Q':
    case ESC:
      exit (0);
      break;
  }
}

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 *
 * @param[in] key betroffene Taste.
 * @param[in] x   x-Position der Maus zur Zeit des Tastendrucks.
 * @param[in] y   y-Position der Maus zur Zeit des Tastendrucks.
 */
static void cbKeyboard(unsigned char key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Tastenloslassen.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 *
 * @param[in] key betroffene Taste.
 * @param[in] x   x-Position der Maus zur Zeit des Loslassens.
 * @param[in] y   y-Position der Maus zur Zeit des Loslassens.
 */
static void cbKeyboardUp(unsigned char key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_UP, GL_FALSE, x, y);
}

/**
 * Callback fuer Druck auf Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 *
 * @param[in] key betroffene Taste.
 * @param[in] x   x-Position der Maus zur Zeit des Tastendrucks.
 * @param[in] y   y-Position der Maus zur Zeit des Tastendrucks.
 */
static void cbSpecial(int key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_DOWN, GL_TRUE, x, y);
}

/**
 * Callback fuer Loslassen von Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 *
 * @param[in] key betroffene Taste.
 * @param[in] x   x-Position der Maus zur Zeit des Loslassens.
 * @param[in] y   y-Position der Maus zur Zeit des Loslassens.
 */
static void cbSpecialUp(int key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_UP, GL_TRUE, x, y);
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und anschliessendes Neuzeichnen,
 * setzt sich selbst erneut als Timer-Callback.
 *
 * @param[in] lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *                         registriert wurde.
 */
static void cbTimer(int lastCallTime)
{
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

  /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden */
  double interval = (double) (thisCallTime - lastCallTime) / 1000.0f;

  /* neue Position berechnen (zeitgesteuert) */
  calcPositions(interval);
  calcMovement();

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc(1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay();
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 *
 * @param[in] w Fensterbreite.
 * @param[in] h Fensterhöhe.
 */
static void cbReshape(int w, int h)
{
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection((GLdouble) w / (GLdouble) h);
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void cbDisplay(void)
{
  /* Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode(GL_MODELVIEW);
  
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  /* Szene zeichnen */
  drawScene();

  /* Objekt anzeigen */
  glutSwapBuffers();
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void registerCallbacks(void)
{
  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc(cbKeyboard);

  /* Tasten-Loslass-Callback - wird ausgefuehrt, wenn eine Taste losgelassen
   * wird */
  glutKeyboardUpFunc(cbKeyboardUp);

  /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
   * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder
   * Einfuegen) gedrueckt wird */
  glutSpecialFunc(cbSpecial);

  /* Spezialtasten-Loslass-Callback - wird ausgefuehrt, wenn eine Spezialtaste
   * losgelassen wird */
  glutSpecialUpFunc(cbSpecialUp);

  /* Automat. Tastendruckwiederholung ignorieren */
  glutIgnoreKeyRepeat(1);

  /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc(1000 / TIMER_CALLS_PS,        /* msecs - bis Aufruf von func */
                cbTimer,                      /* func  - wird aufgerufen     */
                glutGet(GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
                                                 func aufgerufen wird        */

  /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
   * Erzeugen oder Groessenaenderungen des Fensters) */
  glutReshapeFunc(cbReshape);

  /* Display-Callback - wird an mehreren Stellen imlizit (z.B. im Anschluss an
   * Reshape-Callback) oder explizit (durch glutPostRedisplay) angestossen */
  glutDisplayFunc(cbDisplay);
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 *
 * @param[in] title  Beschriftung des Fensters
 * @param[in] width  Breite des Fensters
 * @param[in] height Höhe des Fensters
 *
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int initAndStartIO(char *title, int width, int height)
{
  int windowID = 0;

  /* Kommandozeile immitieren */
  int argc = 1;
  char *argv = "cmd";

  /* Glut initialisieren */
  glutInit(&argc, &argv);

  /* Initialisieren des Fensters */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(25, 25);

  /* Fenster erzeugen */
  windowID = glutCreateWindow(title);

  if (windowID)
  {
    if (initScene())
    {
      registerCallbacks();

      glutMainLoop();
    }
    else
    {
      glutDestroyWindow(windowID);
      windowID = 0;
    }
  }
 
  return windowID;
}
