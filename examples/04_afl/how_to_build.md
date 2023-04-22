# Steps

## for oof.c 

1) `gcc -shared -O3 -I/dir/to/AFLplusplus/include/ -w oof.c -o oof.so -fPIC`
2)  `export AFL_CUSTOM_MUTATOR_LIBRARY="/dir/to/oof.so/`

## for binutils (traditional fuzzing)

1) `git clone https://github.com/bminor/binutils-gdb.git`
2) `cd binutils-gdb`
3) `CC=afl-gcc-fast ./configure`
4) `CC=afl-gcc-fast make -j24`
5) example run: `afl-fuzz -D -i samples/ -t 80 -o sync_dir -M fuzzer00 -- /dir/to/binutils-gdb/binutils/readelf -a @@`

## for libelfmaster (traditional fuzzing)

1) `git clone https://github.com/elfmaster/libelfmaster.git`
2) `cd libelfmaster`
3) `CC=afl-cc CXX=afl-c++ CFLAGS="-g -flto=auto" CXXFLAGS=-g CPPFLAGS=-g AR=llvm-ar PKG_CONFIG_PATH=./build ./configure  --prefix=$PWD` if fails remove dl dependency from REQUIRED
5) `CC=afl-cc CXX=afl-c++ CFLAGS="-g -flto=auto" CXXFLAGS=-g CPPFLAGS=-g AR=llvm-ar make -j24 all` `note: do it again in examples/`
5) `afl-fuzz -D -i samples/ -t 80 -o elfmaster_sync_dir -M fuzzer05 -- /home/dante/golfuzz/examples/04_afl/elfparse @@`

## for elfparser  (traditional fuzzing)
1) `git clone https://github.com/jacob-baines/elfparser.git` 
2) `cd elfparser`
3) `mkdir build`
4) `cmake ..`
5) `CC=afl=gcc-fast CXX=afl-g++-fast make -j24` 
6) copy xcellerator's deadbytes.bin example into `samples/`
7) `afl-fuzz -p fast -D -i samples/ -t 80 -o elfmaster_sync_dir -M fuzzer01 -- ./elfparser-cli @@`

## for radare2 (traditional fuzzing)
1) `git clone https://github.com/radareorg/radare2.git`
2) `cd radare2`
3) `CC=afl-gcc-fast CXX=afl-g++-fast ./configure`
4) `CC=afl-gcc-fast CXX=afl-g++-fast make -j24`
5) `afl-fuzz -p explore -D -i samples/ -t 80 -o r2_sync_dir -M fuzzer01 -- /home/dante/golfuzz/examples/04_afl/radare2/binr/radare2/radare2 -qq -AA @@`

## for finixbit/elf-parser (traditional fuzzing)

1) `git clone https://github.com/finixbit/elf-parser.git`
2) Change `Makefile` to

```Makefile
all: sections symbols segments relocations

sections: sections.cc 
	afl-c++ -o sections sections.cc ../elf_parser.cpp -std=gnu++11 

symbols: symbols.cc 
	afl-c++ -o symbols symbols.cc ../elf_parser.cpp -std=gnu++11 

segments: segments.cc 
	afl-c++ -o segments segments.cc ../elf_parser.cpp -std=gnu++11 

relocations: relocations.cc 
	afl-c++ -o relocations relocations.cc ../elf_parser.cpp -std=gnu++11 

clean:
	rm -f sections symbols segments relocations
```
3) `make all`
4) `cd examples` and choose a target
5) `afl-fuzz -p seek -D -i samples/ -t 80 -o sync_dir -M fuzzer01 -- ./relocations @@

## for eliben/pyelftools
1) git clone `https://github.com/jwilk/python-afl`
2) add `import afl; afl.init()` lines to "readelf.py" sample
3) run `py-afl-fuzz -p seek -D -i `deadbytes_samples_dir/ -t 80 -o readelf -M fuzzer05 -- python readelf.py -S @@`

## for lief-project/LIEF 
1) The project is fuzzed via libFuzzer but I couldnt manage to apply afl instrumentations.

## cea-sec/miasm 
1) `CC=afl-cc CXX=afl-c++ CXXFLAGS="-fno-omit-frame-pointer -g -O2 -fsanitize=address,fuzzer-no-link" CPPFLAGS="-fno-omit-frame-pointer -g -O2 -fsanitize=address,fuzzer-no-link" AR=llvm-ar
python setup.py build`
2) `py-afl-fuzz -p rare -D -i ~/sample_dir/ -t 8000 -o miasm_sync -S f7 -- python ./build/lib.linux-x86_64-cpython-310/b.py`
3) ```python
# b.py content
import sys
import os
import signal
from miasm.analysis.binary import Container
from miasm.analysis.machine import Machine
from miasm.core.locationdb import LocationDB
import afl 


def main():
    #sys.stdin.seek(0) 
    fdesc = sys.stdin.buffer.read()
    loc_db = LocationDB()

    # The Container will provide a *bin_stream*, bytes source for the disasm engine
    # It will prodive a view from a PE or an ELF.
    cont = Container.from_string(fdesc, loc_db)

    # The Machine, instantiated with the detected architecture, will provide tools
    # (disassembler, etc.) to work with this architecture
    machine = Machine("x86_64")

    # Instantiate a disassembler engine, using the previous bin_stream and its
    # associated location DB. The assembly listing will use the binary symbols
    mdis = machine.dis_engine(cont.bin_stream, loc_db=cont.loc_db)

    # Run a recursive traversal disassembling from the entry point
    # (do not follow sub functions by default)
    addr = cont.entry_point
    asmcfg = mdis.dis_multiblock(addr)

    os._exit(0)

if __name__ == "__main__":
    signal.signal(signal.SIGCHLD, signal.SIG_IGN)  # this should have no effect on the forkserver
    while afl.loop():
        main()
```
