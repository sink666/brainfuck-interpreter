#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int interpret(int *prog, int len, char **errstrp)
{
  /* bool errcheck; */
  int ptr = 0, tape[30000] = {0};
  int jumpstack[255] = {0}, jumpctr = 0, depth, peek;
  *errstrp = "compiler appeasement dance";
  
  for(int i = 0; i <= len; ++i)
    {
      switch(prog[i])
        {
        case '+':
          ++tape[ptr];
          if(tape[ptr] > 255)
            { tape[ptr] = 0; }
          break;
        case '-':
          --tape[ptr];
          if(tape[ptr] < 0)
            { tape[ptr] = 255; }
          break;
        case '<':
          --ptr;
          break;
        case '>':
          ++ptr;
          break;
        case '.':
          printf("%c", tape[ptr]);
          break;
        case '[':
          if(tape[ptr])
            { jumpstack[jumpctr++] = i; }
          else
            {
              for(depth = 1; depth > 0 && ++i;)
                {
                  peek = prog[i];
                  if(peek == '[')
                    { ++depth; }
                  else if(peek == ']')
                    { --depth; }
                }
            }
          break;
        case ']':
          if(tape[ptr])
            { i = jumpstack[jumpctr - 1]; }
          else
            { jumpstack[--jumpctr] = 0; }
          break;
        }
    }
  
  return 0;
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char *fname, *errstr;
  int program[30000] = {0}, len = 0;

  if(argc != 2)
    {
      fprintf(stderr, "%s: usage: bfi [file]\n", *argv);
      goto errexit;
    }

  fname = *++argv;
  if((fp = fopen(fname, "r")) == NULL)
    {
      fprintf(stderr, "%s: can't open %s or %s does not exist.\n",
              argv[-1], *argv, *argv);
      goto errexit;
    }
  
  for(int i = 0, c; i < 30000 && (c = getc(fp)) != EOF; i++)
    { if(c == '\n') continue; program[i] = c; len++; }

  fclose(fp);
  
  if(interpret(program, len, &errstr))
    {
      fprintf(stderr, "%s: runtime error; %s\n", argv[-1], errstr);
      goto errexit;
    }

  exit(EXIT_SUCCESS);

 errexit:
  exit(EXIT_FAILURE);
}
