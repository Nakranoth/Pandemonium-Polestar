CC=g++
CFLAGS=-c -g -Wall 
LDFLAGS=-lSDL
SOURCES=Main.cc GUI.cc SurfaceLoader.cc Events.cc ZBRA.cc DimSolv.cc Map.cc Tile.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=PP

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o PP