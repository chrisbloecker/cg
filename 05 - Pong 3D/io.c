/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <limits.h>
#include <GL/glut.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "io.h"
#include "scene.h"
#include "logic.h"
#include "stringOutput.h"
#include "drawing.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
#define WINDOW_X (200)
#define WINDOW_Y (25)

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

static Boolean vertical = TRUE;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
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
static void setProjection(double aspect)
{
  /* perspektivische Projektion */
  gluPerspective( 70.0   /* Oeffnungswinkel      */
                , aspect /* Seitenverhaeltnis    */
                , 0.1    /* nahe Clipping-Ebene  */
                , 30.0   /* ferne Clipping-Ebene */
                );
}

static void setOrtho(double aspect)
{
  /* Koordinatensystem bleibt quadratisch */
  if (aspect <= 1)
    gluOrtho2D(-1.1, 1.1, -1.1 / aspect, 1.1 / aspect);
  else
    gluOrtho2D(-1.1 * aspect, 1.1 * aspect, -1.1, 1.1);
}

/**
 * Setzen der Kameraposition.
 */
static void setCamera(void)
{
  /* Kameraposition */
  Vector3d eye    = logicGetVector(LOGIC_VECTOR_EYE)
         , center = logicGetVector(LOGIC_VECTOR_CENTER)
         ;
  
  /* Kameraposition */
  gluLookAt( eye.x,    eye.y,    eye.z     /* Augpunkt  */
           , center.x, center.y, center.z  /* Zentrum   */
           , 0.0, 1.0, 0.0                 /* Up-Vektor */
           );
}

