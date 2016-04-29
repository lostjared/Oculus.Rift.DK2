CXX=c++
CLANG=clang
OCULUS_SDK=/Users/jared/OculusSDK/LibOVR/include
SDL_PATH=/opt/local

SDL2_FLAGS=-L/opt/local/lib $(SDL_PATH)/lib/libSDL2.a -lm /usr/lib/libiconv.dylib -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit
CV_LIBS=`pkg-config opencv --libs`
#CV_LIBS=./libs/libopencv_calib3d.2.4.10.dylib ./libs/libopencv_contrib.2.4.10.dylib ./libs/libopencv_core.2.4.10.dylib ./libs/libopencv_features2d.2.4.10.dylib ./libs/libopencv_flann.2.4.10.dylib ./libs/libopencv_highgui.2.4.10.dylib ./libs/libopencv_imgproc.2.4.10.dylib ./libs/libopencv_ml.2.4.10.dylib ./libs/libopencv_nonfree.2.4.10.dylib ./libs/libopencv_objdetect.2.4.10.dylib ./libs/libopencv_ocl.2.4.10.dylib ./libs/libopencv_photo.2.4.10.dylib ./libs/libopencv_stitching.2.4.10.dylib ./libs/libopencv_superres.2.4.10.dylib ./libs/libopencv_video.2.4.10.dylib ./libs/libopencv_videostab.2.4.10.dylib
CXX_FLAGS=-O3 -mmacosx-version-min=10.9 `sdl2-config --cflags` -I/opt/local/include -I$(OCULUS_SDK) -I/usr/include `pkg-config opencv --cflags` 
LD_FLAGS=-mmacosx-version-min=10.9    -F. -framework LibOVR $(CV_LIBS) $(SDL2_FLAGS) -framework OpenGL /usr/lib/libglew.a 
SOURCE_FILES=ac.vr.cpp
OBJECT_FILES=ac.vr.o

%.o: %.cpp
	$(CXX)  $(CXX_FLAGS) -c -o $@ $<

%.o: %.mm
	$(CLANG) $(CXX_FLAGS) -c -o $@ $<

ac.vr: ac.vr.o ac.o fractal.o 
	$(CXX)  ac.vr.o ac.o fractal.o  -o ac.vr $(LD_FLAGS) 

all: ac.vr


clean:
	rm -f ac.vr *.o

