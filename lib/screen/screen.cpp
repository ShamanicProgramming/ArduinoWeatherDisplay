#include "screen.h"
#include <Arduino.h>
#include "WeatherData.h"
#include <algorithm>

U8G2_ST7920_128X64_F_SW_SPI u8g2Screen(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/U8X8_PIN_NONE);

void drawHumidity(int humidity)
{
  char humidityString[40];

  snprintf(humidityString, 40, "Humidity: %d%%", humidity);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 40, humidityString);
  u8g2Screen.sendBuffer();
}

void drawCurrentTemp(float currentTemp)
{
  char currentResult[40];

  snprintf(currentResult, 40, "Temp: %.1f", currentTemp);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(73, 30, currentResult);
  u8g2Screen.sendBuffer();
}

void drawApparentTemp(float apparentTemp)
{
  char apparentResult[40];

  snprintf(apparentResult, 40, "Aprnt. Temp: %.1f", apparentTemp);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 50, apparentResult);
  u8g2Screen.sendBuffer();
}

void drawUVIndex(float uvIndex)
{
  char uvString[40];

  snprintf(uvString, 40, "UV Index: %.1f", uvIndex);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 40, uvString);
  u8g2Screen.sendBuffer();
}

void drawPrecip(int precipPropMax, float precipSum)
{
  char precipProbResult[40];
  char precipSumResult[40];

  snprintf(precipProbResult, 40, "Precip. Prob.: %d%%", precipPropMax);
  snprintf(precipSumResult, 40, "Precip. Sum: %.1f mL", precipSum);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 50, precipProbResult);
  u8g2Screen.drawStr(0, 60, precipSumResult);
  u8g2Screen.sendBuffer();
}

void drawTemperature(float maxTemp, float minTemp)
{
  char maxResult[40];
  char minResult[40];

  snprintf(maxResult, 40, "Max: %.1f", maxTemp);
  snprintf(minResult, 40, "Min: %.1f", minTemp);

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(73, 30, maxResult);
  u8g2Screen.drawStr(73, 40, minResult);
  u8g2Screen.sendBuffer();
}

