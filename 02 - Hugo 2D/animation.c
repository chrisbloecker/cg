/**
 * @file
 *
 * Das Modul stellt Animationsfunktionen zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * System
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <math.h>

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * Eigene
 * -------------------------------------------------------------------------- */
#include "animation.h"

/**
 * Sorgt dafür, dass eine Art Hüpfen erzeugt wird.
 * Wird benutzt, damit sich die Baumkronen im Wind wiegen.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationBounce(double shift)
{
  glTranslatef(shift, 0.0, 0.0);
}

/**
 * Sorgt dafür, dass eine Art Schaukeln erzeugt wird.
 * Wird benutzt, damit die Bojen schaukeln.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationBlink(double shift)
{
  glScalef(1.0 - 3 * fabs(shift), 1.0 - 3 * fabs(shift), 0.0);
}

/**
 * Sorgt dafür, dass eine Rotation erzeugt wird.
 * Wird benutzt, damit sich die Palmen drehen.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationRotate(double shift)
{
  glRotatef(720 * shift, 0.0, 0.0, 1.0);
}
