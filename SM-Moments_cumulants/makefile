# All Submodule directorie
FFT_benchmark = SM-FFT_benchmark
Buffered_arrays = SM-Buffered_arrays
Scoped_timer = SM-Scoped_timer
Moments_cumulants = SM-Moments_cumulants
Histograms = SM-Histograms
OMP_EXTRA = SM-Omp_extra
MATH_EXTRA = SM-Math_extra
Time_quadratures = SM-Time_quadratures
Fast_convolution = SM-Fast_convolution
Windowing = SM-Windowing
Special_functions = SM-Special_functions
Numerical_integration = SM-Numerical_integration

# standard subdirectories
IDIR = includes
ODIR = obj
LDIR = lib
SDIR = src

# All the external objects that the current submodule depends on
# Those objects have to be up to date
tempo1 = $(wildcard ../SM-Math_extra/*.o)
tempo2 = $(wildcard ../SM-Omp_extra/obj/*.o)
EXTERNAL_OBJ = $(tempo1) $(tempo2)

# TARGET
TARGET = moments_cumulants.a
SRC  = $(wildcard *.cpp)
OBJ  = $(patsubst %.cpp, %.o, $(SRC))
DEPS = $(OBJ:.o=.d)

# Lits of .c and corresponding .o and .h
SRC  = $(wildcard $(SDIR)/*.cpp)
OBJ  = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))

# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)g++

# flags
CFLAGS = -Ofast -march=native -std=c++14 -MMD -MP -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot) -I$(IDIR)
OMPFLAGS = -fopenmp -fopenmp-simd
# FFTWFLAGS = -lfftw3 -lfftw3f -lm
SHRFLAGS = -fPIC -shared

# Python directories
PY = $(OS:Windows_NT=/c/Anaconda2/)python
ifeq ($(USERNAME),simon)
    PY = $(OS:Windows_NT=/cygdrive/c/Anaconda2/)python
endif
ifeq ($(USERNAME),Sous-sol)
    PY = $(OS:Windows_NT=/cygdrive/c/ProgramData/Anaconda2/)python
endif

# includes
PYINCL := $(shell $(PY) -m pybind11 --includes)
ifneq ($(OS),Windows_NT)
    PYINCL += -I /usr/include/python2.7/
endif

# libraries
LDLIBS = $(OS:Windows_NT=-L /c/Anaconda2/libs/ -l python27) $(PYINCL)
ifeq ($(USERNAME),simon)
    LDLIBS = $(OS:Windows_NT=-L /cygdrive/c/Anaconda2/libs/ -l python27) $(PYINCL)
endif
ifeq ($(USERNAME),Sous-sol)
    LDLIBS = $(OS:Windows_NT=-L /cygdrive/c/ProgramData/Anaconda2/libs/ -l python27) $(PYINCL) 
endif
    
$(TARGET) : $(OBJ)
	@ echo " "
	@ echo "---------Compiling static library $(TARGET)---------"
	ar cr $(TARGET) $(OBJ) $(EXTERNAL_OBJ) 

$(ODIR)/%.o : $(SDIR)/%.cpp
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) $(SHRFLAGS) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)

-include $(DEPS)

clean:
	@rm -f $(TARGET) $(OBJ)
	 	 
.PHONY: clean