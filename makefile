PROG = sland
CC = gcc
OUT = bin

SEARCHPATH += src
vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS += defs.h structs.h

_OBJS += draw.o
_OBJS += init.o input.o
_OBJS += main.o
_OBJS += stage.o
_OBJS += util.o

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<
	
# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) -rf $(OUT) $(PROG)

CXXFLAGS += `sdl2-config --cflags`
CXXFLAGS += -g -lefence

LDFLAGS += `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

# linking the program.
$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
