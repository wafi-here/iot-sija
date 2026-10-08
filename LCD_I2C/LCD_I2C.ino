#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C (Alamat 0x27, 16 Kolom, 2 Baris)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Struktur data untuk menyimpan baris lirik dan durasi tampil
struct LyricFrame {
  const char* line1;
  const char* line2;
  unsigned int durationMs;
};

// Array lirik beserta penyesuaian durasi (Tempo ~135 BPM)
LyricFrame lyrics[] = {
  {"Teman ku semua", "pada jahat tante", 3500},
  {"aku lagi susah",  "mereka ga ada",    3500},
  {"coba kalau lagi", "jaya",            3000},
  {"aku di puja",     "puja tante",  4000}
};

const int totalFrames = sizeof(lyrics) / sizeof(lyrics[0]);

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  for (int i = 0; i < totalFrames; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lyrics[i].line1);
    lcd.setCursor(0, 1);
    lcd.print(lyrics[i].line2);
    delay(lyrics[i].durationMs);
  }
}