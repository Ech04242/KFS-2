#nasm -felf32 ./Amorcage/boot.asm -o boot.o

#gcc -m32 -fno-builtin -fno-exception -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs






#########################
#      WRITE FUNCT      #
#########################

grey  = /bin/echo -e "\x1b[30m$1\x1b[0m"
red   = /bin/echo -e "\x1b[31m$1\x1b[0m"
green = /bin/echo -e "\x1b[32m$1\x1b[0m"
yelow = /bin/echo -e "\x1b[33m$1\x1b[0m"
blue  = /bin/echo -e "\x1b[34m$1\x1b[0m"
purple= /bin/echo -e "\x1b[35m$1\x1b[0m"
cyan  = /bin/echo -e "\x1b[36m$1\x1b[0m"
white = /bin/echo -e "\x1b[37m$1\x1b[0m"


#########################
#      DEFINE SRCS      #
#########################

NAME = kfs2

CC = gcc
FLAG = -MMD -Wall -g3 -Werror -Wextra -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs

DIR_HEADER  = headers/
SRC_PATH = C/src/
SRC =	main.c	printk.c utils.c kernel.c input.c print_message.c init.c
OBJ_PATH	=	.obj/
OBJ		=	$(SRC:.c=.o)
OBJS	=	$(addprefix $(OBJ_PATH), $(OBJ))

DEP_PATH	=	.dep/
DEP		=	$(SRC:.c=.d)
DEPS	=	$(addprefix $(DEP_PATH), $(DEP))

RM		=	rm -f
RMDIR	=	rm -rf
MKDIR	=	mkdir -p

#########################
#        INCLUDES       #
#########################

$(NAME): $(OBJS)
	@nasm -f elf32 NASM/boot.asm -o .obj/boot.o
	@nasm -f elf32 NASM/utils.asm -o .obj/nasm_utils.o
	$(call green,"Compilation de kfs...")
	@ld -m elf_i386 -T linker/linker.ld -o $(NAME).bin .obj/boot.o .obj/nasm_utils.o $(OBJS)
	$(call purple,"Build terminé avec succès!")


#########################
#       BUILD RULES     #
#########################

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(FLAG) -c $< -o $@ -I $(DIR_HEADER)
	@$(call green,"$< ✅")

build_iso:
	docker build -t iso_maker ./Docker
	mkdir -p ./iso/boot/grub
	cp $(NAME).bin iso/boot/$(NAME).bin
	cp grub.cfg iso/boot/grub/grub.cfg
	docker run --mount type=bind,source=./iso,target=/iso iso_maker


#########################
#       CLEAN RULES     #
#########################

clean:
	@rm -rf .obj
	@$(call yelow,"clean kfs ok ✅")

#########################
#      FCLEAN RULES     #
#########################

fclean:
	@rm -f $(NAME)
	@rm -rf .obj
	@rm -rf iso/
	@rm -f $(NAME).bin
	@$(call yelow,"fclean kfs ✅")

#######################
#       RE RULES      #
#######################

re: fclean
	@make

#######################
#      ALL RULES      #
#######################

run:
	qemu-system-i386 -kernel $(NAME).bin

run_iso:
	qemu-system-i386 -cdrom ./iso/$(NAME).iso

run_debug:
	qemu-system-i386 -kernel $(NAME).bin -s -S & gdb -x .gdbinit

all: fclean $(NAME) build_iso run_iso

.PHONY: all clean fclean re all