ifeq ($(VERSION),)
VERSION := $(shell git describe --always --dirty)
endif

SOURCES += $(wildcard *.cpp applets/*.cpp common/*.cpp libsatoshi/*.cpp)

$(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard *.cpp applets/*.cpp libsatoshi/*.cpp)) : CPPFLAGS += -I.


BTCTOOL_OBJECTS := $(addprefix $(OBJDIR)/,btctool.o util.o $(addprefix applets/,address.o addresses.o decode16.o decode58.o decode64.o decrypt.o disasm.o encode16.o encode58.o encode64.o encrypt.o privkey.o privkeys.o pubkey.o pubkeys.o rmd160.o sha256.o) $(addprefix common/,aes.o base64.o cli.o fd.o ecp.o fp.o hex.o io.o mpn.o pbkdf2.o ripemd.o scrypt.o sha.o termios.o) $(addprefix libsatoshi/,base58check.o script.o types.o))
BTCTOOL := $(BINDIR)/btctool$(EXEC_SUFFIX)
ALL += $(BTCTOOL)

$(OBJDIR)/btctool.o : CPPFLAGS += -DVERSION='"$(VERSION)"'
$(BTCTOOL) : CPPFLAGS += -pthread
$(BTCTOOL) : LDLIBS += -lgmp
$(BTCTOOL) : $(BTCTOOL_OBJECTS)
	$(DO_LINK.cpp)


ROLLKEY_OBJECTS := $(addprefix $(OBJDIR)/,rollkey.o util.o $(addprefix common/,ecp.o fp.o hex.o io.o mpn.o ripemd.o sha.o) $(addprefix libsatoshi/,base58check.o types.o))
ROLLKEY := $(BINDIR)/rollkey$(EXEC_SUFFIX)
ALL += $(ROLLKEY)

$(ROLLKEY) : LDLIBS += -lgmp
$(ROLLKEY) : $(ROLLKEY_OBJECTS)
	$(DO_LINK.cpp)
