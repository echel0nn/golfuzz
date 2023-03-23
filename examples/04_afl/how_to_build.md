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
3) `CC=afl-gcc-fast CXX=afl-g++-fast PKG_CONFIG_PATH=./build ./configure --prefix=$PWD` if fails remove dl dependency from REQUIRED
5) `CC=afl-gcc-fast CXX=afl-g++-fast -I $PWD/../src/ make -j24 all <- actually need only elfparse.c to be compiled, if others fail it's going to be ok
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

