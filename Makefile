all:
	g++ -l SDL -g Main.cc GUI.cc ZBRA.cc Map.cc Tile.cc SurfaceLoader.cc Events.cc -o PP

clean:
	rm PP
