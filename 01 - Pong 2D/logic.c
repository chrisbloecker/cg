/**
 * @file
 *
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung des Balls sowie des
 * Schlägers.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"
#include "logic.h"
#include "vector.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* Ballgeschwindigkeit */
#define BALL_SPEED          (0.8)
#define BALL_SPEED_INCREASE (0.08)

/* Startwerte des Balls */
#define BALL_START_X (-0.2)
#define BALL_START_Y ( 0.0)

/* Startwerte des Schlägers */
#define RACKET_START_X     (-0.85)
#define RACKET_START_Y     ( 0.0 )
#define RACKET_START_ANGLE ( 0.0 )

/** Schläger-Werte */
#define RACKET_STEPS_PS  ( 1.0 )
#define RACKET_ANGLE_PS  (90.0 )
#define RACKET_X         ( 0.05)
#define RACKET_Y         ( 0.2 )
#define RACKET_Y_MAX     ( 0.6 )
#define RACKET_ANGLE_MAX (35   )

/** Ball-Werte */
#define BALL_RADIUS       ( 0.1 )
#define BALL_COLORS       ( 6   )
#define BALL_ANGLE_MAX    (45   )

/* Balkenwerte */
#define STRIP_REFLECTBORDER (  0.8)
#define STRIP_END_LEFT      (- 0.9)

/** Spielwerte */
#define LIVES 3

/* ---- Macros ---- */
/* Umrechnung vom Grad- ins Bogenmaß */
#define DEGTORAD(x) ((x)*(2)*(PI)/(360))

/* Umrechnung vom Bogen- ins Gradmaß */
#define RADTODEG(x) ((x)*(360)/((2)*(PI)))

/* Signumfunktion */
#define SIGNUM(x) (((x) > (0)) ? (+1) : (-1))

/* Holt den Wert x in die Grenzen [-b, +b] zurück, GEFÄHRLICHES MACRO! */
#define TOBOUNDS(x, b) (((fabs(x)) > (b)) ? ((SIGNUM(x)) * (b)) : (x))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */
/* Bewegungsstatus des Schlägers. Für alle vier Richtungen wird angegeben, ob
 * sich der Schläger in die jeweilige Richtung bewegt. */
static Boolean move[4] = {FALSE, FALSE, FALSE, FALSE};

/* Der Spielstand */
static GameState gameState;

/* Der Ball */
static Ball ball;

/* Der Schläger */
static Racket racket;

/* Farben, die der Ball annehmen kann */
static RGBColor ballColors[BALL_COLORS] = {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.5, 0.5, 1.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}};

/* Schlägerpunkte */
static Rect racketPoints = {{+RACKET_X, +RACKET_Y}, {+RACKET_X, -RACKET_Y}, {-RACKET_X, -RACKET_Y}, {-RACKET_X, +RACKET_Y}};

/* Nächste Reflexion am Schläger möglich? */
static Boolean racketCollisionPossible = TRUE;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Berechnet, ob eine Kollision zwischen Ball und Schläger aufgetreten ist.
 * Dazu wird zunächst eine Gerade durch die rechten Eckpunkte des Schlägers
 * gelegt. Befindet sich der Ball auf der Geraden, so wird geprüft, ob er sich
 * innerhalb des Bereiches befindet, der durch die Geraden definiert wird, die
 * durch die Ober- und Unterkankte des Schlägers definiert wird.
 */
