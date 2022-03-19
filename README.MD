# Simple Clock for M5StickCPlus
Very simple clock for a M5StickCPlus with NTP time-sync based on
LordLeXaT's [m5StickC-Clock](https://github.com/LordLeXaT/m5StickC-Clock).

### What does it do?
It's showing the time :D
* Showing data and time
* Short press button A(Home) to turn off LCD (power saving) or
  set the minimum brightness
* Long press (2 seconds) button B forces resync with NTP-Server
  via your WIFI hotspot

![Demo Picture](https://raw.githubusercontent.com/xyb/M5StickCPlus-Clock/master/img/stickClock.jpg)

## Getting Started
* Change your WIFI credentials(`wificonfig.h`), timezone and maybe NTP
  Server(`timeconfig.h`)
* Compile `m5stickcplus-clock.ino` file and upload it to M5StickCPlus
