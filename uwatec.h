#ifndef UWATEC_H
#define UWATEC_H

#define UWATEC_SUCCESS         0
#define UWATEC_ERROR          -1
#define UWATEC_ERROR_IO       -2
#define UWATEC_ERROR_MEMORY   -3
#define UWATEC_ERROR_PROTOCOL -4
#define UWATEC_ERROR_TIMEOUT  -5

typedef void (*dive_callback_t) (const unsigned char *data, unsigned int size, void *userdata);

#include "uwatec_aladin.h"
#include "uwatec_memomouse.h"
#include "uwatec_smart.h"

#endif /* UWATEC_H */