static Boolean collision()
{
  Boolean res = FALSE;
  
  double distanceRight; /* Distanz zur rechten Seite des Schlägers */
  
  Line ballLine    /* Ball */
     , racketLine; /* Schläger */
  
  /* Gerade, die durch die rechte Seite des Schlägers definiert wird */
  racketLine.p = racket.vertices[0];
  racketLine.q = vectorSub(racket.vertices[1], racket.vertices[0]);
  
  /**
   * Gerade, die durch die aktuelle Position des Balls und einen Vektor
   * orthogonal zur rechten Schlägerseite definiert wird
   */
  ballLine.p = ball.p;
  ballLine.q = vectorOrtho(racketLine.q);

  /**
   * Entfernung des Balls von der Gerade, die durch die rechte Seite des
   * Schlägers definiert wird
   */
  distanceRight = fabs(pointDistance(ballLine, racketLine));
  
  /**
   * "Berührt" der Ball diese Gerade, so muss geprüft werden, ob er "innerhalb"
   * der Schlägergrenzen ist
   */
  if (distanceRight - BALL_RADIUS < DBL_EPSILON)
  {
    double distanceTop     /* Distanz zur oberen Seite des Schlägers  */
         , distanceBottom; /* Distanz zur unteren Seite des Schlägers */
    
    Line topLine     /* Obere Kante  */
       , bottomLine; /* Untere Kante */
    
    /* Gerade, die durch die oberen Eckpunkte des Schlägers definiert wird */
    topLine.p = racket.vertices[0];
    topLine.q = vectorSub(racket.vertices[3], racket.vertices[0]);
    
    /* Gerade, die durch die unteren Eckpunkte des Schlägers definiert wird */
    bottomLine.p = racket.vertices[1];
    bottomLine.q = vectorSub(racket.vertices[2], racket.vertices[1]);
    
    /* Gerade, durch die Position des Balls und  */
    ballLine.q = vectorNorm(racketLine.q);
    
    /* Entfernung zum oberen und unteren Rand bestimmen */
    distanceTop    = fabs(pointDistance(ballLine, topLine));
    distanceBottom = fabs(pointDistance(ballLine, bottomLine));
    
    /**
     * Der Ball befindet sich auf dem Schlägerrand, wenn die Summe der Distanzen
     * von Ober- und Unterkante kleiner ist als die Länge der Längsseite des
     * Schlägers.
     */
    res = (distanceTop + distanceBottom - 2 * RACKET_Y - BALL_RADIUS) < DBL_EPSILON;
  }
  
  return res;
}

/**
 * Reflektiert den Ball am Schläger.
 * Dazu wird zunächst der Aufprallpunkt berechnet. Dann wird eine Gerade anhand
 * des Aufprallpunktes und der Senkrechten auf die rechte Schlägerseite definiert.
 * Die Ballposition wird dann an dieser Geradenn gespiegelt und anhand der neuen
 * Position die neue Bewegungsrichtung des Balls bestimmt.
 */
static void reflectBall(void)
{
  Line ballLine         /* Ballgerade                     */
     , ballOrthoLine    /* orthogonal zur Ballgeraden     */
     , racketLine       /* Schlägergerade                 */
     , racketOrthoLine; /* orthogonal zur Schlägergeraden */
  
  Vector2d collision    /* Aufprallpunkt Schläger/Ball       */
         , reflected;   /* Reflektierter Punkt ffür den Ball */
  
  double d
       , s;
  
  /* Gerade, die sich durch Position des Balls und seine Richtung ergibt */
  ballLine.p = ball.p;
  ballLine.q = vectorNorm(ball.direction);
  
  /* Gerade, die durch die rechte Seite des Schlägers definiert wird */
  racketLine.p = racket.vertices[0];
  racketLine.q = vectorNorm(vectorSub(racket.vertices[1], racket.vertices[0]));
  
  /* Entfernung vom Ball zur Schlägergeraden */
  s = pointDistance(ballLine, racketLine);
  
  /* Aufprallpunkt auf dem Schläger */
  collision = vectorAdd(ballLine.p, vectorScale(ballLine.q, s));
  
  /**
   * Gerade, die durch die "Position" des Balls und den Richtungsvektor der
   * Schlägergeraden definiert wird 
   */
  ballOrthoLine.p = vectorSub(collision, ball.direction);
  ballOrthoLine.q = vectorNorm(racketLine.q);
  
  /**
   * Gerade, die durch den Aufprallpunkt verläuft und orthogonal zur
   * Schlägergeraden steht
   */
  racketOrthoLine.p = collision;
  racketOrthoLine.q = vectorNorm(vectorOrtho(racketLine.q));
    
  /* Entfernung vom Ball zur Schlägerlotgeraden */
  d = pointDistance(ballOrthoLine, racketOrthoLine);
  
  /* Spiegelung der Ballposition an der Schlägerlotgeraden */
  reflected = vectorAdd(ballOrthoLine.p, vectorScale(ballOrthoLine.q, 2 * d));
  
  /* Neue Bewegungsrichtung des Balls */
  ball.direction = vectorNorm(vectorSub(reflected, collision));
}

