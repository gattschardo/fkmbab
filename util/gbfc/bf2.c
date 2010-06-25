/* bf2.c
 * brainfuck to c compiler
 *
 * (c) R. Molitor, 2010
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DOT '.'

char *subst(char *s);
void compile(FILE *in, FILE *out);

const char *head =
 "#include <stdio.h>\n" \
 "int main(void) {\n" \
 " int a[5000];\n" \
 " int *p = a;\n";

const char *tail =
 " return 0;\n}";

int main(int argc, char **argv)
{
 FILE *in, *out;
 char *name_i, *name_o;

 while (--argc) {
  name_i = argv[argc];
  name_o = subst(name_i);

  in = fopen(name_i, "r");
  out = fopen(name_o, "w");
  free(name_o);

  fputs(head, out);
  compile(in, out);
  fputs(tail, out);

  fclose(in);
  fclose(out);
 }

 return 0;
}

char *subst(char *s)
{
 char *t, *dot;

 dot = strrchr(s, DOT);
 int baselen = dot - s;

 t = malloc(sizeof(char) * (baselen + 3));

 strncpy(t, s, baselen);
 t[baselen++] = DOT;
 t[baselen++] = 'c';
 t[baselen] = '\0';

 return t;
}

void compile(FILE *in, FILE *out)
{
 int c, i, indent;

 indent = 1;
#define INDENT for(i = 0; i < indent; i++) fputc(' ', out);

 while ((c = fgetc(in)) != EOF) {
  switch(c) {
  case '>':
   INDENT
   fputs("++p;\n", out);
   break;
  case '<':
   INDENT
   fputs("--p;\n", out);
   break;
  case '+':
   INDENT
   fputs("++*p;\n", out);
   break;
  case '-':
   INDENT
   fputs("--*p;\n", out);
   break;
  case '.':
   INDENT
   fputs("putchar(*p);\n", out);
   break;
  case ',':
   INDENT
   fputs("*p = getchar();\n", out);
   break;
  case '[':
   INDENT
   fputs("while (*p) {\n", out);
   indent++;
   break;
  case ']':
   indent--;
   INDENT
   fputs("}\n", out);
   break;
  }
 }

 if (indent != 1) {
  puts("Syntax error: mismatched [ ]");
 }
}
