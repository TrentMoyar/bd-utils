#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void recurdir(char *dirname) {
    struct stat fileinfo;
    DIR *dfd;
    struct dirent *dp;
    char *data = malloc(1024);
    dfd = opendir(dirname);
    printf("%s\n",dirname);
    if(dfd == NULL) {
        printf("Can't open the folder.\n");
        return;
    }
    while((dp = readdir(dfd)) != NULL) {
        if(dp->d_name[0] == '.') {
            continue;
        }
        sprintf(data,"%s/%s",dirname,dp->d_name);
        stat(data,&fileinfo);
        if((fileinfo.st_mode & S_IFMT) == S_IFDIR) {
            recurdir(data);
        }
        if((fileinfo.st_mode & S_IFMT) == S_IFREG) {
            printf("%s\n",data);
        }
    }
    free(data);
}

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Please enter a filename.\n");
    }
    recurdir(argv[1]);
}