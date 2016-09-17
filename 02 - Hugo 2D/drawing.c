/**
 * @file
 *
 * Das Modul stellt Zeichenfunktionen zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <GL/glu.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "drawing.h"

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet ein Quadrat im Ursprung
 */
static void drawSquare(void)
{
  glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0,  1.0);
    glVertex2f( 1.0,  1.0);
    glVertex2f( 1.0, -1.0);
  glEnd();
}

/**
 * Zeichnet einen Kreis im Ursprung
 */
static void drawCircle(void)
{
  /* Quadrisches Hilfsobject */
  static GLUquadricObj *q;
  q = gluNewQuadric();

  gluDisk(
    q,   /* Quadric Objekt */
    0.0, /* Kein Loch in der Mitte */
    1.0, /* Radius (füllend) */
    360, /* Anzahl Tortenstücke */
    1    /* Nur ein "innerer" Kreis */
  ); 
}

/**
 * Zeichnet einen Halbkreis im Ursprung
 */
static void drawHalfCircle(void)
{
  static GLUquadricObj *q;
  q = gluNewQuadric();

  gluPartialDisk(
    q,   /* Quadric Objekt */
    0.0, /* Kein Loch in der Mitte */
    1.0, /* Radius (füllend) */
    360, /* Anzahl Tortenstücke */
    1,   /* Nur ein "innerer" Kreis */
    270, /* Startwinkel */
    180  /* Winkel des Scheibenstücks */
  ); 
}

/**
 * Zeichnet die Ecke rechts oben von einem Baum
 */
static void drawTreeSub(void)
{
  /* Baum Ecke rechts oben*/
  glBegin(GL_POLYGON);
    /* Mittelpunkt */
    glVertex2f( 0.0,  0.0); 
    /* Mitte oben */
    glVertex2f( 0.0,  1.0);
    /* Baumverlauf */
    glVertex2f( 0.2,  1.0);
    glVertex2f( 0.3,  0.9);
    glVertex2f( 0.5,  0.9);
    glVertex2f( 0.6,  0.8);
    glVertex2f( 0.65,  0.65);
    glVertex2f( 0.8,  0.6);
    glVertex2f( 0.9,  0.5);
    glVertex2f( 0.9,  0.3);
    glVertex2f( 1.0,  0.2);
    /* Mitte Rechts */
    glVertex2f( 1.0,  0.0);
  glEnd();
}

/**
 * Zeichnet ein Blatt einer Palme vertikal von 0 0 bis 0 1
 */
static void drawPalmSub(void)
{
  /* Baum Ecke rechts oben*/
  glBegin(GL_POLYGON);
  
    /* Linke Seite vom Blatt aufwärts */
    glVertex2f( -0.1, -0.1); 
    glVertex2f( -0.1,  0.1); 
    glVertex2f( -0.2,  0.2); 
    glVertex2f( -0.1,  0.3); 
    glVertex2f( -0.2,  0.4); 
    glVertex2f( -0.1,  0.5); 
    glVertex2f( -0.2,  0.6); 
    glVertex2f( -0.1,  0.7); 
    glVertex2f( -0.15,  0.8); 
    
    /* Spitze des Blatts */
    glVertex2f(  0.0,  1.0); 

    /* Rechte Seite vom Blatt abwärts */
    glVertex2f(  0.15,  0.8); 
    glVertex2f(  0.1,  0.7); 
    glVertex2f(  0.2,  0.6);
    glVertex2f(  0.1,  0.5);
    glVertex2f(  0.2,  0.4);
    glVertex2f(  0.1,  0.3);
    glVertex2f(  0.2,  0.2);
    glVertex2f(  0.1,  0.1); 
    glVertex2f(  0.1, -0.1); 
  glEnd(); 
}

/**
 * Zeichnet eine Kokusnuss, passend positioniert zum Palmenblatt
 */
