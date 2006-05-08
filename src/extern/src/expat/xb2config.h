// config file for Xenon platform
#ifndef XB2CONFIG_H
#define XB2CONFIG_H


#include <memory.h>
#include <string.h>

#define XML_NS 1
#define XML_DTD 1
#define XML_CONTEXT_BYTES 1024

/* we will assume all Windows platforms are little endian */
#define BYTEORDER 1234

/* Windows has memmove() available. */
#define HAVE_MEMMOVE

#endif /* ndef XB2CONFIG_H*/
