#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int read_unblocked(int argc, char *argv[]) {
	printf("Reading from file \n");
	int res;
	char buf[255];
	int fd = open("/dev/myresource", O_RDONLY |  O_NONBLOCK);

	res = read(fd, buf, 100);
	buf[res] = '\0';

	printf("Read: %s\n", buf);

	close(fd);
	return EXIT_SUCCESS;
}
