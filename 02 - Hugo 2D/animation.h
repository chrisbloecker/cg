#ifndef __ANIMATION_H__
#define __ANIMATION_H__
/**
 * @file
 * Das Modul stellt Animationsfunktionen zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/**
 * Sorgt dafür, dass eine Art Hüpfen erzeugt wird.
 * Wird benutzt, damit sich die Baumkronen im Wind wiegen.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationBounce(double shift);

/**
 * Sorgt dafür, dass eine Art Schaukeln erzeugt wird.
 * Wird benutzt, damit die Bojen schaukeln.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationBlink(double shift);

/**
 * Sorgt dafür, dass eine Rotation erzeugt wird.
 * Wird benutzt, damit sich die Palmen drehen.
 *
 * @param[in] shift Auslenkung in x-Richtung.
 */
extern void animationRotate(double shift);

#endif
