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


CC          = gcc
CCN         = nasm
FLAG        = -MMD -Wall -g3 -Werror -Wextra -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs
FLAGNASM    = -f


DIR_HEADER  = headers/
SRC_PATH    = C/src/
NASM_PATH   = NASM/
OBJ_PATH    = .obj/
DEP_PATH    = .dep/
OBJNASM_PATH= .objnasm/


SRC     =   main.c  printk.c utils.c kernel.c input.c print_message.c init.c commande.c
NASM    =   boot.asm nasm_utils.asm #GDT.asm

OBJ     =   $(SRC:.c=.o)
OBJS    =   $(addprefix $(OBJ_PATH), $(OBJ))

DEP     =   $(SRC:.c=.d)
DEPS    =   $(addprefix $(DEP_PATH), $(DEP))

OBJNASM =   $(NASM:.asm=.o)
OBJNASMS=   $(addprefix $(OBJNASM_PATH), $(OBJNASM))


RM      =   rm -f
RMDIR   =   rm -rf
MKDIR   =   mkdir -p


#########################
#        INCLUDES       #
#########################


$(NAME): $(OBJS) $(OBJNASMS)
	$(call green,"Compilation de kfs...")
	@ld -m elf_i386 -T linker/linker.ld -o $(NAME).bin $(OBJS) $(OBJNASMS)
	$(call purple,"Build terminé avec succès!")


#########################
#       BUILD RULES     #
#########################

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(MKDIR) $(OBJ_PATH)
	@$(CC) $(FLAG) -c $< -o $@ -I $(DIR_HEADER)
	@$(call green,"$< ✅")


$(OBJNASM_PATH)%.o: $(NASM_PATH)%.asm
	@$(MKDIR) $(OBJNASM_PATH)
	@$(CCN) $(FLAGNASM) elf32 $< -o $@ 
	@$(call green,"$< ✅")


build_iso:
	docker build -t iso_maker ./Docker
	$(MKDIR) ./iso/boot/grub
	cp $(NAME).bin iso/boot/$(NAME).bin
	cp grub.cfg iso/boot/grub/grub.cfg
	docker run --mount type=bind,source=./iso,target=/iso iso_maker



#########################
#       CLEAN RULES     #
#########################


clean:
	@$(RMDIR) .obj
	@$(RMDIR) .objnasm
	@$(call yelow,"clean kfs ok ✅")


#########################
#      FCLEAN RULES     #
#########################


fclean:
	@$(RM) $(NAME)
	@$(RMDIR) .obj
	@$(RMDIR) .objnasm
	@$(RMDIR) iso/
	@$(RM) $(NAME).bin
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
debug: fclean $(NAME) build_iso run_debug 


.PHONY: all clean fclean re all