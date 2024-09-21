# -----------------------------------------------------------------------------

MAIN_INC=main/inc
MAIN_SRC=main/src
TEST_INC=test/inc
TEST_SRC=test/src

BIN=bin

DLL_NAME=jccbasic
DLL=$(BIN)/$(DLL_NAME).dll

CC=gcc
CC_FLAGS_MAIN=-c -g -Wall -I$(MAIN_INC)
CC_FLAGS_TEST=-c -g -Wall -I$(MAIN_INC) -I$(TEST_INC)

LD=gcc
LD_FLAGS_MAKE=-shared
LD_FLAGS_LINK=-L$(BIN) -l$(DLL_NAME)

# -----------------------------------------------------------------------------

HEADERS=\
	$(MAIN_INC)/asc.h \
	$(MAIN_INC)/cdbl.h \
	$(MAIN_INC)/chr.h \
	$(MAIN_INC)/cint.h \
	$(MAIN_INC)/command.h \
	$(MAIN_INC)/date_time.h \
	$(MAIN_INC)/fix.h \
	$(MAIN_INC)/hex.h \
	$(MAIN_INC)/inkey.h \
	$(MAIN_INC)/int.h \
	$(MAIN_INC)/jccbasic_version.h \
	$(MAIN_INC)/lbound_ubound.h \
	$(MAIN_INC)/ltrim.h \
	$(MAIN_INC)/mkd_cvd.h \
	$(MAIN_INC)/mki_cvi.h \
	$(MAIN_INC)/oct.h \
	$(MAIN_INC)/randomize_rnd.h \
	$(MAIN_INC)/rtrim.h \
	$(MAIN_INC)/sgn.h \
	$(MAIN_INC)/sleep.h \
	$(MAIN_INC)/space.h \
	$(MAIN_INC)/str.h \
	$(MAIN_INC)/stringd.h \
	$(MAIN_INC)/timer.h \
	\
	$(TEST_INC)/assert.h

MAIN_OBJS=\
	$(BIN)/asc.o \
	$(BIN)/cdbl.o \
	$(BIN)/chr.o \
	$(BIN)/cint.o \
	$(BIN)/command.o \
	$(BIN)/date_time.o \
	$(BIN)/fix.o \
	$(BIN)/hex.o \
	$(BIN)/inkey.o \
	$(BIN)/int.o \
	$(BIN)/jccbasic_version.o \
	$(BIN)/lbound_ubound.o \
	$(BIN)/ltrim.o \
	$(BIN)/mkd_cvd.o \
	$(BIN)/mki_cvi.o \
	$(BIN)/oct.o \
	$(BIN)/randomize_rnd.o \
	$(BIN)/rtrim.o \
	$(BIN)/sgn.o \
	$(BIN)/sleep.o \
	$(BIN)/space.o \
	$(BIN)/str.o \
	$(BIN)/stringd.o \
	$(BIN)/timer.o

TEST_EXES=\
	$(BIN)/test_asc.exe \
	$(BIN)/test_cdbl.exe \
	$(BIN)/test_chr.exe \
	$(BIN)/test_cint.exe \
	$(BIN)/test_command.exe \
	$(BIN)/test_date_time.exe \
	$(BIN)/test_fix.exe \
	$(BIN)/test_hex.exe \
	$(BIN)/test_inkey.exe \
	$(BIN)/test_int.exe \
	$(BIN)/test_jccbasic_version.exe \
	$(BIN)/test_lbound_ubound.exe \
	$(BIN)/test_ltrim.exe \
	$(BIN)/test_mkd_cvd.exe \
	$(BIN)/test_mki_cvi.exe \
	$(BIN)/test_oct.exe \
	$(BIN)/test_randomize_rnd.exe \
	$(BIN)/test_rtrim.exe \
	$(BIN)/test_sgn.exe \
	$(BIN)/test_sleep.exe \
	$(BIN)/test_space.exe \
	$(BIN)/test_str.exe \
	$(BIN)/test_stringd.exe \
	$(BIN)/test_timer.exe

# -----------------------------------------------------------------------------

.PHONY: all clean

.PRECIOUS: $(BIN)/%.o

all: $(BIN) $(DLL) $(TEST_EXES)

clean:
	-rm -rf $(BIN)

# -----------------------------------------------------------------------------

$(BIN):
	-mkdir $(BIN)

$(BIN)/%.o: $(MAIN_SRC)/%.c $(HEADERS)
	$(CC) -o $@ $(CC_FLAGS_MAIN) $<

$(DLL): $(MAIN_OBJS)
	$(LD) -o $@ $(LD_FLAGS_MAKE) $^

# -----------------------------------------------------------------------------

$(BIN)/%.o: $(TEST_SRC)/%.c $(HEADERS) $(DLL)
	$(CC) -o $@ $(CC_FLAGS_TEST) $<

$(BIN)/%.exe: $(BIN)/%.o
	$(LD) -o $@ $(LD_FLAGS_LINK) $<

# -----------------------------------------------------------------------------
