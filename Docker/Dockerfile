FROM debian:stable-slim


RUN apt update
RUN apt install grub-pc-bin mtools xorriso -y


WORKDIR /iso

CMD ["grub-mkrescue", "-o", "kfs2.iso", "."]