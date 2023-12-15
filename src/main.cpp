#include <Arduino.h>
#include "secrets.h"
#include "WiFiS3.h"
#include <NTPClient.h>
#include "screen.h"
#include "WeatherData.h"
#include "Location.h"
#include "constants.h"
#include "webUtil.h"
#include "CurrentWeatherData.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
bool weatherAndTimeUpdated = false; // Makes sure we don't continuously update weather after midnight

int datePos = 0;
int locationPos = 0;
int moveMillis = 0; // the result of millis() when the screen was last moved. We need this to prevent moving through the data too fast.

Location locations[MAX_LOCATIONS];
WeatherData weatherData[MAX_LOCATIONS][MAX_FORCAST];

int joystickSwitchPin = 2;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  pinMode(joystickSwitchPin, INPUT_PULLUP);

  // start screen and display loading
  startScreen();
  setLoadingScreen("Connecting to network", "");

  // attempt to connect to WiFi network
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
  }

  Serial.println("Connected.");

  setLoadingScreen("Getting current time", "");

  // update current time
  timeClient.setTimeOffset(39600);
  timeClient.begin();
  while (!timeClient.update())
    ; // continously attempt an update since we usually get a few fails in startup

  setLoadingScreen("Preparing locations", "");

  // set locations
  char locNames[][MAX_LOCATION_NAME_SIZE] = {"Sydney", "Oatley", "Fairfield", "Winmalee"};
  float locLats[] = {-33.8688, -33.9788, -33.8706, -33.6722};
  float locLongs[] = {151.2093, 151.0796, 150.9564, 150.6094};

  // check we haven't exceeded max location count
  if ((sizeof(locNames) / MAX_LOCATION_NAME_SIZE / sizeof(char *)) > MAX_LOCATIONS)
  {
    setErrorScreen("Number of locations was", "greater than max locations");
    while (true)
      ;
  };

  // set locations
  for (int i = 0; i < MAX_LOCATIONS; i++)
  {
    strncpy(locations[i].name, locNames[i], sizeof(locNames[i]));
    locations[i].latCoord = locLats[i];
    locations[i].longCoord = locLongs[i];
  }

  for (int i = 0; i < MAX_LOCATIONS; i++)
  {
    setLoadingScreen("Getting weather for", locations[i].name);
    if (retrieveWebWeatherForecast(weatherData[i], locations[i]) != 0)
    {
      setErrorScreen("Could not retrieve", "weather data");
      while (true)
        ;
    }
  }

  setForecastScreen(locations[locationPos].name, weatherData[locationPos][datePos]);
}

void loop()
{
  bool screenRefreshNeeded = false; // allows us to only ever call setForecastScreen once per loop

  // attempt to connect to WiFi network
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
  }

  // request update time and weather after midnight
  if (timeClient.getHours() == 0 && !weatherAndTimeUpdated)
  {
    timeClient.update();
    for (int i = 0; i < MAX_LOCATIONS; i++)
    {
      setLoadingScreen("Getting weather for", locations[i].name);
      if (retrieveWebWeatherForecast(weatherData[i], locations[i]) != 0)
      {
        setErrorScreen("Could not retrieve", "weather data");
        while (true)
          ;
      }
    }
    weatherAndTimeUpdated = true;
    screenRefreshNeeded = true;
  }

  // prep for updating weather next midnight
  if (timeClient.getHours() == 1 && weatherAndTimeUpdated)
  {
    weatherAndTimeUpdated = false;
  }

  // handle pressing the joystick for current weather
  int joystickSwitch = digitalRead(joystickSwitchPin);
  if (joystickSwitch == 0)
  {
    CurrentWeatherData currentWeatherData;
    
    char currentLoadingLine2[MAX_LOCATION_NAME_SIZE + 5] = "for ";
    strcat(currentLoadingLine2, locations[locationPos].name);
    setLoadingScreen("Getting current weather", currentLoadingLine2);

    if (retrieveCurrentWebWeather(currentWeatherData, locations[locationPos]) == 0)
    {
      setCurrentWeatherScreen(locations[locationPos].name, currentWeatherData);
    }
    else
    {
      setErrorScreen("Couldn't get weather", "Returning to forecasts");
    }
    delay(5000);
    setForecastScreen(locations[locationPos].name, weatherData[locationPos][datePos]);
  }

  // handle moving across the display
  int joystickX = analogRead(A4);
  int joystickY = analogRead(A3);

  if (millis() - moveMillis > 500)
  {
    if (joystickX > 765 & datePos < MAX_FORCAST - 1)
    {
      datePos++;
      screenRefreshNeeded = true;
      moveMillis = millis();
    }
    else if (joystickX < 255 & datePos > 0)
    {
      datePos--;
      screenRefreshNeeded = true;
      moveMillis = millis();
    }
    if (joystickY > 765 & locationPos < MAX_LOCATIONS - 1)
    {
      locationPos++;
      screenRefreshNeeded = true;
      moveMillis = millis();
    }
    else if (joystickY < 255 & locationPos > 0)
    {
      locationPos--;
      screenRefreshNeeded = true;
      moveMillis = millis();
    }
  }

  // do any updates at the end so we only update once
  if (screenRefreshNeeded)
  {
    setForecastScreen(locations[locationPos].name, weatherData[locationPos][datePos]);
  }
}