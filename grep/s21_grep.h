#ifndef S21_GREP_H
#define S21_GREP_H
#define BUF_SIZ 4096

#include <stdlib.h>

struct flags {
  unsigned int c : 1;
  unsigned int e : 1;
  unsigned int f : 1;
  unsigned int h : 1;
  unsigned int i : 1;
  unsigned int l : 1;
  unsigned int n : 1;
  unsigned int o : 1;
  unsigned int s : 1;
  unsigned int v : 1;
};

struct patterns {
  char *e_patterns;
  size_t e_count;
  char *pattern_files;
  size_t f_count;
};

struct patterns get_flags(struct flags *f, int argc, char **argv);
struct patterns get_patterns_from_file(struct patterns *p, int flag_f,
                                       int flag_s);
void parse_patterns(char *patterns, struct flags *f, char *buffer,
                    char *file_name, int count_matches, int line_count,
                    int files_count, int count_prints);
int match(char *buffer, char *pattern, int flag_i);
int work_with_flags(struct flags *f, char *buffer, char *pattern,
                    char *file_name, int line_count, int files_count,
                    int count_prints, int l_printed);
int print_results(char *buffer, char *pattern, struct flags *f, char *file_name,
                  int line_count, int files_count, int count_prints);
int print_counts(int flag_h, char *file_name, int count_matches,
                 int files_count);
void free_memory(char *buffer, struct patterns *p);

#endif