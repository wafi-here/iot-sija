#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);

void setup(){
    lcd.begin(16,2);
    Serial.begin(9600);
    lcd.setCursor(0,0);
    lcd.print("Halo")

    lcd.setCursor(0,1);
    lcd.print("Saya Wafi");
}

void loop(){
    
}