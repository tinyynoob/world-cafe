cc = gcc
ccflags = -O2

world_cafe: world_cafe.c permutation.c semi_sudoku.c
	$(cc) $(ccflags) $^ -o $@

clean:
	-rm world_cafe world_cafe.csv world_cafe.tmp