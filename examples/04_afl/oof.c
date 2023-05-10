/*
   AFL++ Custom Mutator for LibGolf
   Written by @echel0n
   $ gcc -O3 -fPIC -shared -o golf_mutator.so -I ~/AFLplusplus/include/ oof.c 
 */
#include "libgolf.h"
#include "shellcode.h"
#include "afl-fuzz.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define uchar unsigned char
#define DATA_SIZE 0x100

typedef struct my_mutator {
  afl_state_t *afl;
  // any additional data here!
  size_t trim_size_current;
  int trimmming_steps;
  int cur_step;
  u8 *mutated_out, *post_process_buf, *trim_buf;
} my_mutator_t;

my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {
  srand(seed); // needed also by surgical_havoc_mutate()
  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));
  if (!data) {
    perror("afl_custom_init alloc");
    return NULL;
  }
  if ((data->mutated_out = (u8 *)malloc(MAX_FILE)) == NULL) {
    perror("afl_custom_init malloc");
    return NULL;
  }
  if ((data->post_process_buf = (u8 *)malloc(MAX_FILE)) == NULL) {
    perror("afl_custom_init malloc");
    return NULL;
  }
  if ((data->trim_buf = (u8 *)malloc(MAX_FILE)) == NULL) {
    perror("afl_custom_init malloc");
    return NULL;
  }
  data->afl = afl;
  return data;
}

size_t afl_custom_fuzz(my_mutator_t *data, uint8_t *in_buf, size_t buf_size,
                       u8 **out_buf, uint8_t *add_buf,
                       size_t add_buf_size, // add_buf can be NULL
                       size_t max_size) {

  RawBinary elf_obj;
  RawBinary *elf = &elf_obj;
  elf->isa = 62;
  Elf64_Ehdr *ehdr;
  Elf64_Phdr *phdr;
  copy_text_segment(elf, buf, sizeof(buf));
  ehdr = populate_ehdr(elf);
  phdr = populate_phdr(elf);
  set_entry_point(elf);

  size_t mutated_size = ehdr_size + phdr_size + elf->text.text_size;
  int pos = 0;

  ehdr->e_ident[EI_CLASS] = (uint8_t *)(in_buf + pos);
  pos = pos + 1;
  ehdr->e_ident[EI_DATA] = (uint8_t *)(in_buf + pos);
  pos = pos + 1;
  ehdr->e_ident[EI_VERSION] = (uint8_t *)(in_buf + pos);
  pos = pos + 1;
  ehdr->e_ident[EI_OSABI] = (uint8_t *)(in_buf + pos);
  pos = pos + 1;
  for (int i = 0x8; i < 0x10; ++i) {
    (ehdr->e_ident)[i] = (uint8_t *)(in_buf + pos);
    pos = pos + 1;
  }

  ehdr->e_version = (uint32_t *)(in_buf + pos);
  pos = pos + 4;
  // sections headers
  ehdr->e_shoff = (uint64_t *)(in_buf + pos);
  pos = pos + 8;
  ehdr->e_shentsize = (uint16_t *)(in_buf + pos);
  pos = pos + 2;
  ehdr->e_shnum = (uint16_t *)(in_buf + pos);
  pos = pos + 2;
  ehdr->e_shstrndx = (uint16_t *)(in_buf + pos);
  pos = pos + 2;
  ehdr->e_flags = (uint32_t *)(in_buf + pos);
  pos = pos + 4;
  // physical addr
  phdr->p_paddr = (uint64_t *)(in_buf + pos);
  pos = pos + 8;
  phdr->p_align = (uint64_t *)(in_buf + pos);
  pos = pos + 8;

  /* let's mimic GEN_ELF()
   * Write:
   * - ELF Header
   * - Program Header
   * - Text Segment
   */
  memcpy(data->mutated_out, ehdr, ehdr_size);
  memcpy(data->mutated_out + ehdr_size, phdr, phdr_size);
  memcpy(data->mutated_out + ehdr_size + phdr_size, elf->text.text_segment,
         elf->text.text_size);

  *out_buf = data->mutated_out;
  return mutated_size;
}
void afl_custom_deinit(my_mutator_t *data) {
  free(data->post_process_buf);
  free(data->mutated_out);
  free(data->trim_buf);
  free(data);
}
