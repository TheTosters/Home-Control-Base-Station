/*
 * Helper.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#include <misc/Helper.h>
#include <sys/time.h>
#include "unistd.h"

long long Helper::currentTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}
