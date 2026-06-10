CC = gcc
APP = base60tok
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

CFLAGS = -std=c23 -Wall -Wextra -g \
	$(shell pkg-config --cflags gtk4)

LDLIBS = \
	$(shell pkg-config --libs gtk4)

all: $(APP) 

$(APP): main.c
	$(CC) $(CFLAGS) main.c -o $(APP) $(LDLIBS)

install: $(APP)
	install -Dm755 $(APP) $(DESTDIR)$(BINDIR)/$(APP)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)$(APP)

clean:
	rm -f $(APP)

