#pragma once

#include "Uhr.h"
#include <Arduino.h>
#include <cmath>

/*--------------------------------------------------
List of Conditions http://openweathermap.org/weather-conditions
Weather condition code (wetterid):

2xx Thunderstorm
3xx Drizzle
5xx Rain
6xx Snow
7xx Clouds (Warning)
800 Clear
80x Clouds
--------------------------------------------------*/

class OpenWMap {
private:
    const char *server =
        "api.openweathermap.org"; // Openweather server's address
    const char *resource1 =
        "/data/2.5/forecast?id="; // Openweather API URL part 1
    const char *resource2 =
        "&units=metric&APPID=";       // Openweather API URL part 2
    const char *resource3 = "&cnt=8"; // Openweather API forecast time
    char resource[100];
    char response[3500];     // fixed size buffer
    uint16_t weatherCounter; // counter fuer Wetterdaten abrufen
    int8_t wTempNow;
    uint16_t wWeatherNow;
    uint16_t wHour;
    uint16_t wWeatherSwitch;

private:
    void determineDaytime(uint8_t hour) {
        uint8_t countId = 0;
        hour += 24; // Offset by 24 hours
        hour -= 3;  // Offset to get corrospondance
        hour %= 24; // Offset for nighttime

        if (hour < 6) { // Nacht: Vanaf 00:00 uur tot 06:00 uur
            wHour = 3;
        } else if (hour < 12) { // Ochtend: Vanaf 06:00 uur tot 12:00 uur
            wHour = 4;
        } else if (hour < 18) { // Middag: Vanaf 12:00 uur tot 18:00 uur
            wHour = 1;
        } else { // Avond: Vanaf 18:00 uur tot 00:00 uur (volgende dag)
            wHour = 2;
        }
    }

    //------------------------------------------------------------------------------

    void determineWTemp(const double srctemp, int8_t &destTemp) {
        if (srctemp >= 35) {
            destTemp = 35;
        } else if (srctemp >= 0 && srctemp <= 5) {
            destTemp = 1;
        } else if (srctemp < 0 && srctemp >= -5) {
            destTemp = -1;
        } else if (srctemp <= -25) {
            destTemp = -25;
        } else {
            destTemp = static_cast<int8_t>(srctemp - fmod(srctemp, 5));
        }
    }

    //------------------------------------------------------------------------------

    void determineWid(uint16_t srcId, uint16_t &destId) {
        uint8_t countId = 0;
        if (srcId == 800) {
            destId = 800;
        } else if (srcId > 801) {
            destId = 802;
        } else {
            for (uint8_t i = 0; i < 8; i++) {
                if (srcId < 100) {
                    if (countId < 2) {
                        Serial.println("[ERROR] determineWid() Out of bounds");
                    }
                    destId = countId * 100;
                }
                srcId -= 100;
                countId++;
            }
        }
    }

    //------------------------------------------------------------------------------

    void printDeterminedData() {
        Serial.print("Temp_Now - ");
        Serial.println(wTempNow);
        Serial.print("wWeatherNow - ");
        Serial.println(wWeatherNow);
        Serial.print("wHour - ");
        Serial.println(wHour);
        Serial.println("--------- ");
    }

    //------------------------------------------------------------------------------

