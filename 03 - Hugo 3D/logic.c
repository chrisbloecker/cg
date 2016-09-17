/**
 * @file
 *
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "logic.h"
#include "drawing.h"
#include "gen.h"
#include "vector.h"
#include "animation.h"
#include "collision.h"
#include "displaylist.h"

#define LOGIC
#include "constants.h"
#undef LOGIC

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Buffer für Bewegungen, die Hugo ausführen soll */
typedef struct {
  Direction buf[2];     /* Bewegungen                        */
  double interpolation; /* Stand der aktuellen Interpolation */
} LogicBuf;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* Level- und Lebensanzahl */
#define LOGIC_LEVELS (3)
#define LOGIC_LIVES  (100)

/* Schritte, die Hugo pro Sekunde ausführen soll */
#define LOGIC_STEPS_PS (1.0)

#define LOGIC_CAM_ANGLE_PS  (90.0)
#define LOGIC_CAM_HEIGHT_PS (2.5)
#define LOGIC_LIGHTHOUSE_ANGLE_PS (45)

/* Interpolation für Hugos Seitensprünge */
#define LOGIC_INTERPOLATION_TIME (0.3 / LOGIC_STEPS_PS)

/* Anzahl der Punkte, die ein Item bringt */
#define LOGIC_ITEM_POINTS (50)

/* Ab wo sollen Objekte angezeigt werden */
#define LOGIC_CLIP_Z (10)

#define LOGIC_CLIP_VIEWABLE (- 1.3)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* Signumfunktion */
#define SIGNUM(x) (((x) > (0)) ? (+1) : (-1))

/* Holt den Wert x in die Grenzen [-b, +b] zurück, GEFÄHRLICHES MACRO! */
#define TOBOUNDS(x, b) (((fabs(x)) > (b)) ? ((SIGNUM(x)) * (b)) : (x))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Spielstatus */
static Game game;

/* Zuordnung der Zeichenfunktionen zu den Levels */
static DrawFunction functionsDrawing[LOGIC_LEVELS][DF_BUILDING + 1] = 
  {
    {drawStreet, drawStreetBackground, drawStreetSurface, drawTree, drawCoin,     drawCar,      drawHugo, drawNothing}
  , {drawRiver,  drawRiverBackground,  drawRiverSurface,  drawBuoy, drawStarfish, drawShip,     drawHugo, drawNothing}
  , {drawBeach,  drawBeachBackground,  drawBeachSurface,  drawPalm, drawIce,      drawSunshade, drawHugo, drawLighthouse}
  };

/* Zuordnung der Animationsfunktionen zu den Levels */
static AnimationFunction functionsAnimation[LOGIC_LEVELS] =
  { animationBounce
  , animationBlink
  , animationRotate
  };

/* Zuordnung der Kollisionserkennungsfunktionen zu den Levels */
static CollisionDetectionFunction functionsCollision[LOGIC_LEVELS][2] =
  {
    {collisionCoin,     collisionCar}
  , {collisionStarfish, collisionShip}
  , {collisionShell,    collisionCrab}
  };

/* Zuordnung der Geschwindigkeitsmultiplikatoren zu dden Levels */
static double speed[3] = {1.0, 1.5, 2.0};

/* Buffer für Bewegungen, die Hugo machen soll */
static LogicBuf buf;

/* Index des aktuellen Items und Hindernisses für Iteration */
static unsigned itemCurrent
              , obstacleCurrent;

/* Gibt an, ob die Hilde ausgegeben werden soll */
static Boolean showHelp = FALSE
             , pause    = FALSE;

/* Bewegung der Kamera */
static Boolean camMovement[4] = {FALSE, FALSE, FALSE, FALSE}
             , camDrive = FALSE
             ;

static double camRotation = 0.0        /* Aulenkung der Kamera           */
            , camHeight = 0.0          /* Höhe der Kamera                */
            , lighthouseRotation = 0.0 /* Auslenkung des Spotlights      */
            , camDriveAngle = 0.0      /* Auslenkung bei der Kamerafahrt */
            ;

static Boolean light[2] = {TRUE, FALSE};

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Bestimmt, ob die durch p gegebene Position sichtbar ist.
 * Dies ist der Fall, wenn sie sich innerhalb der y-Grenzen befindet.
 *
 * @param[in] p Position.
 *
 * @return true  wenn p sichtbar ist
 *         false sonst.
 */
static Boolean isVisiblePosition(Vector3d p)
{
  return fabs(p.z - game.hugo.z) < LOGIC_CLIP_Z;
}

