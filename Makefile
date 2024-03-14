GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = src/kernel/kernel.o src/kernel/gdt.o src/boot/loader.o

%.o: %.c
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.asm
	as $(ASPARAMS) -o $@ $<

shinobi.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: shinobi.bin 
	sudo cp $< /boot/shinobi.bin

shinobi.iso: shinobi.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Shinobi" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/shinobi.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -f $(objects) shinobi.bin shinobi.iso