static void drawPalmCoconut(void)
{  
  /* Eine Kokosnuss */
  glPushMatrix();
    glTranslatef(0.1, 0.2, 0.0);
    glScalef(0.1, 0.1, 0.1);
    drawCircle();
  glPopMatrix();
}

/**
 * Zeichnet einen Arm vom Seestern
 */
static void drawStarfishArm(void)
{
  glBegin(GL_POLYGON);
    /* Unten */
    glVertex2f( 0.3, 0.0);
    glVertex2f( 0.0, -0.3);
    glVertex2f(-0.3, 0.0);
  
    /* Links hoch */
    glVertex2f(-0.2, 0.7);
    glVertex2f(-0.1, 0.95);
    
    /* Spitze */
    glVertex2f( 0.0, 1.0); 
    
    /* Rechts runter */
    glVertex2f( 0.1, 0.95);
    glVertex2f( 0.2, 0.7);
  glEnd();
}

/**
 * Zeichnet die linke Seite einer Krabbe
 */
static void drawCrabLeft(void) 
{
  /* Beine Links */
  glBegin(GL_POLYGON);
    glVertex2f(-0.7,  0.9);
    glVertex2f(-0.6,  1.0);
    glVertex2f( 0.0,  0.0);
    glVertex2f(-0.1, -0.1);
  glEnd();  
  glBegin(GL_POLYGON);
    glVertex2f(-0.9,  0.6);
    glVertex2f(-0.8,  0.7);
    glVertex2f( 0.0,  0.0);
    glVertex2f(-0.2, -0.2);
  glEnd();  
  glBegin(GL_POLYGON);
    glVertex2f(-1.0,  0.0);
    glVertex2f(-1.0,  0.2);
    glVertex2f( 0.1,  0.1);
    glVertex2f(-0.2,  0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex2f(-1.0, -0.2);
    glVertex2f(-0.9, -0.4);
    glVertex2f( 0.0,  0.1);
    glVertex2f( 0.0, -0.1);
  glEnd();
  
  /* Schere Links */
  glBegin(GL_POLYGON);
    glVertex2f(-0.7, -0.8);
    glVertex2f(-0.8, -0.6);
    glVertex2f(-0.2,  0.0);
    glVertex2f( 0.0, -0.1);
    glVertex2f(-0.5, -0.6);
  glEnd();
  glBegin(GL_TRIANGLES);
    glVertex2f(-0.7, -0.8);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-0.4, -1.0);

    glVertex2f(-0.7, -0.8);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-0.2, -0.8);
  glEnd();
}

/**
 * Zeichnet Hugos Körper
 */
static void drawHugoBody(void) 
{
  glBegin(GL_POLYGON);
    /* Rechts */
    glVertex2f( 0.625,  0.5);
    glVertex2f( 0.875,  0.375);
    glVertex2f( 1.0,    0.25);
    glVertex2f( 1.0,   -0.25);
    glVertex2f( 0.875, -0.375);
    glVertex2f( 0.625, -0.5);
    /* Links */
    glVertex2f(-0.625, -0.4);
    glVertex2f(-0.875, -0.375);
    glVertex2f(-1.0,   -0.25);
    glVertex2f(-1.0,    0.25);
    glVertex2f(-0.875,  0.375);
    glVertex2f(-0.625,  0.5);
  glEnd();  
}

/**
 * Zeichnet Hugos Arme
 */
static void drawHugoArms(void) 
{
  /* Arm rechts */
  glPushMatrix();
    glTranslatef( 0.8, 0.0, 0.0);
    glScalef(0.2, 0.2, 1.0);
    drawCircle();
  glPopMatrix();
  /* Arm links */
  glPushMatrix();
    glTranslatef(-0.8, 0.0, 0.0);
    glScalef(0.2, 0.2, 1.0);
    drawCircle();
  glPopMatrix();
}

/**
 * Zeichnet Hugos Kopf und Nase
 */