/**
 * Schiebt den Bewegungspuffer einmal nach links und füllt von rechts mit "0"
 * auf.
 *
 * @param[in] b Buffer, der geschoben werden soll.
 *
 * @return geschobener Buffer.
 */
static LogicBuf shiftBuf(LogicBuf b)
{
  b.buf[0] = b.buf[1];
  b.buf[1] = DIR_EMPTY;
  b.interpolation = 0.0;
  
  return b;
}

/**
 * Leert den Bewegungspuffer durch zweimaliges Schieben.
 *
 * @param[in] b Buffer, der geleert werden soll.
 *
 * @return geleerter Buffer.
 */
static LogicBuf cleanBuf(LogicBuf b)
{
  return shiftBuf(shiftBuf(b));
}

/**
 * Korrigiert Hugos Position derart, dass er sich auf einer ganzzahligen
 * x-Position befindet.
 *
 * @param[in] hugo Position, an der sich Hugo befindet.
 *
 * @return korrigierte Position.
 */
static Vector3d correctHugoPos(Vector3d hugo)
{
  hugo.x = (int) (hugo.x + (0.5 * SIGNUM(hugo.x)));
  
  return hugo;
}

/**
 * Beendet das Spiel und gibt die Punkte aus.
 *
 * @param[in] b Gibt an, ob das Spiel gewonnen oder verloren wurde.
 */
