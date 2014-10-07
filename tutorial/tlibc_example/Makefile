BINARY=bin/tlibc_example
TARGET= $(BINARY)
TLIBC=/usr/local/tlibc
HOME=../
CINC=-I$(TLIBC)/include -I$(TLIBC)/tdr -Itdr -Itdr/common
LDPATH=-L$(TLIBC)/lib
DEPLIBS=-lrt -lz -ltprotocol -ltcore
DEPOFILE=
TDRINC=-I$(tlibc)/tdr -Itdr

CFILE=$(wildcard source/*.c)
TYPES_TDR_FILE=$(wildcard tdr/*.tdr)
TYPES_TDR_FILE+=$(wildcard tdr/common/*.tdr)

READER_TDR_FILE=$(wildcard tdr/*.tdr)
READER_TDR_FILE+=$(wildcard tdr/common/*.tdr)

WRITER_TDR_FILE=$(wildcard tdr/*.tdr)
WRITER_TDR_FILE+=$(wildcard tdr/common/*.tdr)

include $(HOME)/build.mk

dep:
	@mkdir -p bin

install:
	mkdir -p $(PREFIX)
	cp -rf bin $(PREFIX)
	cp -rf tdr $(PREFIX)
	find $(PREFIX) -name "*.d" -or -name "*.o" | xargs rm -f
