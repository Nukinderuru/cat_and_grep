# Cat and grep utils

## Overview

Here you can find my implementations of these basic utils.

* Cat program works with the following flags:
	* -b, --number-nonblank    number nonempty output lines, overrides -n
 	* -e                       display non-printing characters using ^ and M-notation, except for LFD and TAB, display $ at end of each line
	*  -n, --number             number all output lines
	*  -s, --squeeze-blank      suppress repeated empty output lines
	*  -t                       display non-printing characters using ^ and M-notation, except for LFD,  display TAB characters as ^I
	*  -v, --show-nonprinting   use ^ and M- notation, except for and TAB
	The flags work both separately and combined.

* Grep program works with these flags:
	* -e PATTERNS, --regexp=PATTERNS
              Use  PATTERNS  as  the  patterns.   If  this option is used
              multiple times or is combined with the -f (--file)  option,
              search  for all patterns given.  This option can be used to
              protect a pattern beginning with “-”.

       * -f FILE, --file=FILE
              Obtain patterns from FILE, one per line.  If this option is
              used  multiple  times or is combined with the -e (--regexp)
              option, search for all  patterns  given.   The  empty  file
              contains zero patterns, and therefore matches nothing.

       * -i, --ignore-case
              Ignore  case  distinctions,  so that characters that differ
              only in case match each other.

       * -v, --invert-match
              Invert the sense of matching, to select non-matching lines.
       * -c, --count
              Suppress normal output; instead print a count  of  matching
              lines  for  each  input  file.  With the -v, --invert-match
              option (see below), count non-matching lines.
       * -o, --only-matching
              Print  only  the  matched  (non-empty)  parts of a matching
              line, with each such part on a separate output line.
       * -s, --no-messages
              Suppress  error  messages  about  nonexistent or unreadable
              files.
       * -h, --no-filename
              Suppress  the  prefixing  of file names on output.  This is
              the default when there is only one file (or  only  standard
              input) to search.
       * -n, --line-number
              Prefix each line of output with  the  1-based  line  number
              within its input file.
       * -l, --files-with-matches
              Suppress  normal  output;  instead  print  the name of each
              input file from  which  output  would  normally  have  been
              printed.  The scanning will stop on the first match.
              
	The flags work both separately and combined.
