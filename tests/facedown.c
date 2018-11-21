#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "face.h"

struct orientation_range range = {{180, 0, 180}, 2, 2, 0};
const int child_number = 3;
const int child_child_number = 4;

int main()
{
    int event_id[3];
    for (int i = 0; i < child_number; i++){
        event_id[i] = orientevt_create(&range);
        for (int j = 0; j < child_child_number; j++){
            int c_pid = fork();
            if (c_pid < 0){
                printf("father %d's %dth child_child fork faild\n", i, j);
                return EXIT_FAILURE;
            }
            else if (c_pid == 0){
                for (int count = 0;; count++){
                    int event_wait = orientevt_wait(event_id[i]);
                    printf("event_wait: %d\n",event_wait);
                    if (event_wait < 0){
                        printf("error occered when wait, father %d's %dth child process exiting\n", i, j);
                        return EXIT_FAILURE;
                    }
                    else if (event_wait == 0)
                        printf("count: %d, father %d's %dth child: face down!\n", count, i, j);
                    else{
                        printf("event %d not found, it might be destoried, father %d's %dth child process exiting\n", event_id[i], i, j);
                        return EXIT_SUCCESS;
                    }
                    sleep(1);
                }
            }
        }
    }
    sleep(60);
    for (int i = 0; i < child_number; i++){
        orientevt_destroy(event_id[i]);
        printf("event_id %i destoried, father %d is exiting\n", event_id[i], i);
    }
    return EXIT_SUCCESS;
}