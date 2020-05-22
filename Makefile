target_all = sqrt sqrt_ut
all: $(target_all)

DIR_BUILD = build

#=============================================================================
# There are two compilation options
sqrt_D_DEBUG = -D IMGRASS_DEBUG
sqrt_D_UT = -D IMGRASS_UT

sqrt: sqrt.c
ifdef debug
	gcc -o $(DIR_BUILD)/$@ $^ $(sqrt_D_DEBUG)
else
	gcc -o $(DIR_BUILD)/$@ $^
endif

sqrt_ut: sqrt.c
	gcc -o $(DIR_BUILD)/$@ $^ $(sqrt_D_UT) -lm

#=============================================================================
clean:
	rm -f *.o
	find $(DIR_BUILD)/ -maxdepth 1 -perm /a+x -type f -exec rm -f {} \;
