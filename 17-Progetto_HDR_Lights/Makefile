# $@ nome del target
# @^ lista delle dipendenze

CC = C:/Root/Dev/Tools/msys64/mingw32/bin/g++.exe
#CCFLAGS = -O3 -s -DNDEBUG # RELEASE CONFIG
CCFLAGS = -g # DEUG CONFIG

ifeq ($(OS),Windows_NT)
	BASEDIR = ../base
	INCLUDEDIRS += -I$(BASEDIR)/freeglut/include
	INCLUDEDIRS += -I$(BASEDIR)/glew/include
	INCLUDEDIRS += -I$(BASEDIR)/glm
	INCLUDEDIRS += -I$(BASEDIR)/assimp

	LIBDIRS += -L$(BASEDIR)/freeglut/lib
	LIBDIRS += -L$(BASEDIR)/glew/lib/Release/Win32
	LIBDIRS += -L$(BASEDIR)/assimp/lib

	LIBS += -lfreeglut 
	LIBS += -lopengl32 
	LIBS += -lglew32
	LIBS += -lassimp

else
	LIBS += -lglut 
	LIBS += -lGLEW
	LIBS += -lGL
	LIBS += -lassimp
endif

OBJS = main.o utilities.o common.o transform.o camera.o shaderclass.o myhdrclass.o myshaderclass.o myhdrshaderclass.o
OBJS += light.o texture2D.o cube.o octahedron.o mesh.o
OBJS += teapot.o skull.o flower.o dragon.o boot.o marius.o

program.exe : $(OBJS)
	$(CC) $(CCFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@

main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

utilities.o : utilities.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

common.o : common.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

transform.o : transform.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

camera.o : camera.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

shaderclass.o : shaderclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

myhdrclass.o : myhdrclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

myhdrshaderclass.o : myhdrshaderclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

myshaderclass.o : myshaderclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

light.o : light.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

texture2D.o : texture2D.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

cube.o : cube.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

octahedron.o : octahedron.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

mesh.o : mesh.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

teapot.o : teapot.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

skull.o : skull.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

boot.o : boot.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

dragon.o : dragon.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

flower.o : flower.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

marius.o : marius.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

.PHONY clean:
clean:
#	rm *.o *.exe
	del /f /q *.o *.exe 2>nul || exit 0


