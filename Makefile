CXXFLAGS=-Wall -Wextra -Werror -g -std=c++14 `pkg-config --cflags $(LIBS)`

LIBS=OpenCL python3
LDLIBS=`pkg-config --libs $(LIBS)`

VPATH=src

OBJ=main.o io.o app.o linear.o pathfinding.o gpu.o wspd.o

all: standalone

libpython: CXXFLAGS+=-fPIC
libpython: $(OBJ)
	$(CXX) -shared $(LDFLAGS) $^ $(LDLIBS) -o tspanner.so

standalone: $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) tspanner.so standalone $(OBJ) output.png

proper:
	$(RM) $(OBJ)
