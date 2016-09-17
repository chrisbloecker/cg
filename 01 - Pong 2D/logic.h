#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung des Balls sowie des
 * Schlägers.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ---- Eigene Header einbinden ---- */
#include "types.h"

/**
 * Gibt einen Zeiger auf den Schläger zurück.
 * Zeiger, damit nicht bei jedem Aufruf alle Werte kopiert werden.
 *
 * @return Zeiger auf den Schläger.
 */
Racket * getRacket(void);

/**
 * Gibt einen Zeiger auf den Ball zurück.
 * Zeiger, damit nicht bei jedem Aufruf alle Werte kopiert werden.
 *
 * @return Zeiger auf den Ball.
 */
Ball * getBall(void);

/**
 * Initialisiert den Spielstatus.
 */
void initGame(void);

/**
 * Initialisiert den Ball.
 * Dazu gehören
 *   - Position
 *   - Geschwindigkeit
 *   - Richtung
 */
void initBall(void);

/**
 * Initialisiert den Schläger.
 * Dazu gehören
 *   - Position
 *   - Auslenkungswinkel
 */
void initRacket(void);

/**
 * Erzeugt eine Farbe mit den Werten r, g, b und gibt einen Zeiger darauf zurück.
 *
 * @param[in] r Rotanteil.
 * @param[in] g Grünanteil.
 * @param[in] b Blauanteil.
 *
 * @return Zeiger auf die erzeugte Farbe.
 */
RGBColor * makeColor(double r, double g, double b);

/**
 * Berechnet neue Position von Ball und Schläger.
 *
 * @param[in] interval Dauer der Bewegung in Sekunden.
 */
void calcPositions(double interval);

/**
 * Berechnet die Bewegungsrichtung des Balls.
 * Diese ändert sich, wenn der Ball mit einem der Balken oder dem Schläger
 * zusammenstößt.
 */
void calcMovement(void);

/**
 * Setzt den Bewegunsstatus des Schlägers.
 *
 * @param[in] direction Bewegungsrichtung deren Status verändert werden soll.
 * @param[in] status    neuer Status der Bewegung.
 */
void setMovement(Direction direction, Boolean status);

#endif