static void drawHugoHead(void) 
{
  /* Kopf */
  glBegin(GL_POLYGON);
    /* Mit Nasespitze anfangen */
    glVertex2f( 0.0,  0.5);
    
    /* Rechte Seite */
    glVertex2f( 0.1,  0.5);
    glVertex2f( 0.2,  0.5);
    glVertex2f( 0.4,  0.4);
    glVertex2f( 0.5,  0.2);
    glVertex2f( 0.5,  0.0);
    glVertex2f( 0.3, -0.2);
    
    /* Linke Seite */
    glVertex2f(-0.3, -0.2);
    glVertex2f(-0.5,  0.0);
    glVertex2f(-0.5,  0.2);
    glVertex2f(-0.4,  0.4);
    glVertex2f(-0.2,  0.5);
    glVertex2f(-0.1,  0.5);
  glEnd();
  
  /* Nase */
  glPushMatrix();
    glTranslatef( 0.0,  0.55, 0.0);
    glScalef(0.1, 0.1, 1.0);
    drawCircle();
  glPopMatrix();
}

/**
 * Zeichnet Hugos Ohren
 */
static void drawHugoEars(void) 
{
  /* Ohren rechts und links */
  glPushMatrix();
    glTranslatef( 0.3,  0.1, 0.0);
    glScalef(0.1, 0.06, 1.0);
    drawCircle();
  glPopMatrix();
  glPushMatrix();
    glTranslatef( -0.3,  0.1, 0.0);
    glScalef(0.1, 0.06, 1.0);
    drawCircle();
  glPopMatrix();
}

/**
 * Zeichnet Hugos Augen
 */
static void drawHugoEyes(void) 
{
  /* Ohren rechts und links */
  glPushMatrix();
    glTranslatef( 0.2, 0.4, 0.0);
    glScalef(0.1, 0.05, 1.0);
    drawCircle();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(-0.2, 0.4, 0.0);
    glScalef(0.1, 0.05, 1.0);
    drawCircle();
  glPopMatrix();
}

/**
 * Zeichnet Hugos Schwanz
 */
static void drawHugoTail(void) 
{
  glPushMatrix();
    glScalef(0.1, 0.3, 1.0);
    glTranslatef( 0.0, -1.7, 0.0);
    drawCircle();
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Ground
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Hintergrund der Strasse
 * Dieser ist statisch als Hintergrund zur Szene
 */
extern void drawStreetBackground(void)
{
  /* Grünes Quadrat = Strassenrand
   * (Strassenbelag kommt in der drawStreet()) */
  glColor3f(0.3, 0.62, 0.0);
  drawSquare();
}

/**
 * Zeichnet den Strassenbelag
 * Dieser wird zusammen mit der Szene bewegt.
 */
extern void drawStreetSurface(void)
{  
  /* Zwei Spurstreifen zeichnen */
  
  /* Weißer Balken rechts */
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f(0.3, -0.3);
    glVertex2f(0.3,  0.3);
    glVertex2f(0.35,  0.3);
    glVertex2f(0.35, -0.3);
  glEnd();

  /* Weißer Balken rechts */
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f(-0.3, -0.3);
    glVertex2f(-0.3,  0.3);
    glVertex2f(-0.35,  0.3);
    glVertex2f(-0.35, -0.3);
  glEnd();
}

/**
 * Zeichnet den Hintergrund vom Fluss
 */
extern void drawRiverBackground(void)
{
  /* Blaues Wasser */
  glColor3f(0.3, 0.6, 0.6);
  drawSquare();
}

/**
 * Zeichnet die Wellen auf dem Fluss
 */
extern void drawRiverSurface(void)
{
  /* Zwei Wellen zeichnen */
  
  /* Wellen oben */
  glColor3f(0.78, 0.78, 0.78);
  glBegin(GL_LINE_STRIP);
    glVertex2f(-0.8,  0.1);
    glVertex2f(-0.4,  0.2);
		glVertex2f( 0.0,  0.1);
		glVertex2f( 0.4,  0.2);
		glVertex2f( 0.8,  0.1);
  glEnd();

  /* Wellen unten */
	glBegin(GL_LINE_STRIP);
    glVertex2f(-0.8, -0.2);
    glVertex2f(-0.4, -0.1);
		glVertex2f( 0.0, -0.2);
		glVertex2f( 0.4, -0.1);
		glVertex2f( 0.8, -0.2);
  glEnd();
}

/**
 * Zeichnet den Hintergrund des Beach
 */
extern void drawBeachBackground(void)
{
  /* Gelber Hintergrund */
  glColor3f(1.0, 0.9, 0.5);
  drawSquare();
}

/**
 * Zeichnet den Sand am Strand
 */
extern void drawBeachSurface(void)
{
  /* Ein paar Sandeigenschaften */
  
  /* Striche */
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
    glVertex2f( 0.1, 0.3);
    glVertex2f( 0.4, 0.5);

    glVertex2f( 0.5, 0.3);
    glVertex2f( 0.6, 0.2);

    glVertex2f(-0.4, -0.5);
    glVertex2f(-0.1, -0.3);
    
    glVertex2f(-0.6, -0.2);
    glVertex2f(-0.5, -0.3);
  glEnd();
  
  /* Punkte */
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_POINTS);
    glVertex2f( 0.7,  0.2);
    glVertex2f( 0.3, -0.2);
    glVertex2f(-0.7, -0.2);
    glVertex2f(-0.3,  0.2);
  glEnd();
}

