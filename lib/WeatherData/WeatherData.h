#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

#include <ArduinoJson.h>

class WeatherData
{
public:
    char month[3];
    char day[3];
    int weatherCode;
    float maxTemp;
    float minTemp;
    float uvIndex;
    float precipSum;
    int precipPropMax;

    void populateData(StaticJsonDocument<2048> &doc, int dateIndex);
};

#endif