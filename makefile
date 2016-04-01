ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/local/lib -framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo -lglfw3 -lGLEW
else
LDFLAGS=-lX11 -lGL -lGLU -lglut -lGLEW -lm
endif

CC = g++
CFLAGS=-g -I/opt/local/include
CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++

# INIT_SHADER = initshader.o

#SOURCES = main.cpp *.cpp GLSLShader.cpp DisplayManager.cpp Camera.cpp transformations.c vectors.cpp
SOURCES = *.cpp transformations.c

main:
	$(CC) -std=c++11 $(SOURCES) $(LDFLAGS) -o main

# main: main.o stb_image.o $(INIT_SHADER) initshader.h
# 	$(CC) $@.o $(INIT_SHADER) $(SOURCES) $(LDFLAGS) -o main

# stb_image.o: stb_image.c
# 	gcc $(CFLAGS) -c stb_image.c -o stb_image.o

# $(INIT_SHADER):
# 	gcc $(CFLAGS) -c initshader.c -o $(INIT_SHADER)

clean: 
	-rm -r main *.o core *.dSYM


# ifeq ($(shell uname -s), Darwin)
# LDFLAGS=-L/opt/local/lib -framework OpenGL -lglfw -lGLEW
# else
# LDFLAGS=-lX11 -lGL -lGLU -lGLEW -lm
# endif

# GG = g++
# CC = gcc
# CFLAGS=-g -I/opt/local/include

# INIT_SHADER = initshader.o
# PROGRAMS = main

# $(PROGRAMS).o:
# 	$(CC) initshader.c -o $(INIT_SHADER) $(LDFLAGS)
# 	$(GG) $(PROGRAMS).cpp draw.cpp callbacks.cpp helper.cpp structs.cpp -o $(PROGRAMS) $(LDFLAGS)

# # main: main.o $(INIT_SHADER) common.h
# # 	$(GG) main.cpp draw.cpp callbacks.cpp helper.cpp structs.cpp $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

# # %.o: %.cpp
# # 	#$(GG) $(PROGRAMS).cpp draw.cpp callbacks.cpp helper.cpp structs.cpp -o $(PROGRAMS) $(LDFLAGS)
# # 	$(CC) $(CFLAGS) -c $< -o $@ 

# clean:
# 	-rm -r main *.o core *.dSYM
