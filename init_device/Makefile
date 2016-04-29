CXX=c++
OCULUS_SDK=/Users/jared/OculusSDK/LibOVR/include


SDL2_FLAGS=-L/opt/local/lib /opt/local/lib/libSDL2.a -lm /usr/lib/libiconv.dylib -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit

CXX_FLAGS=-O3 -std=c++11 `sdl2-config --cflags` -I/opt/local/include -I$(OCULUS_SDK) -I/usr/include 
LD_FLAGS= $(SDL2_FLAGS) -framework OpenGL /usr/lib/libglew.a -F. -framework libOVR
SOURCE_FILES=sdl-test.cpp
OBJECT_FILES=sdl-test.o

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

sdl-test: sdl-test.o 
	$(CXX) sdl-test.o -o sdl-test $(LD_FLAGS)

all: sdl-test


clean:
	rm -f sdl-test *.o

