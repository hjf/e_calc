INCLUDES = 


LIBS = 


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_PKG_DATA_DIR=\""$(pkgdatadir)"\"

	
AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = \
	e_calc

e_calc_SOURCES = \
	butterworth.c   \
	chebyshev.c     \
	capacitor.c     \
	e_calc.c        \
	inductor.c      \
	ohmslaw.c       \
	magloop.c       \
	match_network.c \
	reactance.c     \
	resister.c      \
    snr.c           \
    smith.c         \
	tank.c

e_calc_LDFLAGS = \
	-L../lib/.libs \
	-le_calc \
	-lm
 
