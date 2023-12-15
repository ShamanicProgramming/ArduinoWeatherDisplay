#ifndef SCREEN_H
#define SCREEN_H

#include <U8g2lib.h>
#include <SPI.h>
#include "WeatherData.h"
#include "CurrentWeatherData.h"

void setLoadingScreen(const char *msgln1, const char *msgln2);
void setErrorScreen(const char *msgln1, const char *msgln2);
void setForecastScreen(const char *loc, WeatherData &weatherData);
void setCurrentWeatherScreen(const char *loc, CurrentWeatherData &weatherData);
void startScreen();

#endif