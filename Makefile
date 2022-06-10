cc = gcc
header = permutation.h semi_sudoku.h ffs.h ifs.h
source = world_cafe.c permutation.c semi_sudoku.c
ccflags = -Wall -O2

.PHONY: clean windows run format

run: world_cafe
	./$<

world_cafe: $(source) $(header)
	$(cc) -o $@ world_cafe.c permutation.c semi_sudoku.c $(ccflags)

windows: $(source) $(header)
	x86_64-w64-mingw32-gcc -o world_cafe $(source) $(ccflags)

format: $(header) $(source)
	clang-format -i $^

clean:
	-rm world_cafe world_cafe.csv world_cafe.tmp
	-rm world_cafe.exe