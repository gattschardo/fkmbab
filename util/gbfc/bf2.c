/* bf2.c
 *
 * brainfuck compiler supporting C and x86 ASM output.
 *
 * (c) R. Molitor, 2010
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DOT '.'
#define HEAPSIZE_STR "30000"
#define OUTBUFSIZE_STR "1000"

enum output_t { C_OUTPUT, AS_OUTPUT };

const char *c_head =
 "#include <stdio.h>\n" \
 "int main(void) {\n" \
 " int a[" HEAPSIZE_STR "];\n" \
 " int *p = a;\n";

const char *c_tail =
 " return 0;\n}\n";

const char *as_head =
 ".text\n" \
 ".global _start\n" \
 "_start:\n" \
 " mov $heap, %eax\n";

const char *as_tail =
 " mov $0, %ebx\n" \
 " mov %ebx, %eax\n" \
 " inc %eax\n" \
 " int $0x80\n" \
 "\n" \
 ".bss\n" \
 ".lcomm heap, " HEAPSIZE_STR "\n" \
 ".lcomm buf, " OUTBUFSIZE_STR "\n";

char *subst(char *s, enum output_t format);
void compile(FILE *in, FILE *out, enum output_t format);

int main(int argc, char **argv)
{
 FILE *in, *out;
 int i;
 char *name_i, *name_o;
 enum output_t format;

 format = AS_OUTPUT;
 i = 0;
 while (++i < argc) {
  name_i = argv[i];

  if (name_i[0] == '-') {
   if (name_i[1] == '\0') {
    in  = stdin;
    out = stdout;
   } else {
    if (name_i[1] == 'c' && name_i[2] == '\0')
     format = C_OUTPUT;
    else if (name_i[1] == 'a' && name_i[2] == 's' && name_i[3] == '\0')
     format = AS_OUTPUT;
    else if (name_i[1] == 'h' && name_i[2] == '\0') {
     printf("%s usage:\n" \
            "%s [-c] infile1 [-as] [infile2 ...]\n" \
            "where\n"
            "-c\tswitches to C output (in infile.c)\n" \
            "-as\tswitches to x86 gas output (in infile.s)\n" \
            "-h\tprints this help and exits.\n\n" \
            "You can have as many infiles as you need and may switch between C and Assembler\n"
            "output for each file individually.\n", argv[0], argv[0]);
     return 0;
    } else
     fprintf(stderr, "Ignoring unknown option %s\n", name_i+1);
    continue;
   }
  } else {
   name_o = subst(name_i, format);

   in = fopen(name_i, "r");
   out = fopen(name_o, "w");
   free(name_o);
  }

  if (format == C_OUTPUT)
   fputs(c_head, out);
  else
   fputs(as_head, out);

  compile(in, out, format);

  if (format == C_OUTPUT)
   fputs(c_tail, out);
  else
   fputs(as_tail, out);

  if (in != stdin) {
   fclose(in);
   fclose(out);
  }
 }

 return 0;
}

char *subst(char *s, enum output_t format)
{
 char *t, *dot;
 int baselen;

 dot = strrchr(s, DOT);
 if (dot)
  baselen = dot - s;
 else
  baselen = strlen(s);

 t = malloc(sizeof(char) * (baselen + 3));

 strncpy(t, s, baselen);
 t[baselen++] = DOT;
 if (format == C_OUTPUT)
  t[baselen++] = 'c';
 else
  t[baselen++] = 's';
 t[baselen] = '\0';

 return t;
}

void compile(FILE *in, FILE *out, enum output_t format)
{
 int go;
 int c, i, indent;
 int j, count, last;
 int loop;
 int stck[1000], *sp;

 indent = 1;
#define INDENT for(i = 0; i < indent; i++) fputc(' ', out);

 loop = 0;
 sp = stck;
 last = EOF;
 go = 1;
 while (go) {
  if ((c = fgetc(in)) == EOF)
   go = 0;

  if (c == EOF || c != last && strchr(",.<>[]+-", c)) {
   switch(last) {
   case '>':
    INDENT
    if (count == 1) {
     if (format == C_OUTPUT)
       fputs("++p;\n", out);
      else
       fputs("inc %eax\n", out);
    } else {
     if (format == C_OUTPUT)
      fprintf(out, "p  += %d;\n", count);
     else
      fprintf(out, "add $%d, %%eax\n", count);
    }
    break;
   case '<':
    INDENT
    if (count == 1) {
     if (format == C_OUTPUT)
      fputs("--p;\n", out);
     else
      fputs("dec %eax\n", out);
    } else {
     if (format == C_OUTPUT)
      fprintf(out, "p  -= %d;\n", count);
     else
      fprintf(out, "sub $%d, %%eax\n", count);
    }
    break;
   case '+':
    INDENT
    if (count == 1) {
     if (format == C_OUTPUT)
      fputs("++*p;\n", out);
     else
      fputs("incl (%eax)\n", out);
    } else {
     if (format == C_OUTPUT)
      fprintf(out, "*p += %d;\n", count);
     else
      fprintf(out, "add $%d, (%%eax)\n", count);
    }
    break;
   case '-':
    INDENT
    if (count == 1) {
     if (format == C_OUTPUT)
      fputs("--*p;\n", out);
     else
      fputs("decl (%eax)\n", out);
    } else {
     if (format == C_OUTPUT)
      fprintf(out, "*p -= %d;\n", count);
     else
      fprintf(out, "sub $%d, (%%eax)\n", count);
    }
    break;
   case '.':
    if (format == C_OUTPUT) {
     for(j = 0; j < count; j++) {
      INDENT
      fputs("putchar(*p);\n", out);
     }
    } else {
     INDENT
     if (count > 1) {
      fputs("mov (%eax), %bl\n", out);
      INDENT
      fprintf(out, "mov $%d, %%edx\n", count);
      INDENT
      fputs("mov $buf, %ecx\n", out);
      fprintf(out, "output%d:\n", loop);
      indent++;
      INDENT
      fputs("mov %bl, (%ecx)\n", out);
      INDENT
      fputs("inc %ecx\n", out);
      INDENT
      fputs("dec %edx\n", out);
      INDENT
      fprintf(out, "jz end_output%d\n", loop);
      INDENT
      fprintf(out, "jmp output%d\n", loop);
      fprintf(out, "end_output%d:\n", loop++);
      indent--;
      INDENT
      fprintf(out, "sub $%d, %%ecx\n", count);
     } else {
      fputs("mov %eax, %ecx\n", out);
     }
     INDENT
     fprintf(out, "mov $%d, %%edx\n", count);
     INDENT
     fputs("mov $1, %ebx\n", out);
     INDENT
     fputs("push %eax\n", out);
     INDENT
     fputs("mov $4, %eax\n", out);
     INDENT
     fputs("int $0x80\n", out);
     INDENT
     fputs("pop %eax\n", out);
    }
    break;
   case ',':
    /* multiple reads are the same as one read */
    if (format == C_OUTPUT) {
     INDENT
     fputs("*p = getchar();\n", out);
    } else {
     INDENT
     fputs("mov $1, %edx\n", out);
     INDENT
     fputs("mov %eax, %ecx\n", out);
     INDENT
     fputs("mov $0, %ebx\n", out);
     INDENT
     fputs("push %eax\n", out);
     INDENT
     fputs("mov $3, %eax\n", out);
     INDENT
     fputs("int $0x80\n", out);
     INDENT
     fputs("pop %eax\n", out);
    }
    break;
   case '[':
    for(j = 0; j < count; j++) {
     if (format == C_OUTPUT) {
      INDENT
      fputs("while (*p) {\n", out);
      indent++;
     } else {
      fprintf(out, "loop%d:\n", loop);
      *sp++ = loop;
      indent++;
      INDENT
      fputs("mov $0, %ebx\n", out);
      INDENT
      fputs("movb (%eax), %bl\n", out);
      INDENT
      fputs("cmp $0, %bl\n", out);
      INDENT
      fprintf(out, "je end%d\n", loop++);
     }
    }
    break;
   case ']':
    for(j = 0; j < count; j++) {
     if (format == C_OUTPUT) {
      indent--;
      INDENT
      fputs("}\n", out);
     } else {
      INDENT
      fprintf(out, "jmp loop%d\n", *--sp);
      fprintf(out, "end%d:\n", *sp);
      indent--;
     }
    }
    break;
   }
   last = c;
   count = 1;
  } else if (last == c) {
   count++;
  } /*else {
   fprintf(stderr, "Ignoring %c\n", c);
  }*/
 }

 if (indent != 1) {
  fputs("Syntax error: mismatched [ ]\n", stderr);
 }
}
