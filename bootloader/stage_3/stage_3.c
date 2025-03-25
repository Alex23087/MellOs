#include "stage_3_miniheader.h"
#include "floppy.h"

#define STATUS_BSY      0x80
#define STATUS_RDY      0x40
#define STATUS_DRQ      0x08
#define STATUS_DF       0x20
#define STATUS_ERR      0x01


// TODO implement something on the lines of "took too long to respond"
void wait_BSY(){
	while(inb(0x1F7) & STATUS_BSY){;};
}

void wait_DRQ(){
	while(!(inb(0x1F7) & STATUS_RDY)){;};
}

bool check_ERR(){
	return ((inb(0x1F7) & STATUS_ERR) != 0);
}


void check_ata_error(void) {
    uint8_t status = inb(0x1F7); // Read the status register

    // Check the ERR bit (bit 0)
    if (status & 0x01) {
        uint8_t error = inb(0x1F1); // Read the error register
        // kprint("ATA command error: status = 0x");
        // kprint(tostring_inplace(status, 16));
        // kprint(", error = 0x");
        // kprint(tostring_inplace(error, 16));
        // kprint("\n");
        // ata_print_error(error);
    } else {
        // kprint("ATA command completed successfully, status = 0x");
        // kprint(tostring_inplace(status, 16));
        // kprint("\n");
    }
}

void LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr){
	LBA = LBA & 0x0FFFFFFF;
	
    wait_BSY();
    outb(0x1F6, drive | ((LBA >> 24) & 0xF));
	outb(0x1F1, 0x00);
    outb(0x1F2, sector);
    outb(0x1F3, (uint8_t) LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x20); // 0x20 = 'Read' Command

	
	uint16_t *tmp = addr;
	
    for (int j = 0; j < sector; j ++){
		wait_BSY();
		wait_DRQ();
		for(int i = 0; i < 256; i++){
            tmp[i] = inw(0x1F0);
        }

		tmp += 256;
	}
}



void main(){
	init_floppy();
    kprint("banana");
    while(1);
    LBA28_read_sector(0xA0, 1, 16, 0x9000);
}