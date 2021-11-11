#define MAXLEVEL 2 
#define _XOPEN_SOURCE 500
#include "logger.h"
#include <ftw.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#define LEN_BUFFER SIZENOTIFY * 50000
#define SIZENOTIFY sizeof(struct inotify_event)

#include <unistd.h>

char* paths[5000];

int fd, wd;

char* mystradd(const char *origin, const char *addition){
    char *result = malloc(strlen(origin) + strlen(addition) + 1); 
    strcpy(result, origin);
    strcat(result, addition);
    return result;
}

char * mystrsub(char *origin, int start, int end){
    char* res = malloc((end - start) + 2);
    int i;
    for(i = 0; start < end; i++){
        res[i] = origin[start];
        start++;
    }
    res[i+1]='\0';
    return res;
}

static int display_info(const char *fpath, const struct stat *sb,
                              int tflag, struct FTW *ftwbuf) {
  if (ftwbuf->level > MAXLEVEL) {
    return 0;
  }
    // Inform what files are of interest
  wd = inotify_add_watch(fd, fpath, IN_CREATE | IN_DELETE | IN_MOVE);
  if(wd == -1){
    exit(EXIT_FAILURE);
  }
  paths[wd] = strdup(fpath);
  
  return 0; 
}

int main(int argc, char *argv[]) {
  if(argc<2){
        printf("No directory provide\n");
        return -1;
    }
  char * dir = argv[1];
  printf("Starting File/Directory Monitor on %s\n-----------------------------------------------------\n",dir);

  // Create the inotify instance
  fd = inotify_init();
  if(fd == -1){
    exit(EXIT_FAILURE);
  }
  // Obtained the list of all subdirectories
  nftw((argc < 2) ? "/workspace/ap-labs/labs/c-file-dir-monitor" : dir, display_info, 20, 0);

  ssize_t numRead;
  char buffer[LEN_BUFFER];
  // ciclo para escuchar
  for (;;) {
    // obtain the event notifications
    // After events have occurred, each read() returns a buffer.
    numRead = read(fd, buffer, LEN_BUFFER);
    if(numRead == -1){
      exit(EXIT_FAILURE);
    }
    int i = 0;
    int sizeEvt = 0;
    struct inotify_event *event;
    for(int i = 0; i < numRead; i += SIZENOTIFY + sizeEvt ){

      event = (struct inotify_event*)&buffer[i];
      int pd = event->wd;
      char* contPath = paths[pd];
      char * res = mystrsub(contPath, strlen(dir), strlen(contPath));
      char* result ;
      if(strlen(dir) != strlen(contPath)){ 
        result = mystradd(res,"/");
        result = mystradd(result, event->name);
        result = mystrsub(result, 1, strlen(result));

      }else{
        result = event->name;
      }
      
      if(event->mask & IN_CREATE){
        if(event->mask & IN_ISDIR){
          printf("- [Directory - Create] - %s\n",result);
        }else{
            printf("- [File - Create] - %s\n", result);
        }
      }else if(event->mask & IN_DELETE){
        if(event->mask & IN_ISDIR){
          printf("- [Directory - Removal] - %s\n", result);
        }else{
          printf("- [File - Removal] - %s\n", result);
        }
      }else if(event->cookie != 0){
        if(event->mask & IN_MOVED_FROM){
          if(event->mask & IN_ISDIR){
              printf("- [Directory - Rename] - %s -> ", result);
          }else{
            printf("- [File - Rename] - %s -> ", result);
          }
        }else{
          printf("%s\n", result);
        }
      }
      sizeEvt = event->len;
    }
  }
  return 0;
}