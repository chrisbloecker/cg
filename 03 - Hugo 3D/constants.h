/** COLLISION benötigt DRAWING */
#ifdef COLLISION
#define DRAWING
#endif

/** LOGIC benötigt DRAWING */
#ifdef LOGIC
#define DRAWING
#endif

/** Konstanten zum Zeichnen */
#ifdef DRAWING
  /* Skalierung der Objekte */
  #define SCALE_X_GROUND      ( 0.6)
  #define SCALE_Y_GROUND      ( 1.0)
  #define SCALE_Z_GROUND      (10.0)

  #define SCALE_X_BACKGROUND  ( 5.0)
  #define SCALE_Y_BACKGROUND  ( 1.0)
  #define SCALE_Z_BACKGROUND  ( 5.0)

  #define SCALE_X_ENVIRONMENT (0.3)
  #define SCALE_Y_ENVIRONMENT (0.3)
  #define SCALE_Z_ENVIRONMENT (0.3)

  #define SCALE_X_ITEM        (0.1 )
  #define SCALE_Y_ITEM        (0.1 )
  #define SCALE_Z_ITEM        (0.1 )

  #define SCALE_X_OBSTACLE    (0.15)
  #define SCALE_Y_OBSTACLE    (0.15)
  #define SCALE_Z_OBSTACLE    (0.15)

  #define SCALE_X_HUGO        (0.15)
  #define SCALE_Y_HUGO        (0.15)
  #define SCALE_Z_HUGO        (0.15)

  /* Translation der Objekte */
  #define TRANSLATION_X_ENVIRONMENT (  0.8)  
  #define TRANSLATION_X_ITEM        (  0.4)
  #define TRANSLATION_X_OBSTACLE    (  0.4)
  #define TRANSLATION_Z_HUGO        (- 0.8)
  #define TRANSLATION_X_BUILDING    (  2.5)
  #define TRANSLATION_Z_BUILDING    (- 5.5)
#endif

/** Konstanten für die Kollisionsberechnung */
#ifdef COLLISION
  /* Kollisionsdistanzen zu Gegenständen */
  #define COLLISION_DISTANCE_COIN     (SCALE_X_HUGO + SCALE_X_ITEM)
  #define COLLISION_DISTANCE_STARFISH (SCALE_X_HUGO + SCALE_X_ITEM)
  #define COLLISION_DISTANCE_SHELL    (SCALE_X_HUGO + SCALE_X_ITEM)

  /* Kollisionsdistanzen zu Hindernissen */
  #define COLLISION_DISTANCE_CAR      (SCALE_X_HUGO)
  #define COLLISION_DISTANCE_SHIP     (SCALE_X_HUGO)
  #define COLLISION_DISTANCE_CRAB     (SCALE_X_HUGO + 0.5 * SCALE_X_OBSTACLE)
#endif
