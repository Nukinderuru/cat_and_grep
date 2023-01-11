#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct flags f = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  struct patterns p = get_flags(&f, argc, argv);
  struct patterns p_all = get_patterns_from_file(&p, f.f, f.s);
  char *pattern = argv[optind], *buffer = NULL;
  int current_file = 0;
  if (!f.f && !f.e)
    current_file = ++optind;
  else
    current_file = optind;
  FILE *fp = NULL;
  buffer = malloc(BUF_SIZ);
  int files_count = argc - optind;

  while (current_file < argc) {
    if ((fp = fopen(argv[current_file], "rb")) == NULL) {
      if (!f.s)
        fprintf(stderr, "%s: No such file or directory.\n", argv[current_file]);
    } else {
      int count_matches = 0, count_prints = 0, line_count = 0;
      while (fgets(buffer, BUF_SIZ, fp)) {
        int l_print = 0;
        ++line_count;
        if (count_matches == 0) l_print = 1;
        count_matches +=
            work_with_flags(&f, buffer, pattern, argv[current_file], line_count,
                            files_count, count_prints, l_print);
        if (f.e) {
          parse_patterns(p.e_patterns, &f, buffer, argv[current_file],
                         count_matches, line_count, files_count, count_prints);
        }
        if (f.f) {
          parse_patterns(p_all.pattern_files, &f, buffer, argv[current_file],
                         count_matches, line_count, files_count, count_prints);
        }
        memset(buffer, '\0', BUF_SIZ);
      }
      if (fp) {
        fclose(fp);
      }
      if (f.c && count_prints == 0 && !f.l) {
        count_prints +=
            print_counts(f.h, argv[current_file], count_matches, files_count);
      }
    }
    ++current_file;
  }
  free_memory(buffer, &p);
  return 0;
}

struct patterns get_flags(struct flags *f, int argc, char **argv) {
  static struct option long_options[] = {
      {"count", 0, 0, 'c'},       {"regexp", 0, 0, 'e'},
      {"file", 0, 0, 'f'},        {"no-filename", 0, 0, 'h'},
      {"ignore-case", 0, 0, 'i'}, {"files-with-matches", 0, 0, 'l'},
      {"line-number", 0, 0, 'n'}, {"only-matching", 0, 0, 'o'},
      {"no-messages", 0, 0, 's'}, {"invert-match", 0, 0, 'v'},
      {"help", 0, 0, '?'},        {0, 0, 0, 0}};
  int opt, e_length = 0, e_count = 0, f_length = 0, f_count = 0;
  char *e_pattern = NULL, *pattern_file = NULL, **last_ptr = NULL;
  while ((opt = getopt_long(argc, argv, "ce:f:hilnosv?", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'c':
        f->c = 1;
        break;
      case 'e':
        f->e = 1;
        if (e_count == 0) {
          e_length += strlen(optarg);
          e_pattern = calloc(e_length + 2, sizeof(char));
          last_ptr = &e_pattern;
        } else {
          char temp[e_length + 2];
          e_pattern[e_length - 1] = '\n';
          strcpy(temp, e_pattern);
          e_length += strlen(optarg);
          e_pattern = realloc(e_pattern, (e_length + 2) * sizeof(char));
          if (&e_pattern != last_ptr) {
            memset(e_pattern, '\0', e_length);
            strcpy(e_pattern, temp);
          }
        }
        strcat(e_pattern, optarg);
        ++e_length;
        ++e_count;
        break;
      case 'f':
        f->f = 1;
        if (f_count == 0) {
          f_length += strlen(optarg);
          pattern_file = calloc(f_length + 2, sizeof(char));
          last_ptr = &pattern_file;
        } else {
          char temp[f_length + 2];
          pattern_file[f_length - 1] = '\n';
          strcpy(temp, pattern_file);
          f_length += strlen(optarg);
          pattern_file = realloc(pattern_file, (f_length + 2) * sizeof(char));
          if (&pattern_file != last_ptr) {
            memset(pattern_file, '\0', f_length);
            strcpy(pattern_file, temp);
          }
        }
        strcat(pattern_file, optarg);
        ++f_length;
        ++f_count;
        break;
      case 'h':
        f->h = 1;
        break;
      case 'i':
        f->i = 1;
        break;
      case 'l':
        f->l = 1;
        break;
      case 'n':
        f->n = 1;
        break;
      case 'o':
        f->o = 1;
        break;
      case 's':
        f->s = 1;
        break;
      case 'v':
        f->v = 1;
        break;
      case '?':
        printf("usage: grep [-cefhilosv?] PATTERNS [FILE..]\n");
        exit(1);
    }
  }
  struct patterns p = {e_pattern, e_count, pattern_file, f_count};
  return p;
}

struct patterns get_patterns_from_file(struct patterns *p, int flag_f,
                                       int flag_s) {
  if (flag_f) {
    char *files = malloc(strlen(p->pattern_files) * sizeof(char) + 1);
    strcpy(files, p->pattern_files);
    size_t current_file = 0, count = 0;
    int patterns_length = 0;
    char *patterns = NULL, **last_ptr;
    char *patterns_buffer = calloc(BUF_SIZ, sizeof(char));
    char *file = strtok(files, "\n");
    while (current_file < p->f_count) {
      FILE *fp = fopen(file, "rb");
      if (fp == NULL) {
        if (!flag_s) {
          fprintf(stderr, "No such file or directory:%s\n", file);
        }
      } else {
        while (fgets(patterns_buffer, BUF_SIZ, fp)) {
          if (count == 0) {
            patterns_length += strlen(patterns_buffer);
            patterns = calloc(patterns_length + 1, sizeof(char));
            last_ptr = &patterns;
          } else {
            char temp[patterns_length + 1];
            strcpy(temp, patterns);
            patterns_length += strlen(patterns_buffer);
            patterns = realloc(patterns, (patterns_length + 1) * sizeof(char));
            if (&patterns != last_ptr) {
              memset(patterns, '\0', patterns_length + 1);
              strcpy(patterns, temp);
            }
          }
          strcat(patterns, patterns_buffer);
          ++count;
        }
        if (fp) {
          fclose(fp);
        }
      }
      file = strtok(NULL, "\n");
      ++current_file;
    }
    free(files);
    free(patterns_buffer);
    p->pattern_files =
        realloc(p->pattern_files, (strlen(patterns) + 1) * sizeof(char));
    memset(p->pattern_files, '\0', strlen(patterns) + 1);
    strcpy(p->pattern_files, patterns);
    free(patterns);
    p->f_count = count;
  }
  return *p;
}

void parse_patterns(char *patterns, struct flags *f, char *buffer,
                    char *file_name, int count_matches, int line_count,
                    int files_count, int count_prints) {
  char *temp = malloc((strlen(patterns) + 1) * sizeof(char));
  strcpy(temp, patterns);
  char *pattern = strtok(temp, "\n");
  int l_print = 0;
  if (count_matches == 0) l_print = 1;
  count_matches += work_with_flags(f, buffer, pattern, file_name, line_count,
                                   files_count, count_prints, l_print);
  pattern = strtok(NULL, "\n");
  while (pattern != NULL) {
    count_matches += work_with_flags(f, buffer, pattern, file_name, line_count,
                                     files_count, count_prints, l_print);
    pattern = strtok(NULL, "\n");
  }
  free(temp);
}

int match(char *buffer, char *pattern, int flag_i) {
  int status;
  regex_t re;
  if (flag_i) {
    if (regcomp(&re, pattern, REG_ICASE) != 0) return 0;
    status = regexec(&re, buffer, (size_t)0, NULL, 0);
    regfree(&re);
    if (status == REG_NOMATCH)
      return 2;
    else if (status != 0)
      return 0;
    else
      return 1;
  } else {
    if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0) return 0;
    status = regexec(&re, buffer, (size_t)0, NULL, 0);
    regfree(&re);
    if (status == REG_NOMATCH)
      return 2;
    else if (status != 0)
      return 0;
    else
      return 1;
  }
}

