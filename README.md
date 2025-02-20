# Efforts to fuzz parsers with ELF binaries that are produced via LibGolf (or based on)

Checkout for the fuzzing strategy at ![oof.c](./examples/04_afl/oof.c)
### To-Do
- [ ] fixed input fuzzing 
### In Scope

- [x] binutils-gdb/readelf (no crash yet)
- [x] libelfmaster [/crashes/](./libelfmaster_findings/libelfmaster_crashes/)
- [x] jacob-baines/elfparser [/crashes/](./elfparser_findings/crashes/)
- [x] radare2 (1 crash) (suggested fix merged: https://github.com/radareorg/radare2/pull/21504)
	- [CVE-2023-1605](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2023-1605)
- [x] finixbit/elf-parser [/crashes/](./finixbit_findings/crashes/)
	- relocations 26 crashes 
- [x] eliben/pyelftools (unhandled e_version but not accepted)
- [ ] lief-project/LIEF (cannot build with afl toolchain, no crashes in dumb mode)
- [ ] cea-sec/miasm (fuzzed with py-afl for the meme, exec/sec was 0.12 ran for 2 days nothing
  found, and at the end my mentality crashed and stopped)
    - [the meme](https://twitter.com/echel0n_1881/status/1649153413664370702)



### FUZZ ALL THE ELF PARSERS!

![sc-main](./examples/04_afl/screenshots/first-run.png)


# Original README
## A Library for Binary Golf

This library helps with Binary Golf. The idea is to get out of your way as soon as possible, and you let you get straight to customizing fields within the ELF and Program header.

Just put your shellcode into an array called `buf[]` in a `shellcode.h` file and use the template below. See the [`examples`](./examples) for more.

Currently Supported:
* `X86_64`
* `ARM32`
* `AARCH64`

```c
// x86_64 Example

#include "libgolf.h"
#include "shellcode.h"

int main(int argc, char **argv)
{
    /*
     * Specify architecture - populate 'ehdr' and 'phdr'
     * Format: INIT_ELF(ISA, ARCH)
     * Supported:
     * - ISA: X86_64, ARM32, AARCH64
     * - ARCH: 32, 64
     */
    INIT_ELF(X86_64,64);

    /*
     * Customize any fields you'd like here.
     */
    ehdr->e_version = 0x13371337

    /*
     * Generate the ELF file and cleanup
     */
    GEN_ELF();
    return 0;
}
```
