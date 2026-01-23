_name := cjparse
build := build
cc := clang
cflags := -std=c23 -Wall -Wextra -g
src := $(wildcard src/*.c)
obj := $(patsubst src/%.c,$(build)/src/%.o,$(src))
target := $(build)/$(_name)

# test
shell := $(SHELL)
steps := $(foreach n,1 2 3 4,step$(n))
test_dir := tests
test_json_files := $(shell find $(test_dir) -type f -name '*.json' | sort)

all: $(target)
build: all

$(target): $(obj)
	@mkdir -p $(build)
	@$(cc) $(obj) -o $(target)
	@rm $(obj)

$(build)/%.o: $(src)
	@mkdir -p $(dir $@)
	@$(cc) $(cflags) -c $< -o $@

clean:
	@rm -f $(target)
	@rm -rf $(build)

run:
	@./$(target) #-h

test:
# help
	@echo "testing help command:"
#	$(target) -h

# step through all tests per step directory, and run them
	@cursor="" 												;\
	for file in $(test_json_files); do						 \
  		step=$$(basename $$(dirname $$file))				;\
  		name=$$(basename $$file)							;\
		if [ "$$step" != "$$cursor" ]; then 				 \
			if [ -n "$$cursor" ]; then echo ""; fi			;\
			echo "$$step cases:"							;\
			cursor="$$step"									;\
			i=1												;\
		fi													;\
		printf "running test #%.2d:\n" $$i					;\
		printf "\t simulating: %s -f $$file\n" "$(target)"	;\
		i=$$((i+1))											;\
	done

.PHONY: all clean
