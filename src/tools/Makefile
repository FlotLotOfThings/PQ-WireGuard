# SPDX-License-Identifier: GPL-2.0
#
# Copyright (C) 2015-2019 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.

PKG_CONFIG ?= pkg-config
PREFIX ?= /usr
DESTDIR ?=
SYSCONFDIR ?= /etc
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
MANDIR ?= $(PREFIX)/share/man
BASHCOMPDIR ?= $(PREFIX)/share/bash-completion/completions
SYSTEMDUNITDIR ?= $(shell $(PKG_CONFIG) --variable=systemdsystemunitdir systemd 2>/dev/null || echo "$(PREFIX)/lib/systemd/system")
RUNSTATEDIR ?= /var/run
WITH_BASHCOMPLETION ?=
WITH_WGQUICK ?=
WITH_SYSTEMDUNITS ?=

ifeq ($(WITH_BASHCOMPLETION),)
ifneq ($(strip $(wildcard $(BASHCOMPDIR))),)
WITH_BASHCOMPLETION := yes
endif
endif
ifeq ($(WITH_WGQUICK),)
ifneq ($(strip $(wildcard $(BINDIR)/bash)),)
WITH_WGQUICK := yes
endif
ifneq ($(strip $(wildcard $(DESTDIR)/bin/bash)),)
WITH_WGQUICK := yes
endif
endif
ifeq ($(WITH_SYSTEMDUNITS),)
ifneq ($(strip $(wildcard $(SYSTEMDUNITDIR))),)
WITH_SYSTEMDUNITS := yes
endif
endif

PLATFORM ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')

CFLAGS ?= -O3
CFLAGS += -std=gnu99 -D_GNU_SOURCE
CFLAGS += -Wall -Wextra
CFLAGS += -MMD -MP
CFLAGS += -mavx2 -msse4.2 -maes
CFLAGS += -DRUNSTATEDIR="\"$(RUNSTATEDIR)\""
CFLAGS += -I$(shell pwd)

LDFLAGS += -no-pie

ifeq ($(DEBUG_TOOLS),y)
CFLAGS += -g
endif
ifeq ($(PLATFORM),linux)
LIBMNL_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmnl 2>/dev/null)
LIBMNL_LDLIBS := $(shell $(PKG_CONFIG) --libs libmnl 2>/dev/null || echo -lmnl)
CFLAGS += $(LIBMNL_CFLAGS)
LDLIBS += $(LIBMNL_LDLIBS)
endif
ifeq ($(PLATFORM),haiku)
LDLIBS += -lnetwork -lbsd
endif
ifeq ($(PLATFORM),windows)
CC := x86_64-w64-mingw32-gcc
CFLAGS += -Iwincompat/include -include wincompat/compat.h
LDLIBS += -lws2_32
wg: wincompat/libc.o wincompat/init.o
endif

OBJS := $(patsubst %.c,%.o,$(wildcard *.c mceliece/*.c))
OBJS += $(patsubst %.S,%.o,$(wildcard mceliece/*.S))

ifneq ($(V),1)
BUILT_IN_LINK.o := $(LINK.o)
LINK.o = @echo "  LD      $$(pwd)/$@";
LINK.o += $(BUILT_IN_LINK.o)
BUILT_IN_COMPILE.c := $(COMPILE.c)
COMPILE.c = @echo "  CC      $$(pwd)/$@";
COMPILE.c += $(BUILT_IN_COMPILE.c)
endif

wg: $(OBJS)

ifneq ($(V),1)
clean:
	@echo "  CLEAN   $$(pwd)/{wg,*.d} $(OBJS)"
	@$(RM) wg *.d $(OBJS)
else
clean:
	$(RM) wg *.d $(OBJS)
endif

install: wg
	@install -v -d "$(DESTDIR)$(BINDIR)" && install -v -m 0755 wg "$(DESTDIR)$(BINDIR)/wg"
	@install -v -d "$(DESTDIR)$(MANDIR)/man8" && install -v -m 0644 man/wg.8 "$(DESTDIR)$(MANDIR)/man8/wg.8"
	@[ "$(WITH_BASHCOMPLETION)" = "yes" ] || exit 0; \
	install -v -d "$(DESTDIR)$(BASHCOMPDIR)" && install -v -m 0644 completion/wg.bash-completion "$(DESTDIR)$(BASHCOMPDIR)/wg"
	@[ "$(WITH_WGQUICK)" = "yes" ] || exit 0; \
	install -v -m 0755 wg-quick/$(PLATFORM).bash "$(DESTDIR)$(BINDIR)/wg-quick" && install -v -m 0700 -d "$(DESTDIR)$(SYSCONFDIR)/wireguard"
	@[ "$(WITH_WGQUICK)" = "yes" ] || exit 0; \
	install -v -m 0644 man/wg-quick.8 "$(DESTDIR)$(MANDIR)/man8/wg-quick.8"
	@[ "$(WITH_WGQUICK)" = "yes" -a "$(WITH_BASHCOMPLETION)" = "yes" ] || exit 0; \
	install -v -m 0644 completion/wg-quick.bash-completion "$(DESTDIR)$(BASHCOMPDIR)/wg-quick"
	@[ "$(WITH_WGQUICK)" = "yes" -a "$(WITH_SYSTEMDUNITS)" = "yes" ] || exit 0; \
	install -v -d "$(DESTDIR)$(SYSTEMDUNITDIR)" && install -v -m 0644 systemd/wg-quick@.service "$(DESTDIR)$(SYSTEMDUNITDIR)/wg-quick@.service"

help:
	@cat INSTALL

.PHONY: clean install help

-include *.d
