all:
	g++ -l SDL GUI.cc ZBRA.cc Map.cc Tile.cc -o PP

clean:
	rm PP
