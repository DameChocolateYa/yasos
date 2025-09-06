
TARGET = libys.so

CC = gcc
AS = gcc
YS = yasos
CFLAGS = -Wall -Wextra -O2 -fPIC -g -L/usr/lib/yslib -lys
LDFLAGS = -shared

SRC_C       = $(wildcard *.c)
SRC_S_MANUAL= $(filter-out $(SRC_YS:.ys=.s), $(wildcard *.s))
SRC_YS      = $(wildcard *.ys)
SRC_S_GEN   = $(SRC_YS:.ys=.s)

OBJ_C       = $(SRC_C:.c=.o)
OBJ_S_MANUAL= $(SRC_S_MANUAL:.s=.o)
OBJ_YS      = $(SRC_YS:.ys=.o)

OBJ = $(OBJ_C) $(OBJ_S_MANUAL) $(OBJ_YS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compilación de C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilación de ensamblador escrito a mano
%.o: %.s
	$(AS) -fPIC -c $< -o $@

# Generación de .s a partir de .ys
%.s: %.ys
	$(YS) -S $<

# Los .o de YASOS dependen de los .s generados
%.o: %.ys
	$(YS) -S $<
	$(AS) -fPIC -c $*.s -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(SRC_S_GEN)

distclean: clean

.PHONY: all clean distclean
