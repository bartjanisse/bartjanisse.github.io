
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *p;
  printf("Content-Type: text/plain\n\n");
  if ((p = getenv("QUERY_STRING")) != NULL && *p != '\0')
    printf("Got query string '%s'\n", p);
  else
    printf("No query string given. Try 'cgidemo?test'\n");
  return (0);
}