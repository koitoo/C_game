#
# How to use this Makefile with make command
# make NAME=kadai
#

NAME	= game

all: $(NAME).mb

$(NAME).s: $(NAME).c
	arm-thumb-elf-gcc -S $(NAME).c

$(NAME).o: $(NAME).s
	arm-thumb-elf-as -o $(NAME).o $(NAME).s

$(NAME).out: $(NAME).o
	arm-thumb-elf-ld -Ttext 0x02000000 -o $(NAME).out $(NAME).o

$(NAME).mb: $(NAME).out
	arm-thumb-elf-objcopy -O binary $(NAME).out $(NAME).mb

clean:
	rm *.o *.s *.out *.mb *.BAK

