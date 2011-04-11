DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
AM_CFLAGS = $(CFLAGS) $(SDL_CFLAGS)
AM_CXXFLAGS = $(CXXFLAGS) $(SDL_CFLAGS)
EXTRA_DIST = SDLMain.h SDLMain.m config.h.cmake
MAINTAINERCLEANFILES = Makefile.in config.h.in config.h.in~

bin_PROGRAMS = \
	hex-a-hop

hex_a_hop_SOURCES = \
	gettext.h \
	gfx.cpp \
	gfx_list.h \
	hex_puzzzle.cpp \
	i18n.cpp \
	i18n.h \
	level_list.h \
	menus.h \
	packfile.h \
	savestate.h \
	sfx.cpp \
	sfx.h \
	state.h \
	system-directory.c \
	system-directory.h \
	system-relative.c \
	system-relative.h \
	text.cpp \
	text.h \
	tiletypes.h \
	video.h

hex_a_hop_LDADD = \
	$(SDL_LIBS)
