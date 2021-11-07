CC = g++
PROJ = ASCII_me
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJ) : main.cpp
	$(CC) main.cpp -g -o $(PROJ) $(LIBS)

clean :
	rm -f *.o $(PROJ)