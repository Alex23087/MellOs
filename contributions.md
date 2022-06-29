## Contributions
### 27 APRIL 2022 - Mastcharub:
1. Added a basic GDT for the Kernel instead of using the one of the Bootloader
1. Updated the path for the IDT and IRQ (Interrupts stuff)
---
### 5 JUNE 2022 - Mastcharub:
1. Added a simple Ata HDD (Hard Disk Drive) Polling Driver using PIO Mode (instead of the better DMA)
1. Added `outw`, `inw`, `outl`, `inl` functions to `port_io.cpp`
---
### 29 JUNE 2022 - MAstcharub:
1. Fixed Write Sector Function in the Disk Driver.
1. Added `identify_ata` function in the Disk Driver.

TODO: Add Shell Comand for `identify_ata`.

---