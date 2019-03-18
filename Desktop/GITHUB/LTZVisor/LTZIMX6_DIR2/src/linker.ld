OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)


MEMORY
{
    OCRAM  (rwx) : ORIGIN = 0x00900000, LENGTH = 256K
    DDR_NS (rwx) : ORIGIN = 0x10000000, LENGTH = 0x1FFFFFFF
    DDR_S  (rwx) : ORIGIN = 0x30000000, LENGTH = 0xCFFFFFFF
}

SECTIONS
{

    /* RAM vector table comes at the end of OCRAM */
    .ram_vectors (ORIGIN(OCRAM) + LENGTH(OCRAM) - 72) (NOLOAD) :
    {
        __ram_vectors_start = .;
        . += 72;
        __ram_vectors_end = .;
    } > OCRAM
/*
 * -- DDR --
 */
    /* -- read-only sections -- */
    _start_image_add = ORIGIN(DDR_S);

    .ivt (ORIGIN(DDR_S)) :
    {
        . = . + 0x400;
        *(.ivt)
    } > DDR_S

    .boot_data :
    {
        __start_boot_data = .;
        *(.boot_data)
    } > DDR_S

    /* aligned to ease the hexdump read of generated binary */
    .dcd_hdr : ALIGN(16)
    {
        __start_dcd = .;
        *(.dcd_hdr)
    } > DDR_S
    .dcd_wrt_cmd :
    {
        *(.dcd_wrt_cmd)
    } > DDR_S
    .dcd_data :
    {
        *(.dcd_data)
    } > DDR_S

    .text : ALIGN(8)
    {
        *(.startup)
        *(.text)
        *(.freertos_vectors)
        *(.normal_image1)

	nsguest1_bin_start = .;
		*(.ns_guest1)
	nsguest1_bin_end = .;

	nsguest2_bin_start = .;
		 *(.ns_guest2)
	nsguest2_bin_end = .;

    } > DDR_S

    .rodata :
    {
        *(.rodata)

    } > DDR_S

    .data : ALIGN(8)
    {
        *(.data)
    } > DDR_S

    /* Reserve some space for HAB CSF data */
    .hab.data (_start_image_add + 40M) : ALIGN(4)
    {
        __hab_data = .;
        . += 8K;
        . = ALIGN (32);
        __hab_data_end = .;
    } > DDR_S
    /* End of HAB reserved space (must place it before BSS section) */
    _image_size = . - _start_image_add;

    /* The .bss section comes after the hab data because it is not signed */
    .bss :
    {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    } > DDR_S

    /* Region to contain exception handler stacks */
    .stacks (NOLOAD) :
    {
        __stacks_start = .;
        . += 48K;
        __stacks_end = .;
        top_of_stacks = .;
    } > DDR_S

    _end = .;
}