/**
 * Zeichnet die Strasse mit den Begrenzungslinien
 */
extern void drawStreet(void)
{
  /* Dunkler Teer */
  glColor3f(0.4, 0.4, 0.4);
  drawSquare();
  
  /* Weißer Balken links */
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f(-1.0, 1.0);
    glVertex2f(-0.95, 1.0);
    glVertex2f(-0.95, -1.0);
    glVertex2f(-1.0, -1.0);
  glEnd();

  /* Weißer Balken rechts */
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.95, 1.0);
    glVertex2f(0.95, -1.0);
    glVertex2f(1.0, -1.0);
  glEnd();
}

/**
 * Zeichnet den Fluss
 */
extern void drawRiver(void)
{
  drawRiverBackground();
}

/**
 * Zeichnet den Strand
 */
extern void drawBeach(void)
{
  /* Gelb-oranger Sand */
  glColor3f(1.0, 0.9, 0.5);
  drawSquare();
}

/* ----------------------------------------------------------------------------
 * Environment
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet einen Baum im Ursprung
 */
extern void drawTree(void)
{
  /* Dunkles Grün */
  glColor3f(0.1, 0.55, 0.1);
  
  /* Es wird immer nur die Eckte rechts oben gezeichnet
   * aber dabei 4 mal um 90 Grad gedreht. */
	glPushMatrix();
    drawTreeSub();
    glRotatef(90, 0.0, 0.0, 1.0);
    drawTreeSub();
    glRotatef(90, 0.0, 0.0, 1.0);
    drawTreeSub();
    glRotatef(90, 0.0, 0.0, 1.0);
    drawTreeSub();
	glPopMatrix();
}

/**
 * Zeichnet eine Boje im Ursprung
 */
extern void drawBuoy(void)
{
  /* Es werden mehrere verschiedenfarbige Kreise
   * übereinander gezeichnet. Dazu noch eine Fahne.
   * Das Ergebniss sieht einer Boje "ähnlich". */
  
  /* Weißer Grundkreis */
  glColor3f(1.0, 1.0, 1.0);
  drawCircle();

  /* Roter Kreis */
  glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
    glScalef(0.88, 0.88, 0.88);
    drawCircle();
  glPopMatrix();

  /* Weißer Kreis */
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
    glScalef(0.7, 0.7, 0.7);
    drawCircle();
  glPopMatrix();

  /* Schwarzer Kreis */
  glColor3f(0.0, 0.0, 0.0);
  glPushMatrix();
    glScalef(0.4, 0.4, 0.4);
    drawCircle();
  glPopMatrix();
  
  /* Fahne in Blau */
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_LINE_STRIP);
    glVertex2f( 0.0, 0.0);
    glVertex2f( 0.1, 0.1);
    glVertex2f( 0.2, 0.15);
    glVertex2f( 0.3, 0.2);
    glVertex2f( 0.4, 0.2);
  glEnd();
}

