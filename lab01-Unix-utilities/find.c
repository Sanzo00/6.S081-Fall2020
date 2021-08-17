#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fileName(char *path) {
  char *p = path + strlen(path);
  while (p >= path && *p != '/') {
    --p;
  }
  ++p;
  return p;
}

void find(char *path, char *target) {
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512];
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  switch (st.type) {
    case T_FILE:
      if (strcmp(fileName(path), target) == 0) {
        fprintf(1, "%s\n", path);
      }
      break;
    case T_DIR:
      strcpy(buf, path);
      char *p = buf + strlen(buf);
      *p++ = '/';

      // Directory is a file containing a sequence of dirent structures. 
      while (read(fd, &de, sizeof de) == sizeof de) {
        if (!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, "..")) {
          continue;
        }
        memmove(p, de.name, strlen(de.name));  
//        fprintf(1, "debug dirent.name len: %d\n", strlen(de.name));
        p[strlen(de.name)] = 0;
        if (stat(buf, &st) < 0) {
          fprintf(2, "find: cannot stat %s\n", buf);
          continue;
        }
        if (st.type == T_DIR) {
          find(buf, target);
        } else if (!strcmp(de.name, target)) {
          fprintf(1, "%s\n", buf);
        }
      } 
      break;
  }

  return;
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(2, "Usage: find path target\n");
    exit(1);
  }
  
  find(argv[1], argv[2]);

//  int fd = open(argv[1], 0);
//  struct stat st;
//  struct dirent de; 
//  if (fstat(fd, &st) < 0) {
//    close(fd);
//    exit(1);
//  }
//
//  while(read(fd, &de, sizeof de) == sizeof de) {
//    printf("%d %s len: %d %d\n", de.inum, de.name, strlen(de.name), sizeof de.name); 
//  }
//
  exit(0);
}