int work_with_flags(struct flags *f, char *buffer, char *pattern,
                    char *file_name, int line_count, int files_count,
                    int count_prints, int l_print) {
  int count_matches = 0;
  if (f->v) {
    if (match(buffer, pattern, f->i) == 2) {
      ++count_matches;
      if (f->l) {
        if (l_print == 1) {
          printf("%s\n", file_name);
          ++count_prints;
        }
      } else {
        count_prints += print_results(buffer, pattern, f, file_name, line_count,
                                      files_count, count_prints);
      }
    }
  } else {
    if (match(buffer, pattern, f->i) == 1) {
      ++count_matches;
      if (f->l) {
        if (count_matches == 1) {
          printf("%s\n", file_name);
          ++count_prints;
        }
      } else {
        count_prints += print_results(buffer, pattern, f, file_name, line_count,
                                      files_count, count_prints);
      }
    }
  }
  return count_matches;
}

int print_counts(int flag_h, char *file_name, int count_matches,
                 int files_count) {
  int count_prints = 0;
  if (files_count == 1 || flag_h) {
    printf("%d\n", count_matches);
    ++count_prints;
  } else {
    printf("%s:%d\n", file_name, count_matches);
    ++count_prints;
  }
  return count_prints;
}

int print_results(char *buffer, char *pattern, struct flags *f, char *file_name,
                  int line_count, int files_count, int count_prints) {
  if (!f->c && !f->l) {
    if (f->v && f->o) {
      return count_prints;
    } else if (f->n && f->o) {
      if (files_count == 1 || f->h) {
        printf("%d:%s\n", line_count, pattern);
        ++count_prints;
      } else {
        printf("%s:%d:%s\n", file_name, line_count, pattern);
        ++count_prints;
      }
    } else if (f->n) {
      if (files_count == 1 || f->h) {
        printf("%d:%s", line_count, buffer);
        if (buffer[strlen(buffer) - 1] != '\n') printf("\n");
        ++count_prints;
      } else {
        printf("%s:%d:%s", file_name, line_count, buffer);
        if (buffer[strlen(buffer) - 1] != '\n') printf("\n");
        ++count_prints;
      }
    } else if (f->o) {
      if (files_count == 1 || f->h) {
        printf("%s\n", pattern);
        ++count_prints;
      } else {
        printf("%s:%s\n", file_name, pattern);
        ++count_prints;
      }
    } else {
      if (files_count == 1 || f->h) {
        printf("%s", buffer);
        if (buffer[strlen(buffer) - 1] != '\n') printf("\n");
        ++count_prints;
      } else {
        printf("%s:%s", file_name, buffer);
        if (buffer[strlen(buffer) - 1] != '\n') printf("\n");
        ++count_prints;
      }
    }
  }
  return count_prints;
}

void free_memory(char *buffer, struct patterns *p) {
  free(buffer);
  if (p->e_patterns) free(p->e_patterns);
  if (p->pattern_files) free(p->pattern_files);
}