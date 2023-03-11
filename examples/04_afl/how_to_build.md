# Steps

## for oof.c 

1) `gcc -shared -O3 -I/dir/to/AFLplusplus/include/ -w oof.c -o oof.so -fPIC`
2)  `export AFL_CUSTOM_MUTATOR_LIBRARY="/dir/to/oof.so/`

## for binutils

1) `git clone https://github.com/bminor/binutils-gdb.git`
2) `cd binutils-gdb`
3) `CC=afl-gcc-fast ./configure`
4) `CC=afl-gcc-fast make -j24`
5) example run: `afl-fuzz -D -i samples/ -t 80 -o sync_dir -M fuzzer00 -- /dir/to/binutils-gdb/binutils/readelf -a @@`

## for libelfmaster

1) `git clone https://github.com/elfmaster/libelfmaster.git`
2) `cd libelfmaster`
3) `CC=afl-gcc-fast CXX=afl-g++-fast PKG_CONFIG_PATH=./build ./configure --prefix=$PWD` if fails remove dl dependency from REQUIRED
5) `CC=afl-gcc-fast CXX=afl-g++-fast -I $PWD/../src/ make -j24 all <- actually need only elfparse.c to be compiled, if others fail it's going to be ok
5) `afl-fuzz -D -i samples/ -t 80 -o elfmaster_sync_dir -M fuzzer05 -- /home/dante/golfuzz/examples/04_afl/elfparse @@`

## for elfparser 
1) `git clone https://github.com/jacob-baines/elfparser.git` 
2) `cd elfparser`
3) `mkdir build`
4) `cmake ..`
5) `CC=afl=gcc-fast CXX=afl-g++-fast make -j24` 
6) copy xcellerator's deadbytes.bin example into `samples/`
7) `afl-fuzz -p fast -D -i samples/ -t 80 -o elfmaster_sync_dir -M fuzzer01 -- ./elfparser-cli @@`
