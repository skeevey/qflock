#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "k.h"

K lock(K lock_file, K max_wait_time) {
    if (lock_file->t != -KS || max_wait_time->t != -KI) {
        return krr("type");
    }
    const char *lockFile = lock_file->s;
    int maxWaitTime = max_wait_time->i;

    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = -1;
    while (true) {
        fd = open(lockFile, O_CREAT | O_RDWR, 0644);
        if (fd == -1) {
            return krr("open_failed");
        }

        if (flock(fd, LOCK_EX | LOCK_NB) == 0) { // Exclusive lock acquired
            return ki(fd); // Return file descriptor
        }

        // If max_wait_time is zero, fail
        if (maxWaitTime == 0) {
            close(fd);
            return krr("lock_unavailable");
        }
        clock_gettime(CLOCK_MONOTONIC, &current);
        int elapsed_time = current.tv_sec - start.tv_sec;
        if (elapsed_time >= maxWaitTime) {
            close(fd);
            return krr("timeout");
        }

        close(fd);
        // Wait before retrying
        usleep(500000);
    }
    return krr("stop");
}

K unlock(K fd) {
    if (fd->t != -KI) {
        return krr("type");
    }

    int fileDescriptor = fd->i;
    if (flock(fileDescriptor, LOCK_UN) == -1) {
        return krr("unlock_failed");
    }
    close(fileDescriptor);
    return (K)0; // Return null on success
}
