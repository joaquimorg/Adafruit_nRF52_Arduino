/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{  
  FLASH (rx) : ORIGIN = 0x27000, LENGTH = 0xED000 - 0x27000
  RAM (rwx)  : ORIGIN = 0x20002D80, LENGTH = 0x20040000 - 0x20002D80
}

SECTIONS
{
  . = ALIGN(4);
  .svc_data :
  {
    PROVIDE(__start_svc_data = .);
    KEEP(*(.svc_data))
    PROVIDE(__stop_svc_data = .);
  } > RAM
  
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
} INSERT AFTER .data;

INCLUDE "nrf52_common.ld"
