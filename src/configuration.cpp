#include "configuration.h"

Preferences preferences;

void Configuration::setup(boolean readonly){
  preferences.begin("valkyrie", readonly);
}

void Configuration::read(){
  ssid = preferences.getString("ssid", String(_SSID));
  password = preferences.getString("password", String(PASSWORD));
  dryboxName = preferences.getString("drybox_name", String(DRYBOX_NAME));
  dwcName = preferences.getString("dwc_name", String(DWC_NAME));
}

void Configuration::write(){
  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.putString("drybox_name", dryboxName);
  preferences.putString("dwc_name", dwcName);
}