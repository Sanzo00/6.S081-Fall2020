#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  int p2c[2];
  int c2p[2];
  pipe(p2c);
  pipe(c2p);

  char buf[5];
  int n;
  if (fork() == 0) { // child
    
    // read from parent
    close(p2c[1]);
    n = read(p2c[0], buf, sizeof(buf));
    buf[n] = 0;
    fprintf(1, "%d: received %s\n", getpid(), buf);

    // write to parent
    close(c2p[0]);
    write(c2p[1], "pong", 4);
    exit(0);

  } else { // parent
    // write to child
    close(p2c[0]);
    write(p2c[1], "ping", 4);

    // read from child
    close(c2p[1]);
    n = read(c2p[0], buf, sizeof(buf));
    buf[n] = 0;
    fprintf(1, "%d: received %s\n", getpid(), buf);

  }

  exit(0);
}

