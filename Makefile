CC = gcc
CFLAGS = -g -Wall --std=c99 -D_FORTIFY_SOURCE=2 -O2 \
-Wshadow -Wpointer-arith -pedantic -Wold-style-definition \
-Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 \
-Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith \
-Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline \
-Wundef -Wnested-externs -Wcast-qual -Wunreachable-code \
-ggdb3 -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing
LIBS = -lz -lpng
OBJECTS = main.o ppm.o png.o
BIN = stag
DESTDIR = /usr

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN) $(OBJECTS)
install: $(BIN)
	install -Dm755 $(BIN) $(DESTDIR)/bin/$(BIN)
	install -Dm644 LICENSE $(DESTDIR)/share/licenses/$(BIN)/LICENSE
uninstall:
	-rm $(DESTDIR)/bin/$(BIN)
	-rm $(DESTDIR)/share/licenses/$(BIN)/LICENSE
clean:
	-rm $(BIN) $(OBJECTS)
pkgclean:
	-rm -r $(BIN) pkg/ src/ *.xz
.PHONY: install uninstall