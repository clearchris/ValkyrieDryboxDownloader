#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp32fota.h>
#include "configuration.h"
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <nvs_flash.h>

Configuration config;

WiFiMulti wifiMulti;

char* test_root_ca= \
"-----BEGIN CERTIFICATE-----\n"
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
"-----END CERTIFICATE-----\n";

String otaBranch = "valkyrie-drybox-dwc-stable";
String otaUrl = "https://raw.githubusercontent.com/clearchris/ValkyrieDryboxFirmware/main/" + otaBranch + ".json";
esp32FOTA sota(otaBranch, 0, false, true);

WiFiClientSecure clientForOta;

boolean checkOta = true;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Clearing NVS partition.");
  nvs_flash_erase();
  nvs_flash_init();

  config.setup(false);
  config.read();
  Serial.println("Writing configuration.");
  config.write();

  Serial.print("Wait");
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf(".");
    Serial.flush();
    delay(1000);
  }
  Serial.println("");

  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(config.dryboxName.c_str()); 
  wifiMulti.addAP(config.ssid.c_str(), config.password.c_str());
  Serial.println("Waiting for WiFi to connect.");
  while(wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }
  Serial.println();
  Serial.println("Wifi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  sota.checkURL = otaUrl;
  //sota.setManifestURL( otaUrl );
  //sota.setCertFileSystem(nullptr);
}

void loop() {
  if(checkOta){
    if(sota.execHTTPcheck()){
      Serial.println("Firmware update needed.");
      sota.execOTA();
    }
    else Serial.println("No firmware update found.");
  }
  checkOta = false;
}