#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "/usr/include/libbluray/bluray.h"
#include "/usr/include/libbluray/filesystem.h"

//Guarantees that the parent directories of filename are created
void guar_directory(char *filename) {
    char *new = strdup(filename);
    for(char *temp = strstr(new + 1,"/"); temp != NULL; temp = strstr(temp + 1,"/")) {
        temp[0] = 0;
        if(access(new,F_OK)) {
            mkdir(new,S_IRUSR | S_IWUSR | S_IXUSR);
        }
        temp[0] = '/';
    }
}

void getbdfile(BLURAY *disc, char *filename, char *saveto) {
    char file[1024];
    //NO BEGINNING SLASH
    filename = strstr(filename,"BDMV");
    BD_FILE_H *bdfile = bd_open_file_dec(disc, filename);
    sprintf(file,"%s/%s",saveto,filename);
    guar_directory(file);
    FILE *test = fopen(file,"w");
    printf("Saving to: %s\n",file);
    uint8_t *buffer = malloc(6144);
    uint64_t amount;
    while((amount = (bdfile->read)(bdfile, buffer, 6144))) {
        fwrite(buffer,1,amount,test);
    }
    bdfile->close(bdfile);
    free(buffer);
    fclose(test);
}

void recurdir(BLURAY *disc, char *dirname, char *saveto) {
    struct stat fileinfo;
    DIR *dfd;
    struct dirent *dp;
    char *data = malloc(1024);
    dfd = opendir(dirname);
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
            recurdir(disc,data,saveto);
        }
        if((fileinfo.st_mode & S_IFMT) == S_IFREG) {
            //printf("%s\n",data);
            getbdfile(disc, data, saveto);
        }
    }
    free(data);
}

int main(int argc, char **argv) {
    if(argc < 3) {
        printf("Please enter disc root and folder for storage.\n");
    }
    BLURAY *disc = bd_open(argv[1], "keydb.cfg");
    if(disc == NULL) {
        printf("Could not initialize disc.\n");
        return 0;
    }
    char newname[1024];
    sprintf(newname,"%s/%s",argv[1],"BDMV");
    recurdir(disc,newname,argv[2]);
}