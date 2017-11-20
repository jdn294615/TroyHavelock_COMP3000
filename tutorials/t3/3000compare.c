/* 3000test.c */
/* v1 Oct. 1, 2017 */
/* Licenced under the GPLv3, copyright Anil Somayaji */
/* You really shouldn't be incorporating parts of this in any other code,
   it is meant for teaching, not production */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void report_error(char *error)
{
        fprintf(stderr, "Error: %s\n", error);

        exit(-1);
}

int main(int argc, char *argv[])
{
        struct stat statbuf1;
        struct stat statbuf2;
        char *fn1;
        char *fn2;
        int fd;
        size_t len, i, count;
        ssize_t ret_val;
        
        char *data;

        if (argc < 3) {
                if (argc < 1) {
                        report_error("no command line");
                        fprintf(stderr, "Usage: %s <file>\n", argv[0]); 
                } else {
                        report_error("Not enough arguments");
                        fprintf(stderr, "Usage: %s <file>\n", argv[0]); 
                }
        }

        fn1 = argv[1];
        fn2 = argv[2];

        if (lstat(fn1, &statbuf1)) {
                report_error(strerror(errno));
        }

        if (lstat(fn2, &statbuf2)) {
                report_error(strerror(errno));
        }


        len = statbuf.st_size;
        /*printf("File %s: \n", fn);
        printf("   inode %ld\n", statbuf.st_ino);
        printf("  length %ld\n", len);  */      

        if (S_ISREG(statbuf.st_mode)) {
                fd = open(fn, O_RDONLY);
                if (fd == -1) {
                        report_error(strerror(errno));
                }
                data = (char *) mmap(NULL, len,
                                     PROT_READ, MAP_SHARED, fd, 0);
                if (data == NULL) {
                        report_error(strerror(errno));
                }

                count = 0;
                for (i=0; i<len; i++) {
                        if (data[i] == 'a') {
                                count++;
                        }
                }
                close(fd);

                printf(" a count %ld\n", count);
        }

        if (S_ISLNK(statbuf.st_mode)) {
            char *linkname;
            linkname = malloc(statbuf.st_size + 1);
            ret_val = readlink(fn, linkname, statbuf.st_size + 1);
            if (ret_val > -1) {
                printf(" '%s' points to '%s'\n", fn, linkname);
            }
        }

        return 0;
}
