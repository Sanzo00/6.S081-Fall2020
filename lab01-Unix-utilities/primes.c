#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pipeline(int *p) {
  close(p[1]);
  int n;
  if (read(p[0], &n, sizeof n)) {
    printf("prime %d\n", n);
  } else {
    close(p[0]);
    exit(0);
  }

  int p2[2];
  pipe(p2);
  
  if (fork() == 0) { // child
    pipeline(p2); // recursion
  } else { // parent
    close(p2[0]);
    int m;
    while (read(p[0], &m, sizeof m)) {
      if (m % n) {
        write(p2[1], &m, sizeof m);
      }
    }
    close(p[0]);
    close(p2[1]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  
  int p[2];
  pipe(p);
  if (fork() == 0) {  // child
    pipeline(p);  
    //close(p[1]);
    //int n;
    //while (read(p[0], &n, sizeof n)) {
    //  printf("%d\n", n);
    //}
    //close(p[0]);
  } else { // parent
    close(p[0]);
    for (int i = 2; i <= 35; ++i) {
      write(p[1], &i, sizeof i);
    }
    close(p[1]);
    wait(0);
  }
  
  exit(0);
}
