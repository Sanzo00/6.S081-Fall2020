#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[512];
  char *p = buf;
  char **nargv = malloc((argc + 2) * sizeof(*nargv));
  memmove(nargv, argv, (argc) * sizeof(*nargv));
  nargv[argc + 1] = 0;

  while (read(0, p, 1)) {
    if (*p == '\n') {
      *p = 0;
      if (fork() == 0) { // child do exec
        //memmove(nargv + argc, &buf, sizeof(&buf));
        nargv[argc] = buf;
        exec(argv[1], nargv + 1); 
				//exec("echo", ttt + 1);
				//printf("exec error\n");
      } else { // parent wait child exit
        wait(0);
      } 
      p = buf;
    } else {
      p++;
    }
  }
  // check p == buf ???

  exit(0);
}