void drawWeatherCode(int weatherCode)
{
  int rainValues[] = {21, 23, 24, 25, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 80, 81, 82, 83, 84, 91, 92};
  int snowValues[] = {22, 26, 36, 37, 38, 39, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 85, 86, 87, 88, 93, 94};
  int sandstormValues[] = {6, 7, 8, 9, 30, 31, 32, 33, 34, 35};
  int thunderstormValues[] = {13, 17, 29, 95, 96, 97, 98, 99};
  int fogValues[] = {10, 11, 12, 28, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
  int drizzleValues[] = {20, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
  if (std::any_of(std::begin(rainValues), std::end(rainValues), [=](int code)
                  { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Rain");
  }
  else if (std::any_of(std::begin(snowValues), std::end(snowValues), [=](int code)
                       { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Snow");
  }
  else if (weatherCode == 0 || weatherCode == 1 || weatherCode == 2)
  {
    u8g2Screen.drawStr(0, 30, "Sun");
  }
  else if (weatherCode == 3)
  {
    u8g2Screen.drawStr(0, 30, "Cloud");
  }
  else if (weatherCode == 27 || weatherCode == 89 || weatherCode == 90)
  {
    u8g2Screen.drawStr(0, 30, "Hail");
  }
  else if (std::any_of(std::begin(sandstormValues), std::end(sandstormValues), [=](int code)
                       { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Sandstorm");
  }
  else if (std::any_of(std::begin(thunderstormValues), std::end(thunderstormValues), [=](int code)
                       { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Thunderstorm");
  }
  else if (std::any_of(std::begin(fogValues), std::end(fogValues), [=](int code)
                       { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Fog");
  }
  else if (std::any_of(std::begin(drizzleValues), std::end(drizzleValues), [=](int code)
                       { return weatherCode == code; }))
  {
    u8g2Screen.drawStr(0, 30, "Drizzle");
  }
  else if (weatherCode == 4 || weatherCode == 5)
  {
    u8g2Screen.drawStr(0, 30, "Smoke");
  }
  else if (weatherCode == 14 || weatherCode == 15 || weatherCode == 16)
  {
    u8g2Screen.drawStr(0, 28, "Distant rain");
  }
  else if (weatherCode == 30)
  {
    u8g2Screen.drawStr(0, 28, "Squalls");
  }
  else if (weatherCode == 19)
  {
    u8g2Screen.drawStr(0, 30, "Funnel clouds");
  }

  u8g2Screen.sendBuffer();
}

void drawDate(WeatherData &weatherData)
{
  // month
  char months[][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
  char monthNames[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  char *monthName = "???";
  for (int i = 0; i < 12; i++)
  {
    if (strcmp(weatherData.month, months[i]) == 0)
    {
      monthName = monthNames[i];
      break;
    }
  }

  // day
  char dayStr[5];
  strcat(dayStr, weatherData.day);
  if (weatherData.day[1] == '1' && strcmp(weatherData.day, "11") != 0)
  {
    strcat(dayStr, "st");
  }
  else if (weatherData.day[1] == '2' && strcmp(weatherData.day, "12") != 0)
  {
    strcat(dayStr, "nd");
  }
  else if (weatherData.day[1] == '3' && strcmp(weatherData.day, "13") != 0)
  {
    strcat(dayStr, "rd");
  }
  else
  {
    strcat(dayStr, "th");
  }

  char dateResult[9] = "";
  strcat(dateResult, monthName);
  strcat(dateResult, " ");
  strcat(dateResult, dayStr);

  u8g2Screen.setFont(u8g2_font_6x13_tr);
  u8g2Screen.drawStr(75, 13, dateResult);

  u8g2Screen.sendBuffer();
}

void setLoadingScreen(const char *msgln1, const char *msgln2)
{
  u8g2Screen.clearBuffer();
  u8g2Screen.setFont(u8g2_font_fub14_tr);
  u8g2Screen.drawStr(0, 15, "Loading...");

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 40, msgln1);
  u8g2Screen.drawStr(0, 50, msgln2);

  u8g2Screen.sendBuffer();
}

void setErrorScreen(const char *msgln1, const char *msgln2)
{
  u8g2Screen.clearBuffer();
  u8g2Screen.setFont(u8g2_font_fub14_tr);
  u8g2Screen.drawStr(0, 15, "Error");

  u8g2Screen.setFont(u8g2_font_5x8_tr);
  u8g2Screen.drawStr(0, 40, msgln1);
  u8g2Screen.drawStr(0, 50, msgln2);

  u8g2Screen.sendBuffer();
}

void setForecastScreen(const char *loc, WeatherData &weatherData)
{
  u8g2Screen.clearBuffer();

  // location
  u8g2Screen.setFont(u8g2_font_fub11_tf);
  u8g2Screen.drawStr(0, 13, loc);

  drawDate(weatherData);
  drawWeatherCode(weatherData.weatherCode);
  drawTemperature(weatherData.maxTemp, weatherData.minTemp);
  drawUVIndex(weatherData.uvIndex);
  drawPrecip(weatherData.precipPropMax, weatherData.precipSum);
}

void setCurrentWeatherScreen(const char *loc, CurrentWeatherData &weatherData)
{
  u8g2Screen.clearBuffer();

  // location
  u8g2Screen.setFont(u8g2_font_fub11_tf);
  u8g2Screen.drawStr(0, 13, loc);

  u8g2Screen.setFont(u8g2_font_6x13_tr);
  u8g2Screen.drawStr(80, 13, "Now");

  drawWeatherCode(weatherData.weatherCode);
  drawCurrentTemp(weatherData.currentTemp);
  drawHumidity(weatherData.humidity);
  drawApparentTemp(weatherData.apparentTemp);
}

void startScreen()
{
  u8g2Screen.begin();
}