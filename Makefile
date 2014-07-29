UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
	FLAGS = -framework Cocoa -framework OpenGL -framework GLUT
else
	FLAGS = -I/usr/include -L/usr/lib -lglut -lGL -lGLU -lX11
endif

all: modelviewer

modelviewer: src/object.o src/modelviewer.o src/opengl.o
	g++ src/object.o src/opengl.o src/modelviewer.o -o modelviewer $(FLAGS)

src/object.o: src/object.h src/object.cpp
	g++ -c src/object.cpp -o src/object.o

src/opengl.o: src/opengl.h src/opengl.cpp
	g++ -c src/opengl.cpp -o src/opengl.o

src/modelviewer.o: src/modelviewer.cpp
	g++ -c src/modelviewer.cpp -o src/modelviewer.o

clean:
	rm -rf src/*.o
	rm -rf modelviewer
	rm -rf output.obj