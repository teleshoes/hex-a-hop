#include <cassert>
#include <iostream>
#include <string>
#include <errno.h>
#include <SDL_Pango.h>
#include "i18n.h"
#include "text.h"
#include "video.h"

static SDLPango_Context *context = 0;

/// determine length of longest line with current font (wrapping allowed if text_width != -1)
static int SDLPangoTextHeight(const std::string &text_utf8, int text_width)
{
	// SDLPango_SetMinimumSize limits indeed the maximal size! See
	// http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=438691
	SDLPango_SetMinimumSize(context, text_width, 0);
	SDLPango_SetText(context, text_utf8.c_str(), -1);
	return SDLPango_GetLayoutHeight(context);
}

/** \brief Determine length of longest line with current font
 *
 * Whether line breaks are allowed or not needs to be set before using
 * SDLPango_SetMinimumSize!
 */
static int SDLPangoTextWidth(const std::string &text_utf8)
{
	SDLPango_SetText(context, text_utf8.c_str(), -1);
	return SDLPango_GetLayoutWidth(context);
}

/// Display the specified UTF-8 text left aligned at (x,y)
static void Print_Pango(int x, int y, const std::string &text_utf8)
{
	// Workaround for possible crash, see
	// http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=439071
	if (text_utf8.size() == 0 || (text_utf8.size() == 1 && text_utf8[0]==127))
		return;
	assert(text_utf8.find("\n") == std::string::npos);
	SDLPango_SetMinimumSize(context, SCREEN_W, 0);
	SDLPango_SetText(context, text_utf8.c_str(), -1);
	SDL_Surface *surface = SDLPango_CreateSurfaceDraw(context);
	SDL_Rect dst = {x, y, 1, 1};
	SDL_BlitSurface(surface, NULL, screen, &dst);
	SDL_FreeSurface(surface);
}

/** \brief Display the specified UTF-8 text according to the alignment
 *
 *  If line breaks are already properly set (manually) the will be respected
 *  and no new line breaks will be added. This assumes that th text is not too
 *  wide.
 *
 *  \param x the displayed text is horizontally centered around x
 *  \param y the displayed text starts at y
 *  \param width background window size into which the text needs to fit
 *  \param text_utf8 the text to be displayed, in UTF8 encoding
 *  \param align=1: horizontally centered around (x,y)
 * */
static void Print_Pango_Aligned(int x, int y, int width, const std::string &text_utf8, int align)
{
	// Workaround for possible crash, see
	// http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=439071
	if (text_utf8.size() == 0 || (text_utf8.size() == 1 && text_utf8[0]==127))
		return;
	if (width<=0)
		return;
	SDLPango_SetMinimumSize(context, width, 0);
	int real_width = SDLPangoTextWidth(text_utf8);
	// Workaround for a crash in SDL Pango, see
	// http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=439855
	if (real_width>width)
		SDLPango_SetMinimumSize(context, real_width, 0);

  SDLPango_Alignment alignment;
  if (align==0)
    alignment = SDLPANGO_ALIGN_LEFT;
  else if (align==2) {
    alignment = SDLPANGO_ALIGN_RIGHT;
    x -= width;
  } else {
    alignment = SDLPANGO_ALIGN_CENTER;
    x -= width/2;
  }
	// SDLPango_SetText_GivenAlignment is not (yet?) part of the official Pango
	// distribution, see http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=437865
	SDLPango_SetText_GivenAlignment(context, text_utf8.c_str(), -1, alignment);
	SDL_Surface *surface = SDLPango_CreateSurfaceDraw(context);
	SDL_Rect dst = {x, y, 1, 1};
	SDL_BlitSurface(surface, NULL, screen, &dst);
	SDL_FreeSurface(surface);
}

int SDLPangoTextWidth(const std::string &text_utf8);
void Print_Pango(int x, int y, const std::string &text_utf8);
void Print_Pango_Aligned(int x, int y, int width, const std::string &text_utf8, int align);

/*****************************************************************************/

void TextInit()
{
	SDLPango_Init();
	context = SDLPango_CreateContext_GivenFontDesc("sans-serif bold 12");
	SDLPango_SetDefaultColor(context, MATRIX_TRANSPARENT_BACK_WHITE_LETTER);
	SDLPango_SetMinimumSize(context, SCREEN_W, 0);
}

void TextFree()
{
	SDLPango_FreeContext(context);
}

int TextWidth(const std::string &text_utf8)
{
	return SDLPangoTextWidth(text_utf8);
}

int TextHeight(const std::string &text_utf8, int width)
{
	return SDLPangoTextHeight(text_utf8, width);
}

/// Prints a left aligned string (a single line) beginning at (x,y)
// TODO: Check that the maximal text width is already set
void Print(int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000], tmp_utf8[5000]; // FIXME: Check this limit
	vsprintf((char*)tmp, string, marker);

	ConvertToUTF8(tmp, tmp_utf8, sizeof(tmp_utf8)/sizeof(char));
	Print_Pango(x, y, tmp_utf8);

	va_end( marker );              /* Reset variable arguments.      */
}

/// Prints a string right aligned so that it ends at (x,y)
// TODO: Check that the maximal text width is already set
void PrintR(int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000], tmp_utf8[5000]; // FIXME: Check this limit
	vsprintf((char*)tmp, string, marker);

	ConvertToUTF8(tmp, tmp_utf8, sizeof(tmp_utf8)/sizeof(char));
	Print_Pango(x-SDLPangoTextWidth(tmp_utf8), y, tmp_utf8);

	va_end( marker );              /* Reset variable arguments.      */
}

/** \brief Prints a string horizontally centered around (x,y)
 *
 *  "  " in the string is interpreted as linebreak
*/
void Print_Aligned(bool split, int x, int y, int width, const char * string, int align)
{
	char tmp_utf8[5000]; // FIXME: Check this limit

	ConvertToUTF8(string, tmp_utf8, sizeof(tmp_utf8)/sizeof(char));

	std::string msg(tmp_utf8);
	while (split && msg.find("  ") != std::string::npos)
		msg.replace(msg.find("  "), 2, "\n");

	Print_Pango_Aligned(x, y, width, msg, align);
}

void PrintC(bool split, int x, int y, const char * string, ...)
{
	va_list marker;
	va_start( marker, string );     /* Initialize variable arguments. */

	char tmp[1000]; // FIXME: Check this limit
	vsprintf((char*)tmp, string, marker);

	va_end( marker );              /* Reset variable arguments.      */

	static bool print = true; // avoid flickering!
	if (print) {
		std::cerr << "Warning: don't know window width for message:\n" << tmp << "\n";
		for (unsigned int i=0; i<strlen(tmp); ++i)
			if (!std::isspace(tmp[i]))
				print = false;
	}
	Print_Aligned(split, x, y, 2*std::min(x, SCREEN_W-x), tmp, 1);
}

void ConvertToUTF8(const std::string &text_locally_encoded, char *text_utf8, size_t text_utf8_length)
{
	// Is this portable?
	size_t text_length = text_locally_encoded.length()+1;
	errno = 0;
	static const char *locale_enc = gettext_init.GetEncoding();
	iconv_t cd = iconv_open("UTF-8", locale_enc);
	char *in_buf = const_cast<char *>(&text_locally_encoded[0]);
	char *out_buf = &text_utf8[0];
	iconv(cd, &in_buf, &text_length, &out_buf, &text_utf8_length);
	iconv_close(cd);
	if (errno != 0)
		std::cerr << "An error occurred recoding " << text_locally_encoded << " to UTF8" << std::endl;
}
