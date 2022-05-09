cc = gcc
ccflags = -O2

world_cafe: world_cafe.c permutation.c
	$(cc) $(ccflags) $^ -o $@

clean:
	-rm world_cafe