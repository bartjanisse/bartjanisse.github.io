#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json/json.h>

#include "hue_aux.h"
 
void print_json_value(const char * s, json_object *jobj, const char * suffix);
void json_parse_array( json_object *jobj, char *key); 
void json_parse(json_object * jobj);
void print_devices(json_object * jobj);
void print_device_state(json_object * jobj, const char * id);
void switch_device(const char * id, const char * state);

struct MemoryStruct {
    char * memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
   
    mem->memory = realloc (mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) 
    {
        DieWithError ("Not enough memory (realloc returned NULL)");
        return 0;
    }
   
    memcpy (&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
   
    return realsize;
}

void 
json_parse_array( json_object *jobj, char *key) 
{
    void json_parse(json_object * jobj); /*Forward Declaration*/
    enum json_type type;

    json_object *jarray = jobj;
    if(key) 
    {
        jarray = json_object_object_get (jobj, key);
    }

    int arraylen = json_object_array_length (jarray);
    int i;
    json_object * jvalue;

    for (i=0; i< arraylen; i++)
    {
        jvalue = json_object_array_get_idx (jarray, i);
        type = json_object_get_type (jvalue);
        if (type == json_type_array) 
        {
            json_parse_array (jvalue, NULL);
        }
        else if (type != json_type_object) 
        {
            printf ("value[%d]: ",i);
            print_json_value ("", jvalue, "");
        }
        else 
        {
            json_parse (jvalue);
        }
    }
}

void 
print_json_value(const char * s, json_object * jobj, const char * suffix)
{
    enum json_type type;

    type = json_object_get_type(jobj); /*Getting the type of the json object*/
    
    switch (type) 
    {
        case json_type_boolean: 
                  printf ("%s=%s%s", s, json_object_get_boolean(jobj)? "true": "false", suffix);             
                  break;
        case json_type_double: 
                  printf ("%s=%lf%s", s, json_object_get_double(jobj), suffix);
                  break;
        case json_type_int: 
                  printf ("%s=%d%s", s, json_object_get_int(jobj), suffix);
                  break;
        case json_type_string: 
                  printf ("%s=\"%s\"%s", s, json_object_get_string(jobj), suffix);
                  break;
        case json_type_object:
                  break;
        case json_type_array:
                  break;
        case json_type_null:
                  break;
    }
}

void 
json_parse(json_object * jobj) 
{
    enum    json_type type;
    char    str[80];

    /*Passing through every array element*/
    json_object_object_foreach (jobj, key, val) 
    { 
        type = json_object_get_type (val);
        switch (type)
        {
          case json_type_boolean: 
                    sprintf (str, "\"%s\"", key);
                    print_json_value (str, val, "");
                    break;
          case json_type_double:
                    printf ("\"%s\"", key);
                    print_json_value (str, val, "");
                    break;
          case json_type_int: 
                    sprintf (str, "\"%s\"", key);
                    print_json_value (str, val, "");
                    break;
          case json_type_string: 
                    sprintf (str, "\"%s\"", key);
                    print_json_value (str, val, "");
                    break; 
          case json_type_object: 
                    printf ("%s{\n", key);
                    json_object * newjobj = json_object_object_get(jobj, key);
                    json_parse (newjobj); 
                    printf ("}\n");
                    break;
          case json_type_array: 
                    printf ("%s[\n", key);
                    json_parse_array (jobj, key);
                    printf ("]\n");
                    break;
          case json_type_null:
                    printf ("type: json_type_null\n");
                    break;
        }
    }
}

void 
print_device_state(json_object * jobj, const char * id)
{
    printf ("\n \"id\"=%s ,", id);
    json_object* lamp  = json_object_object_get (jobj, id);
    json_object* name  = json_object_object_get (lamp, "name");
    json_object* state = json_object_object_get (lamp, "state");
    json_object* on    = json_object_object_get (state, "on");
    json_object* bri   = json_object_object_get (state, "bri");
    json_object* hu    = json_object_object_get (state, "hue");
    json_object* sat   = json_object_object_get (state, "sat");

    print_json_value ("\"name\"", name, ", ");
    print_json_value ("\"on\"",   on,   ", ");
    print_json_value ("\"bri\"",  bri,  ", ");
    print_json_value ("\"hue\"",  hu,  ", ");
    print_json_value ("\"sat\"",  sat,  "");
    printf ("\n\n");
}

void
print_devices(json_object * jobj)
{
    json_object_object_foreach(jobj, id, val) 
    {    
        if(json_object_get_type(val) == json_type_object)
        {
            printf("\t%s", id);
            json_object* lamp  = json_object_object_get (jobj, id);
            json_object* name  = json_object_object_get (lamp, "name");
            print_json_value ("", name, "\n");
        }   
    }
    printf ("\n");
}


void
update_devices(struct MemoryStruct * response)
{
    CURL                *curl;
    CURLcode            ret;
    char                url[200];

    // Free up memory to fill it with new devices
    free(response->memory);
    response->memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    response->size   = 0;          /* no data at this point */ 

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
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, response);

        ret = curl_easy_perform (curl);
        if(ret != CURLE_OK)
        {
            DieWithError ("curl_easy_perform() failed");
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

int 
main (int argc, char *argv[])
{
    int             choice = ' ';
    char            line[80];
    char            device_id[80];
    char            device_state[80];
    json_object *   jobj = NULL;
    struct MemoryStruct response;
   
    // Parse commandline arguments and set program options
    parse_args (argc, argv);

    // Load all available devices from the Hue bridge
    update_devices (&response);

    // Create a structure of Json objects.
    jobj = json_tokener_parse (response.memory);   

    while (choice != 'q')
    {
        printf ("\nMenu\n"
                "========\n"
                "   [a]    get all devices\n"
                "   [g]    get device state\n"
                "   [s]    switch device on/off\n"
                "   [q]    quit\n"
                "Enter choice: "
               );
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");

        switch(choice)
        {
            case 'a':
                /* Print all devices */
                print_devices (jobj);
                break;
            case 'g':
                /* Print state of given device */
                printf ("Enter id: ");
                fgets  (device_id, sizeof (device_id), stdin);
                remove_nl (device_id);

                update_devices (&response);
                jobj = json_tokener_parse (response.memory);
                print_device_state (jobj, device_id);

                break;
            case 's':
                /* Switch device state on/off */
                printf ("Enter id: ");
                fgets  (device_id, sizeof (device_id), stdin);
                remove_nl (device_id);

                printf ("Enter state [0, 1]: ");
                fgets  (device_state, sizeof (device_state), stdin);
                remove_nl (device_state);
            
                switch_device (device_id, device_state);
                break;
            case 'q': 
                /* Quit the application */
                break;
            default:
                printf ("You entered \'%c\' which is not a valid input!\n", choice);
                break;
        }
    }

    free (response.memory);
    return (0);
}


