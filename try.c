#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int total;

  printf(1, "enabling trace...\n");
  total = trace(1);
  printf(1, "trace(1) returned %d (syscalls so far)\n", total);

  printf(1, "doing some work in try...\n");
  printf(1, "another printf to generate syscalls\n");

  total = trace(0);
  printf(1, "trace(0) returned %d (final syscall count)\n", total);

  exit();
}
