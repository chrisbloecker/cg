# --- <Variablen> ---

  # --- <Make> ---
    SHELL             = /bin/bash
  # --- </Make> ---

  # --- <Plug-in> ---
    # Der Dateiname des zu erstellenden Plug-ins
    PLUG_IN_TARGET     = edge_detection
    # Die Quelldateien des zu erstellenden Plug-ins
    PLUG_IN_SRCS       = edge_detection.c plugin.c gpc.c
    # Die Objektdateien des zu erstellenden Plug-ins
    PLUG_IN_OBJS       = $(PLUG_IN_SRCS:.c=.o)
  # --- </Plug-in> ---

  # --- <Preprocessor> ----
    # Optionen fuer den Preprocessor
    CPPFLAGS        = $(shell gimptool-2.0 --cflags-nogimpui) -DDEBUG -D_DEBUG
  # --- </Preprocessor> ---

  # --- <Compiler> ---
    # Compiler-Befehl
    CC              = gcc

    # Optionen fuer den Compiler
    CFLAGS          = -g -ansi -Wall -Wextra -Werror -Wno-unused-parameter
  # --- </Compiler> ---

  # --- <Linker> ---
    # Linker-Befehl
    LD              = gcc

    # Optionen fuer den Linker
    LDFLAGS         = $(shell gimptool-2.0 --libs-nogimpui) -lm
  # --- </Linker> ---

# --- </Variablen> ---

# --- <Targets> ---

.PHONY: all help install uninstall clean doc depend

# Das Standard-Target: Plug-in erstellen und installieren
all: $(PLUG_IN_TARGET) install

# zeigt Informationen zur Benutzung des Makefiles an
help:
	@echo "Optionen:"
	@echo "make all       - Plug-in erzeugen"
	@echo "make install   - Plug-in installieren"
	@echo "make uninstall - Plug-in deinstallieren"
	@echo "make clean     - Kompilierungsergebnisse loeschen"
	@echo "make doc       - HTML-Dokumentation erstellen"
	@echo "make depend    - Abhaengikeiten der Objektdateien von den \
Quellcode-Dateien ermitteln"
	@echo "make help   - Diese Hilfeseite anzeigen"

# Kompilieren der Quelldateien (xxx.c) zu Objektdateien (xxx.o)
%.o: %.c
	@echo
	@echo "  - Erzeuge $@ ..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	@echo "  -  ... Done"

# Linken des Plug-ins
$(PLUG_IN_TARGET): $(PLUG_IN_OBJS)
	@echo
	@echo "  - Linke Plug-in $@ ..."
	$(LD) -o $@ $(PLUG_IN_OBJS) $(LDFLAGS)
	@echo "  -  ... Done"

# Installieren
#   (setzt Schreibrechte im Plug-In-Installationsverzeichnis voraus)
install:
	gimptool-2.0 --install-bin $(PLUG_IN_TARGET)


# DeInstallieren
#   (setzt Schreibrechte im Plug-In-Installationsverzeichnis voraus)
uninstall:
	gimptool-2.0 --uninstall-bin $(PLUG_IN_TARGET)

# HTML-Dokumentation erstellen
doc:
	doxygen

# Aufraeumen
clean:
	@echo
	@echo "  - Loesche Objektdateien, Dokumentation und Programm..."
	rm -rf doc
	rm -f $(PLUG_IN_OBJS) $(PLUG_IN_TARGET) 
	rm -f *~ doxygen.log
	rm -f Makefile.depend
	@echo "  -  ... Done"

# zusaetzliche Abhaengigkeiten automatisch ermitteln
depend:
	@rm -f Makefile.depend
	@echo -n "building Makefile.depend ... "
	@$(foreach SRC, $(PLUG_IN_SRCS), ( $(CC) $(CPPFLAGS) $(SRC) -MM -g0 ) 1>> Makefile.depend;)
	@echo "done"

# --- </Targets> ---

# zusaetzliche Abhaengigkeiten einbinden
-include Makefile.depend
