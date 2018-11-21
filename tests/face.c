/*
 * Columbia University
 * COMS W4118 Fall 2018
 * Homework 3 - orientd.c
 * teamN: UNI, UNI, UNI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "face.h"
#include <unistd.h>

#define __NR_orientevt_create 327
#define __NR_orientevt_destroy 328
#define __NR_orientevt_wait 329

struct dev_orientation
{
    int azimuth; /* angle between the magnetic north and the Y axis, around
		* the Z axis (-180<=azimuth<180)
		*/
    int pitch;   /* rotation around the X-axis: -90<=pitch<=90 */
    int roll;    /* rotation around Y-axis: +Y == -roll, -180<=roll<=180 */
};

struct orientation_range
{
    struct dev_orientation orient; /* device orientation */
    unsigned int azimuth_range;    /* +/- degrees around Z-axis */
    unsigned int pitch_range;      /* +/- degrees around X-axis */
    unsigned int roll_range;       /* +/- degrees around Y-axis */
};

static inline int orientevt_create(struct orientation_range *orient)
{
    return syscall(__NR_orientevt_create, orient);
}

static inline int orientevt_destroy(int event_id)
{
    return syscall(__NR_orientevt_destroy, event_id);
}

static inline int orientevt_wait(int event_id)
{
    return syscall(__NR_orientevt_wait, event_id);
}

static int children_count = 3;
static int parent_life_cycle = 60;
static int child_polling_inteval = 1;

int main(int argc, char **argv)
{
    /* when phone backside faces straigh towards me, azimuth = +/-180, pitch = 0, roll = +/-180 */
    struct dev_orientation orient_facedown = {180, 0, 180};
    /* +/-5 degrees for error, roll error does not matter because it can rotate */
    struct orientation_range orient_r = {orient_facedown, 5, 5, 0};
    printf("[DEBUG] facedown test begin");
    int event_id = orientevt_create(&orient_r);
    printf("[DEBUG], event_id=%d\n",event_id);
    if (event_id < 0)
    {
        printf("orientevt_create failed.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < children_count; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            printf("fork failed\n");
            orientevt_destroy(event_id);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            //int event_id = orientevt_create(&orient_r);
            //printf("[DEBUG], event_id=%d\n", event_id);
            /* child: wait for orient event */
            int count=0;
            while (1)
            {
                printf("sec: %ds\n", count++);
                int wait_ret = orientevt_wait(event_id);
                printf("[DEBUG], wait_ret=%d\n",wait_ret);
                if (wait_ret < 0)
                {
                    /*system failure, exit*/
                    printf("system failure, exiting child process, pid: %d", getpid());
                    exit(EXIT_FAILURE);
                }
                else if (wait_ret > 0)
                {
                    /*event not found, exit*/
                    printf("Event not found, exiting child process, pid: %d", getpid());
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    printf("%d: facing down!", i);
                }

                /* sleep some time according to hw3 instructions */
                sleep(child_polling_inteval);
            }
        }

        /* parent process continue to create next child */
    }

    sleep(parent_life_cycle);

    printf("father process exiting\n");
    /* destroying the events to kill children */
    orientevt_destroy(event_id);

    return EXIT_SUCCESS;
}
