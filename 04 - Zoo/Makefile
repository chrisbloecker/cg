# Objektdateien
OBJS             = $(SRCS:.c=.o)

# Compiler
CC               = gcc

# Precompiler flags
CPPFLAGS_LINUX   =
CPPFLAGS_LINUX64 =
CPPFLAGS_MACOSX  = -DMACOSX
CPPFLAGS_COMMON  = -I./
CPPFLAGS         = $(CPPFLAGS_COMMON) $(CPPFLAGS_$(OS))

# Compiler flags
CFLAGS_LINUX   =
CFLAGS_LINUX64 =
CFLAGS_MACOSX  =
CFLAGS_COMMON  = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -pg
CFLAGS         = $(CFLAGS_COMMON) $(CFLAGS_$(OS))

# Linker
LD               = gcc

# Linker flags
LDFLAGS_LINUX    = -L/usr/X11R6/lib -L/usr/lib
LDFLAGS_LINUX64  = -L/usr/X11R6/lib64
LDFLAGS_MACOSOX  =
LDFLAGS_COMMON   = -pg
LDFLAGS          = $(LDFLAGS_COMMON) $(LDFLAGS_$(OS))

# Linker libraries
XLIBS            = -lXmu -lXi -lX11
GLLIBS           = -lglut -lGLU -lGL

LDLIBS_LINUX     = $(GLLIBS) $(XLIBS)
LDLIBS_LINUX64   = $(GLLIBS) $(XLIBS)
LDLIBS_MACOSX    = -framework OpenGL -framework GLUT -framework Foundation
LDLIBS_COMMON    = -lm
LDLIBS           = $(LDLIBS_COMMON) $(LDLIBS_$(OS))

# welches Betriebssystem?
ifeq ($(findstring Linux,$(shell uname -s)),Linux)
  OS = LINUX
  ifeq ($(shell uname -m),x86_64)
    OS = LINUX64
  endif
else
  ifeq ($(shell uname -s),Darwin)
    OS = MACOSX
  endif
endif

# Debugging-Informationen aktivieren
DEBUG = yes

# Wenn Debugging-Informationen aktiviert werden sollen, entsprechende
# Praeprozessorflags setzen
ifeq ($(DEBUG),yes)
CPPFLAGS_COMMON+=-g -DDEBUG
endif

# zusaetzliche Abhaengigkeiten einbinden
-include Makefile.depend

# Quelldateien
SRCS             = main.c io.c logic.c vector.c scene.c level.c drawing.c material.c displaylist.c stringOutput.c texture.c picking.c

# ausfuehrbares Ziel
TARGET           = ueb04

# Pfad zum ImageLoader
IMGLOADER_DIR = ./imageLoader

# Name der ImageLoader-Bibliothek
IMGLOADER_LIB = libcgimage.a

# Ist libz verfügbar?
HAVE_LIBZ = yes

# Precompiler flags
# Pfad der Schnittstelle des ImageLoaders hinzufuegen
CPPFLAGS_COMMON  += -I$(IMGLOADER_DIR)/include

# Linker flags
# Pfad der ImageLoader-Bibliothek hinzufuegen
LDFLAGS_COMMON  += -L$(IMGLOADER_DIR)/lib

# Linker libraries
# ImageLoader-Bibliothek linken
LDLIBS += -lcgimage

# Wenn libz verfuegbar ist, muessen wir sie mit einlinken, um die
# Bibliothek auch zum Laden von PNG-Bildern verwenden zu koennen.
ifeq ($(HAVE_LIBZ),yes)
LDLIBS += -lz
endif


.SUFFIXES: .o .c
.PHONY: all clean distclean depend $(TARGET) distcleanimageloader

# TARGETS
all: depend $(TARGET) ueb04

doc:
	doxygen ../common/Doxyfile

# Linken des ausfuehrbaren Programms
$(TARGET): $(OBJS) $(IMGLOADER_DIR)/lib/$(IMGLOADER_LIB)
	$(LD) $(OBJS) $(LDLIBS) -o $(TARGET) $(LDFLAGS)

# Kompilieren der Objektdateien
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $*.o $*.c

# einfaches Aufraeumen
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

# alles loeschen, was erstellt wurde
distclean: clean
	rm -f *~
	rm -f Makefile.depend
	rm -rf doc

# Abhaengigkeiten automatisch ermitteln
depend:
	@rm -f Makefile.depend
	@echo -n "building Makefile.depend ... "
	@$(foreach SRC, $(SRCS), ( $(CC) $(CPPFLAGS) $(SRC) -MM -g0 ) 1>> Makefile.depend;)
	@echo "done"

# Regel zur Erstellung der ImageLoader-Bibliothek
$(IMGLOADER_DIR)/lib/$(IMGLOADER_LIB):
	(cd $(IMGLOADER_DIR) && ./configure)
	$(MAKE) -C $(IMGLOADER_DIR)

# Vollstaendiges Aufraeumen beinhaltet auch Aufraeumen des ImageLoaders
distclean: distcleanimageloader

# Regel zum Aufraeumen des ImageLoaders
distcleanimageloader:
	if [ -f $(IMGLOADER_DIR)/Makefile ]; then $(MAKE) -C $(IMGLOADER_DIR) HAVE_LIBZ=$(HAVE_LIBZ) distclean; fi
