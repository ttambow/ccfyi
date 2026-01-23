subdirs := $(notdir $(shell find . -mindepth 1 -maxdepth 1 -type d))
dir_goals := $(filter $(subdirs),$(MAKECMDGOALS))

all:
	@for dir in $(subdirs); do				 \
		if [ -f $$dir/makefile ]; then		 \
			echo "main | entering $$dir"	;\
			$(MAKE) -C $$dir				;\
		fi									;\
	done

# 'make clean' - checks all subdirs by default
ifeq ($(dir_goals),)
clean:
	@for dir in $(subdirs); do				 \
		if [ -f $$dir/makefile ]; then		 \
			echo "main | cleaning $$dir"	;\
			$(MAKE) -C $$dir clean			;\
		fi									;\
	done
else
clean:
	@true # falls through to `$(subdir)`
endif

# ensures that `make` and `make build` are equivalent
ifeq ($(dir_goals),)
build: all
else
build:
	@true # falls through to `$(subdir)`
endif

# make [build|clean] <subdir>
$(subdirs):
	@target=$(filter-out $@,$(MAKECMDGOALS))				;\
	if [ -z "$$target" ] || [ "$$target" = "build" ]; then	 \
		target=all											;\
	fi														;\
	echo "main | entering $@ (target: $$target)"			;\
	$(MAKE) -C $@ $$target

.PHONY: all clean build $(subdirs)