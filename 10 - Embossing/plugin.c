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

/*Maximalwerte fuer Eingabe in die Spinbuttons*/
#define MAX_RANGE (360.0)
/* Anzahl der Zeilen fuer Plugin-eingabefelder */
#define ROWS (2)
/* Anzahl der Spalten fuer Plugin-eingabefelder */
#define COLUMNS (1)
/* Initiale Werte fuer die Eingabefelder */
#define INIT_AZIMUTH (0.0)
#define INIT_HEIGHT (20.0)

#define INIT_EMBOSS (1) /* 1 == Emboss; 0 == Pencil Sketch */

#define INIT_ALPHA (1.0)

/* Minmale und maximale Wert fuer die Eingabe im GUI-Dialogs */
#define MIN_AZIMUTH (0.0)
#define MAX_AZIMUTH (360.0)

#define MIN_HEIGHT (0.0)
#define MAX_HEIGHT (90.0)

#define MIN_ALPHA (0.0)
#define MAX_ALPHA (1.0)

/****************************************************************************
 * Datentypen
 ***************************************************************************/
/* Werte fuer Pencil Sketch und Emboss */
enum
{
  FUNCTION_PENCIL = 0, FUNCTION_EMBOSS = 1
};

/**
 * Einstellungen fuer den Emboss-Filter.
 */
typedef struct
{
  /**
   * Lichtwinkel definiert durch 2 Winkel(Azimuth und height)
   * siehe Kugelkoordinaten
   * Emboss oder Pencil Sketch
   */
  gfloat azimuth;
  gfloat elevation;
  gfloat alpha;
  gint32 emboss;

} MyEmbossData;

/****************************************************************************
 * Forward-Deklarationen
 ***************************************************************************/
/**
 * Query-Funktion - wird von GIMP aufgerufen, um die Eigenschaften des Plug-ins
 * abzufragen
 */
static void
query(void);

/**
 * Run-Prozedur - regelt die Interaktion mit GIMP, wenn das Plug-in aufgerufen
 * wird, ruft die Funktion auf, die die eigentliche Arbeit macht.
 */
static void
run(const gchar * name, gint nparams, const GimpParam * param,
    gint * nreturn_vals, GimpParam ** return_vals);

/**
 * Zeigt den Einstellungsdialog fuer das Plug-in an.
 *
 * @param[in] azimuth Einstellungen fuer das Eingabefeld des Azimuths.
 * @param[in] elevation Einstellungen fuer das Eingabefeld der Elevation.
 * @param[in] alpha Einstellungen fuer das Eingabefeld Blendfaktors.
 * @param[in] emboss Einstellungen fuer die Radio-Buttons zur Auswahl des
 *                   Filters (emboss oder pencil-sketch).
 *
 * @return TRUE = dialog closed with OK button, else FALSE
 */
static gboolean
MyEmbossDialog(gfloat * azimuth, gfloat * elevation, gfloat * alpha,
    gint * emboss);

/**
 * Callback, wird aufgerufen, wenn Einstellungsdialog mit OK beendet wird.
 */
static void
transformOkCallback(GtkWidget * widget, gpointer data);

/****************************************************************************
 * Notwendige Dinge fuer GIMP-Plug-ins
 ***************************************************************************/

/** Informationen ueber PDB-Prozeduren des Plug-ins. */
GimpPlugInInfo PLUG_IN_INFO =
  { NULL, /* init_proc */
  NULL, /* quit_proc */
  query, /* query_proc */
  run, /* run_proc */
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
query()
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
  static GimpParamDef args[] =
    {
      { GIMP_PDB_INT32, "run_mode", "Interactive, non-interactive" },
      { GIMP_PDB_IMAGE, "image", "Input image" },
      { GIMP_PDB_DRAWABLE, "drawable", "Input drawable" },
      { GIMP_PDB_FLOAT, "azimuth", "horizontaler Winkel des Lichtes" },
      { GIMP_PDB_FLOAT, "height", "Vertikaler Winkel des Lichtes" },
      { GIMP_PDB_FLOAT, "alpha", "Alpha-Wert fuer Pencil-Sketch-Filter" },
      { GIMP_PDB_INT32, "emboss_or_pencil", "Emboss oder Pencil Scetch" } };
  static int nArgs = G_N_ELEMENTS(args);

  /*
   * Werte fuer die Rueckmeldungen an GIMP.
   * vorerst: keine.
   */
  static GimpParamDef *returnVals = NULL;
  static int nReturnVals = 0;

  /*
   * Registriert das Plug-in bei der PDB.
   */
  gimp_install_procedure(PLUG_IN_NAME, PLUG_IN_BLURB, PLUG_IN_HELP,
      PLUG_IN_AUTHOR, PLUG_IN_COPYRIGHT, PLUG_IN_COPYRIGHT_DATE,
      PLUG_IN_MENU_ENTRY, PLUG_IN_IMAGE_TYPES, GIMP_PLUGIN, nArgs, nReturnVals,
      args, returnVals);
}