    void pullWeatherData() {

        // connect to server
        bool ok = client.connect(server, 80);
        bool beginFound = false;

        Serial.println("");
        Serial.println("--------------------------------------");
        Serial.println("Connecting to Openweathermap.org");
        Serial.println("--------------------------------------");

        // create calling URL
        memset(resource, 0, sizeof(resource));
        strncat(resource, resource1, 22);
        strncat(resource, G.openWeatherMap.cityid, 7);
        strncat(resource, resource2, 20);
        strncat(resource, G.openWeatherMap.apikey, 35);
        strncat(resource, resource3, 6);

        Serial.print("Calling URL: ");
        Serial.println(resource);

        if (ok == 1) {

            // Send request to resource
            client.print("GET ");
            client.print(resource);
            client.println(" HTTP/1.1");
            client.print("Host: ");
            client.println(server);
            client.println("Connection: close");
            client.println();

            delay(100);

            // Reading stream and remove headers
            client.setTimeout(10000);

            bool ok_header = client.find("\r\n\r\n");

            while (!ok_header) {
                // wait
            }

            memset(response, 0, sizeof(response));
            client.readBytes(response, 3500);

            Serial.println("Antwort: ");
            Serial.println(response);

            int eol = sizeof(response);
            Serial.print("Length = ");
            Serial.println(eol);

            // process JSON
            DynamicJsonDocument doc(6144);

            // But.....make sure the stream header is valid
            // Sometime OWM includes invalid data after the header
            // Parsing fails if this data is not removed

            if (int(response[0]) != 123) {
                Serial.println("Wrong start char detected");
                uint32_t i = 0;
                while (!beginFound) {
                    if (int(response[i]) == 123) { // check for the "{"
                        beginFound = true;
                        Serial.println("{ found at ");
                        Serial.println(i);
                    }
                    i++;
                }

                int eol = sizeof(response);
                Serial.println("Length = ");
                Serial.println(eol);

                // restructure by shifting the correct data
                Serial.println("restructure");
                for (uint32_t c = 0; c < (eol - i); c++) {
                    response[c] = response[((c + i) - 1)];
                    Serial.println(response[c]);
                }

                Serial.println("Done...!");
            }

            auto error = deserializeJson(doc, response);
            if (error) {
                Serial.print(F("deserializeJson() failed with code "));
                Serial.println(error.c_str());
                return;
            } else {
                Serial.println("JSON parsing worked!");
            }

            // Fill Variable with json information
            const char *location = doc["city"]["name"];
            const char *wetter_now = doc["list"][0]["weather"][0]["description"];
            const int wetterid_now = doc["list"][0]["weather"][0]["id"];
            double temp_now = doc["list"][1]["main"]["temp"];

            Serial.print("*** ");
            Serial.print(location);
            Serial.println(" ***");
            Serial.println("----------");
            Serial.println("Now");
            Serial.print("Type: ");
            Serial.println(wetter_now);
            Serial.print("Wetter ID: ");
            Serial.println(wetterid_now);
            Serial.print("Temp: ");
            Serial.print(temp_now);
            Serial.println("°C");
            Serial.println("Hour");
            Serial.println(_hour);
            Serial.println("----------");

            determineWTemp(temp_now, wTempNow);
            determineWid(wetterid_now, wWeatherNow);

            determineDaytime(_hour);

            printDeterminedData();
        }
        client.stop(); // disconnect from server
    }

    //------------------------------------------------------------------------------

    bool checkWeatherCounter() {
        if (weatherCounter == 0) {
            weatherCounter = 600;
            return true;
        }
        return false;
    }

public:
    OpenWMap(/* args */) = default;
    ~OpenWMap() = default;

    //------------------------------------------------------------------------------

    void calcWeatherClockface() {
        switch (wWeatherNow) {
        case 200:
            usedUhrType->show(FrontWord::w_gewitter);
            break;
        case 300:
            usedUhrType->show(FrontWord::w_regen);
            break;
        case 500:
            usedUhrType->show(FrontWord::w_regen);
            break;
        case 600:
            usedUhrType->show(FrontWord::w_schnee);
            break;
        case 700:
            usedUhrType->show(FrontWord::w_warnung);
            break;
        case 800:
            // Only show sunny if not at night
            if(wHour != 3) {
                usedUhrType->show(FrontWord::w_klar);
            }
            break;
        case 801:
            if(wHour != 3) {
                usedUhrType->show(FrontWord::w_klar);
            }
            usedUhrType->show(FrontWord::w_wolken);
            break;
        case 802:
            usedUhrType->show(FrontWord::w_wolken);
            break;
        }
    }

    //------------------------------------------------------------------------------

    void loop() {
        if (WiFi.status() == WL_CONNECTED && checkWeatherCounter()) {
            Serial.println("Pulling Weather Data");
            pullWeatherData();
        }
        weatherCounter--;
    }
};