#ifndef _ORIENTD_H
#define _ORIENTD_H

#include <asm/unistd.h>
#include <asm-generic/syscall.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/types.h>

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

struct orientation_range {
    struct dev_orientation orient; /* device orientation */
    unsigned int azimuth_range; /* +/- degrees around X-axis */
    unsigned int pitch_range; /* +/- degrees around Y-axis */
    unsigned int roll_range; /* +/- degrees around Z-axis */
};

static inline int charon_abs(int n)
{
    return n>0?n:-n;
}

struct orient_event{
    unsigned int id;
    int is_alive;  
    struct orientation_range o_range;
    struct list_head list;
    wait_queue_head_t wait_queue;
};


//static struct dev_orientation sys_orient; // store the orient of system

/* Helper function to determine whether an orientation is within a range. */
static inline int orient_within_range(struct dev_orientation *orient,struct orientation_range *range)
{
    struct dev_orientation *target = &range->orient;
    unsigned int azimuth_diff = charon_abs(target->azimuth - orient->azimuth);
    unsigned int pitch_diff = charon_abs(target->pitch - orient->pitch);
    unsigned int roll_diff = charon_abs(target->roll - orient->roll);
    return (!range->azimuth_range || azimuth_diff <= range->azimuth_range
            || 360 - azimuth_diff <= range->azimuth_range)
            && (!range->pitch_range || pitch_diff <= range->pitch_range)
            && (!range->roll_range || roll_diff <= range->roll_range
            || 360 - roll_diff <= range->roll_range);
}

#endif /* _ORIENTD_H */