/**
 * Zeichnet eine Palme im Ursprung
 */
extern void drawPalm(void)
{
  /* Es werden zuerst die Palmenwedel und dann die Kokosnüsse
   * in 72 Grad Abständen 5 mal gezeichnet. */
  
  /* Palmenwedel in dunklem Grün */
  glColor3f(0.1, 0.5, 0.1);
  glPushMatrix();
    drawPalmSub();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmSub();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmSub();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmSub();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmSub();
  glPopMatrix();

  /* Kokosnüsse in dunklem Braun */
  glColor3f(0.321, 0.0, 0.0);
  glPushMatrix();
    drawPalmCoconut();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmCoconut();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmCoconut();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmCoconut();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawPalmCoconut();
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Items
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet eine Münze im Ursprung
 */
extern void drawCoin(void)
{
  /* Orange Grundfläche */
  glColor3f(1.0, 0.5, 0.0);
  drawCircle();

  /* Hell-Gelber Innenraum */
  glColor3f(1.0, 1.0, 0.5);
  glPushMatrix();
    glScalef(0.9, 0.9, 0.9);
    drawCircle();
  glPopMatrix();
  
  /* Zahl in Orange (eine 1) */
  glColor3f(1.0, 0.5, 0.0);
  glBegin(GL_POLYGON);
    glVertex2f( 0.0, -0.6);
    glVertex2f( 0.0, 0.6);
    glVertex2f( 0.2, 0.6);
    glVertex2f( 0.2, -0.6);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex2f( 0.0, 0.45);
    glVertex2f(-0.3, 0.2);
    glVertex2f(-0.4, 0.35);
    glVertex2f( 0.0, 0.6);
  glEnd();
}

/**
 * Zeichnet einen Seestern im Ursprung
 */
extern void drawStarfish(void)
{
  /* 5 Seestern-Arme in Orange*/
  glColor3f(1.0, 0.352, 0.156);
  glPushMatrix();
    drawStarfishArm();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawStarfishArm();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawStarfishArm();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawStarfishArm();
    glRotatef(72, 0.0, 0.0, 1.0);
    drawStarfishArm();
  glPopMatrix();
}

/**
 * Zeichnet eine Muschel im Ursprung
 */
extern void drawShell(void)
{
  /* Halbkreis als Grundfläche */
  glColor3f(0.9, 0.8, 0.7);
  drawHalfCircle();
  
  /* Kleiner Halbkreis unten am Rand */
  glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glScalef(0.4, 0.4, 0.4);
    drawHalfCircle();
  glPopMatrix();
  
  /* Dreieck passend drüber legen */
  glBegin(GL_POLYGON);
    glVertex2f(-1.0,  0.0);
    glVertex2f( 1.0,  0.0);
    glVertex2f( 0.0, -1.0);
  glEnd();
  
  /* Mit ein paar Strichen das ganze aufhübschen */
  glColor3f(0.5, 0.6, 0.7);
  glBegin(GL_LINES);
    /* Strich links */
    glVertex2f( 0.0, -1.0);
    glVertex2f(-0.95, 0.4);

    glVertex2f( 0.0, -1.0);
    glVertex2f(-0.8,  0.5);

    glVertex2f( 0.0, -1.0);
    glVertex2f(-0.6,  0.6);

    glVertex2f( 0.0, -1.0);
    glVertex2f(-0.4,  0.7);

    glVertex2f( 0.0, -1.0);
    glVertex2f(-0.2,  0.8);
  
    /* Vertikaler Strich */
    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.0,  0.9);
    
    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.2,  0.8);

    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.4,  0.7);
    
    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.6,  0.6);
    
    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.8,  0.5);
    
    /* Strich rechts */
    glVertex2f( 0.0, -1.0);
    glVertex2f( 0.95, 0.4);
  glEnd();
}

