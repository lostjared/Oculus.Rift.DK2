AM_CXXFLAGS = -std=c++11 -O2 $(LIBSDL_CFLAGS) -I$(top_srcdir)/include $(CV_CXXFLAGS) 
AM_LDFLAGS = $(LIBSDL_LIBS) -F$(top_srcdir)/frameworks -framework LibOVR $(CV_LDFLAGS) $(top_srcdir)/libglew.a 
bin_PROGRAMS=AcidCam_VR
AcidCam_VR_SOURCES=ac.cpp ac.vr.cpp fractal.cpp ac.h fractal.h alert_message.mm


