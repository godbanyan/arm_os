/*
mmu.c:
Copyright (C) 2009  david leels <davidontech@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
*/
#ifdef TEST_MMU
#include <stdio.h>
#endif

#define PTE_BITS_L1_SECTION				(0x2)
#define PTE_L1_SECTION_PADDR_BASE_MASK	(0xfff00000)
#define PAGE_TABLE_L1_BASE_ADDR_MASK	(0xffffc000)
#define VIRT_TO_PTE_L1_INDEX(addr)		(((addr) & 0xfff00000) >> 18)

#define RPI2

#ifdef RPI2

#define PTE_L1_SECTION_NO_CACHE_AND_WB	(0x0<<2)
#define PTE_L1_SECTION_DOMAIN_DEFAULT	(0x0<<5)
#define PTE_ALL_AP_L1_SECTION_DEFAULT	(0x1<<10)

#define L1_PTR_BASE_ADDR			0x30700000
#define PHYSICAL_MEM_ADDR			0x30000000
#define VIRTUAL_MEM_ADDR			0x30000000
#define MEM_MAP_SIZE				0x00800000
#define PHYSICAL_IO_ADDR			0x48000000
#define VIRTUAL_IO_ADDR				0xc8000000
#define IO_MAP_SIZE					0x18000000
#else
/*mask for page table base addr*/


#define PTE_L1_SECTION_NO_CACHE_AND_WB	(0x0<<2)
#define PTE_L1_SECTION_DOMAIN_DEFAULT	(0x0<<5)
#define PTE_ALL_AP_L1_SECTION_DEFAULT	(0x1<<10)

#define L1_PTR_BASE_ADDR			0x30700000
#define PHYSICAL_MEM_ADDR			0x30000000
#define VIRTUAL_MEM_ADDR			0x30000000
#define MEM_MAP_SIZE				0x800000
#define PHYSICAL_IO_ADDR			0x48000000
#define VIRTUAL_IO_ADDR				0xc8000000
#define IO_MAP_SIZE					0x18000000
#endif

#ifndef TEST_MMU
void start_mmu(void){
	unsigned int ttb = L1_PTR_BASE_ADDR;

	asm (
		"mcr p15,0,%0,c2,c0,0\n"    /* set base address of page table*/
		"mvn r0,#0\n"                  
		"mcr p15,0,r0,c3,c0,0\n"    /* enable all region access*/

		"mov r0,#0x1\n"
		"mcr p15,0,r0,c1,c0,0\n"    /* set back to control register */
		"isb\n"
		: :"r" (ttb) :"r0"
	);
}
#endif

unsigned int gen_l1_pte(unsigned int paddr)
{
	return (paddr & PTE_L1_SECTION_PADDR_BASE_MASK);
}

unsigned int gen_l1_pte_addr(unsigned int baddr, unsigned int vaddr)
{
	return (baddr & PAGE_TABLE_L1_BASE_ADDR_MASK) | VIRT_TO_PTE_L1_INDEX(vaddr);
}


void mmap(unsigned va, unsigned pa, unsigned sz)
{
	unsigned pte, pte_addr, i;
	unsigned sec = sz >> 20;

	for (i = 0; i < sz >> 20; i++) {
		pte = gen_l1_pte(pa + (i << 20));
		pte |= PTE_BITS_L1_SECTION;
		pte |= PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte |= PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte |= PTE_L1_SECTION_DOMAIN_DEFAULT;

		pte_addr = gen_l1_pte_addr(L1_PTR_BASE_ADDR, va + (i << 20));
#ifdef TEST_MMU
		printf("%d ## pte_addr: %x, pte: %x\n", i, pte_addr, pte);
#else
		*(volatile unsigned int *)pte_addr = pte;
#endif
	}
}
		


void init_sys_mmu(void)
{
	unsigned pte;
	unsigned pte_addr;
	int j;

	mmap(VIRTUAL_MEM_ADDR, PHYSICAL_MEM_ADDR, MEM_MAP_SIZE);
	mmap(VIRTUAL_IO_ADDR, PHYSICAL_IO_ADDR, IO_MAP_SIZE);
}

#ifdef TEST_MMU
int main(int argc, char *argv[])
{

  printf("L1_PTR_BASE_ADDR: %x\n", L1_PTR_BASE_ADDR);
  init_sys_mmu();
  
  return 0;
}
#endif
