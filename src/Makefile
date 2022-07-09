
INC=-I $(CURDIR) -I $(CURDIR)/includes
LIB = -Wall -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -ldl -lGLESv2  -lassimp

default:	main

main:	main.cpp
	g++ $(INC) main.cpp glad.c -o bin/triangle $(LIB) && bin/triangle 
# && clear
clean:
	rm -r $(CURDIR)/bin/triangle
