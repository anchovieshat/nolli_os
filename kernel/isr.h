#ifndef _ISR_H
#define _ISR_H

#include "common.h"

typedef struct __attribute__((packed)) {
	u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32 int_no, err_code;
} registers_t;

bool shifting = false;
char key_map[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	'0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '0',
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
};

char to_upper(char c) {
	return c - 32;
}

void isr_handler(registers_t reg) {
	printf("Got interrupt %p: ", reg.int_no);

	if (reg.int_no == 0xE) {
		printf("page fault!\n");
		u8 fault_type = (reg.err_code << 4) >> 4;

		if (fault_type & 0b0001) {
			printf("page not present\n");
		} else {
			printf("page protection violation\n");
		}

		if (fault_type & 0b0010) {
			printf("fault on write\n");
		} else {
			printf("fault on read\n");
		}
		asm ("hlt");
	}
}

void irq_handler(registers_t reg) {
    u8 scan_code = inb(0x60);
	if (scan_code & 0x80) {
		if (scan_code == 170) {
			shifting = false;
		}
	} else {
		if (scan_code == 42) {
			shifting = true;
		}

		if (scan_code == 72) {
			y_pos--;
		} else if (scan_code == 75) {
			x_pos--;
		} else if (scan_code == 77) {
			x_pos++;
		} else if (scan_code == 80) {
			y_pos++;
		}

		update_cursor();

		char c = key_map[scan_code];
        if (shifting) {
			c = to_upper(c);
		}
		putc(c);
	}


	outb(0x20, 0x20);
	if(reg.int_no >= 8) {
		outb(0xA0, 0x20);
	}
}

#endif