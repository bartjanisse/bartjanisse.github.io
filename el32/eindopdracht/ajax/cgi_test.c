#include "stdio.h"
#include <sys/time.h>
#include "lib/example.h"

int main(int argc, char* argv[])
{
  struct timeval tv;
  int x,t;
  
  test();
  
  printf("Content-type: text/html\n\n");
  gettimeofday(&tv,NULL);
  
  //printf("S: %d M: %d \n", tv.tv_sec, tv.tv_usec);
  // iedere 5 seconden van links naar rechts over 500 pixels=> per honderste seconde = 1 pixel
  t=100*(tv.tv_sec%10)+tv.tv_usec /10000;
  
  //printf("%d\n",t);
  t=t % 1000;
  if (t > 500) x= 1000-t; else x=t;
  printf("%3d;%3d",x,x+40);
  
  return 0;
}
