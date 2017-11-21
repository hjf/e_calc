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
	capacitor.c     \
	chebyshev.c     \
	current.c       \
	decibel.c       \
	distance.c      \
	frequency.c     \
	inductor.c      \
	io.c            \
	magloop.c       \
	match_network.c \
	power.c         \
	resister.c      \
	snr.c           \
	tank.c          \
    voltage.c

