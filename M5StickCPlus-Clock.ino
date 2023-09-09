#include <M5StickCPlus.h>
#include <WiFi.h>
#include "time.h"
#include "wificonfig.h"
#include "timeconfig.h"

// delay workarround
int tcount = 0;

// LCD Status
int LCD = 2;  // init with highest brightness

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

void turn_off_lcd() {
  M5.Axp.SetLDO2(false);
}

void turn_on_lcd() {
  M5.Axp.SetLDO2(true);
}

void set_maximum_brightness() {
  M5.Axp.ScreenBreath(12);
}

void set_minimum_brightness() {
  M5.Axp.ScreenBreath(7);
}

void buttons_code() {
  // Button A control the LCD (ON/OFF) and brightness
  if (M5.BtnA.wasPressed()) {
    LCD = (LCD + 1) % 3;
    Serial.println(LCD);
    if (LCD == 2) {
      turn_on_lcd();
      set_maximum_brightness();
    } else if (LCD == 1) {
      turn_on_lcd();
      set_minimum_brightness();
    } else {
      turn_off_lcd();
      set_minimum_brightness();
    }
  }
  // Button B doing a time resync if pressed for 2 sec
  if (M5.BtnB.pressedFor(2000)) {
    timeSync();
  }
}

// Printing time to LCD
void doTime() {
  if (timeToDo(1000)) {
    Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(0, 0, 0));
    M5.Rtc.GetTime(&rtc_time);
    M5.Rtc.GetDate(&rtc_date);

    Disbuff.setTextSize(4);
    Disbuff.setTextColor(TFT_WHITE);

    Disbuff.setCursor(25, 50);
    Disbuff.printf("%02d:%02d:%02d",
                   rtc_time.Hours,
                   rtc_time.Minutes,
                   rtc_time.Seconds);

    Disbuff.fillRect(0, 0, 240, 25, M5.Lcd.color565(20, 20, 20));
    Disbuff.setCursor(60, 5);
    Disbuff.setTextSize(2);
    Disbuff.printf("%04d-%02d-%02d",
                   rtc_date.Year,
                   rtc_date.Month,
                   rtc_date.Date);

    Disbuff.pushSprite(0, 0);
  }
}

//delays stopping usualy everything using this workaround
bool timeToDo(int tbase) {
  tcount++;
  if (tcount == tbase) {
    tcount = 0;
    return true;
  } else {
    return false;
  }
}

// Syncing time from NTP Server
void timeSync() {
  M5.Lcd.setTextSize(1);
  Serial.println("Syncing Time");
  Serial.printf("Connecting to %s ", ssid);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 15);
  M5.Lcd.printf("connecting WiFi: %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    M5.Lcd.print(".");
  }
  Serial.println(" CONNECTED");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 15);
  M5.Lcd.println("Connected");
  // Set ntp time to local
  configTime(timeZone, 0, ntpServer);

  // Get local time
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    // Set RTC time
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours   = timeInfo.tm_hour;
    TimeStruct.Minutes = timeInfo.tm_min;
    TimeStruct.Seconds = timeInfo.tm_sec;
    M5.Rtc.SetTime(&TimeStruct);

    RTC_DateTypeDef DateStruct;
    DateStruct.WeekDay = timeInfo.tm_wday;
    DateStruct.Month = timeInfo.tm_mon + 1;
    DateStruct.Date = timeInfo.tm_mday;
    DateStruct.Year = timeInfo.tm_year + 1900;
    M5.Rtc.SetDate(&DateStruct);
    Serial.println("Time now matching NTP");
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(20, 15);
    M5.Lcd.println("S Y N C");
    delay(500);
    M5.Lcd.fillScreen(BLACK);
  }
}

void setup() {
  M5.begin();

  M5.Lcd.setRotation(3);

  Disbuff.createSprite(240, 135);
  Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(10, 10, 10));
  Disbuff.pushSprite(0, 0);
}

void loop() {
  M5.update();
  buttons_code();
  doTime();
}
