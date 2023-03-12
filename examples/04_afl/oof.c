/*
   AFL++ Custom Mutator for LibGolf
   Written by @echel0n
   You need to use -I/path/to/AFLplusplus/include -I.
 */
#include "../../libgolf.h"
#include "shellcode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define uchar unsigned char

typedef struct post_state {
  uchar *buf;
  size_t size;
} post_state_t;

void *afl_custom_init(void *afl) {
  post_state_t *state = malloc(sizeof(post_state_t));
  if (!state) {
    perror("malloc");
    return NULL;
  }

  state->buf = calloc(sizeof(unsigned char), 4096);
  if (!state->buf) {

    free(state);
    perror("calloc");
    return NULL;
  }

  return state;
}

/*
 *
 * NEEDS TO BE FIXED: not producing ELFs that are loadable by Linux Loader
 */
size_t afl_custom_post_process(post_state_t *data, unsigned char *in_buf,
                               unsigned int len, unsigned char **out_buf) {

  if (len < 126)
    return 0; // libgolf's smallest bin

  // need to keep the position
  unsigned int pos = 0;
  unsigned char *new_buf = malloc(sizeof(RawBinary));
  INIT_ELF(X86_64, 64);

  ehdr->e_ident[EI_CLASS] = (uint8_t *)(in_buf + pos);
  ++pos;
  ehdr->e_ident[EI_DATA] = (uint8_t *)(in_buf + pos);
  ++pos;
  ehdr->e_ident[EI_VERSION] = (uint8_t *)(in_buf + pos);
  ++pos;
  ehdr->e_ident[EI_OSABI] = (uint8_t *)(in_buf + pos);
  ++pos;
  for (int i = 0; i < 0x10; ++i) {
    (ehdr->e_ident)[i] = (uint8_t *)(in_buf + pos);
    ++pos;
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

  // let's mimic GEN_ELF()
  /*
   * Write:
   * - ELF Header
   * - Program Header
   * - Text Segment
   */
  memcpy(new_buf, ehdr, ehdr_size);
  memcpy(new_buf + ehdr_size, phdr, phdr_size);
  memcpy(new_buf + ehdr_size + phdr_size, elf->text.text_segment,
         elf->text.text_size);
  *out_buf = new_buf;
  return ehdr_size + phdr_size + elf->text.text_size;
}
void afl_custom_deinit(post_state_t *data) {
  free(data->buf);
  free(data);
}