/* ----------------------------------------------------------------------------
 * Obstacles
 * -------------------------------------------------------------------------- */
 
/**
 * Zeichnet ein (cooles) Auto im Ursprung
 */
extern void drawCar(void)
{  
  /* Reifenfarbe: Schwarz */
  glColor3f(0.0, 0.0, 0.0);
  
  /* Reifen hinten */
  glBegin(GL_POLYGON);
    glVertex2f( 0.65, 0.8);
    glVertex2f( 0.65, 0.5);
    glVertex2f(-0.65, 0.5);
    glVertex2f(-0.65, 0.8);
  glEnd();

  /* Reifen vorne */
  glBegin(GL_POLYGON);
    glVertex2f( 0.65, -0.5);
    glVertex2f( 0.65, -0.8);
    glVertex2f(-0.65, -0.8);
    glVertex2f(-0.65, -0.5);
  glEnd();

  /* Blau */
  glColor3f(0.0, 0.0, 1.0);
  
  /* Karosserie mit abgerundeten Ecken */
  glBegin(GL_POLYGON);
    /* Rechts oben */
    glVertex2f( 0.5,  1.0);
    glVertex2f( 0.6,  0.9);
    
    /* Rechts unten */
    glVertex2f( 0.6, -0.9);
    glVertex2f( 0.5, -1.0);
    
    /* Links unten */
    glVertex2f(-0.5, -1.0);
    glVertex2f(-0.6, -0.9);
    
    /* Links oben */
    glVertex2f(-0.6,  0.9);
    glVertex2f(-0.5,  1.0);
  glEnd();
  
  /* Rückspiegel rechts und links */
  glBegin(GL_POLYGON);
    glVertex2f( 0.6, -0.3);
    glVertex2f( 0.7, -0.3);
    glVertex2f( 0.6, -0.45);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex2f(-0.6, -0.3);
    glVertex2f(-0.6, -0.45);
    glVertex2f(-0.7, -0.3);
  glEnd();

  /* Scheinwerfer rechts und links */
  glColor3f(1.0, 1.0, 0.5);
  glBegin(GL_POLYGON);
    glVertex2f( 0.25, -1.00);
    glVertex2f( 0.30, -0.95);
    glVertex2f( 0.40, -0.95);
    glVertex2f( 0.45, -1.00);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex2f(-0.25, -1.00);
    glVertex2f(-0.30, -0.95);
    glVertex2f(-0.40, -0.95);
    glVertex2f(-0.45, -1.00);
  glEnd();

  /* Rückleuchte rechts und links */
  glColor3f(1.0, 0.2, 0.2);
  glBegin(GL_POLYGON);
    glVertex2f( 0.2,  1.0);
    glVertex2f( 0.4,  1.0);
    glVertex2f( 0.4,  0.9);
    glVertex2f( 0.2,  0.9);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex2f(-0.2,  1.0);
    glVertex2f(-0.4,  1.0);
    glVertex2f(-0.4,  0.9);
    glVertex2f(-0.2,  0.9);
  glEnd();

  /* Dach: helleres BLAU */
  glColor3f(0.5, 0.5, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f( 0.4,  0.7);
    glVertex2f( 0.4, -0.1);
    glVertex2f(-0.4, -0.1);
    glVertex2f(-0.4,  0.7);
  glEnd();

  /* Windschutzscheibe: Helles Grau */
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_POLYGON);
    glVertex2f( 0.4, -0.1);
    glVertex2f( 0.35, -0.4);
    glVertex2f(-0.35, -0.4);
    glVertex2f(-0.4, -0.1);
  glEnd();  
}

/**
 * Zeichnet ein Schiff im Ursprung 
 */
