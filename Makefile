COMPILER = clang
SOURCE_LIBS = -I lib/
OSX_OPT = -L lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
OSX_OUT = -o "bin/build_osx"
CFILES = src/*.c

build_osx:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)