#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;

  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void
find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(path), name) == 0)
      printf(1, "%s\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      printf(2, "find: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;

      if(de.name[0] == '.' &&
         (de.name[1] == 0 ||
         (de.name[1] == '.' && de.name[2] == 0)))
        continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      find(buf, name);
    }
    break;
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(2, "usage: find startdir name\n");
    exit();
  }

  find(argv[1], argv[2]);
  exit();
}
