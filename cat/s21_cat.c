#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct flags f = {0, 0, 0, 0, 0, 0, 0, 0};
  get_options(&f, argc, argv);

  FILE *fp = NULL;
  int current_file = optind;
  while (current_file < argc) {
    if ((fp = fopen(argv[current_file], "rb")) == NULL) {
      fprintf(stderr, "No such file or directory:%s\n", argv[current_file]);
      ++current_file;
      continue;
    } else {
      char c = 0, prev = 0;
      int start_of_line = 1, symbols_in_line = 0, line_number = 1,
          non_empty_line_number = 1, prev_line_empty = 0, line_not_empty = 0;
      while (1) {
        c = fgetc(fp);
        if (c == EOF) {
          if (feof(fp) != 0) {
            break;
          } else {
            fprintf(stderr, "An error occured while reading the file.\n");
            break;
          }
        }
        if (prev == '\n') {
          start_of_line = 1;
          if (!f.s) {
            line_number++;
          }
          if (symbols_in_line == 0) {
            prev_line_empty = 1;
          }
          symbols_in_line = 0;
        } else {
          ++symbols_in_line;
        }
        line_not_empty =
            work_with_flags(&f, c, start_of_line, symbols_in_line, line_number,
                            non_empty_line_number, prev, prev_line_empty);
        if (line_not_empty == 1) {
          non_empty_line_number++;
        } else if (line_not_empty == 2) {
          if (f.s && start_of_line) {
            line_number++;
          }
        }
        start_of_line = 0;
        prev = c;
        prev_line_empty = 0;
      }
      if (fp != NULL) fclose(fp);
      ++current_file;
    }
  }
  return 0;
}

void get_options(struct flags *f, int argc, char **argv) {
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {"show-nonprinting", 0, 0, 'v'},
                                         {"help", 0, 0, '?'},
                                         {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "benstv?TE", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 's':
        f->s++;
        break;
      case 'b':
        f->b++;
        break;
      case 'n':
        f->n++;
        break;
      case 'e':
        f->e++;
        break;
      case 't':
        f->t++;
        break;
      case 'v':
        f->v++;
        break;
      case 'T':
        f->T++;
        break;
      case 'E':
        f->E++;
        break;
      case '?':
        printf("usage: cat [-benstv] [file...]\n");
        fputs(
            "\
\n\
  -b, --number-nonblank    number nonempty output lines, overrides -n\n\
  -e                       display non-printing characters using ^ and M- notation, except for LFD and TAB, display $ at end of each line\n\
  -n, --number             number all output lines\n\
  -s, --squeeze-blank      suppress repeated empty output lines\n\
  -t                       display non-printing characters using ^ and M- notation, except for LFD,  display TAB characters as ^I\n\
  -v, --show-nonprinting   use ^ and M- notation, except for and TAB\n\
",
            stdout);
        return;
    }
  }
}

void print_non_printing(char c, int flag_t) {
  if (c < -96) {
    printf("M-^%c", c + 192);
  } else if (c < 0) {
    printf("M-%c", c + 128);
  } else if (c < 32 && c != 9 && c != 10) {
    printf("^%c", c + 64);
  } else if (c == 127) {
    printf("^?");
  } else if (flag_t && c == 9) {
    return;
  } else
    printf("%c", c);
}

int print_tabs(char c) {
  if (c == 9) {
    printf("^%c", c + 64);
    return 1;
  }
  return 0;
}

int show_end_of_line(char c, int flag_t, int flag_E) {
  if (c == '\n') {
    if (flag_t && flag_E) {
      printf("$");
      return 1;
    } else {
      printf("$%c", c);
      return 1;
    }
  }
  return 0;
}

int cut_empty_lines(char c, int symbols_in_line, char prev,
                    int prev_line_empty) {
  if ((prev == '\n') && (prev_line_empty) && (symbols_in_line == 0) &&
      (c == '\n')) {
    return 1;
  }
  return 0;
}

int numerate_non_empty_lines(char c, int start_of_line, int line_number) {
  if (start_of_line && c != '\n') {
    printf("%*d	", 6, line_number);
    return 1;
  }
  return 0;
}

void numerate_all_lines(int start_of_line, int line_number) {
  if (start_of_line) {
    printf("%*d	", 6, line_number);
  }
}

int work_with_flags(struct flags *f, char c, int start_of_line,
                    int symbols_in_line, int line_number,
                    int non_empty_line_number, char prev, int prev_line_empty) {
  int line_not_empty = 0, line_empty = 0, non_printing = 0, no_tabs = 0;
  if (f->t || f->T) {
    no_tabs = 1;
  }
  if (f->s) {
    if (cut_empty_lines(c, symbols_in_line, prev, prev_line_empty) != 0) {
      non_printing = 1;
      line_empty = 1;
    } else
      line_not_empty = 2;
  }
  if (f->b) {
    if (numerate_non_empty_lines(c, start_of_line, non_empty_line_number) !=
        0) {
      line_not_empty = 1;
    }
  }
  if (f->n && !f->b && !line_empty) {
    numerate_all_lines(start_of_line, line_number);
  }
  if (f->v && !f->e && !f->t && !line_empty && !f->E && !f->T) {
    print_non_printing(c, no_tabs);
    non_printing = 1;
  }
  if (f->T && !f->t && !f->v) {
    if (print_tabs(c) != 0) {
      non_printing = 1;
    }
  }
  if (f->E && !f->e && !line_empty && !f->v) {
    if (show_end_of_line(c, f->t, f->E) != 0) {
      non_printing = 1;
    }
  }
  if (f->t || (f->T && f->v)) {
    if (c == 9) {
      print_tabs(c);
    } else if (!f->e && !line_empty) {
      print_non_printing(c, no_tabs);
    }
    non_printing = 1;
  }
  if (f->e || (f->E && f->v)) {
    if (!line_empty) {
      if (c == '\n') {
        show_end_of_line(c, f->t, f->E);
      } else {
        print_non_printing(c, no_tabs);
      }
    }
    non_printing = 1;
  }
  if (!non_printing) {
    printf("%c", c);
  }
  return line_not_empty;
}
