
INC=-I $(CURDIR) -I $(CURDIR)/includes -I $(CURDIR)/includes/imgui -I $(CURDIR)/includes/imgui/backends
LIB = -Wall -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -ldl -lGLESv2  -lassimp

default:	main

main:	main.cpp
	g++ $(INC) main.cpp glad.c -o bin/triangle $(LIB) && bin/triangle 
# && clear
clean:
	rm -r $(CURDIR)/bin/triangle

#g++ -I /home/obliterator/Euclid_Engine/src -I /home/obliterator/Euclid_Engine/src/includes -I /home/obliterator/Euclid_Engine/src/includes/imgui -I /home/obliterator/Euclid_Engine/src/includes/imgui/backends main.cpp glad.c -o bin/triangle imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_impl_glfw.o imgui_impl_opengl3.o -Wall -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -ldl -lGLESv2  -lassimp && bin/triangle 