static void gameOver(Boolean b)
{
  fprintf(stdout, "  _____          __  __ ______    ______      ________ _____         \n");
  fprintf(stdout, " / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\   \n");
  fprintf(stdout, "| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |   \n");
  fprintf(stdout, "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  /   \n");
  fprintf(stdout, "| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\    \n");
  fprintf(stdout, " \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n");
  
  if (b)
  {
    fprintf(stdout, "                  __    __                    \n");
    fprintf(stdout, "/\\_/\\___  _   _  / / /\\ \\ (_)_ __         \n");
    fprintf(stdout, "\\_ _/ _ \\| | | | \\ \\/  \\/ / | '_ \\      \n");
    fprintf(stdout, " / \\ (_) | |_| |  \\  /\\  /| | | | |        \n");
    fprintf(stdout, " \\_/\\___/ \\__,_|   \\/  \\/ |_|_| |_|      \n");
  }
  else
  {
    fprintf(stdout, "__   __          _                            \n");
    fprintf(stdout, "\\ \\ / /__ _  _  | |   ___  ___ ___ ___      \n");
    fprintf(stdout, " \\ V / _ \\ || | | |__/ _ \\/ _ (_-</ -_)    \n");
    fprintf(stdout, "  |_|\\___/\\_,_| |____\\___/\\___/__/\\___|  \n");
  }
  
  fprintf(stdout, "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
  fprintf(stdout, "| Punktestand :: %i Leben : %i Punkte --> %i + %i * %i = %i\n"
         , game.lives, game.points, game.points, game.lives, game.points, (1 + game.lives) * game.points);
  fprintf(stdout, "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

  exit(0);
}

/**
 * Lässt Hugo sterben.
 *
 * @param[in]     g Spiel.
 * @param[in,out] b Bewegungspuffer.
 *
 * @return neue Spielsituation.
 */
static Game die(Game g, LogicBuf * b)
{
  if (g.lives == 0)
    gameOver(FALSE);
  
  *b = cleanBuf(*b);
  
  g.lives -= 1;
  g.l = genLevel();
  g.hugo = vectorMake(0.0, 0.0, TRANSLATION_Z_HUGO);
  
  return g;
}

/**
 * Prüft ob eine Kollision zwischen Hugo und einem Gegenstand oder einem
 * Hindernis aufgetreten ist.
 *
 * @param[in] g Spielsituation.
 *
 * @return neue Spielsituation.
 */
static Game collisionDetection(Game g)
{
  Boolean dead = FALSE;
  
  unsigned item     = g.l.itemFirst      /* Gegenstände im Level */
         , obstacle = g.l.obstacleFirst  /* Hindernisse im Level */
         ;
  
  Vector3d v
         , hugo = vectorAdd(g.hugo, vectorMake(1.0, 0.0, TRANSLATION_Z_HUGO))
         ;
  
  /* Solange noch Gegenstände im Sichtbereich zu betrachten sind */
  while ((item < g.l.itemLast)
     &&  (isVisiblePosition(v = g.l.items[item])))
  {
    /* Wenn Hugo mit dem Gegenstand irgendwie in Kollision tritt */
    if (functionsCollision[g.level][COLLISION_ITEM](hugo, v))
    {
      g.l = levelItemRemoveAt(g.l, item);
      
      g.points += LOGIC_ITEM_POINTS;
    }
    else
      ++item;
  }
  
  /* Solange noch Hindernisse im Sichtbereich zu betrachten sind */
  while ( !dead
     &&  (obstacle < g.l.obstacleLast)
     &&  (isVisiblePosition(v = g.l.obstacles[obstacle])))
  {
    /* Wenn Hugo unters Auto gerät */
    if (functionsCollision[g.level][COLLISION_OBSTACLE](hugo, v))
    {
      dead = TRUE;
      camDrive = TRUE;
    }
    else
      ++obstacle;
  }
  
  return g;
}

/**
 * Räumt den Level derart auf, dass Gegenstände und Hindernisse, die bereits
 * den Sichtbereich durchqueert haben, "entfernt" werden.
 *
 * @param[in] l Level.
 *
 * @return aufgeräumter Level.
 */
static Level cleanLevel(Level l)
{
  unsigned item     = l.itemFirst      /* Aktueller Gegenstand */
         , obstacle = l.obstacleFirst; /* Aktuelles Hindernis  */
  
  /* Gegenstände löschen */
  while ((item < l.itemLast)
     &&  (l.items[item].z - game.hugo.z < LOGIC_CLIP_VIEWABLE))
    l = levelItemRemoveAt(l, item);
  
  /* Hindernisse löschen */
  while ((obstacle < l.obstacleLast)
     &&  (l.obstacles[obstacle].z - game.hugo.z < LOGIC_CLIP_VIEWABLE))
   l = levelObstacleRemoveAt(l, obstacle);
  
  /* fertig */
  return l;
}

/**
 * Gibt an, ob Hugo sich bewegen darf.
 * Dies ist der Fall, wenn er nicht gleichzeitig links steht und sich nach
 * links bewegen soll bzw. wenn er nicht gleichzeitig rechts steht und sich
 * nach rechts bewegen soll.
 *
 * @param[in] hugo Hugos Position.
 * @param[in] b    Bewegungspuffer.
 *
 * @return true  wenn Hugo sich bewegen darf
 *         false sonst.
 */
static Boolean hugoMayMove(Vector3d hugo, LogicBuf b)
{
  return ((buf.buf[0] != DIR_LEFT  || hugo.x > - 1)
       && (buf.buf[0] != DIR_RIGHT || hugo.x <   1));
}

/**
 * Startet den nächsten Level bzw. beendet das Spiel, wenn es vorbei ist.
 */
static Game nextLevel(Game g)
{
  g.level += 1;
  
  if (g.level == 2)
    light[1] = TRUE;
  
  if (g.level > 2)
    gameOver(TRUE);
  
  g.hugo = vectorMake(0.0, 0.0, TRANSLATION_Z_HUGO);
  g.l    = genLevel();
  
  return g;
}

/**
 * Berechnet die Position der Kamera und rotiert die Birne des Leuchtturms.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcCams(double interval)
{
  if (camMovement[CAM_UP])
    camHeight += interval * LOGIC_CAM_HEIGHT_PS;
  if (camMovement[CAM_DOWN])
    camHeight -= interval * LOGIC_CAM_HEIGHT_PS;
  if (camMovement[CAM_LEFT])
    camRotation -= interval * LOGIC_CAM_ANGLE_PS;
  if (camMovement[CAM_RIGHT])
    camRotation += interval * LOGIC_CAM_ANGLE_PS;
  
  while (camRotation < 0.0)
    camRotation += 360.0;
  
  while (camRotation > 360.0)
    camRotation -= 360.0;
  
  lighthouseRotation += interval * LOGIC_LIGHTHOUSE_ANGLE_PS;
  
  while (lighthouseRotation > 360.0)
    lighthouseRotation -= 360.0;
}

/**
 * Lässt die Kamera eine 360° Fahrt machen. Dies tritt ein, wenn Hugo stirbt.
 *
 * @param[in] interval Zeit seit dem letzten Aufruf.
 * @param[in] g        Spielstatus.
 * @param[in] b        Bewegungsbuffer.
 *
 * @return neue Spielsituation.
 */
static Game letOurCamMove(double interval, Game g, LogicBuf * b)
{
  /* Bewegungsbuffer immer leeren */
  cleanBuf(*b);
  
  /* Kamera nach rechts rotieren */
  camMovement[CAM_RIGHT] = TRUE;
  camMovement[CAM_LEFT]  = FALSE;
  camMovement[CAM_UP]    = FALSE;
  camMovement[CAM_DOWN]  = FALSE;
  
  /* Weiterdrehen */
  camDriveAngle += interval * LOGIC_CAM_ANGLE_PS;
  camRotation = camDriveAngle;
  
  /* Rotation fertig, Hugo sterben assen und Kamerapositionen setzen */
  if (camDriveAngle > 360.0)
  {
    camMovement[CAM_RIGHT] = FALSE;
    camDriveAngle = 0.0;
    camRotation = 0.0;
    camDrive = FALSE;
    g = die(g, b);
  }
  
  return g;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert das Spiel und sagt "1".
 *
 * @return 1.
 */
extern int logicInit(void)
{
  displaylistInit();
  displaylistSet();
  
  game.level  = 0;
  game.points = 0;
  game.lives  = LOGIC_LIVES;
  game.l      = genLevel();
  game.hugo   = vectorMake(0.0, 0.0, TRANSLATION_Z_HUGO);
  
  return 1;
}

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval)
{
  calcCams(interval);
  
  if (camDrive)
  {
    game = letOurCamMove(interval, game, &buf);
  }
  else if (!showHelp && !pause)
  {
    /* Hugo weitersetzen */
    game.hugo.z += interval * LOGIC_STEPS_PS * speed[game.level];
    
    /* Solange Hugo sich nicht bewegen darf, den Bewegungspuffer weiterschieben */
    while (!hugoMayMove(game.hugo, buf))
      buf = shiftBuf(buf);
    
    /* Wenn Hugo sich bewegen darf, dann die Bewegung interpolieren */
    switch (buf.buf[0])
    {
      case DIR_EMPTY:
        break;
      case DIR_LEFT:
        game.hugo.x       -= interval / LOGIC_INTERPOLATION_TIME * speed[game.level];
        buf.interpolation += interval / LOGIC_INTERPOLATION_TIME * speed[game.level];
        break;
      case DIR_RIGHT:
        game.hugo.x       += interval / LOGIC_INTERPOLATION_TIME * speed[game.level];
        buf.interpolation += interval / LOGIC_INTERPOLATION_TIME * speed[game.level];
        break;
    }
    
    /* "Das ist mein Tanzbereich, das ist deiner!", Hugo, bleib in deinem ... */
    game.hugo.x = TOBOUNDS(game.hugo.x, 1);
    
    /**
     * Wenn die Interpolation abgeschlossen ist, dann den Bewegungspuffer schieben
     * und Hugo auf eine ganzzahlige Position setzen
     */
    if (buf.interpolation >= 1.0)
    {
      buf = shiftBuf(buf);
      game.hugo = correctHugoPos(game.hugo);
    }
    
    /* Level aufräumen */
    game.l = cleanLevel(game.l);
    
    /* Kollisionserkennung durchführen */
    game = collisionDetection(game);
    
    /**
     * Nächsten Level starten, wenn weder Gegenstände noch Hindernisse übrig sind
     */
    if (levelItemsLeft(game.l) == 0
    &&  levelObstaclesLeft(game.l) == 0)
    {
      game = nextLevel(game);
      displaylistSet();
    }
  }
}

/**
 * Gibt zurück, wie weit Hugo vorangeschritten ist.
 *
 * @return Hugos y-Position.
 */
extern double logicGetShift(void)
{
  return game.hugo.z;
}

/**
 * Gibt Hugos x-Position zurück.
 *
 * @return Hugos x-Position;
 */
extern double logicGetHugoShift(void)
{
  return game.hugo.x;
}

/**
 * Fügt eine Bewegung zum Bewegungspuffer hinzu.
 * Wenn der Puffer bereits 2 Einträge enthält, so wird nichts gemacht.
 *
 * @param[in] d Richtung, die hinzugefügt werden soll.
 */
extern void logicAddMovement(Direction d)
{
  if(!showHelp)
  {
    if (buf.buf[0] == DIR_EMPTY)
      buf.buf[0] = d;
    else if (buf.buf[1] == DIR_EMPTY)
      buf.buf[1] = d;
  }
}

/**
 * Gibt die zum aktuellen Level zugeordnete Zeichenfunktion zurück, die den
 * Objekttypen t zeichnet.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t und zum aktuellen Level gehörende Zeichenfunktion.
 */
DrawFunction logicGetDrawFunction(DrawFunctionType t)
{
  return functionsDrawing[game.level][t];
}

/**
 * Gibt die zum aktuellen Level zugeordnete Animationsfunktion zurück.
 *
 * @return zum aktuellen Level gehörende Animationsfunktion.
 */
extern AnimationFunction logicGetAnimationFunction(void)
{
  return functionsAnimation[game.level];
}

/**
 * Initialisiert den Iterator für die Items.
 */
extern void logicItemIterInit(void)
{
  itemCurrent = game.l.itemFirst;
}

/**
 * Initialisiert den Iterator für die Hindernisse.
 */
extern void logicObstacleIterInit(void)
{
  obstacleCurrent = game.l.obstacleFirst;
}

/**
 * Prüft, ob es noch weitere Gegenstände innerhalb des Sichtbereiches gibt.
 *
 * @return true  wenn es noch Gegenstände im Sichtbereich gibt
 *         false sonst.
 */
extern Boolean logicItemHasNext(void)
{
  return itemCurrent < game.l.itemLast && isVisiblePosition(game.l.items[itemCurrent]);
}

/**
 * Prüft, ob es noch weitere Hindernisse innerhalb des Sichtbereiches gibt.
 *
 * @return true  wenn es noch Hindernisse im Sichtbereich gibt
 *         false sonst.
 */
extern Boolean logicObstacleHasNext(void)
{
  return obstacleCurrent < game.l.obstacleLast && isVisiblePosition(game.l.obstacles[obstacleCurrent]);
}

/**
 * Gibt die Position des nächsten Gegenstandes zurück und setzt den Zähler des
 * aktuellen Gegenstandes hoch.
 *
 * @return Position des nächsten Gegenstandes.
 */
extern Vector3d logicItemNext(void)
{
  return game.l.items[itemCurrent++];
}

/**
 * Gibt die Position des nächsten Hindernisses zurück und setzt den Zähler des
 * aktuellen Hindernisses hoch.
 *
 * @return Position des nächsten Hindernisses.
 */
extern Vector3d logicObstacleNext(void)
{
  return game.l.obstacles[obstacleCurrent++];
}

/**
 * Stellt Textinformationen anhand des aktuellen Spielstandes zusammen.
 *
 * @return Textinformationen.
 */
extern Text logicGetText(void)
{
  Text t;
  
  /* füllen */
  t.points = game.points;
  t.level  = game.level + 1;
  t.lives  = game.lives;
  
  return t;
}

/**
 * Gibt an, ob die Spielhilfe ausgegeben werden soll.
 *
 * @return true  wenn die Hilfe ausgegeben werden soll
 *         false sonst.
 */
extern Boolean logicShowHelp(void)
{
  return showHelp;
}

/**
 * Legt fest, ob die Spielhilfe ausgegeben werden soll.
 * showHelp <- !showHelp
 */
extern void logicSetHelp(void)
{
  showHelp = !showHelp;
}

extern void logicSetPause(void)
{
  pause = !pause;
}

/**
 * Gibt dem Spieler ein Leben dazu.
 */
extern void logicAddLife(void)
{
  ++game.lives;
}

/**
 * Wechselt zu Level i.
 *
 * @param[in] i Level.
 */
extern void logicChangeLevel(unsigned i)
{
  assert(i <= LOGIC_LEVELS);
  
  game.level = i - 1;
  game.l     = genLevel();
  game.hugo  = vectorMake(0.0, 0.0, TRANSLATION_Z_HUGO);
  
  displaylistSet();
}

/**
 * Setzt den Bewegungsstatus der Kamera in Richtung cd auf b.
 *
 * @param[in] cd Bewegungsrichtung für die Kamera.
 * @param[in] b  Status der Bewegung in Richtung cd.
 */
extern void logicSetCamMovement(LogicCamDirection cd, Boolean b)
{
  camMovement[cd] = b;
}

/**
 * Gibt die Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß
 * zurück.
 *
 * @return Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß.
 */
extern int logicGetCamRotation(void)
{
  return camRotation;
}

/**
 * Gibt die Höhe der Kamera bezogen auf die Grundposition zurück.
 *
 * @return Höhe der Kamera bezogen auf die Grundposition.
 */
extern double logicGetCamHeight(void)
{
  return camHeight;
}

/**
 * Gibt die Auslenkung der Lichtquelle des Leuchtturms bezogen auf die
 * Grundposition im Gradmaß zurück.
 *
 * @return Auslenkung der Lichtquelle des Leuchtturms bezogen auf die
 *         Grundposition im Gradmaß.
 */
extern double logicGetLighthouseRotation(void)
{
  return lighthouseRotation;
}

/**
 * Schaltet das Licht mit der Nummer i ein bzw. aus.
 */
extern void sceneSetLight(unsigned i)
{
  light[i] = !light[i];
  
  fprintf(stderr, "DEBUG :: Light %i %s.\n", i, light[i] ? "on" : "off");
}

/**
 * @return Gibt zurück, ob Licht i eingeschaltet ist.
 */
extern Boolean logicGetLight(unsigned i)
{
  return light[i];
}
