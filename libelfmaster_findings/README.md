# Findings

## `1.bin` (ELF64) crashes at `libelfmaster.c:3033 elf_open_object`

```c 
			if (obj->phdr64[i].p_type == PT_NOTE) {
```

## `2.bin` (ELF64) crashes at `libelfmaster.c:2370 elf_segment_iterator_next`

```c 
			segment->memsz = obj->phdr64[iter->index].p_memsz;
```

* insufficient check when e_phnum is bigger than file size 

## `3.bin` (ELF64) crashes at `libelfmaster.c:2366 elf_segment_iterator_next`

```c 
2366                    segment->offset = obj->phdr64[iter->index].p_offset;
```

## `4.bin` (ELF32) crashes at `libelfmaster.c:1911 elf_relocation_iterator_init`

```c 
1911                            unsigned int type = shdr32[i].sh_type;
```

## `5.bin` (ELF32) crashes at `libelfmaster.c:2353 elf_segment_iterator_next`

```c 
2353                    segment->flags = obj->phdr32[iter->index].p_flags;
```


## `6.bin` (ELF32) crashes at `libelfmaster.c:2669 elf_open_object`

```c 
			if (offset_len > obj->size) { // SIGSEGV
```

## `7.bin` (ELF64) crashes at `libelfmaster.c:2366 elf_segment_iterator_next`

```c 
2366                    segment->offset = obj->phdr64[iter->index].p_offset;
```

## `8.bin` (ELF32) crashes at `libelfmaster.c:2353 elf_segment_iterator_next`

```c 
2353                    segment->flags = obj->phdr32[iter->index].p_flags;
```

## [INTERESTING CRASH] `9.bin` (ELF32) erronous section name

* Executable base: 0 Section: 0 
* calling `elf_section_iterator_init(&obj, &s_iter);` rsi is a string ("__pthre")
```c
	iter->index = 0;
	iter->obj = obj;
	ret
```
```c 
0x00005555555577f6      57                      printf("Name: %s\n", section.name ? section.name : "");
► 0x55555555786b <main+770>    call   printf@plt                <printf@plt>
        format: 0x55555557d042 ◂— 'Name: %s\n'
        vararg: 0x80003e48757f

// section object
$9 = {
  name = 0x80003e48757f <error: Cannot access memory at address 0x80003e48757f>,
  type = 978868225,
  link = 4194424,
  info = 0,
  flags = 1482184792,
  align = 0,
  entsize = 1073741824,
  offset = 4063233,
  address = 1482184792,
  size = 1482184822
}
```

## [INTERESTING CRASH] `10.bin` (ELF32) crashes at bugs at reading info from section 73 crashes at section 74's name

```
<..>
Section 73
Name: ELFX
Addr:   0
Off:    0
Size:   0
Info:   1
Flags:
Link:   8488076
Align:  c2da062c
EntSiz: 4490

Section 74
```

```c 
pwndbg> print section // section 74
$11 = {
  name = 0x8000f7fc2fff <error: Cannot access memory at address 0x8000f7fc2fff>,
  type = 4294967295,
  link = 319028959,
  info = 1810769,
  flags = 3994557,
  align = 4571,
  entsize = 0,
  offset = 1678573905,
  address = 24,
  size = 0
}
```

## `11.bin` (ELF64) crashes at `libelfmaster.c:1406 original_ep`

```c 
► 1406                         if (memcmp(&inst[i], GLIBC_START_CODE_64,
   1407                             sizeof(GLIBC_START_CODE_64) - 1) == 0)
```

## [INTERESTING CRASH] `12.bin` (ELF32)

```c 
section_name_cmp (p0=0x7fffffffdd98, p1=0x16000) /libelfmaster/src/internal.c:131
131             return strcmp(s0->name, s1->name); 
```

```c 
pwndbg> print s0
$18 = (const struct elf_section *) 0x7fffffffdda0
pwndbg> print *s0
$19 = {
  name = 0x55555557d3ea ".dynamic",
  type = 0,
  link = 0,
  info = 0,
  flags = 0,
  align = 0,
  entsize = 0,
  offset = 0,
  address = 0,
  size = 0
}
pwndbg> print *s1
Cannot access memory at address 0x16000 //
```


## `13.bin` (ELF32) crashes at 
```c 
0x000055555556c008 in original_ep (obj=obj@entry=0x7fffffffe0d0) at /home/dante/golfuzz/examples/04_afl/libelfmaster/src/internal.c:1413
► 1413                         if (memcmp(&inst[i], GLIBC_START_CODE_32,
   1414                             sizeof(GLIBC_START_CODE_32) - 1) == 0) 
```

## `14.bin` (ELF64) crashes at 
```c 
resolve_plt_addr (obj=obj@entry=0x7fffffffe0d0) at /home/dante/golfuzz/examples/04_afl/libelfmaster/src/internal.c:1348
1348                    if (*inst != 0x48 && *(inst + 1) != 0x83)
``` 

