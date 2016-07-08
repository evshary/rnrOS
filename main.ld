ENTRY(main)
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}


SECTIONS
{
	.text :
	{
        . = ALIGN(4);
		KEEP(*(.isr_vector))
 		*(.text)
 		*(.text.*)
		*(.rodata)
		*(.rodata*)
		_sromdev = .;
		*(.rom.*)
		_eromdev = .;
        . = ALIGN(4);
        _sidata = .;
	} >FLASH

	/* Initialized data will initially be loaded in FLASH at the end of the .text section. */
	.data : AT(_sidata)
	{
        . = ALIGN(4);
		_sdata = .;
		*(.data)		/* Initialized data */
		*(.data*)
		_edata = .;
        . = ALIGN(4);
	} >RAM

	.bss :
	{
        . = ALIGN(4);
		_sbss = .;
		*(.bss)         /* Zero-filled run time allocate data memory */
		_ebss = .;
        . = ALIGN(4);
	} >RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
