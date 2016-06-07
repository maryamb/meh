#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, char * argv[]) {
    int ofd = open("meh", O_APPEND);
    FILE * maps_fp;
    int mem_fd;
    char * line = NULL;
    char * pch;
    char * addr_range;
    char * permission;
    char * buf;
    unsigned long offset;
    size_t len = 0;
    char mem_file_name[] = "/proc/self/mem";
    char maps_file_name[] = "/proc/self/maps";
    unsigned long addr1, addr2;

    maps_fp = fopen(maps_file_name, "r");
    mem_fd = open(mem_file_name, O_RDONLY);
    while (getline(&line, &len, maps_fp) != -1) {
        addr_range = strtok (line, " ");
        permission = strtok(NULL, " ");
        offset = strtol(strtok(NULL, " "), NULL, 0);
        pch = strtok(addr_range, "-");
        addr1 = strtol(pch, NULL, 0);
        pch = strtok(NULL, "-");
        addr2 = strtol(pch, NULL, 0);

        if (permission[0] != 'r')
            continue;
        for(; addr1 < addr2; addr1 += sizeof(unsigned long)) {
            lseek(mem_fd, offset, SEEK_SET);
            read(mem_fd, buf, _SC_PAGE_SIZE);
            printf("%s\n", buf);
        }
    }

    close(ofd);
    fclose(maps_fp);
    close(mem_fd);
}
