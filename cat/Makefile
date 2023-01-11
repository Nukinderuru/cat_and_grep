all: s21_cat

s21_cat: s21_cat.c
	gcc -g -Wall -Wextra -Werror s21_cat.c -o s21_cat

style_fix:
	clang-format -i s21_cat.c s21_cat.h 

style_check:
	clang-format -n s21_cat.c s21_cat.h 

memory_leaks_check:
	valgrind --leak-check=full ./s21_cat test1.txt

functional_tests:
	bash functional_tests.sh

clean:
	rm -rf *.o s21_cat *results.txt
