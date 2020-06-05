#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6 //여기에 해당하는 단자를 나노리프 D번과 연결(아두이노 나노는 D6번에 해당)

const int my_i2c_addr = 7;  // 각 나노마다 다르게 변경하여 컴파일/업로드 - 주소가 달라야 함 
const int led_pin =  13;  

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(led_pin, OUTPUT);

  Wire.begin(my_i2c_addr);
  Wire.onReceive(recvData);
  //Wire.onRequest(sendData); 수신할 필요가 없기 때문에 사용 X
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  delay(100);
}

void recvData(int byte_count) {
  while( Wire.available()) {
    int on_off = Wire.read();
    if (on_off == 0) {
      
      rainbow(20); // rainbow
    }
    if (on_off == 1) {
      
      colorWipe(strip.Color(255, 0, 0), 50); // Red
    }
    else if (on_off == 2) {
      
      colorWipe(strip.Color(255, 94, 0), 50); // Ornage
    }
     else if (on_off == 3) {
      
      colorWipe(strip.Color(255, 255, 0), 50); // Yellow
    }
    else if (on_off == 4) {
      
      colorWipe(strip.Color(0, 255, 0), 50); // Green
    }
    else if (on_off == 5) {
      
      colorWipe(strip.Color(0, 0, 255), 50); // Blue
    }
    else if (on_off == 6) {
      
      colorWipe(strip.Color(95, 0, 255), 50); // Purple
    }
    else if (on_off == 7) {
      
      colorWipe(strip.Color(255, 0, 255), 50); //Pink
    }
    else if (on_off == 8) {
      
      colorWipe(strip.Color(255, 255, 255), 50); // Blue
    }
    else {
      //digitalWrite(led_pin, LOW);
      colorWipe(strip.Color(0, 0, 0), 50); //stay?
    }
  }
}

void sendData() {
  Wire.write(1);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
