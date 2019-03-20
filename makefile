# Directories (Include, Object, Library, Source)
IDIR=include
ODIR=src/obj
LDIR=lib
SDIR=src

# Compiler Options
CC=g++
CFLAGS=-I$(IDIR) -L$(LDIR) $(LIBS) -g
#OSX
CFLAGS+=-L/usr/X11/lib

# Library includes

#OSX
LIBS  =-lglfw3 -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor
#Linux
#LIBS  =-lrt -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor
#All
LIBS +=-lm -pthread -ldl

# Dependencies and Objects lists
_DEPS = glad.h shader.h stb_image.h camera.h board.h game_defs.h player.h tower.h
DEPS  = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = santorini.o glad.o stb_image.o
OBJ  = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Compile all C++ objects
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Compile all C objects
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Compile executable
santorini: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Clean
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
