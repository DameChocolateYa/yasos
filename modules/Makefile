#
 # yasos - a compiler for yasos language
 # Copyright (c) 2025 DameChocolateYa
 # Licensed under the BSD 3-Clause License.
 # See LICENSE file in the project root for full license text.
#

TARGET = libys.so

CC = gcc
AS = gcc
YS = yasos
CFLAGS = -Wall -Wextra -O0 -fPIC -g -L/usr/lib/yslib -lys -fvisibility=hidden
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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) -fPIC -c $< -o $@

%.s: %.ys
	$(YS) -S $<

%.o: %.ys
	$(YS) -S $<
	$(AS) -fPIC -c $*.s -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(SRC_S_GEN)

distclean: clean

.PHONY: all clean distclean
