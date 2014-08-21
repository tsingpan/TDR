BINARY=bin/tsf4g_example
TARGET= $(BINARY)
TSF4G=/usr/local/tsf4g
HOME=../
CINC=-I$(TSF4G)/include -I$(TSF4G)/tdr
LDPATH=
DEPLIBS=-lrt
DEPOFILE=
TDRINC=-I$(TSF4G)/tdr

CFILE=$(wildcard source/*.c)
TYPES_TDR_FILE=$(wildcard tdr/*.tdr)
READER_TDR_FILE=$(wildcard tdr/*.tdr)

include $(HOME)/build.mk

dep:
	@mkdir -p bin

install:
	mkdir -p $(PREFIX)
	cp -rf bin $(PREFIX)
	cp -rf tdr $(PREFIX)
	find $(PREFIX) -name "*.d" -or -name "*.o" | xargs rm -f
