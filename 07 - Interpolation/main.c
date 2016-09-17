/**
 * @file
 * Hauptprogramm.
 * Startet die IO-Verwaltung ...
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "logic.h"
#include "io.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
/* Fenstername */
#define WINDOW_NAME "Interpolation"

/* Fenstergröße */
#define WINDOW_SIZE_X (640)
#define WINDOW_SIZE_Y (480)

/**
 * Initialisierung und Starten der Ereignisbehandlung.
 *
 * @param[in] argc Anzahl der Kommandozeilenparameter.
 * @param[in] argv Kommandozeilenparameter.
 *
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int main(int argc, char * argv[])
{
  /* Funktion, die beim Beenden des Programms aufgerufen werden soll */
  atexit(logicCleanUp);
  
  /* Initialisierung des I/O-Sytems
     (inkl. Erzeugung des Fensters und Starten der Ereignisbehandlung). */
  if (!initAndStartIO(WINDOW_NAME, WINDOW_SIZE_X, WINDOW_SIZE_Y))
  {
    fprintf(stderr, "Initialisierung fehlgeschlagen!\n");
    return 1;
  }
  else
    return 0;
}
