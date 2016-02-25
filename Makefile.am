pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = e_calc.pc

SUBDIRS = lib src

e_caldocdir = ${prefix}/doc/e_cal
e_caldoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(e_caldoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done
