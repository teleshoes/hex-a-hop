AC_PREREQ([2.58])
AC_INIT([Hex-a-hop],[1.2.0],[http://hexahop.sourceforge.net],[hex-a-hop])
AC_CONFIG_AUX_DIR([msc])
AC_CONFIG_MACRO_DIR([msc])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([src/config.h])
AM_MAINTAINER_MODE

AC_SEARCH_LIBS([strerror],[cposix])
AC_PROG_CC
AC_PROG_CXX

AC_HEADER_STDC

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18])

AC_CHECK_HEADERS(stdint.h windows.h)
AC_CHECK_LIB(m, main)

AM_ICONV
if test "$am_cv_func_iconv" = yes; then
       AC_DEFINE_UNQUOTED(ICONV_CONST, [$am_cv_proto_iconv_arg1],
               [Define as const if the declaration of iconv() needs const.])
fi

# Check for SDL.
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))

# Arguments.
AC_ARG_ENABLE([debug], AS_HELP_STRING([--disable-debug],[Do not compile with debugging symbols]), [want_debug=no], [want_debug=yes])
AC_ARG_ENABLE([sound], AS_HELP_STRING([--disable-sound],[Do not compile with sound support]), [want_sound=no], [want_sound=yes])
AC_ARG_ENABLE([sdlttf], AS_HELP_STRING([--disable-sdlttf],[Use SDL_Pango instead of SDL_ttf]), [want_ttf=no], [want_ttf=yes])
AC_ARG_ENABLE([relpath], AS_HELP_STRING([--enable-relpath],[Install everything under a single directory]), [want_relative=yes], [want_relative=no])

# Compilation flags.
if test "$want_debug" = yes; then
	CXXFLAGS="$CXXFLAGS -g -Wall"
else
	CXXFLAGS="$CXXFLAGS -DNDEBUG -Wall"
fi

# Check for SDL_image.
SDL_LIBS="$SDL_LIBS -lSDL_image"
SDL_CFLAGS="$SDL_CFLAGS"

# Conditional sound compilation.
if test "$want_sound" = yes; then
	MAYBE_SOUND="sound"

	# Check for SDL_mixer.
	SDL_LIBS="$SDL_LIBS -lSDL_mixer"
	SDL_CFLAGS="$SDL_CFLAGS"
else
	MAYBE_SOUND=
	CXXFLAGS="$CXXFLAGS -DDISABLE_SOUND"
fi

# Conditional Pango/TTF support.
if test "$want_ttf" = yes; then
	# Check for SDL_ttf.
	SDL_LIBS="$SDL_LIBS -lSDL_ttf"
	SDL_CFLAGS="$SDL_CFLAGS"
else
	# Check for SDL_Pango.
	SDL_LIBS="$SDL_LIBS -lSDL_Pango"
	SDL_CFLAGS="$SDL_CFLAGS"
	CXXFLAGS="$CXXFLAGS -DENABLE_PANGO"
fi

# Relative paths.
if test "$want_relative" = yes; then
	APPDIR="${prefix}/data"
	DATADIR="${prefix}/data"
	bindir="${prefix}"
	docdir="${prefix}/data"
	htmldir="${prefix}/data"
	mandir="${prefix}/data"
	CXXFLAGS="$CXXFLAGS -DRELATIVE_PATHS"
	LDFLAGS="-Wl,-rpath -Wl,\\\$\$ORIGIN/lib $LDFLAGS"
else
	APPDIR="${datadir}/applications"
	DATADIR="${datadir}/${PACKAGE}"
	CXXFLAGS="$CXXFLAGS -DDATADIR=\\\"\"${datadir}/${PACKAGE}\"\\\""
fi

# Export configuration.
AC_SUBST([CXXFLAGS])
AC_SUBST([APPDIR])
AC_SUBST([DATADIR])
AC_SUBST([MAYBE_SOUND])
AC_SUBST([SDL_LIBS])
AC_SUBST([SDL_CFLAGS])

AC_CONFIG_FILES([
Makefile
data/Makefile
src/Makefile
po/Makefile.in
])
AC_OUTPUT
