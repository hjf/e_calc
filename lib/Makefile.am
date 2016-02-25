INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g
	 -m

lib_LTLIBRARIES = \
	libe_calc.la

libe_calc_la_SOURCES = \
	butterworth.c   \
	chebyshev.c     \
	frequency.c     \
	resister.c      \
	decibel.c       \
	capacitor.c     \
	inductor.c
