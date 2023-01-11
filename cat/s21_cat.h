#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdlib.h>

struct flags {
  unsigned int b : 1;
  unsigned int e : 1;
  unsigned int n : 1;
  unsigned int s : 1;
  unsigned int t : 1;
  unsigned int v : 1;
  unsigned int T : 1;
  unsigned int E : 1;
};

void get_options(struct flags *f, int argc, char **argv);
void print_non_printing(char c, int flag_t);
int print_tabs(char c);
int show_end_of_line(char c, int flag_t, int flag_E);
int cut_empty_lines(char c, int symbols_in_line, char prev,
                    int prev_line_empty);
int numerate_non_empty_lines(char c, int start_of_line, int line_number);
void numerate_all_lines(int start_of_line, int line_number);
int work_with_flags(struct flags *f, char c, int start_of_line,
                    int symbols_in_line, int line_number,
                    int non_empty_line_number, char prev, int prev_line_empty);

#endif