#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define die(msg) do { fprintf(stderr, "ERROR: %s\n", msg); exit(1); } while (0)


int main(int argc, char *argv[]) {

    // Validate number of args
    if (argc != 2) {
        printf("usage: xcheck [xv6 filesystem image]\n");
        exit(1);
    }

    // Open the filesystem image
    char *fs_img = argv[optind];
    int fd = open(fs_img, O_RDONLY);
    if (fd == -1) {
		die("file open failed");
    }


	
    return 0;
}
