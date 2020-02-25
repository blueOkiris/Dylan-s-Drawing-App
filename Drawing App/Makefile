# Name of application
OBJNAME=  draw

# Location of files
SRCFLDRS= src
INCFLDRS= headers /usr/include/SDL2
LIBFLDRS= 

# Compiler options
# NOTE: Requires libsdl2-dev & libsdl2-image-dev & libsd2-ttf-dev
CC=       gcc
CFLAGS=   -g -Wall -O2
LDFLAGS=  -lSDL2 -lSDL2_image -lSDL2_ttf -lm

# Get real options from setup
SRC=      $(foreach folder,$(SRCFLDRS),$(wildcard $(folder)/*.c))
INC=      $(addprefix -I,$(INCFLDRS))
LIB=      $(addprefix -L,$(LIBFLDRS))
INCFILES= $(foreach folder,$(INCFLDRS),$(wildcard $(folder)/*.h))

# Compile the application
$(OBJNAME) : $(SRC) $(INCFILES)
	$(CC) $(CFLAGS) $(LIB) $(INC) -o $(OBJNAME) $(SRC) $(LDFLAGS)
