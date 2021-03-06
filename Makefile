CC=gcc
BUILDDIR=bin
OBJECTDIR=objects
CFLAGS=-Wall -std=gnu99 -g
ifeq ($(NO-OPTIMIZE),1)
else
	CFLAGS+=-O3
endif
LINKFLAGS=-flto

LIBINC=-I/usr/local/include -Ilibibur/bin

DIRS=test cipher hash bn misc pk
BUILDDIRS=$(patsubst %,$(BUILDDIR)/$(OBJECTDIR)/%,$(DIRS))

SOURCES:= 
TESTSOURCES:= 
HEADERS:=

HEADERDIR=$(BUILDDIR)/include/ibcrypt

#LIBINC+=-I$(HEADERDIR)

include $(patsubst %,%/inc.mk,$(DIRS))

OBJECTS:=$(patsubst %.c,$(BUILDDIR)/$(OBJECTDIR)/%.o,$(SOURCES))
TESTSOURCES+=$(SOURCES)
TESTOBJECTS:=$(patsubst %.c,$(BUILDDIR)/$(OBJECTDIR)/%.o,$(TESTSOURCES))

BUILDHEADERS:=$(patsubst %.h,$(HEADERDIR)/%.h,$(notdir $(HEADERS)))

.PHONY: libibur libheaders lib clean install

lib: libibur $(BUILDDIR) $(BUILDHEADERS) $(OBJECTS)
	ar -rs bin/libibcrypt.a $(OBJECTS)

test: libibur $(BUILDDIR) $(BUILDHEADERS) $(TESTOBJECTS)
	gcc $(LINKFLAGS) $(TESTOBJECTS) -o $(BUILDDIR)/test

libibur:
	git submodule update --init --recursive
	$(MAKE) -C libibur

$(BUILDDIR)/$(OBJECTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $(LIBINC) $< -o $@

# find a better way to do this later
.SECONDEXPANSION:
$(HEADERDIR)/%.h: $$(wildcard */%.h)
	cp $< $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR) $(BUILDDIRS) $(HEADERDIR)

install:
	cp bin/libibcrypt.a /usr/local/lib/
	cp -r bin/include/ibcrypt /usr/local/include/

clean:
	rm -rf $(BUILDDIR)

