#ifndef CURRENT_WEATHER_DATA_H
#define CURRENT_WEATHER_DATA_H

#include <ArduinoJson.h>

class CurrentWeatherData
{
public:
    int weatherCode;
    float currentTemp;
    float apparentTemp;
    int humidity;

    void populateData(StaticJsonDocument<1000> &doc);
};

#endif