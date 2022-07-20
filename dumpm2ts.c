#include "/usr/include/libbluray/bluray.h"
#include "/usr/include/libbluray/filesystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void getbdfile(BLURAY *disc, char *filesuf, char *folder) {
    char file[1024];
    //NO BEGINNING SLASH
    strcpy(file,"BDMV/STREAM/");
    strcat(file,filesuf);
    BD_FILE_H *bdfile = bd_open_file_dec(disc, file);
    strcpy(file,folder);
    strcat(file,"/");
    strcat(file,filesuf);
    FILE *test = fopen(file,"w");
    uint8_t *buffer = malloc(6144);
    uint64_t amount;
    while(amount = (bdfile->read)(bdfile, buffer, 6144)) {
        fwrite(buffer,1,amount,test);
    }
    bdfile->close(bdfile);
    free(buffer);
    fclose(test);
}

int main(int argv, char **argc) {
    if(argv < 2) {
        printf("Please enter the root directory of the disc.\n");
        return 0;
    }
    DIR *dfd;
    struct dirent *dp;
    char dirname[1024];
    strcpy(dirname,argc[1]);
    strcat(dirname,"/BDMV/STREAM");
    
    dfd = opendir(dirname);
    if(dfd == NULL) {
        printf("Can't open the folder.\n");
        return 0;
    }
    errno = 0;
    BLURAY *disc = bd_open(argc[1], "keydb.cfg");
    while((dp = readdir(dfd)) != NULL) {
        //printf("%s\n",dp->d_name);
        if(dp->d_name[0] != '.' && strcmp("00800.m2ts",dp->d_name) != 0) {
            getbdfile(disc,dp->d_name,argc[2]);
            printf("%s acquired\n",dp->d_name);
        }
    }
    if(errno != 0) {
        printf("Something bad happened.\n");
    }
    bd_close(disc);
    return 0;
}