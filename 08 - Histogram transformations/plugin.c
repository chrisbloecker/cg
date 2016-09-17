/**
 * @file plugin.c Gimp-Integrations-Modul eines Plug-ins fuer GIMP 2.2 und
 * hoeher.
 */

/* Definitionen fuer Plug-in-Konstanten etc. */
#include "gpc.h"
#include "plugin.h"


/****************************************************************************
 * Makros
 ***************************************************************************/
/* Anzahl der Zeilen fuer Plugin-eingabefelder */
#define ROWS (1)
/* Anzahl der Spalten fuer Plugin-eingabefelder */
#define COLUMNS (2)

/* Anfangswert fuer Plugin-Eingabefeld "k" */
#define INIT_K (1.0)
/* Anfangswert fuer Plugin-Eingabefeld "h" */
#define INIT_H (0.0)
/* Maximalwert fuer die Eingabe eines "k"-Wertes */
#define MAX_K (255.0)
/* Minimalwert fuer die Eingabe eines "k"-Wertes */
#define MIN_K (0.00001)
/* Maximalwert fuer die Eingabe eines "h"-Wertes */
#define MAX_H (255.0)
/* Minmalwert fuer die Eingabe eines "h"-Wertes */
#define MIN_H (-256.0)


/****************************************************************************
 * Datentypen
 ***************************************************************************/
/**
 * Einstellungen fuer die Histogrammtransformation.
 */
typedef struct {
  /**
   * Histogrammtransformationen:
   * 0 = lineare Histogrammanpassung,
   * 1 = Aequilibrieren,
   * 2 = exponentielle Anpassung.
   */
  gint transformType;
  /** Alpha-Wert der exponentiellen Anpassung */
  gfloat alpha;
  /** Steigungs-Wert der linearen Anpassung */
  gfloat k;
  /** Verschiebung-Wert auf der y-Achse der linearen Anpassung */
  gfloat h;
} TransformData;

/****************************************************************************
 * Forward-Deklarationen
 ***************************************************************************/
/**
 * Query-Funktion - wird von GIMP aufgerufen, um die Eigenschaften des Plug-ins
 * abzufragen
 */
static void query (void);

/**
 * Run-Prozedur - regelt die Interaktion mit GIMP, wenn das Plug-in aufgerufen
 * wird, ruft die Funktion auf, die die eigentliche Arbeit macht.
 */
static void run (const gchar * name,
                 gint nparams,
                 const GimpParam * param,
                 gint * nreturn_vals, GimpParam ** return_vals);


/**
 * Zeigt den Einstellungsdialog fuer das Plug-in an.
 *
 * @param[in,out] transformType Einstellungen fuer die
 *                Transformations-Radio-Buttons.
 * @param[in,out] alpha Einstellungen fuer den Alpha-Wert-Schieberegler.
 * @param[in,out] k Einstellung für die Steigung der linearen Anpassung.
 * @param[in,out] h Einstellung für die y-Achsenabschnitt der linearen
 *                  Anpassung.
 *
 * @return TRUE = dialog closed with OK button, else FALSE
 */
static gboolean transformDialog (gint transformType[], gfloat * alpha, gfloat * k, gfloat * h);

/**
 * Callback, wird aufgerufen, wenn Einstellungsdialog mit OK beendet wird.
 */
static void transformOkCallback(GtkWidget * widget, gpointer data);

/**
 * Callback, wird aufgerufen, wenn Slider fuer Alpha-Wert geaendert wird.
 */
static void alphaSliderCallback (GtkAdjustment * adj, gfloat * alpha);

/****************************************************************************
 * Notwendige Dinge fuer GIMP-Plug-ins
 ***************************************************************************/

/** Informationen ueber PDB-Prozeduren des Plug-ins. */
GimpPlugInInfo PLUG_IN_INFO = {
  NULL,  /* init_proc */
  NULL,  /* quit_proc */
  query, /* query_proc */
  run,   /* run_proc */
};

MAIN ()


/****************************************************************************
 * Lokale Variablen
 ***************************************************************************/
/**
 * Der Rueckgabewert des Einstellungsdialoges.
 */
static gboolean dialogReturnValue = FALSE;


/****************************************************************************
 * Prozeduren
 ***************************************************************************/
