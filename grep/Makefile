all: s21_grep

s21_cat: s21_grep.c s21_grep.h
	gcc -g -Wall -Wextra -Werror s21_grep.c -o s21_grep

style_fix:
	clang-format -i s21_grep.c s21_grep.h 

style_check:
	clang-format -n s21_grep.c s21_grep.h 

memory_leaks_check:
	valgrind --leak-check=full ./s21_grep text test1.txt

functional_tests:
	bash functional_tests.sh

clean:
	rm -rf *.o s21_grep *results.txt
