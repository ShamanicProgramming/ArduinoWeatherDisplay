#include "webUtil.h"
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include "WiFiS3.h"
#include "Location.h"
#include "constants.h"
#include "CurrentWeatherData.h"

WiFiClient wifiClient;

/// @brief Sends a web request to populate the weather data objects using the data in the location object
/// @return 0 if successful, else error
int retrieveWebWeatherForecast(WeatherData weatherData[], Location &location)
{
    StaticJsonDocument<2048> doc;

    // These requests would ideally be non blocking (handle the response when it actually gets here rather than waiting for it)
    // This HTTPClient library only has blocking request methods at time of writing
    // We can settle with blocking for this project since we're not constantly sending requests. Otherwise I would have found a different solution

    HttpClient client = HttpClient(wifiClient, "api.open-meteo.com", 80);
    char request[300];
    snprintf(request, 300, "/v1/forecast?latitude=%f&longitude=%f&daily=weather_code,temperature_2m_max,temperature_2m_min,uv_index_max,precipitation_sum,precipitation_probability_max&timezone=Australia%%2FSydney&forecast_days=%d",
             location.latCoord,
             location.longCoord,
             MAX_FORCAST);
    if (client.get(request) != 0)
    {
        return 1;
    }
    int status = client.responseStatusCode();

    if (status == 200)
    {
        deserializeJson(doc, client.responseBody());
    }

    for (int i = 0; i < MAX_FORCAST; i++)
    {
        weatherData[i].populateData(doc, i);
    }

    return 0;
}

/// @brief Sends a web request to populate the weather data object using the data in the location object
/// @return 0 if successful, else error
int retrieveCurrentWebWeather(CurrentWeatherData &currentWeatherData, Location &location)
{
    StaticJsonDocument<1000> doc;

    // These requests would ideally be non blocking (handle the response when it actually gets here rather than waiting for it)
    // This HTTPClient library only has blocking request methods at time of writing
    // We can settle with blocking for this project since we're not constantly sending requests. Otherwise I would have found a different solution

    HttpClient client = HttpClient(wifiClient, "api.open-meteo.com", 80);
    char request[300];
    snprintf(request, 300, "/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code&timezone=Australia%%2FSydney",
             location.latCoord,
             location.longCoord);
    if (client.get(request) != 0)
    {
        return 1;
    }
    int status = client.responseStatusCode();

    if (status == 200)
    {
        deserializeJson(doc, client.responseBody());
    }
    
    currentWeatherData.populateData(doc);

    return 0;
}