static void
query ()
{
  /*
   * Schnittstelle zur PDB. Die ersten drei Zeilen sind fuer alle Plug-ins
   * noetig; die restlichen sind plug-in-spezifisch. Dieses Plug-in hat keine
   * Parameter. Die Namensstrings in der zweiten Spalte dienen der PDB zum
   * Identifizieren. Der Hilfetext in der dritten Spalte wird von vielen
   * Plug-in-Autoren ignoriert, kann aber sehr hilfreich sein, wenn man
   * GIMP-Skripte schreibt (z.B. script-fu) und nicht ueber die Quellen des
   * Plug-ins verfuegt.
   */
  static GimpParamDef args[] = {
    {GIMP_PDB_INT32,    "run_mode",      "Interactive, non-interactive"},
    {GIMP_PDB_IMAGE,    "image",         "Input image"},
    {GIMP_PDB_DRAWABLE, "drawable",      "Input drawable"},
    {GIMP_PDB_INT32,    "transformType", "Transformation type" },
    {GIMP_PDB_FLOAT,    "alpha",         "Alpha value" },
  };
  static int nArgs = G_N_ELEMENTS (args);

  /*
   * Werte fuer die Rueckmeldungen an GIMP.
   * vorerst: keine.
   */
  static GimpParamDef *returnVals = NULL;
  static int nReturnVals = 0;

  /*
   * Registriert das Plug-in bei der PDB.
   */
  gimp_install_procedure (PLUG_IN_NAME,
                          PLUG_IN_BLURB,
                          PLUG_IN_HELP,
                          PLUG_IN_AUTHOR,
                          PLUG_IN_COPYRIGHT,
                          PLUG_IN_COPYRIGHT_DATE,
                          PLUG_IN_MENU_ENTRY,
                          PLUG_IN_IMAGE_TYPES,
                          GIMP_PLUGIN, nArgs, nReturnVals, args, returnVals);
}

static void
run (const gchar * name,
     gint nparams,
     const GimpParam * param, gint * nreturn_vals, GimpParam ** return_vals)
{
  /* Drawable mit dem gerarbeitet wird */
  GimpDrawable *drawable;
  /* Modus in dem das Plug-in laeuft */
  GimpRunMode runMode;
  /* Status nach Ablauf des Plug-ins */
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  /* Rueckgabewerte */
  static GimpParam values[1];

  /* Speicher fuer die Plug-in-Daten */
  static TransformData pluginData;
  /* Histogrammtransformation:
     wenn Position 0 auf 1 gesetzt ist, ist der Typ lineare Anpassung,
     wenn Position 1 auf 1 gesetzt ist, ist der Typ Aequilibrieren,
     wenn Position 2 auf 1 gesetzt ist, ist der Typ exponentielle Anpassung. */
  gint transformType[] = {1,0,0};
  /* Alpha-Wert der exponentiellen Anpassung. */
  gfloat alpha = 1.0;
  /* Steigung der lin. Anpassung*/
  gfloat k = INIT_K;
  /* y-Achsenabschnitt der lin. Anpassung*/
  gfloat h = INIT_H;


  /* Drawable ermitteln */
  drawable = gimp_drawable_get (param[2].data.d_drawable);
  /* Modus ermitteln */
  runMode = param[0].data.d_int32;

  /* Initialisieren der Reuckgabewerte */
  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
  *nreturn_vals = 1;
  *return_vals = values;

  switch (runMode) {
    /* Plug-in laeuft interaktiv -> Dialog-Box anzeigen. */
    case GIMP_RUN_INTERACTIVE:
      if(!gimp_get_data (PLUG_IN_NAME, &pluginData)) {
        pluginData.transformType = 0;
        pluginData.alpha = 1.0;
        pluginData.k = INIT_K;
        pluginData.h = INIT_H;
      }

      transformType[0] = pluginData.transformType == 0;
      transformType[1] = pluginData.transformType == 1;
      transformType[2] = pluginData.transformType == 2;

      alpha = pluginData.alpha;


      if (!transformDialog (transformType, &alpha, &k, &h))
        return;

      if (transformType[0])      pluginData.transformType = 0;
      else if (transformType[1]) pluginData.transformType = 1;
      else if (transformType[2]) pluginData.transformType = 2;

      pluginData.alpha = alpha;
      pluginData.k = k;
      pluginData.h = h;

      break;

    /*
     * Wenn das Plug-in nicht interaktiv laeuft (moeglichweise skriptgesteuert)
     * kommen die Werte der Plug-in-parameter ueber das param[] array.
     * Sicherstellen, dass alle Parameter gueltige Werte haben.
     */
    case GIMP_RUN_NONINTERACTIVE:
      if (nparams != 7) {
        status = GIMP_PDB_CALLING_ERROR;
      } else {
        pluginData.transformType = param[3].data.d_int32;
        pluginData.alpha = param[4].data.d_float;
        pluginData.k = param[5].data.d_float;
        pluginData.h = param[6].data.d_float;
      }
      break;

    /*
     * Wenn das Plug-in mit den Werten des letzten Aufrufs laeuft, dann diese
     * Werte holen.
     */
    case GIMP_RUN_WITH_LAST_VALS:
      gimp_get_data (PLUG_IN_NAME, &pluginData);
      break;
  }

  if (status == GIMP_PDB_SUCCESS)
    {
      /* Auf geht es! */
      if (!filterDrawable (drawable,
                           pluginData.transformType,
                           pluginData.alpha, pluginData.k, pluginData.h))
        {
          /* Ein Fehler ist aufgetreten ... */
          status = GIMP_PDB_EXECUTION_ERROR;
        }
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      /* Wenn das das Plugin interaktiv laeuft, Darstellung aktualisieren. */
      if (runMode != GIMP_RUN_NONINTERACTIVE)
        {
          gimp_displays_flush ();
        }

      /* Wenn ein Einstellungsdialog angezeigt wurde, Daten sichern. */
      if (runMode == GIMP_RUN_INTERACTIVE)
        {
          gimp_set_data (PLUG_IN_NAME, &pluginData, sizeof(pluginData));
        }
    }

  /* Fertig!
   * Status setzen, sodass GIMP ihn sehen kann und Drawable freigegeben. */
  values[0].data.d_status = status;
  gimp_drawable_detach (drawable);
}

