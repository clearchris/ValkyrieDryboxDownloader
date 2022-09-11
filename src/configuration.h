#include <Preferences.h>
#include "settings.h"

class Configuration
{
  public:
    void setup(boolean readonly);
    void read();
    void write();
    String ssid;
    String password;
    String dwcName;             // name/IP of DWC 
    String dryboxName;          //local name of esp32
    boolean standalone;
    uint8_t ledPin;
    int reportTime;            // report weight to DWC interval
    int refreshTime;          // web site refresh time
    int tempReadTime;          // time to read temperature and RH
  private:
    Preferences preferences;
};