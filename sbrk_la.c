#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char* argv[])
{ 
  sbrk_la(atoi(argv[argc-1]));
  exit();
}
