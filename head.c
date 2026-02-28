#include "types.h"
#include "stat.h"
#include "user.h"

#define STDIN   0
#define STDOUT  1
#define DEFAULT_LINES 10

void
head(int fd, int nlines)
{
  char buf[512];
  int n;
  int lines = 0;
  int i;

  while(lines < nlines && (n = read(fd, buf, sizeof(buf))) > 0){
    for(i = 0; i < n; i++){
      write(STDOUT, &buf[i], 1);

      if(buf[i] == '\n'){
        lines++;
        if(lines >= nlines){
          return;
        }
      }
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd;
  int i;

  if(argc == 1){
    head(STDIN, DEFAULT_LINES);
  } else {
    for(i = 1; i < argc; i++){
      fd = open(argv[i], 0);
      if(fd < 0){
        printf(2, "head: cannot open %s\n", argv[i]);
        continue;
      }
      head(fd, DEFAULT_LINES);
      close(fd);
    }
  }

  exit();
}
