#ifndef _ORIENTD_H
#define _ORIENTD_H

#include <asm/unistd.h>
#include <asm-generic/syscall.h>

#define __NR_set_orientation 326

struct dev_orientation {
	int azimuth; /* rotation around
		      * the X axis (-180<=azimuth<180)
		      */
	int pitch;   /* rotation around the Y-axis: -90<=pitch<=90 */
	int roll;    /* rotation around Z-axis: +Y == -roll, -180<=roll<=180 */
};

/* syscall wrapper */
static inline int set_orientation(struct dev_orientation *orient)
{
	//return syscall(__NR_set_orientation, orient);
	return 1;
}

#endif /* _ORIENTD_H */
