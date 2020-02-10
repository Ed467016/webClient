#include "web.h"

void write_get_cmd(struct file* file) {
	int n;
	char buf[MAX_LINE];

	n = snprintf(buf, MAX_LINE, GET_CMD, file->f_name);

	send(file->f_fd, buf, n, 0);
	file->f_flags = F_READING;

	FD_SET(file->f_fd, &rset);
	if (file->f_fd > maxfd)
		maxfd = file->f_fd;
}