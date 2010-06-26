/* bf2.c
 * brainfuck to c compiler
 *
 * (c) R. Molitor, 2010
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DOT '.'

enum output_t { C_OUTPUT, AS_OUTPUT };

const char *c_head =
 "#include <stdio.h>\n" \
 "int main(void) {\n" \
 " int a[5000];\n" \
 " int *p = a;\n";

const char *c_tail =
 " return 0;\n}\n";

const char *as_head =
 ".text\n" \
 ".global _start\n" \
 "_start:\n" \
 " mov $HEAP, %eax\n";

const char *as_tail =
 " mov $0, %ebx\n" \
 " mov %ebx, %eax\n" \
 " inc %eax\n" \
 " int $0x80\n" \
 "\n" \
 ".bss\n" \
 ".lcomm HEAP, 2000\n";

char *subst(char *s, enum output_t format);
void compile(FILE *in, FILE *out, enum output_t format);

int main(int argc, char **argv)
{
 FILE *in, *out;
 char *name_i, *name_o;
 enum output_t format;

 format = AS_OUTPUT;
 while (--argc) {
  name_i = argv[argc];

  name_o = subst(name_i, format);

  in = fopen(name_i, "r");
  out = fopen(name_o, "w");
  free(name_o);

  if (format == C_OUTPUT)
   fputs(c_head, out);
  else
   fputs(as_head, out);

  compile(in, out, format);

  if (format == C_OUTPUT)
   fputs(c_tail, out);
  else
   fputs(as_tail, out);

  fclose(in);
  fclose(out);
 }

 return 0;
}

char *subst(char *s, enum output_t format)
{
 char *t, *dot;

 dot = strrchr(s, DOT);
 int baselen = dot - s;

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
 int c, i, indent;
 int j, count, last;
 int loop_head, loop_end;

 indent = 1;
#define INDENT for(i = 0; i < indent; i++) fputc(' ', out);

 loop_head = loop_end = 0;
 last = EOF;
 while ((c = fgetc(in)) != EOF) {
  if (c != last) {
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
     fprintf(out, "mov $%d, %%edx\n", count);
     INDENT
     fputs("mov %eax, %ecx\n", out);
     INDENT
     fputs("mov $1, %ebx\n", out);
     INDENT
     fprintf(out, "add $%d, %%eax\n", count - 1);
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
    if (format == C_OUTPUT) {
     for(j = 0; j < count; j++) {
      INDENT
      fputs("*p = getchar();\n", out);
     }
    } else {
     INDENT
     fprintf(out, "mov $%d, %%edx\n", count);
     INDENT
     fputs("mov %eax, %ecx\n", out);
     INDENT
     fputs("mov $0, %ebx\n", out);
     INDENT
     fprintf(out, "add $%d, %%eax\n", count - 1);
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
      fprintf(out, "loop%d:\n", loop_head++);
      /*indent++;*/
      fputs("mov $0, %ebx\n", out);
      INDENT
      fputs("movb (%eax), %bl\n", out);
      INDENT
      fputs("cmp $0, %bl\n", out);
      INDENT
      fprintf(out, "je end%d\n", loop_head - 1);
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
      fprintf(out, "jmp loop%d\n", loop_head-1);
      fprintf(out, "end%d:\n", loop_head-1);
     }
    }
    break;
   }
   last = c;
   count = 1;
  } else {
   count++;
  }
 }

 if (indent != 1) {
  puts("Syntax error: mismatched [ ]");
 }
}