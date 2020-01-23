# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)g++
PKG_CFG = $(OS:Windows_NT=x86_64-w64-mingw32-)pkg-config

TARGET = math_extra.a
SRC  = $(wildcard *.cpp)
OBJ  = $(patsubst %.cpp, %.o, $(SRC))
DEPS = $(OBJ:.o=.d)

# flags
CFLAGS = -Ofast -march=native -MMD -MP -Wall $(OS:Windows_NT=-DMS_WIN64 -D_hypot=hypot)
SHRFLAGS = -fPIC -shared

# libraries
LDLIBS = -lm 
	
$(TARGET) : $(OBJ)
	@ echo " "
	@ echo "---------Compiling static library $(TARGET)---------"
	ar cr $(TARGET) $(OBJ) $(EXTERNAL_OBJ) 

%.o : %.cpp
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) $(SHRFLAGS) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)

-include $(DEPS)

clean:
	@rm -f $(TARGET) $(OBJ)

.PHONY: all clean force 