extern void drawShip(void)
{
  /* Helles Grau */
  glColor3f(0.95, 0.95, 0.95);
  
  /* Rumpf */
  glBegin(GL_POLYGON);
    /* Unterster Punkt */
    glVertex2f( 0.0, -1.0);
    
    /* Linke Seite rauf */
    glVertex2f(-0.15, -0.9);
    glVertex2f(-0.25, -0.7);
    glVertex2f(-0.4 , -0.4);
    glVertex2f(-0.6 ,  0.2);
    glVertex2f(-0.6 ,  0.7);
    glVertex2f(-0.5 ,  1.0);
    glVertex2f(-0.4 ,  1.0);
    glVertex2f(-0.2 ,  0.8);
    
    /* Rechte Seite runter */
    glVertex2f( 0.2 ,  0.8);
    glVertex2f( 0.4 ,  1.0);
    glVertex2f( 0.5 ,  1.0);
    glVertex2f( 0.6 ,  0.7);
    glVertex2f( 0.6 ,  0.2);
    glVertex2f( 0.4 , -0.4);
    glVertex2f( 0.25, -0.7);
    glVertex2f( 0.15, -0.9);
  glEnd();

  /* Dunkles Grau */
  glColor3f(0.3, 0.3, 0.3);

  /* Kajüte */
  glBegin(GL_LINE_STRIP);
  
    /* Linke Seite */
    glVertex2f(-0.4,  1.0);
    glVertex2f(-0.5,  0.6);
    glVertex2f(-0.4,  0.0);
    glVertex2f(-0.3, -0.3);
    
    /* Rechte Seite */
    glVertex2f( 0.3, -0.3);
    glVertex2f( 0.4,  0.0);
    glVertex2f( 0.5,  0.6);
    glVertex2f( 0.4,  1.0);
  glEnd();
  
  /* Windschutzscheibe */
  glBegin(GL_LINES);
    glVertex2f(-0.4,  0.0);
    glVertex2f( 0.4,  0.0);
    
    glVertex2f( 0.0,  0.0);
    glVertex2f( 0.0, -0.3);
  glEnd();

  /* Motor */
  glBegin(GL_POLYGON);
    glVertex2f(-0.1,  0.9);
    glVertex2f( 0.1,  0.9);
    glVertex2f( 0.1,  0.6);
    glVertex2f(-0.1,  0.6);
  glEnd();
}

/**
 * Zeichnet eine Krabbe im Ursprung
 */
