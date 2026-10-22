#include <DHT.h>

// Definisi Pin dan Jenis Sensor
#define DHTPIN 2     // Pin digital Arduino tempat pin DATA DHT11 terhubung
#define DHTTYPE DHT11   // Menentukan jenis sensor yaitu DHT11

DHT dht(DHTPIN, DHTTYPE);

const int ledKanan = 5;     // LED Kanan (Suhu Terendah)
const int ledTengah = 6;    // LED Tengah (Suhu Menengah)
const int ledKiri = 7;      // LED Kiri (Suhu Tertinggi)

void setup() {
  // Mengatur pin LED sebagai OUTPUT
  pinMode(ledKanan, OUTPUT);
  pinMode(ledTengah, OUTPUT);
  pinMode(ledKiri, OUTPUT);
  
  // Memulai komunikasi Serial untuk memantau suhu
  Serial.begin(9600);
  
  // Memulai inisialisasi sensor DHT
  dht.begin();
}

void loop() {
  // Sensor DHT11 memerlukan jeda minimal 2 detik antar pembacaan
  delay(2000);

  // Membaca suhu dalam satuan Celsius
  float temperatureC = dht.readTemperature();
  
  // Memeriksa apakah pembacaan dari sensor gagal (NaN = Not a Number)
  if (isnan(temperatureC)) {
    Serial.println("Gagal membaca dari sensor DHT11! Periksa kembali kabel data.");
    return;
  }
  
  // Menampilkan hasil pembacaan suhu pada Serial Monitor
  Serial.print("Suhu Saat Ini: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
  
  // Mematikan semua LED terlebih dahulu di setiap awal perulangan
  digitalWrite(ledKanan, LOW);
  digitalWrite(ledTengah, LOW);
  digitalWrite(ledKiri, LOW);
  
  // Logika pengkondisian LED berdasarkan tingkat suhu
  if (temperatureC < 25.0) {
    digitalWrite(ledKanan, HIGH); // Suhu rendah -> LED Kanan menyala
  } 
  else if (temperatureC >= 25.0 && temperatureC < 30.0) {
    digitalWrite(ledTengah, HIGH); // Suhu menengah -> LED Tengah menyala
  } 
  else {
    digitalWrite(ledKiri, HIGH); // Suhu tinggi -> LED Kiri menyala
  }
}