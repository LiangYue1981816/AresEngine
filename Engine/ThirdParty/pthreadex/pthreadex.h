#pragma once
#include "pthread.h"


#ifdef __cplusplus
extern "C" {
#endif

void pthread_set_name(pthread_t thread, const char *name);

#ifdef __cplusplus
}
#endif
