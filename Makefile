CC = gcc
APP = tiktok
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

DATADIR = $(PREFIX)/share
APPDIR = $(DATADIR)/applications
ICONDIR = $(DATADIR)/icons/hicolor/scalable/apps

DESKTOP_FILE = data/$(APP).desktop
ICON_FILE = data/$(APP).svg

CFLAGS = -std=c23 -Wall -Wextra -g \
	$(shell pkg-config --cflags gtk4)

LDLIBS = \
	$(shell pkg-config --libs gtk4)

all: $(APP) 

$(APP): main.c
	$(CC) $(CFLAGS) main.c -o $(APP) $(LDLIBS)

install: $(APP)
	install -Dm755 $(APP) $(DESTDIR)$(BINDIR)/$(APP)
	install -Dm644 $(DESKTOP_FILE) $(DESTDIR)$(APPDIR)/$(APP).desktop
	install -Dm644 $(ICON_FILE) $(DESTDIR)$(ICONDIR)/$(APP).svg

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(APP)
	rm -f $(DESTDIR)$(APPDIR)/$(APP).desktop
	rm -f $(DESTDIR)$(ICONDIR)/$(APP).svg

clean:
	rm -f $(APP)
