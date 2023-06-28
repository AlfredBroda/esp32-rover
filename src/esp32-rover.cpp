/*********
  Based on ESP32-cam code by Rui Santos at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <math.h>
#include "esp_timer.h"
#include "Arduino.h"
#include "soc/soc.h"          // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems
#include "esp_http_server.h"

#include "secrets.h"
#include "state/structs.h"
#include "state/tracker.h"

#define CUTTER_RELAY
#include "cutter/relay.cpp"

#include "sensors/mpu.h"

#define LED_PIN 2        // GPIO pin connected to the built LED
#define EMERGENCY_PIN 33 // GPIO pin connected to the emergency stop button

// GPIO pin connected to the cutter driver
#define CUTTER_PIN 4

Cutter cutter = Cutter(CUTTER_PIN);

// GPIO pins for the motor driver
#define MOTOR_1_PIN_1 27
#define MOTOR_1_PIN_2 26
#define MOTOR_2_PIN_1 13
#define MOTOR_2_PIN_2 12

const char *HOSTNAME = "esp32-mower";

httpd_handle_t esp_httpd = NULL;

Tracker stateTracker = Tracker(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_2_PIN_1, MOTOR_2_PIN_2, IMU());

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32 Rover</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #2f4468;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
      .abort {
        background-color: #ff0000;
      }
      img {  width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
    </style>
  </head>
  <body>
    <h1>ESP32 Rover</h1>
    <table>
      <tr>
        <td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');">Forward</button></td>
      </tr><tr>
        <td align="center"><button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Left</button></td>
        <td align="center"><button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">Stop</button></td>
        <td align="center"><button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Right</button></td>
      </tr><tr>
        <td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');">Backward</button></td>
      </tr>
      <tr>
        <td align="center"><button class="button" onmousedown="toggleCheckbox('cutter');" ontouchstart="toggleCheckbox('cutter');">Cutter Start</button></td>
        <td align="center"><button class="button abort" onmousedown="toggleCheckbox('abort');" ontouchstart="toggleCheckbox('abort');">Emergency Stop</button></td>
        <td align="center"><button class="button" onmousedown="toggleCheckbox('cutter_stop');" ontouchstart="toggleCheckbox('cutter_stop');">Cutter Stop</button></td>
      </tr>
    </table>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?go=" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

static esp_err_t cmd_handler(httpd_req_t *req)
{
  char *buf;
  size_t buf_len;
  char variable[32] = {
      0,
  };

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1)
  {
    buf = (char *)malloc(buf_len);
    if (!buf)
    {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
    {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK)
      {
      }
      else
      {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    }
    else
    {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  }
  else
  {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  int res = 0;

  if (!strcmp(variable, "abort"))
  {
    cutter.stop();
    stateTracker.emergencyStop();
  }
  else if (!strcmp(variable, "forward"))
  {
    stateTracker.setState(FORWARD);
  }
  else if (!strcmp(variable, "left"))
  {
    stateTracker.setState(LEFT);
  }
  else if (!strcmp(variable, "right"))
  {
    stateTracker.setState(RIGHT);
  }
  else if (!strcmp(variable, "backward"))
  {
    stateTracker.setState(BACKWARD);
  }
  else if (!strcmp(variable, "stop"))
  {
    stateTracker.setState(STOP);
  }
  else if (!strcmp(variable, "cutter"))
  {
    Serial.println("Cutter ON");
    cutter.start();
  }
  else if (!strcmp(variable, "cutter_stop"))
  {
    Serial.println("Cutter OFF");
    cutter.stop();
  }
  else
  {
    res = -1;
  }

  if (res)
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startServer()
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = index_handler,
      .user_ctx = NULL};

  httpd_uri_t cmd_uri = {
      .uri = "/action",
      .method = HTTP_GET,
      .handler = cmd_handler,
      .user_ctx = NULL};
  if (httpd_start(&esp_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(esp_httpd, &index_uri);
    httpd_register_uri_handler(esp_httpd, &cmd_uri);
  }
}

void setup()
{
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  pinMode(LED_PIN, OUTPUT);
  pinMode(EMERGENCY_PIN, INPUT);

  // setup drive motors
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);

  Serial.begin(115200);
  Serial.setDebugOutput(false);

  // Wi-Fi connection
  WiFi.begin(SSID, PASSWORD);
  WiFi.setHostname(HOSTNAME);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Start generating signal for the cutter motor
  Serial.println("Cutter init");
  cutter.init();

  stateTracker.init();

  Serial.print("Rover Ready! Go to: http://");

  Serial.println(WiFi.localIP());

  digitalWrite(LED_PIN, HIGH);

  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASS);
  // Password can be set with it's md5 value as well
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  // Start web server
  startServer();
}

unsigned long lastTime = 0;
unsigned long timeElapsed = 0;
int blinkTime = 1000;
bool ledState = false;

void loop()
{
  ArduinoOTA.handle();
  stateTracker.loop();

  if (digitalRead(EMERGENCY_PIN) == HIGH)
  {
    stateTracker.emergencyStop();
    cutter.stop();
    sleep(30);
  }

  timeElapsed = millis() - lastTime;
  if (timeElapsed > blinkTime)
  {
    // Debug output
    Serial.printf("Current heading: %d, State: %s, %s\n", stateTracker.getActualHeading(), stateTracker.getStateString(), stateTracker.getCurrentAction());

    digitalWrite(LED_PIN, ledState);
    lastTime = millis();
    ledState = !ledState;
  }
}
