#include "/usr/include/libbluray/bluray.h"
#include "/usr/include/libbluray/filesystem.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    BLURAY *disc = bd_open("mount", "keydb.cfg");
    //BLURAY_DISC_INFO *info = bd_get_disc_info(disc);
    BD_FILE_H *file = bd_open_file_dec(disc, "BDMV/STREAM/00800.m2ts");
    FILE *test = fopen("jurrtest.m2ts","w");

    uint8_t *buffer = malloc(6144);
    uint64_t amount;
    while(amount = (file->read)(file, buffer, 6144)) {
        fwrite(buffer,1,amount,test);
    }
    file->close(file);
    fclose(test);
    bd_close(disc);
    return 0;
}