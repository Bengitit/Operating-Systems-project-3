#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main(void)
{
    int fd;
    int i;
    unsigned char *p_map;

    fd = open("/proc/mydir/myinfo", O_RDWR);

    if (fd < 0) {
        printf("open fail\n");
        return 1;
    }

    //printf("open successfully\n");
    printf("open successfully by Abass\n");

    p_map = mmap(NULL,
                 PAGE_SIZE,
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED,
                 fd,
                 0);

    if (p_map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

/*
    printf("Data from kernel:\n");

    for (i = 0; i < 12; i++)
        printf("%d ", p_map[i]);

    printf("\n");
*/

	/* read data from p_map */
	for (i = 0; i < 12; i++)
	{
	    printf("%d\n", p_map[i]);
	}

	printf("Printed by Abass\n");

    munmap(p_map, PAGE_SIZE);

    close(fd);

    return 0;
}
