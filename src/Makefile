
INC=-I $(CURDIR)
LIB = -Wall -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

default:	main

main:	main.cpp
	g++ $(INC) main.cpp glad.c -o bin/triangle $(LIB) && clear && bin/triangle 

clean:
	rm -r $(CURDIR)/bin/triangle
