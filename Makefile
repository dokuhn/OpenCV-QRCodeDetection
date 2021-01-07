# Makefile for QRCodeDetection sample program with an industrial camera of the Basler scout Series
.PHONY: all clean

# The program to build
NAME       := QRCodeDetection

# Installation directories for pylon
PYLON_ROOT ?= /opt/pylon

# Build tools and flags
LD         := $(CXX)
CPPFLAGS   := -I /opt/pylon/include -I /usr/local/include/opencv4
CXXFLAGS   := #e.g., CXXFLAGS=-g -O0 for debugging
LDFLAGS    := -Wl,--enable-new-dtags -Wl,-rpath,/opt/pylon/lib
LDLIBS     := -L/opt/pylon/lib -L/usr/local/lib -Wl,-E -lpylonbase -lpylonutility -lGenApi_gcc_v3_1_Basler_pylon -lGCBase_gcc_v3_1_Basler_pylon -lopencv_core -lopencv_video -lopencv_highgui -lopencv_world -lopencv_objdetect -lopencv_imgproc

# Rules for building
all: $(NAME)

$(NAME): $(NAME).o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(NAME).o: $(NAME).cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(NAME).o $(NAME)
