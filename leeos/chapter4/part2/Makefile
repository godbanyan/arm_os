#Makefile:
#Copyright (C) 2009  david leels <davidontech@gmail.com>

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see http://www.gnu.org/licenses/.
#
TOOL_PREFIX=$(HOME)/arm_os/leeos_tools/bin

CC=$(TOOL_PREFIX)/arm-elf-gcc
LD=$(TOOL_PREFIX)/arm-elf-ld
OBJCOPY=$(TOOL_PREFIX)/arm-elf-objcopy

CFLAGS= -O2 -g
ASFLAGS= -O2 -g
LDFLAGS=-Tleeos.lds -Ttext 30000000 

OBJS=init.o start.o boot.o abnormal.o mmu.o print.o

%.o:%.c
	$(CC) $(CFLAGS) -c $<
%.o:%.s
	$(CC) $(ASFLAGS) -c $<

leeos:$(OBJS)
	$(CC) -static -nostartfiles -nostdlib $(LDFLAGS) $^ -o $@ -lgcc
	$(OBJCOPY) -O binary $@ leeos.bin

run:leeos
	@if [ -n $(echo `uname` | grep -i "cygwin") ]; then \
		skyeye; \
	else \
		skyeye -n; \
	fi

clean:
	rm *.o leeos leeos.bin -f
