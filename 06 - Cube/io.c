/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/glut.h>
#include <stdlib.h>

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
#include "types.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Datentyp für Mausereignisse. */
typedef enum { 
  MOUSE_BUTTON
, MOUSE_MOTION
, MOUSE_PASSIVE
} MouseEventType;

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

static Boolean leftclick  = FALSE
             , rightclick = FALSE
             ;

static int X
         , Y
         ;

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
  LogicSwitchable s = LOGIC_SWITCHABLE_DUMMY;
  
  LogicToggleable t = LOGIC_TOGGLEABLE_DUMMY;
  
  /** Keycode der ESC-Taste */
  #define ESC (27)
  
  if (status == GLUT_DOWN)
  {
    if (!isSpecialKey)
    {
      switch (key)
      {
        /* Mehr Dreiecke */
        case '+':
          logicSetSubdivides(+ 1);
          break;
        /* Weniger Dreiecke */
        case '-':
          logicSetSubdivides(- 1);
          break;
        /* Programm beenden */
        case 'q':
        case 'Q':
        case ESC:
          exit(0);
          break;
        case 'l':
        case 'L':
          sceneSwitchWireframeMode();
          break;
        case 'a':
        case 'A':
          logicSwitchNode(- 1);
          break;
        case 'd':
        case 'D':
          logicSwitchNode(+ 1);
          break;
        case 's':
        case 'S':
          t = LOGIC_TOGGLEABLE_SPHERES;
          break;
        case 'c':
        case 'C':
          logicSwitchColor();
          break;
        case 'h':
        case 'H':
          t = LOGIC_TOGGLEABLE_HELP;
          break;
      }
    }
    else /* isSpecialKey */
    {
      switch (key)
      {
        case GLUT_KEY_UP:
          s = LOGIC_SWITCHABLE_CAMDOWN;
          break;
        case GLUT_KEY_LEFT:
          s = LOGIC_SWITCHABLE_CAMRIGHT;
          break;
        case GLUT_KEY_DOWN:
          s = LOGIC_SWITCHABLE_CAMUP;
          break;
        case GLUT_KEY_RIGHT:
          s = LOGIC_SWITCHABLE_CAMLEFT;
          break;
        case GLUT_KEY_PAGE_UP:
          s = LOGIC_SWITCHABLE_CAMFORWARD;
          break;
        case GLUT_KEY_PAGE_DOWN:
          s = LOGIC_SWITCHABLE_CAMBACKWARD;
          break;
      }
    }
    
    if (s != LOGIC_SWITCHABLE_DUMMY)
      while (!logicGetSwitchable(s))
        logicSwitch(s);
    
    if (t != LOGIC_TOGGLEABLE_DUMMY)
      logicToggle(t);
  }
  else /* GLUT_UP */
  {
    if (!isSpecialKey)
    {
      switch (key)
      {
      }
    }
    else /* isSpecialKey */
    {
      switch (key)
      {
        case GLUT_KEY_UP:
          s = LOGIC_SWITCHABLE_CAMDOWN;
          break;
        case GLUT_KEY_LEFT:
          s = LOGIC_SWITCHABLE_CAMRIGHT;
          break;
        case GLUT_KEY_DOWN:
          s = LOGIC_SWITCHABLE_CAMUP;
          break;
        case GLUT_KEY_RIGHT:
          s = LOGIC_SWITCHABLE_CAMLEFT;
          break;
        case GLUT_KEY_PAGE_UP:
          s = LOGIC_SWITCHABLE_CAMFORWARD;
          break;
        case GLUT_KEY_PAGE_DOWN:
          s = LOGIC_SWITCHABLE_CAMBACKWARD;
          break;
      }
    }
    
    if (s != LOGIC_SWITCHABLE_DUMMY)
      while (logicGetSwitchable(s))
        logicSwitch(s);
  }
}

/**
 * Verarbeitung eines Mausereignisses.
 * Durch Bewegung der Maus bei gedrueckter Maustaste kann die aktuelle
 * Zeichenfarbe beeinflusst werden.
 * Falls Debugging aktiviert ist, wird jedes Mausereignis auf stdout
 * ausgegeben.
 *
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param eventType Typ des Ereignisses.
 * @param button ausloesende Maustaste (nur bei Ereignissen vom Typ mouseButton).
 * @param buttonState Status der Maustaste (nur bei Ereignissen vom Typ mouseButton).
 */
static void handleMouseEvent(int x, int y, MouseEventType eventType, int button, int buttonState)
{
  X = x;
  Y = y;
  
  switch (eventType)
  {
    case MOUSE_BUTTON:
      switch (button)
      {
        case GLUT_LEFT_BUTTON:
          leftclick = buttonState == GLUT_DOWN;
          break;
        case GLUT_RIGHT_BUTTON:
          rightclick = buttonState == GLUT_DOWN;
          break;
        default:
          break;
        }
      break;
    default:
      break;
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
 * Mouse-Button-Callback.
 *
 * @param button Taste, die den Callback ausgeloest hat.
 * @param state Status der Taste, die den Callback ausgeloest hat.
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
static void cbMouseButton(int button, int state, int x, int y)
{
  handleMouseEvent(x, y, MOUSE_BUTTON, button, state);
}

/**
 * Mouse-Motion-Callback.
 *
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
static void cbMouseMotion(int x, int y)
{
  logicMove(vector3dMake(  0.1 * (leftclick  ? - X + x : 0)
                        ,  0.1 * (leftclick  ?   Y - y : 0)
                        , 0.01 * (rightclick ? - Y + y : 0)
                        )
           );
  
  X = x;
  Y = y;
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
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection ((GLdouble) w / (GLdouble) h);
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
  /* Frames pro Sekunde */
  static double fps = 0.0;
  
  static float textColor[3] = { 1.0f, 0.0f, 0.0f };
  /* Buffer zuruecksetzen */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode(GL_MODELVIEW);

  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity();

  setCamera();

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
  /* Mouse-Button-Callback (wird ausgefuehrt, wenn eine Maustaste
   * gedrueckt oder losgelassen wird) */
  glutMouseFunc(cbMouseButton);
  
  glutMotionFunc(cbMouseMotion);
  
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
