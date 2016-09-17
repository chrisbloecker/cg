/**
 * @file plugin.c Gimp-Integrations-Modul eines Plug-ins fuer GIMP 2.2 und
 * hoeher.
 */

/* Definitionen fuer Plug-in-Konstanten etc. */
#include "gpc.h"
#include "plugin.h"

/****************************************************************************
 * Datentypen
 ***************************************************************************/
/**
 * Einstellungen fuer den Kanten-Filter.
 */
typedef struct {
  /**
   * Filter-Operation:
   * 0 = Sobel,
   * 1 = Mexican-Hat
   */
  gint filterType;
  /**
   * Modus der Randbehandlung:
   * 0 = Konstante Hintergrundfarbe,
   * 1 = Konstante Fortsetzung,
   * 2 = Periodische Fortsetzung.
   */
  gint borderMode;
} FilterData;

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
 * @param[in] filterType Einstellungen fuer die filterType-Radio-Buttons.
 * @param[in] borderMode Einstellungen fuer die borderMode-Radio-Buttons.
 *
 * @return TRUE = dialog closed with OK button, else FALSE
 */
static gboolean filterDialog (gint filterType[],
                              gint borderMode[]);

/**
 * Callback, wird aufgerufen, wenn Einstellungsdialog mit OK beendet wird.
 */
static void filterOkCallback(GtkWidget * widget, gpointer data);

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
    {GIMP_PDB_INT32,    "run_mode",   "Interactive, non-interactive"},
    {GIMP_PDB_IMAGE,    "image",      "Input image"},
    {GIMP_PDB_DRAWABLE, "drawable",   "Input drawable"},
    {GIMP_PDB_INT32,    "filterType", "Filter Type" },
    {GIMP_PDB_INT32,    "borderMode",   "Border Mode" },
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
  static FilterData pluginData;
  /* Filter-Typ:
     wenn Position 0 auf 1 gesetzt ist, ist der Typ Sobel X,
     wenn Position 1 auf 1 gesetzt ist, ist der Typ Sobel Y,
     wenn Position 2 auf 1 gesetzt ist, ist der Typ Sobel Kombiniert.
     wenn Position 2 auf 1 gesetzt ist, ist der Typ Mexican-Hat */
  gint filterType[] = {1,0,0,0};
  /* Modus der Randbehandlung des Kernels:
     wenn Position 0 auf 1 gesetzt ist, ist der Modus konst. Hingergrund,
     wenn Position 1 auf 1 gesetzt ist, ist der Modus konst. Fortsetzung,
     wenn Position 2 auf 1 gesetzt ist, ist der Modus period. Fortsetzung. */
  gint borderMode[] = {1,0,0};

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
        pluginData.filterType = 0;
        pluginData.borderMode = 0;
      }

      filterType[0] = pluginData.filterType == 0;
      filterType[1] = pluginData.filterType == 1;
      filterType[2] = pluginData.filterType == 2;
      filterType[3] = pluginData.filterType == 3;

      borderMode[0] = pluginData.borderMode == 0;
      borderMode[1] = pluginData.borderMode == 1;
      borderMode[2] = pluginData.borderMode == 2;

      if (!filterDialog (filterType, borderMode))
        return;

      if (filterType[0])      pluginData.filterType = 0;
      else if (filterType[1]) pluginData.filterType = 1;
      else if (filterType[2]) pluginData.filterType = 2;
      else if (filterType[3]) pluginData.filterType = 3;

      if (borderMode[0])      pluginData.borderMode = 0;
      else if (borderMode[1]) pluginData.borderMode = 1;
      else if (borderMode[2]) pluginData.borderMode = 2;

      break;

    /*
     * Wenn das Plug-in nicht interaktiv laeuft (moeglichweise skriptgesteuert)
     * kommen die Werte der Plug-in-parameter ueber das param[] array.
     * Sicherstellen, dass alle Parameter gueltige Werte haben.
     */
    case GIMP_RUN_NONINTERACTIVE:
      if (nparams != 5) {
        status = GIMP_PDB_CALLING_ERROR;
      } else {
        pluginData.filterType = param[5].data.d_int32;
        pluginData.borderMode = param[5].data.d_int32;
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
                           pluginData.filterType,
                           pluginData.borderMode))
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
filterOkCallback(GtkWidget * widget, gpointer data)
{
  dialogReturnValue = TRUE;
  gtk_widget_destroy (GTK_WIDGET(data));
}

