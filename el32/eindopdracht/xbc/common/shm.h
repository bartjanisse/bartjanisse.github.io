
#ifndef _SHM_H_
#define _SHM_H_

#include "buttons.h"


button *createButtonSHM(char *name);
button *openButtonSHM(char *name);
void closeButtonSHM(char *name);


#endif