/****************************************************************************
 * GUI Prozeduren/Funktionen
 ***************************************************************************/
static void
transformOkCallback(GtkWidget * widget, gpointer data)
{
  dialogReturnValue = TRUE;
  gtk_widget_destroy (GTK_WIDGET(data));
}

static void
alphaSliderCallback (GtkAdjustment * adj, gfloat * alpha)
{
  *alpha = adj->value;
}

static void
spinButtonCallback (GtkAdjustment * adj, gfloat * coordinate)
{
  *coordinate = adj->value;
}

static gboolean
transformDialog(gint transformType[], gfloat * alpha, gfloat * k, gfloat * h)
{
  /* Verschiedene GUI Elemente */
  GtkWidget *dlg, *frame, *table, *sampleType_vbox, *alpha_hscale, *pointCoo_table, *spinButton;
  GSList *radio_group = NULL;
  GtkObject *alpha_adj;
  GtkAdjustment *spinner_adj;

  /* Laufvariablen und Feld mit Koordinaten um GUI zu initialisieren */
  gfloat * linVal[] = {k,h};
  gint i = 0, j = 0, index = 0;

  /* Parameter zum Initialisierung von GTK */
  gchar **argv;
  gint argc;

  /* Initialisierungen */
  dialogReturnValue = FALSE;

  argc = 1;
  argv = g_new(gchar *, 1);
  argv[0] = g_strdup("Histogram transformation");

  gtk_init(&argc, &argv);
  gtk_rc_parse(gimp_gtkrc());

  /* Dialogfenster oeffnen, Title und destroy callback setzen. */
  dlg = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dlg), "Histogram transformation");
  gtk_window_position(GTK_WINDOW(dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect(GTK_OBJECT(dlg), "destroy",
                     (GtkSignalFunc) gpc_close_callback, NULL);

  /* Basis-Container fuer radio buttons (inkl. Beschriftung), etc. */
  frame = gtk_frame_new("Parameter");
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
  gtk_container_border_width(GTK_CONTAINER(frame), 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), frame, TRUE, TRUE, 0);
  table = gtk_table_new(4, 2, FALSE);
  gtk_container_border_width(GTK_CONTAINER(table), 10);
  gtk_container_add(GTK_CONTAINER(frame), table);
  gtk_widget_show(table);
  gpc_setup_tooltips(table);

  /* OK & Cancel buttons hinzufuegen . */
  gpc_add_action_button("OK", (GtkSignalFunc) transformOkCallback, dlg,
                        "Accept parameter and apply histogram transformation");
  gpc_add_action_button("Cancel", (GtkSignalFunc) gpc_cancel_callback, dlg,
                        "Abort without applyin the histogram transformation");

  /* Beschriftung fuer Sample-Typ-Auswahl. */
  gpc_add_label("Interpolationsmethode:", table, 0, 1, 0, 1);

  /* Container fuer Sample-Typ-Auswahl. */
  sampleType_vbox = gtk_vbox_new(FALSE, 3);
  gtk_container_border_width(GTK_CONTAINER(sampleType_vbox), 5);
  gtk_table_attach(GTK_TABLE(table), sampleType_vbox, 1, 2, 0, 1,
                   GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

  /* Sample-Typ-Auswahl. */
  gpc_add_radio_button(&radio_group, "Linear adjustment", sampleType_vbox,
                       &(transformType[0]), "Linear adjustment");
  gpc_add_radio_button(&radio_group, "Equalize", sampleType_vbox,
                       &(transformType[1]), "Equalize");
  gpc_add_radio_button(&radio_group, "Exponential adjustment", sampleType_vbox,
                       &(transformType[2]), "Exponential adjustment");


  /* Beschriftung fuer Alpha-Wert-Auswahl. */
  gpc_add_label("Alpha:", table, 0, 1, 2, 3);

  /* Alpha-Wert-Auswahl. */
  alpha_adj = gtk_adjustment_new (*alpha, 0.0, 5.0, 1.0, 0.0, 0.0);
  gtk_signal_connect (GTK_OBJECT (alpha_adj), "value_changed",
                      GTK_SIGNAL_FUNC (alphaSliderCallback), alpha);
  alpha_hscale = gtk_hscale_new (GTK_ADJUSTMENT (alpha_adj));

  gtk_widget_set_usize (GTK_WIDGET (alpha_hscale), 200, 30);
  gtk_scale_set_digits (GTK_SCALE (alpha_hscale), 2);
  gtk_scale_set_value_pos (GTK_SCALE (alpha_hscale), GTK_POS_TOP);
  gtk_scale_set_draw_value (GTK_SCALE (alpha_hscale), TRUE);
  gtk_table_attach (GTK_TABLE (table), alpha_hscale, 1, 2, 2, 3,
                    GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

  /* Erzeugen der Anzeige fuer Einstellung der neuen Koordinaten */
  pointCoo_table = gtk_table_new(3,3,FALSE);
  gpc_add_label("Lineare Anpassungswerte:", table, 0, 1, 4, 5);

  for (i = 0; i < ROWS ; i++)
  {
    for (j = 0; j < COLUMNS; j++)
    {
      spinner_adj = (GtkAdjustment *) gtk_adjustment_new(*(linVal[index]), MIN_H, MAX_H, 1., 5., 5.);
      gtk_signal_connect (GTK_OBJECT (spinner_adj), "value_changed", GTK_SIGNAL_FUNC (spinButtonCallback), linVal[index]);
      spinButton = gtk_spin_button_new(spinner_adj, 1. , 0.);
      gtk_table_attach(GTK_TABLE(pointCoo_table), spinButton, j+1,j+2,i+1,i+2,GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);
      gtk_widget_show(spinButton);
      index++;
    }
  }

  /* Beschriftungen fuer Koordinateneinstellungen und Hinzufuegen zum Gesamtcontainer */
  gtk_container_border_width(GTK_CONTAINER(pointCoo_table), 5);
  gtk_table_attach(GTK_TABLE(table), pointCoo_table, 1, 2, 4, 5,GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

  gpc_add_label("k", pointCoo_table, 1, 2, 0, 1);
  gpc_add_label("h", pointCoo_table, 2, 3, 0, 1);
  gtk_widget_show(pointCoo_table);

  /* Alle anzeigen. */
  gtk_widget_show (alpha_hscale);
  gtk_widget_show(frame);
  gtk_widget_show(dlg);
  gtk_main();
  gdk_flush();

  return dialogReturnValue;
}