static gboolean
filterDialog(gint filterType[], gint borderMode[])
{
  /* Verschiedene GUI Elemente */
  GtkWidget *dlg, *frame, *table, *borderMode_vbox, *filterType_vbox;
  GSList *radio_groups[] = {NULL, NULL};

  /* Parameter zum Initialisierung von GTK */
  gchar **argv;
  gint argc;

  /* Initialisierungen */
  dialogReturnValue = FALSE;

  argc = 1;
  argv = g_new(gchar *, 1);
  argv[0] = g_strdup("Kantenfilter");

  gtk_init(&argc, &argv);
  gtk_rc_parse(gimp_gtkrc());

  /* Dialogfenster oeffnen, Title und destroy callback setzen. */
  dlg = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dlg), "Kantenfilter");
  gtk_window_position(GTK_WINDOW(dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect(GTK_OBJECT(dlg), "destroy",
                     (GtkSignalFunc) gpc_close_callback, NULL);

  /* Basis-Container fuer radio buttons (inkl. Beschriftung), etc. */
  frame = gtk_frame_new("Einstellungen");
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
  gtk_container_border_width(GTK_CONTAINER(frame), 10);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), frame, TRUE, TRUE, 0);
  table = gtk_table_new(4, 2, FALSE);
  gtk_container_border_width(GTK_CONTAINER(table), 10);
  gtk_container_add(GTK_CONTAINER(frame), table);
  gtk_widget_show(table);
  gpc_setup_tooltips(table);

  /* OK & Cancel buttons hinzufuegen . */
  gpc_add_action_button("OK", (GtkSignalFunc) filterOkCallback, dlg,
                        "Einstellungen akzeptieren und Filter anwenden");
  gpc_add_action_button("Cancel", (GtkSignalFunc) gpc_cancel_callback, dlg,
                        "Dialog beenden ohne Filter anzuwenden");


  /* Beschriftung fuer Filter-Typ-Auswahl. */
  gpc_add_label("Filter-Typ:", table, 0, 1, 2, 3);

  /* Container fuer Filter-Typ-Auswahl. */
  filterType_vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_border_width(GTK_CONTAINER(filterType_vbox), 5);
  gtk_table_attach(GTK_TABLE(table), filterType_vbox, 1, 2, 2, 3,
                   GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

  /* Filter-Typ-Auswahl. */
  gpc_add_radio_button(&radio_groups[0], "Sobel X", filterType_vbox,
                       &(filterType[0]), "Sobel X");
  gpc_add_radio_button(&radio_groups[0], "Sobel Y", filterType_vbox,
                       &(filterType[1]), "Sobel Y");
  gpc_add_radio_button(&radio_groups[0], "Sobel Kombiniert", filterType_vbox,
                       &(filterType[2]), "Sobel Kombiniert");
  gpc_add_radio_button(&radio_groups[0], "Mexican-Hat", filterType_vbox,
                       &(filterType[3]), "Mexican-Hat");


  /* Beschriftung fuer Randbehandlungs-Modus-Auswahl. */
  gpc_add_label("Randbehandlung:", table, 0, 1, 4, 5);

  /* Container fuer Randbehandlungs-Modus-Auswahl. */
  borderMode_vbox = gtk_vbox_new(FALSE, 3);
  gtk_container_border_width(GTK_CONTAINER(borderMode_vbox), 5);
  gtk_table_attach(GTK_TABLE(table), borderMode_vbox, 1, 2, 4, 5,
                   GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

  /* Randbehandlungs-Modus-Auswahl. */
  gpc_add_radio_button(&radio_groups[1], "Konstante Hintergrundfarbe", borderMode_vbox,
                       &(borderMode[0]), "Konstante Hintergrundfarbe");
  gpc_add_radio_button(&radio_groups[1], "Konstante Fortsetzung", borderMode_vbox,
                       &(borderMode[1]), "Konstante Fortsetzung");
  gpc_add_radio_button(&radio_groups[1], "Periodische Fortsetzung", borderMode_vbox,
                       &(borderMode[2]), "Periodische Fortsetzung");


  /* Alle anzeigen. */
  gtk_widget_show(frame);
  gtk_widget_show(dlg);
  gtk_main();
  gdk_flush();

  return dialogReturnValue;
}
