#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define DIR_MAX 15
#define FILE_MAX 80

void recursionDir(const char *name){

    DIR *dir;
    struct dirent *dirc;
    char temp[DIR_MAX][FILE_MAX];
    char *dir_path[DIR_MAX];
    int i = 0;
    int j = 0;
    
    for(int k = 0; k < DIR_MAX; k++){
        dir_path[k] = NULL;
    }

    for(int m = 0; m < DIR_MAX; m++){
        for(int n = 0; n < FILE_MAX; n++){
            temp[m][n] = NULL;
        }
    }

    if (!(dir = opendir(name))) {
        return;
    }

    printf("%s: \n", name);
    while ((dirc = readdir(dir)) != NULL) {
        if (dirc->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(dirc->d_name, ".") == 0 || strcmp(dirc->d_name, "..") == 0 || strcmp(dirc->d_name, ".git") == 0){
                continue;
            }
                
            snprintf(path, sizeof(path), "%s/%s", name, dirc->d_name);
            for(int c = 0; c< strlen(path); c++){
                temp[i][c] = path[c];
            }

            dir_path[i] = temp[i];
            i++;
            printf("%s%5s", dir_path[i-1], "");

        } else {
            printf("%s%5s", dirc->d_name, "");
        }
    }  

    printf("\n\n");
    while(dir_path[j] != NULL){
        if(dir_path[j] == NULL) {
            break;
        }

        recursionDir(dir_path[j]);
        j++;
    }
    closedir(dir);
}

int main(void) {
    recursionDir(".");
    return 0;
}
