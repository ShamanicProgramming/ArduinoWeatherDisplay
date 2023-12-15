#include <ArduinoJson.h>
#include "CurrentWeatherData.h"

void CurrentWeatherData::populateData(StaticJsonDocument<1000> &doc)
{
    weatherCode = doc["current"]["weather_code"];
    currentTemp = doc["current"]["temperature_2m"];
    apparentTemp = doc["current"]["apparent_temperature"];
    humidity = doc["current"]["relative_humidity_2m"];
};