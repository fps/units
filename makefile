PREFIX ?= /usr/local

INSTALL ?= install
SED ?= sed

INCLUDE_PATH = $(PREFIX)/include/units-0
PKGCONFIG_DIR ?= $(PREFIX)/lib/pkgconfig

.PHONY: install all clean

all: libunits-0.so units-0-test

libunits-0.so: units-0/units.cc units-0/units.h
	g++ -I . -fPIC -shared -o libunits-0.so units-0/units.cc `pkg-config jack --cflags --libs`

install: all
	$(INSTALL) -d $(PKGCONFIG_DIR)
	$(INSTALL) units-0.pc $(PKGCONFIG_DIR)/
	$(SED) -i -e s@PREFIX@$(PREFIX)@g $(PKGCONFIG_DIR)/units-0.pc 
	$(INSTALL) -d $(INCLUDE_PATH)
	$(INSTALL) units-0/*.h $(INCLUDE_PATH)

units-0-test: units-test.cc libunits-0.so
	g++ -I .  -ansi -Wall -g -O0 -o units-0-test  units-test.cc -L . -lunits-0 -Wl,-rpath,.

docs:
	doxygen

clean:
	rm -f units-0-test libunits-0.so
	
