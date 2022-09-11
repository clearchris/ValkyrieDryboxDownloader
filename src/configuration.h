#include <Preferences.h>
#include "settings.h"

class Configuration
{
  public:
    Configuration();
    Configuration(boolean);
    void write();
    String ssid;
    String password;
    String dwcName;             // name/IP of DWC 
    String dryboxName;          //local name of esp32
    int reportTime;            // report weight to DWC interval
    int refreshTime;          // web site refresh time
    int ahtReadTime;          // time to read temperature and RH
  private:
    Preferences preferences;
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};