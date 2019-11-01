
	# histograms.dll/.so depends on all the .o files (in /obj) which, in turn, 
# depend on their correspondind .c file (in /src) which, inturn depend on their 
# corresponding .h (in /includes)
# For this simple project all %.c have a correspondin %.h and generate a corresponding %.o which are all linked
# in histograms.dll/.so 

# subdirectories
IDIR = includes
ODIR = obj
LDIR = lib
SDIR = src

PY = $(OS:Windows_NT=/c/Anaconda2/)python
ifeq ($(USERNAME),simon)
    PY = $(OS:Windows_NT=/cygdrive/c/Users/simon/Anaconda3/envs/Python27/)python
endif
ifeq ($(USERNAME),Sous-sol)
    PY = $(OS:Windows_NT=/cygdrive/c/ProgramData/Anaconda2/)python
endif

# Lits of .c and corresponding .o and .h
SRC  = $(wildcard $(SDIR)/*.cpp)
OBJ  = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))
DEPS = $(OBJ:.o=.d)
HEAD = $(patsubst $(SDIR)/%.cpp,$(IDIR)/%.h,$(SRC))

# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)g++

# flags
CFLAGS = -Ofast -march=native -std=c++14 -MMD -MP -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot) -I$(IDIR)

OMPFLAGS = -fopenmp -fopenmp-simd
FFTWFLAGS = -lfftw3 -lfftw3f -lm
SHRFLAGS = -fPIC -shared

# includes
PYINCL = `$(PY) -m pybind11 --includes`
ifneq ($(OS),Windows_NT)
    PYINCL += -I /usr/include/python2.7/
endif

# libraries
LDLIBS = -lmpfr $(OS:Windows_NT=-L /c/Anaconda2/libs/ -l python27) $(PYINCL)
ifeq ($(USERNAME),simon)
    LDLIBS = -lmpfr $(OS:Windows_NT=-L /cygdrive/c/Users/simon/Anaconda3/envs/Python27/libs/ -l python27) $(PYINCL) 
endif
ifeq ($(USERNAME),Sous-sol)
    LDLIBS = -lmpfr $(OS:Windows_NT=-L /cygdrive/c/ProgramData/Anaconda2/libs/ -l python27) $(PYINCL) 
endif

# Change the target extension depending on OS (histograms.dll or histograms.so)

SHREXT = $(if $(filter $(OS),Windows_NT),.pyd,.so)
TARGET = time_domain$(SHREXT)

all: $(TARGET)

# The library is recompiled if any object aren't up to date 
$(TARGET): $(OBJ)
	@ echo " "
	@ echo "---------Compile library $(TARGET)---------"
	$(CC) $(SHRFLAGS) -o $(TARGET) $(OBJ) $(CFLAGS) $(OMPFLAGS) $(FFTWFLAGS) $(LDLIBS)
	
$(ODIR)/%.o : $(SDIR)/%.cpp
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) $(SHRFLAGS) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)

-include $(DEPS)

clean:
	@rm -f $(TARGET) $(OBJ)
	 	 
.PHONY: clean, all