/**
 * Lässt den Spieler ein Leben verlieren, gibt den aktuellen Punktestand aus
 * und initialisiert Ball und Schläger neu.
 */
static void loseLife(void)
{
  --gameState.lives;
  
  fprintf(stdout, "Punkte: %i\n", gameState.points);
  fprintf(stdout, "Leben:  %i\n", gameState.lives);
  fprintf(stdout, "\n");
  
  if (gameState.lives > 0)
  {
    gameState.points = 0;
    initBall();
  }
  else
  {
    fprintf(stdout, "Leider verloren ...\n");
    ball.p.x   = BALL_START_X;
    ball.p.y   = BALL_START_Y;
    ball.speed = 0.0;
  }
}

/**
 * Bestimmt eine neue Farbe für den Ball.
 *
 * @param[in] not Farbe, die es nicht sein darf, da der Ball diese momentan hat.
 *
 * @return neue Farbe für den Ball.
 */
static unsigned newBallColor(unsigned not)
{
  unsigned color = rand() % BALL_COLORS;
  
  return (color != not)
       ? color
       : newBallColor(not);
}

/**
 * Färbt den Ball neu.
 */
static void dyeBall(void)
{
  /* neue Farbe für den Ball bestimmen */
  ball.color = newBallColor(ball.color);
  
  ball.c[0] = ballColors[ball.color][0];
  ball.c[1] = ballColors[ball.color][1];
  ball.c[2] = ballColors[ball.color][2];
}

/**
 * Erzeugt eine Bewegungsrichtung für den Ball.
 *
 * @return Normierter Richtungsvektor für den Ball.
 */
