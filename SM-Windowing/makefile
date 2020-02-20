# The directory of the time domain library
LIBRARY_DIR = $(shell pwd)

# standard subdirectories
IDIR = includes
ODIR = obj
LDIR = lib
SDIR = src

# All the external objects that the current submodule depends on
# Those objects have to be up to date
tempo1 = $(wildcard ../SM-Special_functions/*.o)
EXTERNAL_OBJ = $(tempo1)

TARGET =  Windowing.a
SRC  = $(wildcard $(SDIR)/*.cpp)
OBJ  = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))
DEPS = $(OBJ:.o=.d)

# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)g++

# flags
CFLAGS = -Ofast -march=native -std=c++14 -MMD -MP -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot)
OMPFLAGS = -fopenmp -fopenmp-simd
SHRFLAGS = -fPIC -shared
# libraries
LDLIBS = -lm 

$(TARGET) : $(OBJ)
	@ echo " "
	@ echo "---------Compiling static library $(TARGET)---------"
	ar cr $(TARGET) $(OBJ) $(EXTERNAL_OBJ) 
	
$(ODIR)/%.o : $(SDIR)/%.cpp
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) $(SHRFLAGS) -c -Wall -o $@ $< $(CFLAGS) $(OMPFLAGS) $(LDLIBS) 

-include $(DEPS)

clean:
	@rm -f $(TARGET) $(OBJ)
	 	 
.PHONY: clean, dummy