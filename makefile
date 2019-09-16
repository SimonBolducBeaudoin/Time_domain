
	# histograms.dll/.so depends on all the .o files (in /obj) which, in turn, 
# depend on their correspondind .c file (in /src) which, inturn depend on their 
# corresponding .h (in /includes)
# For this simple project all %.c have a correspondin %.h and generate a corresponding %.o which are all linked
# in histograms.dll/.so 

# subdirectories
#SDIR = src
IDIR = includes
ODIR = obj
LDIR = lib
SDIR = src
GCHDIR = gch

PY = $(OS:Windows_NT=/c/Anaconda2/)python
ifeq ($(USERNAME),simon)
    PY = $(OS:Windows_NT=/cygdrive/c/Users/simon/Anaconda3/envs/Python27/)python
endif
ifeq ($(USERNAME),Sous-sol)
    PY = $(OS:Windows_NT=/cygdrive/c/ProgramData/Anaconda2/)python
endif

# Lits of .c and corresponding .o and .h
SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))
HEAD= $(patsubst $(SDIR)/%.cpp,$(IDIR)/%.h,$(SRC))
COMPHEAD = $(patsubst $(SDIR)/%.cpp,$(GCHDIR)/%.h.gch,$(SRC))

# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)g++

# flags
#CFLAGS = -Ofast -march=native -std=c++14 -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot) -I$(IDIR)
# for debuging
CFLAGS = -DDEBUG -Ofast -march=native -std=c++14 -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot) -I$(IDIR) -I$(GCHDIR)

OMPFLAGS = -fopenmp -fopenmp-simd
FFTWFLAGS = -lfftw3_omp -lfftw3 -lfftw3f_omp -lfftw3f -lm 
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

# The library is recompiled if any object aren't up to date 
$(TARGET): $(OBJ) $(GCHDIR)/common.h.gch
	@ echo " "
	@ echo "---------Compile library $(TARGET)---------"
	$(CC) $(SHRFLAGS) -o $(TARGET) $(OBJ) $(CFLAGS) $(OMPFLAGS) $(FFTWFLAGS) $(LDLIBS)
	
###############
# if the object as a .tpp and a .h.gch 
$(ODIR)/%.o : $(SDIR)/%.cpp $(SDIR)/%.tpp $(GCHDIR)/%.h.gch
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)
# else the object as .h.gch 
$(ODIR)/%.o : $(SDIR)/%.cpp $(GCHDIR)/%.h.gch
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)
###############

# Precompiled headers
$(GCHDIR)/%.h.gch : $(IDIR)/%.h
	@ echo " "
	@ echo "---------Compile header $@ --------"
	$(CC) -Wall -o $@ $< $(CFLAGS) $(PYINCL)


clean:
	@rm -f $(TARGET) $(OBJ)

clean_all :
	@rm -f $(TARGET) $(OBJ) $(COMPHEAD)
	 
# Prevents precompiled headers form beeing deleted	
.PRECIOUS: $(COMPHEAD)
	 
.PHONY: clean, clean_all, debugmode
#.PHONY: all clean force 