/**
 * Verarbeitung eines Tasturereignisses.
 *
 * @param key Taste, die das Ereignis ausgeloest hat. (ASCII-Wert oder WERT des
 *        GLUT_KEY_<SPECIAL>.
 * @param status Status der Taste, GL_TRUE=gedrueckt, GL_FALSE=losgelassen.
 * @param isSpecialKey ist die Taste eine Spezialtaste?
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
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
        case 'W':
          logicSwitch(LOGIC_SWITCHABLE_CAMDOWN);
          break;
        case 'a':
        case 'A':
          logicSwitch(LOGIC_SWITCHABLE_CAMRIGHT);
          break;
        case 's':
        case 'S':
          logicSwitch(LOGIC_SWITCHABLE_CAMUP);
          break;
        case 'd':
        case 'D':
          logicSwitch(LOGIC_SWITCHABLE_CAMLEFT);
          break;
        case 'c':
        case 'C':
          logicSwitch(LOGIC_SWITCHABLE_CHEAT);
          break;
        case 'r':
        case 'R':
          logicSwitch(LOGIC_SWITCHABLE_CUBEROTATION);
          break;
        case 't':
        case 'T':
          textureSwitch();
          break;
        /* Programm beenden */
        case 'q':
        case 'Q':
        case ESC:
          sceneCleanup();
          exit (0);
          break;
        case 'l':
        case 'L':
          sceneSwitchWireframeMode();
          break;
        case 'm':
        case 'M':
          logicSwitch(LOGIC_SWITCHABLE_SHADOWS);
          break;
        case 'n':
        case 'N':
          logicSwitch(LOGIC_SWITCHABLE_SHADOWVOLUMES);
          break;
        case 'u':
        case 'U':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTBACKWARD);
          break;
        case 'j':
        case 'J':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTFORWARD);
          break;
        case 'h':
        case 'H':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTLEFT);
          break;
        case 'k':
        case 'K':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTRIGHT);
          break;
        case 'z':
        case 'Z':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTDOWN);
          break;
        case 'i':
        case 'I':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTUP);
          break;
        case 'p':
        case 'P':
          logicSwitch(LOGIC_SWITCHABLE_PAUSE);
          break;
        case 'v':
        case 'V':
          vertical = !vertical;
          break;
      }
    }
    else /* isSpecialKey */
    {
      switch (key)
      {
        case GLUT_KEY_LEFT:
          logicSwitch(LOGIC_SWITCHABLE_RACKETLEFT);
          break;
        case GLUT_KEY_RIGHT:
          logicSwitch(LOGIC_SWITCHABLE_RACKETRIGHT);
          break;
        case GLUT_KEY_UP:
          logicSwitch(LOGIC_SWITCHABLE_RACKETUP);
          break;
        case GLUT_KEY_DOWN:
          logicSwitch(LOGIC_SWITCHABLE_RACKETDOWN);
          break;
        case GLUT_KEY_F1:
          logicSwitch(LOGIC_SWITCHABLE_LIGHT0);
          break;
        case GLUT_KEY_F2:
          logicSwitch(LOGIC_SWITCHABLE_LIGHT1);
          break;
        case GLUT_KEY_PAGE_UP:
          logicSwitch(LOGIC_SWITCHABLE_CAMFORWARD);
          break;
        case GLUT_KEY_PAGE_DOWN:
          logicSwitch(LOGIC_SWITCHABLE_CAMBACKWARD);
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
        case 'W':
          logicSwitch(LOGIC_SWITCHABLE_CAMDOWN);
          break;
        case 'a':
        case 'A':
          logicSwitch(LOGIC_SWITCHABLE_CAMRIGHT);
          break;
        case 's':
        case 'S':
          logicSwitch(LOGIC_SWITCHABLE_CAMUP);
          break;
        case 'd':
        case 'D':
          logicSwitch(LOGIC_SWITCHABLE_CAMLEFT);
          break;
        case 'u':
        case 'U':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTBACKWARD);
          break;
        case 'j':
        case 'J':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTFORWARD);
          break;
        case 'h':
        case 'H':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTLEFT);
          break;
        case 'k':
        case 'K':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTRIGHT);
          break;
        case 'z':
        case 'Z':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTDOWN);
          break;
        case 'i':
        case 'I':
          logicSwitch(LOGIC_SWITCHABLE_LIGHTUP);
          break;
      }
    }
    else /* isSpecialKey */
    {
      switch (key)
      {
        case GLUT_KEY_LEFT:
          logicSwitch(LOGIC_SWITCHABLE_RACKETLEFT);
          break;
        case GLUT_KEY_RIGHT:
          logicSwitch(LOGIC_SWITCHABLE_RACKETRIGHT);
          break;
        case GLUT_KEY_UP:
          logicSwitch(LOGIC_SWITCHABLE_RACKETUP);
          break;
        case GLUT_KEY_DOWN:
          logicSwitch(LOGIC_SWITCHABLE_RACKETDOWN);
          break;
        case GLUT_KEY_PAGE_UP:
          logicSwitch(LOGIC_SWITCHABLE_CAMFORWARD);
          break;
        case GLUT_KEY_PAGE_DOWN:
          logicSwitch(LOGIC_SWITCHABLE_CAMBACKWARD);
          break;
      }
    }
  }
}

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 *
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void cbKeyboard(unsigned char key, int x, int y)
{
  handleKeyboardEvent(key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 *
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
static void cbReshape(int w, int h)
{
  /* Erster Viewport, 3D */
  vertical 
    ? glViewport(0, 0, w / 2, h)
    : glViewport(0, h / 2, w, h / 2)
    ;
  
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  vertical
    ? setProjection((GLdouble) w / (GLdouble) h / 2)
    : setProjection((GLdouble) w / (GLdouble) h * 2)
    ;
  
  /* Zweiter Viewport, 2D */
  vertical
    ? glViewport(w / 2, 0, w / 2, h)
    : glViewport(0, 0, w, h / 2)
    ;
    
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode(GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();
  
  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  vertical
    ? setOrtho((GLdouble) w / (GLdouble) h / 2)
    : setOrtho((GLdouble) w / (GLdouble) h * 2)
    ;
}

/**
 * Berechnung der Frames pro Sekunde.
 *
 * @return aktuelle Framerate.
 */
static double frameRate(void)
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
  int w  = glutGet(GLUT_WINDOW_WIDTH)
    , h = glutGet(GLUT_WINDOW_HEIGHT)
    ;
  
  /* Frames pro Sekunde */
  static double fps = 0.0;
  
  /* Damit fps nicht unbenutzt aussieht */
  fps *= 1;
  
  /*static float textColor[3] = { 1.0f, 0.0f, 0.0f };*/
  /* Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  /* Erster Viewport, 3D */
  {
    vertical 
      ? glViewport(0, 0, w / 2, h)
      : glViewport(0, h / 2, w, h / 2)
      ;
        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    vertical
      ? setProjection((GLdouble) w / (GLdouble) h / 2)
      : setProjection((GLdouble) w / (GLdouble) h * 2)
      ;
    
    /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
    glMatrixMode(GL_MODELVIEW);
  
    /* Matrix zuruecksetzen - Einheitsmatrix laden */
    glLoadIdentity();
  
    setCamera();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
    /* Szene zeichnen */
    sceneDraw(TRUE);
  
    /* Framerate ausgegeben 
    drawString(0.05, 0.05, textColor, "FPS: %.2f", fps);*/
  }
  
  /* Zweiter Viewport, 2D */
  {
    vertical
      ? glViewport(w / 2, 0, w / 2, h)
      : glViewport(0, 0, w, h / 2)
      ;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    vertical
      ? setOrtho((GLdouble) w / (GLdouble) h / 2)
      : setOrtho((GLdouble) w / (GLdouble) h * 2)
      ;
    
    /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
    glMatrixMode(GL_MODELVIEW);
  
    /* Matrix zuruecksetzen - Einheitsmatrix laden */
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    glPushMatrix();
      glRotatef( 90, 1, 0, 0);
      glRotatef(-90, 0, 1, 0);
      /* Szene zeichnen */
      sceneDraw(FALSE);
    glPopMatrix();
  
    /* Framerate ausgegeben 
    drawString(0.05, 0.05, textColor, "FPS: %.2f", fps);*/
  }
  
  /* Objekt anzeigen */
  glutSwapBuffers();
  
  /* Framerate berechnen */
  fps = frameRate();
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
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
extern int initAndStartIO(char *title, int width, int height)
{
  int windowID = 0;

  /* Kommandozeile immitieren */
  int argc = 1;
  char *argv = "cmd";

  /* Glut initialisieren */
  glutInit (&argc, &argv);

  /* Initialisieren des Fensters */
  /* RGB-Framewbuffer, Double-Buffering und z-Buffer anfordern */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);
  glutInitWindowPosition(WINDOW_X, WINDOW_Y);

  /* Fenster erzeugen */
  windowID = glutCreateWindow (title);

  if (windowID)
    {

      if (logicInit() && sceneInit ())
        {
          registerCallbacks ();

          glutMainLoop ();
        }
      else
        {

          glutDestroyWindow (windowID);
          windowID = 0;
        }
    }
  else
    {
    }

  return windowID;
}
