#ifndef _CURLAUX_H_
#define _CURLAUX_H_

#include <curl/curl.h>

struct Hue {
    char * devices;
    size_t size;
}hue;

extern size_t MyWriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

extern void update_devices(struct Hue * h);
extern void switch_device(const char * id, const char * state);

#endif