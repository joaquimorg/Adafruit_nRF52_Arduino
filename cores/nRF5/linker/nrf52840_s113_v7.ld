/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{  
  FLASH (rx) : ORIGIN = 0x1C000, LENGTH = 0xED000 - 0x1C000

  /* Because of SPIM3 anomoly, SoftDevice RAM size must be multiple of 8K of at most 56K */
  SOFTDEVICE_RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x2F10
  
  /* SPIM3 needs a dedicated physical RAM block that won't be touched by the CPU during DMA. */
  SPIM3_RAM (rw) : ORIGIN = 0x20002F10, LENGTH = 0x2000
  
  /* Application RAM size is 256K - SoftDevice RAM - 8K */
  RAM (rwx) :  ORIGIN = 0x20004F10, LENGTH = 0x20040000 - 0x20004F10

}

SECTIONS
{
    .spim3 :
    {
        PROVIDE(spim3_rx_buffer = .);
        PROVIDE(spim3_tx_buffer = . + LENGTH(SPIM3_RAM) / 2);
    } > SPIM3_RAM
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
