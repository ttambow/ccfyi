_name := ccwc
builddir := build
cc := clang
cflags := -std=c23 -Wall -Wextra -g
src := $(wildcard src/*.c)
obj := $(patsubst src/%.c,$(builddir)/%.o,$(src))
target := $(builddir)/$(_name)
testdir := tests
testfile := test.file
testnofile := no.file

all: $(target)
build: all

$(target): $(obj)
	@mkdir -p $(builddir)
	@$(cc) $(obj) -o $(target)
	@rm $(obj)

$(builddir)/%.o: src/%.c
	@mkdir -p $(builddir)
	@$(cc) $(cflags) -c $< -o $@

clean:
	@rm -f $(target)
	@rm -rf $(builddir)

test: clean all # todo: not test this way lmao
	@echo -n  "[help  test] "				;\
	./$(target) -h							;\
	echo -n "[bytes test] "					;\
	./$(target) -c $(testdir)/$(testfile)	;\
	echo -n  "[lines test] "				;\
	./$(target) -l $(testdir)/$(testfile)	;\
	echo -n  "[chars test] "				;\
	./$(target) -m $(testdir)/$(testfile)	;\
	echo -n  "[words test] "				;\
	./$(target) -w $(testdir)/$(testfile)	;\
	echo -n  "[nofiletest] "				;\
	./$(target) -w $(testdir)/$(testnofile)	;\
	echo -n  "[isdir test] "				;\
	./$(target) -w $(testdir)/				;\
	echo "\nall tests complete!"

	@make clean > /dev/null 2>&1

run: all
	@./$(target) -h

.PHONY: all build clean
