#ifndef LOCATION_H
#define LOCATION_H

#include "constants.h"

class Location
{
public:
    char name[MAX_LOCATION_NAME_SIZE];
    float latCoord;
    float longCoord;
};

#endif