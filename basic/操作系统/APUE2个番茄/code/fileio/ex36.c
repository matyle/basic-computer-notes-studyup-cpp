#include "apue.h"
#include <fcntl.h>

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";
char    buf3[4096];

int
main(void)
{
	int		fd;

	if ((fd = open("file.hole", O_RDWR|O_APPEND)) < 0)
		err_sys("open error");

	// if (write(fd, buf1, 10) != 10)
	// 	err_sys("buf1 write error");
	// /* offset now = 10 */

	if (lseek(fd, 3, SEEK_SET) == -1)
		err_sys("lseek error");
	/* offset now = 16384 */
    if(read(fd,buf3,10)!=10)
        err_sys("read error");
	if (write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");
	/* offset now = 16394 */
    if (write(STDOUT_FILENO, buf3, 10) != 10)
		err_sys("buf2 write error");

	exit(0);
}
