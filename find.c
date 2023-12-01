#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path) {
    char *p;
    char *filename;
    int length;

    // Find first character after last slash.
    for(p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;

    // Allocate memory for the filename.
    length = strlen(p);
    filename = malloc(length + 1); // +1 for null terminator
    if (filename == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    strcpy(filename, p);
    return filename;
}


void find(char *path, char *name, int *found) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  char *filename; // 用于存储fmtname返回的动态分配字符串

  if((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
    case T_FILE:
      filename = fmtname(path);
      if(strcmp(filename, name) == 0) {
        printf("%s\n", path);
        *found = 1;
      }
      free(filename); // 释放动态分配的内存
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        if(st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
          find(buf, name, found);
        } else if(st.type == T_FILE) {
          filename = fmtname(buf);
          if(strcmp(filename, name) == 0) {
            printf("%s\n", buf);
            *found = 1;
          }
          free(filename); // 释放动态分配的内存
        }
      }
      break;
  }
  close(fd);
}


int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(1);
  }

  int found = 0;
  find(argv[1], argv[2], &found);

  if (!found) {
    printf("No files found matching '%s'\n", argv[2]);
  }

  exit(0);
}
