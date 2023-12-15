#include <ArduinoJson.h>
#include "WeatherData.h"

void WeatherData::populateData(StaticJsonDocument<2048> &doc, int dateIndex)
{
    const char *date = doc["daily"]["time"][dateIndex];
    month[0] = date[5];
    month[1] = date[6];
    month[2] = '\0';

    day[0] = date[8];
    day[1] = date[9];
    day[2] = '\0';

    weatherCode = doc["daily"]["weather_code"][dateIndex];
    maxTemp = doc["daily"]["temperature_2m_max"][dateIndex];
    minTemp = doc["daily"]["temperature_2m_min"][dateIndex];
    uvIndex = doc["daily"]["uv_index_max"][dateIndex];
    precipSum = doc["daily"]["precipitation_sum"][dateIndex];
    precipPropMax = doc["daily"]["precipitation_probability_max"][dateIndex];
};