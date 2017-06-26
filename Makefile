CXXFLAGS=-Wall -Wextra -Werror -Og -g `pkg-config --cflags $(LIBS)`

LIBS=OpenCL python3
LDLIBS=`pkg-config --libs $(LIBS)`

VPATH=src

OBJ=main.o io.o app.o linear.o pathfinding.o

libpython: CXXFLAGS+=-fPIC
libpython: $(OBJ)
	$(CXX) -shared $(LDFLAGS) $^ $(LDLIBS) -o spanner.so

standalone: $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

all: standalone

clean:
	$(RM) spanner.so standalone $(OBJ) output.png