static Vector2d makeBallDirection(void)
{
  Vector2d res;
  double angle = DEGTORAD(rand() % (BALL_ANGLE_MAX + 1)) * ((rand() % 2) & 1 ? (+1) : (-1));
  
  res.x = 1.0;
  res.y = 0.0;
  
  return vectorNorm(vectorRotate(res, angle));
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert den Spielstatus.
 */
void initGame(void)
{
  srand(time(NULL));
  gameState.lives  = LIVES;
  gameState.points = 0;
}

/**
 * Initialisiert den Ball.
 * Dazu gehören
 *   - Position
 *   - Geschwindigkeit
 *   - Richtung
 */
void initBall(void)
{
  unsigned i;

  /* Position des Balls */  
  ball.p.x = BALL_START_X;
  ball.p.y = BALL_START_Y;
  
  /* Farbe des Balls */
  ball.color = 0;
  dyeBall();
  
  /* Bewegungsrichtung */
  ball.direction = makeBallDirection();
  
  /* Bewegungsgeschwindigkeit */
  ball.speed = BALL_SPEED;
  
  /* Berechnen und skalieren des Einheitskreises zum Zeichnen des Balls */
  for (i = 0; i < BALL_ITERATIONS; ++i)
  {
    ball.vertices[i].x = BALL_RADIUS * cos(DEGTORAD(i * 360 / BALL_ITERATIONS));
    ball.vertices[i].y = BALL_RADIUS * sin(DEGTORAD(i * 360 / BALL_ITERATIONS));
  }
}

/**
 * Initialisiert den Schläger.
 * Dazu gehören
 *   - Position
 *   - Auslenkungswinkel
 */
void initRacket(void)
{
  /* Position */
  racket.p.x   = RACKET_START_X;
  racket.p.y   = RACKET_START_Y;
  
  /* Auslenkungswinkel */
  racket.angle = RACKET_START_ANGLE;
}

/**
 * Gibt einen Zeiger auf den Schläger zurück.
 * Zeiger, damit nicht bei jedem Aufruf alle Werte kopiert werden.
 *
 * @return Zeiger auf den Schläger.
 */
Racket * getRacket(void)
{
  return &racket;
}

/**
 * Gibt einen Zeiger auf den Ball zurück.
 * Zeiger, damit nicht bei jedem Aufruf alle Werte kopiert werden.
 *
 * @return Zeiger auf den Ball.
 */
Ball * getBall(void)
{
  return &ball;
}

/**
 * Berechnet neue Position von Ball und Schläger.
 *
 * @param[in] interval Dauer der Bewegung in Sekunden.
 */
void calcPositions(double interval)
{
  unsigned i;
  
  /* Winkel und Winkelwerte zur Berechnung der Schlägerecken */
  double a = DEGTORAD(racket.angle);
  
  /* Bewegung nach links und rechts */
  if (move[dirLeft])
    racket.angle += RACKET_ANGLE_PS * interval;
  if (move[dirRight])
    racket.angle -= RACKET_ANGLE_PS * interval;
  
  /* Auslenkungswinkel des Schlägers innerhalb des Bereiches halten */
  racket.angle = TOBOUNDS(racket.angle, RACKET_ANGLE_MAX);
  
  /* Bewegung nach oben und unten */
  if (move[dirUp])
    racket.p.y += RACKET_STEPS_PS * interval;
  if (move[dirDown])
    racket.p.y -= RACKET_STEPS_PS * interval;

  /* Position des Schlägers innerhalb des Bereiches halten */  
  racket.p.y = TOBOUNDS(racket.p.y, RACKET_Y_MAX);
  
  /* Berechnung der Eckpunkte des Schlägers */
  for (i = 0; i < 4; ++i)
    racket.vertices[i] = vectorAdd(vectorRotate(racketPoints[i], a), racket.p);
  
  /* Ball bewegen */
  ball.p = vectorAdd(ball.p, vectorScale(ball.direction, ball.speed * interval));
}

/**
 * Berechnet die Bewegungsrichtung des Balls.
 * Diese ändert sich, wenn der Ball mit einem der Balken oder dem Schläger
 * zusammenstößt.
 */
void calcMovement(void)
{
  /**
   * Rechter Balken, der Ball muss außerdem in einer Bewegung nach rechts sein,
   * damit er hier reflektiert werden kann.
   */
  if ((STRIP_REFLECTBORDER - ball.p.x - BALL_RADIUS) < DBL_EPSILON 
     && ball.direction.x > 0.0)
  {
    ball.direction.x *= -1;
    racketCollisionPossible = TRUE;
  }
  
  /**
   * Oberer und unterer Balken, der Ball muss außerdem in einer Bewegung nach 
   * oben bzw. unten sein, damit er hier reflektiert werden kann.
   * Außerdem darf er nicht auf der linken Seite den Bereich der Balken
   * verlassen haben.
   */
   if ((STRIP_REFLECTBORDER - fabs(ball.p.y) - BALL_RADIUS) < DBL_EPSILON 
     && ball.p.x > STRIP_END_LEFT)
     if (  (ball.p.y > 0.0 && ball.direction.y > 0.0)
        || (ball.p.y < 0.0 && ball.direction.y < 0.0))
     {
       ball.direction.y *= -1;
       racketCollisionPossible = TRUE;
     }
  
  /* Wenn der Ball das Spielfeld verlässt, verliert der Spieler ein Leben. */
  if (ball.p.x < -1)
    loseLife();
  
  /* Schlägerkollision */
  if (racketCollisionPossible && collision(ball, racket))
  {
    racketCollisionPossible = FALSE;
    ++gameState.points;
    reflectBall();
    dyeBall();
    ball.speed += BALL_SPEED_INCREASE;
  }
}

/**
 * Setzt den Bewegunsstatus des Schlägers.
 *
 * @param[in] direction Bewegungsrichtung deren Status verändert werden soll.
 * @param[in] status    neuer Status der Bewegung.
 */
void setMovement(Direction direction, Boolean status)
{
  if (direction <= dirDown)
    move[direction] = status;
}
