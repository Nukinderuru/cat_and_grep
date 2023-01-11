# -e

./s21_grep -e text test1.txt > s21_grep_results.txt
grep -e text test1.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -e "[123]" patterns1.txt > s21_grep_results.txt
grep -e "[123]" patterns1.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -e "!" patterns2.txt > s21_grep_results.txt
grep -e "!" patterns2.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

# 2 -e

./s21_grep -e text -e "[1-9]" test1.txt > s21_grep_results.txt
grep -e text -e "[1-9]" test1.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -e "[123]" -e m patterns1.txt > s21_grep_results.txt
grep -e "[123]" -e m patterns1.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -e "!" -e Hu patterns2.txt > s21_grep_results.txt
grep -e "!" -e Hu patterns2.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt

# -f

./s21_grep -f test1.txt s21_grep.h > s21_grep_results.txt
grep -f test1.txt s21_grep.h > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -f test1.txt s21_grep.c  > s21_grep_results.txt
grep -f test1.txt s21_grep.c  > grep_results.txt
diff s21_grep_results.txt grep_results.txt

./s21_grep -f patterns2.txt patterns1.txt > s21_grep_results.txt
grep -f patterns2.txt patterns1.txt > grep_results.txt
diff s21_grep_results.txt grep_results.txt