extern void drawCrab(void)
{
  /* Orange */
  glColor3f(1.0, 0.3, 0.0);
  
  /* Körper */
  glPushMatrix();
    glTranslatef(0.0, 0.1, 0.0);
    glScalef(0.6, 0.5, 0.6);
    drawCircle();
  glPopMatrix();
  
  /* Seite links und rechts */
  glPushMatrix();
    drawCrabLeft();
    /* Spiegeln */
    glScalef(-1.0, 1.0, 1.0);
    drawCrabLeft();
  glPopMatrix();
  
  /* Augen */
  /* Weißer Teil */
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
    glTranslatef(-0.2, -0.3, 0.0);
    glScalef(0.12, 0.12, 1.0);
    drawCircle();
  glPopMatrix();
  glPushMatrix();
    glTranslatef( 0.2, -0.3, 0.0);
    glScalef(0.12, 0.12, 1.0);
    drawCircle();
  glPopMatrix();
  /* Schwarze Iris */
  glColor3f(0.1, 0.1, 0.1);
  glPushMatrix();
    glTranslatef(-0.2, -0.35, 0.0);
    glScalef(0.05, 0.05, 1.0);
    drawCircle();
  glPopMatrix();
  glPushMatrix();
    glTranslatef( 0.2, -0.35, 0.0);
    glScalef(0.05, 0.05, 1.0);
    drawCircle();
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Hugo
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet Hugo mit Strassenkleidung im Ursprung 
 */
extern void drawHugoStreetWear(void)
{
  /* Hose in Grün */
  glColor3f(0.0, 0.8, 0.0);
  glPushMatrix();
    glScalef(0.8, 0.8, 1.0);
    drawHugoBody();
  glPopMatrix();

  /* Körper in Hautfarbe */
  glColor3f(0.8, 0.6, 0.0);
  glPushMatrix();
    glScalef(0.7, 0.7, 1.0);
    drawHugoBody();
  glPopMatrix();
  
  /* Arme */
  drawHugoArms();
  
  /* Hosenträger in Grün */
  glColor3f(0.0, 0.8, 0.0);
  glBegin(GL_POLYGON);
    glVertex2f( -0.5,  0.0);
    glVertex2f( -0.2, -0.4);
    glVertex2f( -0.3, -0.4);
    glVertex2f( -0.6,  0.0);
    glVertex2f( -0.5,  0.4);
    glVertex2f( -0.4,  0.4);
  glEnd();

  /* Dunklere Körperfarbe */
  glColor3f(0.9, 0.7, 0.0);

  /* Kopf */
  drawHugoHead();
  
  /* Noch dunklere Körperfarbe */
  glColor3f(0.6, 0.3, 0.0);

  /* Ohren */
  drawHugoEars();
  
  /* Schwanz */
  drawHugoTail();
  
  /* Augen in Weiß */
  glColor3f(1.0, 1.0, 1.0);
  drawHugoEyes();
}

/**
 * Zeichnet Hugo mit Schwimmanzug im Ursprung 
 */
extern void drawHugoSwimmingSuit(void)
{
  /* Schwarz */
  glColor3f(0.0, 0.0, 0.0);
  
  /* Schwimmflossen */
  glBegin(GL_POLYGON);
    /* Rechts */
    glVertex2f( 0.4,  0.0);
    glVertex2f( 0.7, -0.8);
    glVertex2f( 0.1, -0.8);
  glEnd(); 
  glBegin(GL_POLYGON);
    /* Links */
    glVertex2f( -0.4,  0.0);
    glVertex2f( -0.7, -0.8);
    glVertex2f( -0.1, -0.8);
  glEnd(); 
  
  /* Badeanzug */
  glPushMatrix();
    glScalef(0.8, 0.8, 1.0);
    drawHugoBody();
  glPopMatrix();

  /* Körperfarbe */
  glColor3f(0.8, 0.6, 0.0);
  
  /* Arme */
  drawHugoArms();

  /* Dunklere Körperfarbe */
  glColor3f(0.9, 0.7, 0.0);

  /* Kopf */
  drawHugoHead();

  /* Noch dunklere Körperfarbe */
  glColor3f(0.6, 0.3, 0.0);

  /* Ohren */
  drawHugoEars();
  
  /* Schwanz */
  drawHugoTail();
  
  /* Taucherbrille in Grau */
  glColor3f(0.5, 0.5, 0.5);
  glPushMatrix();
    glScalef(1.2, 1.2, 1.0);
    drawHugoEyes();
  glPopMatrix();
}

/**
 * Zeichnet Hugo mit Strandkleidung im Ursprung 
 */
extern void drawHugoBermudaShorts(void)
{
  
  /* Hose in Gelb */
  glPushMatrix();
    glScalef(0.8, 0.8, 1.0);
    drawHugoBody();
  glPopMatrix();

  /* Hautfarbe */
  glColor3f(0.8, 0.6, 0.0);
  
  /* Körper */
  glColor3f(0.8, 0.6, 0.0);
  glPushMatrix();
    glScalef(0.7, 0.7, 1.0);
    drawHugoBody();
  glPopMatrix();
  
  /* Arme */
  drawHugoArms();

  /* Dunklere Körperfarbe */
  glColor3f(0.9, 0.7, 0.0);

  /* Kopf */
  drawHugoHead();

  /* Noch dunklere Körperfarbe */
  glColor3f(0.6, 0.3, 0.0);

  /* Ohren */
  drawHugoEars();
  
  /* Schwanz */
  drawHugoTail();
  
  /* Augen in Weiß */
  glColor3f(1.0, 1.0, 1.0);
  drawHugoEyes();
}
