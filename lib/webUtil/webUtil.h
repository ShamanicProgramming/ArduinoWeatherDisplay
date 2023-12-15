#ifndef WEB_UTIL_H
#define WEB_UTIL_H

#include "WeatherData.h"
#include "Location.h"
#include "CurrentWeatherData.h"

/// @brief Sends a web request to populate the weather data objects using the data in the location object
/// @return 0 if successful, else error
int retrieveWebWeatherForecast(WeatherData weatherData[], Location &location);
int retrieveCurrentWebWeather(CurrentWeatherData &currentWeatherData, Location &location);

#endif