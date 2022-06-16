 
/***************************************************

Графический тест

Эта библиотека предназначена для работы с TFT-дисплеями (с SPI-интерфейсом) от Adafruit. Библиотека поддерживает следующие устройства:

* 1,8-дюймовую TFT-плату с SD-картой:
  https://www.adafruit.com/products/358
* 1,8-дюймовый TFT-модуль:
  https://www.adafruit.com/product/802
* 1,44-дюймовую TFT-плату:
  https://www.adafruit.com/product/2088
* «Голый» 1,8-дюймовый TFT-дисплей (без плат и модулей):
  https://www.adafruit.com/products/618  

Руководства и схемы подключения ищите по ссылкам выше. Этим дисплеям 
для коммуникации требуется SPI-интерфейс с 4 или 5 контактами (контакт 
RST опционален).

Adafruit инвестировала время и ресурсы, создавая эту библиотеку с 
открытым кодом. Пожалуйста, поддержите Adafruit и оборудование с 
открытым кодом, покупая продукты Adafruit!

Библиотека написана Лимор Фрид (Limor Fried, Ladyada) для Adafruit 
Industries. Весь текст выше должен быть включен при любом повторном 
распространении.

****************************************************/

#include <Adafruit_GFX.h>    // подключаем графическую библиотеку
#include <Adafruit_ST7735.h> // подключаем библиотеку для управления дисплеем
#include <SPI.h>


// для TFT-платы можно использовать 2-3 контакта;
// эти контакты будут работать и для 1,8-дюймового TFT-модуля:
#define TFT_CS     10
#define TFT_RST    7  // этот контакт можно подключить к RESET-
                      // контакту Arduino, но тогда вместе «9» 
                      // надо указать «0».
#define TFT_DC     5

// Вариант 1 (рекомендуемый): используются аппаратные SPI-контакты
// (на UNO: SCLK – это 13-ый контакт, а MOSI – это 11-ый контакт),
// а 10-ый контакт должен быть выходным. Этот способ быстрее, и его
// необходимо использовать, если вы работаете с картой microSD
// (подробнее смотрите в примере, где рисуется изображение):
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Вариант 2: можно использовать любые контакты,
// но процесс будет идти медленней!
#define TFT_SCLK 13   // здесь можно задать любой контакт
#define TFT_MOSI 11   // здесь можно задать любой контакт
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}

float p = 3.1415926;
byte azimuth(0);
byte angle(0);
byte tilt(0);

char degree = 0xF8;

void setup(void) {
  // используйте этот инициализатор, если работаете 
  // с 1,44-дюймовым TFT-дисплеем (нужно раскомментировать):
  tft.initR(INITR_144GREENTAB);   // инициализируем чипa ST7735S, зеленый ярлычок
  tft.setRotation(90);
  
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  tft.drawCircle(tft.width()/2, tft.height()/2, 32, ST7735_WHITE);
}

void loop() {
  
  for(byte i(0); i < 360; i++){
    tft.drawCircle(tft.width()/2, tft.height()/2, 25, ST7735_WHITE);
    tft.drawLine(tft.width()/2, tft.height()/2-25, tft.width()/2, tft.height()/2+25, ST7735_WHITE);
    tft.drawRect(tft.width()/2-12, tft.height()/2 + 16, 24, 11, ST7735_WHITE);
  
    tft.drawLine(tft.width()/2-25, tft.height()/2, tft.width()/2+14, tft.height()/2, ST7735_WHITE);

    tft.drawRect(tft.width()/2+14,tft.height()/2-5,19, 11, ST7735_WHITE);
    tft.fillRect(tft.width()/2+15, tft.height()/2-4, 17, 9, ST7735_BLACK);
    tft.fillRect(tft.width()/2-11, tft.height()/2 + 17, 22, 9, ST7735_BLACK);
  
  if (i < 10)
    tft.setCursor(tft.width()/2-5, tft.height()/2 + 18);
  else if (i > 9 && i < 100)
    tft.setCursor(tft.width()/2-8, tft.height()/2 + 18);
  else
    tft.setCursor(tft.width()/2-10, tft.height()/2 + 18);
    
  tft.print(i);
  tft.print(degree);
  
  tft.setCursor(tft.width()/2+16, tft.height()/2-3);
  tft.print((int)(i/10));
  tft.print(degree);

  tft.drawLine(tft.width()/2 + 25.0*cos(i*p/180), tft.height()/2 + 25.0*sin(i*p/180), tft.width()/2 + 32.0*cos(i*p/180), tft.height()/2 + 32.0*sin(i*p/180), ST7735_WHITE);
  
  delay(500);}
  tft.fillScreen(ST7735_BLACK);

}
