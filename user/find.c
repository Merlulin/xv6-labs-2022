/**
 * Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *path, char *template) {
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512], *p;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    } 

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ ='/';
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        // p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", path);
            continue;
        }
        switch(st.type) {
            case T_FILE:
                if (strcmp(template, de.name) == 0) {
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0)) {
                    find(buf, template);
                }
        }   
    }
    close(fd);
    return;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "ERROR: find required path and template!\n");
        exit(1);
    }
    char *path = argv[1];
    char *template = argv[2];
    find(path, template);
    exit(0);
}