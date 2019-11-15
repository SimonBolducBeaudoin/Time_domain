# Toolchain, using mingw on windows
CC = $(OS:Windows_NT=x86_64-w64-mingw32-)gcc
PKG_CFG = $(OS:Windows_NT=x86_64-w64-mingw32-)pkg-config

TARGET = fresnel.o
SUB_DIR  = fresnel
SRC  = $(wildcard $(SUB_DIR)/*.c)
OBJ  = $(patsubst %.c, %.o, $(SRC))

# flags
CFLAGS = -Ofast -march=native -Wall 
SHRFLAGS = -fPIC -shared

# libraries
LDLIBS = -lm

$(TARGET): $(OBJ)
	@ echo " "
	@ echo "---------Compile library $(TARGET)---------"
	$(CC) $(SHRFLAGS) -o $(TARGET) $(OBJ) $(CFLAGS) $(LDLIBS)
	
$(SUB_DIR)/%.o : $(SUB_DIR)/%.c
	@ echo " "
	@ echo "---------Compile object $@ from $<--------"
	$(CC) $(SHRFLAGS) -c -Wall -o $@ $< $(CFLAGS) $(FFTWFLAGS) $(OMPFLAGS) $(LDLIBS)

clean:
	@rm -f $(TARGET) $(OBJ)

.PHONY: all clean force 