# Copyright 2007 Miriam Ruiz <little_miry@yahoo.es>
# Copyright 2007 Jens Seidel <jensseidel@users.sf.net>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.

LOCALEDIR=./

all: hex-a-hop.pot build-stamp

# xgettext fails on _("Welcome to " GAMENAME "!") (doesn't extract anything
# behind the macro, so we preprocess hex_puzzzle.cpp first
hex_puzzzle_preprocessed.cpp: ../../src/hex_puzzzle.cpp ../../src/menus.h ../../src/level_list.h
	cd ../..; $(CXX) $(CXXFLAGS) -E src/hex_puzzzle.cpp -o debian/i18n/$@

hex-a-hop.pot: hex_puzzzle_preprocessed.cpp ../../src/gfx.cpp ../hex-a-hop.6
	@echo Creating Template \"$@\"
# Yes, we specify both hex_puzzzle.cpp AND debian/i18n/hex_puzzzle_preprocessed.cpp
# as the preprocessed file does not contain all strings as preprocessor
# conditionals are already evaluated for the second file. We also want proper
# path references to source files (no ../.. prefix).
	cd ../..; xgettext --add-comments="TRANSLATORS:" -k_ -o "debian/i18n/$@.templ" \
	  src/gfx.cpp src/hex_puzzzle.cpp src/menus.h src/level_list.h debian/i18n/hex_puzzzle_preprocessed.cpp \
		--msgid-bugs-address=hex-a-hop@packages.debian.org \
		--copyright-holder="Tom Beaumont"
	cd ../..; po4a-updatepo -f man -m debian/hex-a-hop.6 -p debian/i18n/hex-a-hop.6.pot
	@# lets now merge the POT files containing messages from source and manpage
	sed 's/charset=CHARSET/charset=ascii/' hex-a-hop.pot.templ > hex-a-hop.pot.templ.po
	sed 's/charset=CHARSET/charset=ascii/' hex-a-hop.6.pot > hex-a-hop.6.pot.po
	@# The first sed commands prints only the header, the seconds inserts hex-a-hop.pot.comments
	sed -n '1,/^$$/ p' hex-a-hop.pot.templ | sed "/^#$$/ r hex-a-hop.pot.comments" > hex-a-hop.pot
	msgcat --width=79 hex-a-hop.pot.templ.po hex-a-hop.6.pot.po | sed -n '1,/^$$/ !p' >> hex-a-hop.pot
	rm -f hex-a-hop.pot.templ hex-a-hop.6.pot hex-a-hop.pot.templ.po hex-a-hop.6.pot.po

build: build-stamp
build-stamp: hex-a-hop.pot
	@# Merge all (old) PO files with POT file to ship up-to-date files!
	@# (it cannot happen as usual in the clean target as there are not all patches
	@# applied yet)
	@set -e; cd ../..; cat debian/i18n/languages.list | while read lang enc; do \
		echo Updating \"$$lang\" translation; \
		msgmerge --width=79 --update debian/i18n/$$lang.po debian/i18n/hex-a-hop.pot; \
	done
	@set -e; cat languages.list | while read lang enc; do \
		echo Building \"$$lang\" translation; \
		msgfmt $$lang.po -o $$lang.mo; \
		addendum=""; test -r ../hex-a-hop.$$lang.add && addendum="-a ../hex-a-hop.$$lang.add"; \
		sed -n '/^msgid_plural / !p' $$lang.po | \
		sed -n '/^msgstr\[[1-9]\] / !p' | \
		sed 's/^msgstr\[0\] /msgstr /' > plural_forms_stripped.po; \
		options=""; [ "$$enc" ] && options="-L $$enc"; \
		po4a-translate $$options -f man -m ../hex-a-hop.6 -p plural_forms_stripped.po $$addendum -l ../hex-a-hop.$$lang.6; \
	done
	rm -f plural_forms_stripped.po # see bug #439842
	touch build-stamp

clean:
	rm -f *.mo
	rm -f hex_puzzzle_preprocessed.cpp
	rm -f build-stamp
	@cat languages.list | while read lang enc; do \
	  rm -f ../hex-a-hop.$$lang.6; \
	done

install: build-stamp
	@cat languages.list | while read lang enc; do \
		echo Installing \"$$lang\" translation; \
		install -p -d -m 755 $(LOCALEDIR)$$lang/LC_MESSAGES ; \
		install -m 644 $$lang.mo $(LOCALEDIR)$$lang/LC_MESSAGES/hex-a-hop.mo ; \
		encdir=""; [ "$$enc" = utf-8 ] && encdir=.UTF-8; \
		install -p -d -m 755 $(MANDIR)$$lang$$encdir/man6/ ; \
		install -m 644 ../hex-a-hop.$$lang.6 $(MANDIR)$$lang$$encdir/man6/hex-a-hop.6 ; \
		gzip -9 $(MANDIR)$$lang$$encdir/man6/hex-a-hop.6; \
	done
