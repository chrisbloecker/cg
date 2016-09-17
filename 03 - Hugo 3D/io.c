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
#include "drawing.h"
#include "stringOutput.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define WINDOW_X (100)
#define WINDOW_Y (25)
 
/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60

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
  
  gluPerspective(90.0, 1.3, 0.1, 45.0);
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
  /** Keycode der ESC-Taste */
  #define ESC 27

  if (status == GLUT_DOWN)
  {
    if (!isSpecialKey)
    {
      switch (key)
      {
        /* Mehr Dreiecke */
        case '+':
          drawingSetSubdivides(+ 1);
          break;
        /* Weniger Dreiecke */
        case '-':
          drawingSetSubdivides(- 1);
          break;
        case 'w':
          logicSetCamMovement(CAM_DOWN, TRUE);
          break;
        case 'a':
          logicSetCamMovement(CAM_RIGHT, TRUE);
          break;
        case 's':
          logicSetCamMovement(CAM_UP, TRUE);
          break;
        case 'd':
          logicSetCamMovement(CAM_LEFT, TRUE);
          break;
        case '1':
          logicChangeLevel(1);
          break;
        case '2':
          logicChangeLevel(2);
          break;
        case '3':
          logicChangeLevel(3);
          break;
        /* Pause */
        case 'p':
        case 'P':
          logicSetPause();
          break;
        case 'r':
          sceneSetLightRotation();
          break;
        /* Hilfe */
        case 'h':
        case 'H':
          logicSetHelp();
          break;
        /* Lines */
        case 'l':
        case 'L':
          drawingSetMode(GL_LINE_LOOP);
          break;
        /* Fill */
        case 'f':
        case 'F':
          drawingSetMode(GL_TRIANGLES);
          break;
        case 'n':
        case 'N':
          drawingSetNormals();
          break;
        /* Programm beenden */
        case 'q':
        case 'Q':
        case ESC:
          exit (0);
          break;
      }
    }
    else /* isSpecialKey */
    {
      switch (key)
      {
        case GLUT_KEY_LEFT:
          logicAddMovement(DIR_LEFT);
          break;
        case GLUT_KEY_RIGHT:
          logicAddMovement(DIR_RIGHT);
          break;
        case GLUT_KEY_F1:
          sceneSetLight(0);
          break;
        case GLUT_KEY_F2:
          sceneSetLight(1);
          break;
        case GLUT_KEY_F10:
          sceneSetDisplayListUsage();
          break;
      }
    }
  }
  else /* GLUT_UP */
  {
    if(!isSpecialKey)
    {
      switch (key)
      {
        case 'w':
          logicSetCamMovement(CAM_DOWN, FALSE);
          break;
        case 'a':
          logicSetCamMovement(CAM_RIGHT, FALSE);
          break;
        case 's':
          logicSetCamMovement(CAM_UP, FALSE);
          break;
        case 'd':
          logicSetCamMovement(CAM_LEFT, FALSE);
          break;
      }
    }
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

  /* zeitgesteuerte Ereignisse */
  logicCalcScene(interval);

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
 * Berechnung der Frames pro Sekunde.
 * @return aktuelle Framerate.
 */
double frameRate(void)
{
  /* Zeitpunkt der letzten Neuberechnung */
  static int timebase = 0;
  /* Anzahl der Aufrufe seit letzter Neuberechnung */
  static int frameCount = 0;
  /* Zuletzt berechneter FPS-Wert */
  static float fps = 0.0;
  /* aktuelle "Zeit" */
  static int time = 0;

  /* Diesen Aufruf hinzuzaehlen */
  frameCount++;

  /* seit dem Start von GLUT vergangene Zeit ermitteln */
  time = glutGet(GLUT_ELAPSED_TIME);

  /* Eine Sekunde ist vergangen */
  if (time - timebase > 1000)
  {
    /* FPS-Wert neu berechnen */
    fps = frameCount * 1000.0 / (time - timebase);

    /* Zureuecksetzen */
    timebase = time;
    frameCount = 0;
  }

  /* Aktuellen FPS-Wert zurueckgeben */
  return fps;
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void cbDisplay(void)
{
  /* Frames pro Sekunde */
  static double fps = 0.0;
  
  static float textColor[3] = { 1.0f, 0.0f, 0.0f };
  
  /* Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode(GL_MODELVIEW);
  
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  /* Szene zeichnen */
  sceneDraw();
  
  /* Framerate ausgegeben */
  drawString(0.05, 0.05, textColor, "FPS: %.2f", fps);
  
  /* Objekt anzeigen */
  glutSwapBuffers();
  
  /* Framerate berechnen */
  fps = frameRate();
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(WINDOW_X, WINDOW_Y);

  /* Fenster erzeugen */
  windowID = glutCreateWindow(title);

  if (windowID)
  {
    if (logicInit() && sceneInit())
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
