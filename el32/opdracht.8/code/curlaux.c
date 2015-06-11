#include <string.h>
#include <stdlib.h>
#include "curlaux.h"

/*struct Devices {
    char * memory;
    size_t size;
};*/

size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct Hue *hue = (struct Hue *)userp;
   
    hue->devices = realloc (hue->devices, hue->size + realsize + 1);
    if(hue->devices == NULL) 
    {
        //DieWithError ("Not enough memory (realloc returned NULL)");
        return 0;
    }
   
    memcpy (&(hue->devices[hue->size]), contents, realsize);
    hue->size += realsize;
    hue->devices[hue->size] = 0;
   
    return realsize;
}

void
update_devices(struct Hue * h)
{
    CURL                *curl;
    CURLcode            ret;
    char                url[200];

    // Free up memory to fill it with new devices
    free(h->devices);
    h->devices = malloc(1);  /* will be grown as needed by the realloc above */ 
    h->size   = 0;          /* no data at this point */ 

    curl = curl_easy_init();
    if(curl)
    {
        
        sprintf (url, "http://192.168.123.75/api/linuxdeveloper/%s", "lights");
        curl_easy_setopt (curl, CURLOPT_URL, url);
        curl_easy_setopt (curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt (curl, CURLOPT_USERAGENT, "curl/7.35.0");
        curl_easy_setopt (curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt (curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt (curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, h);

        ret = curl_easy_perform (curl);
        if(ret != CURLE_OK)
        {
            //DieWithError ("curl_easy_perform() failed");
        }
    }
    curl_easy_cleanup (curl); 
    curl = NULL;
}

void 
switch_device(const char * id, const char * state)
{
    CURL    *curl;
    char    url[200];

    sprintf (url, "http://192.168.123.75/api/linuxdeveloper/lights/%s/state", id);

    curl = curl_easy_init();
    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt (curl, CURLOPT_USERAGENT, "curl/7.35.0");
    curl_easy_setopt (curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt (curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt (curl, CURLOPT_TCP_KEEPALIVE, 1L);

    if(*state == '0')
    {
        curl_easy_setopt (curl, CURLOPT_POSTFIELDS, "{\"on\":false}");
        curl_easy_setopt (curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)12);
    }
    else
    {
        curl_easy_setopt (curl, CURLOPT_POSTFIELDS, "{\"on\":true}");
        curl_easy_setopt (curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)11);
    }
    curl_easy_perform (curl);

    curl_easy_cleanup (curl);
    curl = NULL;
}

/* EOF */