static void
run(const gchar * name, gint nparams, const GimpParam * param,
    gint * nreturn_vals, GimpParam ** return_vals)
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
  static MyEmbossData pluginData;

  gfloat azim = INIT_AZIMUTH;
  gfloat elev = INIT_HEIGHT;
  gfloat alp = INIT_ALPHA;
  gint32 emb = INIT_EMBOSS;

  /* Drawable ermitteln */
  drawable = gimp_drawable_get(param[2].data.d_drawable);
  /* Modus ermitteln */
  runMode = param[0].data.d_int32;

  /* Initialisieren der Reuckgabewerte */
  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
  *nreturn_vals = 1;
  *return_vals = values;

  /*Auswahl des Arbeitsmodus */
  switch (runMode)
    {
  /* Plug-in laeuft interaktiv -> Dialog-Box anzeigen. */
  case GIMP_RUN_INTERACTIVE:
    if (!gimp_get_data(PLUG_IN_NAME, &pluginData))
      {
        pluginData.azimuth = INIT_AZIMUTH;
        pluginData.elevation = INIT_HEIGHT;
        pluginData.alpha = INIT_ALPHA;
        pluginData.emboss = INIT_EMBOSS;
      }
    azim = pluginData.azimuth;
    elev = pluginData.elevation;
    alp = pluginData.alpha;
    emb = pluginData.emboss;

    if (!MyEmbossDialog(&azim, &elev, &alp, &emb))
      return;

    pluginData.azimuth = azim;
    pluginData.elevation = elev;
    pluginData.alpha = alp;
    pluginData.emboss = emb;

    break;

    /*
     * Wenn das Plug-in nicht interaktiv laeuft (moeglichweise skriptgesteuert)
     * kommen die Werte der Plug-in-parameter ueber das param[] array.
     * Sicherstellen, dass alle Parameter gueltige Werte haben.
     */
  case GIMP_RUN_NONINTERACTIVE:
    if (nparams != 4)
      {
        status = GIMP_PDB_CALLING_ERROR;
      }
    else
      {
        pluginData.azimuth = param[3].data.d_float;
        pluginData.elevation = param[4].data.d_float;
        pluginData.alpha = param[5].data.d_float;
        pluginData.emboss = param[6].data.d_float;
      }
    break;

    /*
     * Wenn das Plug-in mit den Werten des letzten Aufrufs laeuft, dann diese
     * Werte holen.
     */
  case GIMP_RUN_WITH_LAST_VALS:
    gimp_get_data(PLUG_IN_NAME, &pluginData);
    break;
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      /* Auf geht es! */
      if (!filterDrawable (drawable, pluginData.azimuth, pluginData.elevation,
          pluginData.alpha, pluginData.emboss))
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
          gimp_displays_flush();
        }

      /* Wenn ein Einstellungsdialog angezeigt wurde, Daten sichern. */
      if (runMode == GIMP_RUN_INTERACTIVE)
        {
          gimp_set_data(PLUG_IN_NAME, &pluginData, sizeof(pluginData));
        }
    }
  /* Fertig!
   * Status setzen, sodass GIMP ihn sehen kann und Drawable freigegeben. */
  values[0].data.d_status = status;
  gimp_drawable_detach(drawable);
}

