_name := ccwc
build := build
cc := clang
cflags := -std=c23 -Wall -Wextra -g
src := $(wildcard src/*.c)
obj := $(patsubst src/%.c,$(build)/src/%.o,$(src))
target := $(build)/$(_name)
test := test.file

all: $(target)

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

test:
	@./$(target) -c $(test) ;\
	./$(target) -l $(test) ;\
	./$(target) -m $(test) ;\
	./$(target) -w $(test) ;\
	./$(target) -h

run:
	@./$(target) -h

.phony: all clean