/****************************************************************************
 * GUI Prozeduren/Funktionen
 ***************************************************************************/
static void
transformOkCallback(GtkWidget * widget, gpointer data)
{
  dialogReturnValue = TRUE;
  gtk_widget_destroy(GTK_WIDGET(data));
}

static void
doubleSpinButtonCallback(GtkAdjustment * adj, gfloat * coordinate)
{
  *coordinate = adj->value;
}

/*static void
 intSpinButtonCallback(GtkAdjustment * adj, gint * coordinate)
 {
 *coordinate = adj->value;
 }*/

static gboolean
MyEmbossDialog(gfloat * azimuth, gfloat * elevation, gfloat * alpha,
    gint * emboss)
{
  /* Verschiedene GUI Elemente */
  GtkWidget *dlg, *frame, *table, *pointCoo_table, *spinButton;
  GtkAdjustment *spinner_adj;
  GSList * group = NULL;
  GtkWidget *box1;
  GtkWidget *separator;
  GtkWidget *table2;
  GtkWidget *table3;
  GtkWidget *separator2;
  GtkWidget *table4;
  GtkWidget *table5;

  gint pencil[1];

  /* Laufvariablen und Feld mit Koordinaten um GUI zu initialisieren */
  gfloat * linVal[] =
    { azimuth, elevation };

  gfloat bounds[] =
    { MIN_AZIMUTH, MAX_AZIMUTH, MIN_HEIGHT, MAX_HEIGHT, MIN_ALPHA, MAX_ALPHA };

  gint i = 0, j = 0, index = 0;

  /* Parameter zum Initialisierung von GTK */
  gchar **argv;
  gint argc;

  /* Initialisierungen */
  dialogReturnValue = FALSE;

  argc = 1;
  argv = g_new(gchar *, 1);
  argv[0] = g_strdup("Emboss");

  gtk_init(&argc, &argv);
  gtk_rc_parse(gimp_gtkrc());

  /* Dialogfenster oeffnen, Title und destroy callback setzen. */
  dlg = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dlg), "Emboss und Pencil Sketch");
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
  gpc_add_action_button("OK", (GtkSignalFunc) transformOkCallback, dlg,
      "Accept parameter and apply histogram transformation");
  gpc_add_action_button("Cancel", (GtkSignalFunc) gpc_cancel_callback, dlg,
      "Abort without applyin the histogram transformation");

  /* Radiobuttons */
  table2 = gtk_table_new(1, 2, FALSE);
  gtk_table_attach(GTK_TABLE(table), table2, 0, 2, 0, 1, GTK_FILL | GTK_EXPAND,
      GTK_FILL, 5, 0);
  gtk_widget_show(table2);

  separator = gtk_hseparator_new();
  gtk_table_attach(GTK_TABLE(table), separator, 0, 2, 1, 2, GTK_FILL
      | GTK_EXPAND, GTK_FILL, 5, 5);
  gtk_widget_show(separator);


  table3 = gtk_table_new(1, 2, FALSE);
  gtk_table_attach(GTK_TABLE(table2), table3, 1, 2, 0, 1,
      GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 10);
  gtk_widget_show(table3);

  box1 = gtk_vbox_new(FALSE, 3);
  gtk_table_attach(GTK_TABLE(table3), box1, 1, 2, 0, 1, GTK_FILL | GTK_EXPAND,
      GTK_FILL, 5, 0);
  gtk_widget_show(box1);


  gpc_add_radio_button(&group, "Pencil Sketch", box1, pencil, "Pencil Sketch");

  gpc_add_radio_button(&group, "Emboss", box1, emboss, "Emboss");

  gpc_add_label("Filterauswahl:", table2, 0, 1, 0, 1);

  /* Erzeugen der Anzeige fuer Einstellung der Emboss-Parameter */
  pointCoo_table = gtk_table_new(2, 2, FALSE);
  gpc_add_label("Emboss:", table, 0, 1, 2, 3);

  for (i = 0; i < ROWS; i++)
    {
      for (j = 0; j < COLUMNS; j++)
        {
          /*gtk_adjustment_new  (gdouble value, gdouble lower, gdouble upper, gdouble step_increment,  gdouble page_increment, gdouble page_size);*/
          spinner_adj = (GtkAdjustment *) gtk_adjustment_new(*(linVal[index]),
              bounds[i * 2], bounds[i * 2 + 1], 0.1, 1., 0.);
          gtk_signal_connect(GTK_OBJECT(spinner_adj), "value_changed",
              GTK_SIGNAL_FUNC(doubleSpinButtonCallback), linVal[index]);
          /* gtk_spin_button_new (GtkAdjustment *adjustment, gdouble climb_rate, guint digits); */
          spinButton = gtk_spin_button_new(spinner_adj, 1.0, 1);

          /* gtk_table_attach (GtkTable *table, GtkWidget *child,  guint left_attach,
           *  guint right_attach, guint top_attach,guint bottom_attach,
           *   GtkAttachOptions xoptions,GtkAttachOptions yoptions, guint xpadding,
           *   guint ypadding); */
          gtk_table_attach(GTK_TABLE(pointCoo_table), spinButton, j + 1, j + 2,
              i, i + 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);
          gtk_widget_show(spinButton);
          index++;
        }
    }

  table4 = gtk_table_new(2, 2, FALSE);
  gtk_table_attach(GTK_TABLE(table), table4, 0, 2, 3, 4,
      GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 5);
  gtk_widget_show(table4);

  separator2 = gtk_hseparator_new();
  gtk_table_attach(GTK_TABLE(table4), separator2, 0, 2, 0, 1, GTK_FILL
      | GTK_EXPAND, GTK_FILL, 5, 10);
  gtk_widget_show(separator2);

  table5 = gtk_table_new(1, 2, FALSE);
  gtk_table_attach(GTK_TABLE(table4), table5, 1, 2, 1, 2,
      GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 5);
  gtk_widget_show(table5);


  /*gtk_adjustment_new  (gdouble value, gdouble lower, gdouble upper, gdouble step_increment,  gdouble page_increment, gdouble page_size);*/
  spinner_adj = (GtkAdjustment *) gtk_adjustment_new(*alpha, bounds[4],
      bounds[5], 0.01, 1., 0.);
  gtk_signal_connect(GTK_OBJECT(spinner_adj), "value_changed", GTK_SIGNAL_FUNC(
      doubleSpinButtonCallback), alpha);
  /* gtk_spin_button_new (GtkAdjustment *adjustment, gdouble climb_rate, guint digits); */
  spinButton = gtk_spin_button_new(spinner_adj, 1.0, 2);

  /* gtk_table_attach (GtkTable *table, GtkWidget *child,  guint left_attach,
   *  guint right_attach, guint top_attach,guint bottom_attach,
   *   GtkAttachOptions xoptions,GtkAttachOptions yoptions, guint xpadding,
   *   guint ypadding); */
  gtk_table_attach(GTK_TABLE(table5), spinButton, 1, 2, 0, 1, GTK_FILL
      | GTK_EXPAND, GTK_FILL, 5, 0);
  gtk_widget_show(spinButton);

  gpc_add_label("Alpha", table5, 0, 1, 0, 1);

  gpc_add_label("Pencil-Sketch:", table4, 0, 1, 1, 2);


  /* Beschriftungen fuer Koordinateneinstellungen und Hinzufuegen zum Gesamtcontainer */
  gtk_container_border_width(GTK_CONTAINER(pointCoo_table), 5);
  gtk_table_attach(GTK_TABLE(table), pointCoo_table, 1, 2, 2, 3, GTK_FILL
      | GTK_EXPAND, GTK_FILL, 5, 0);

  /*gpc_add_label(char *value, GtkWidget *parent, int left, int right, int top, int bottom);*/
  gpc_add_label("Azimuth", pointCoo_table, 0, 1, 0, 1);
  gpc_add_label("Hoehenwinkel", pointCoo_table, 0, 1, 1, 2);
  gtk_widget_show(pointCoo_table);

  /* Alle anzeigen. */
  gtk_widget_show(frame);
  gtk_widget_show(dlg);
  gtk_main();
  gdk_flush();

  return dialogReturnValue;
}

