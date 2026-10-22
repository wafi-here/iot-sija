// Deklarasi pin
const int pinTMP = A0;
const int ledMerah = 5;
const int ledKuning = 6;
const int ledHijau = 7;

void setup() {
  // Inisialisasi komunikasi serial untuk membaca suhu di Serial Monitor
  Serial.begin(9600);
  
  // Mengatur pin LED sebagai OUTPUT
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
}

void loop() {
  // 1. Membaca nilai analog dari sensor TMP36 (0 - 1023)
  int nilaiAnalog = analogRead(pinTMP);
  
  // 2. Mengubah nilai analog menjadi tegangan (Volt)
  float tegangan = nilaiAnalog * (5.0 / 1023.0);
  
  // 3. Mengubah tegangan menjadi suhu dalam Celcius
  // Rumus TMP36: Suhu (°C) = (Tegangan - 0.5V) * 100
  float suhu = (tegangan - 0.5) * 100.0;
  
  // Menampilkan hasil pembacaan suhu ke Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" C");
  
  // 4. Logika Indikator LED berdasarkan suhu
  if (suhu < 25.0) {
    // Suhu Dingin / Normal: LED Hijau Menyala
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledMerah, LOW);
  } 
  else if (suhu >= 25.0 && suhu <= 35.0) {
    // Suhu Hangat: LED Kuning Menyala
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledMerah, LOW);
  } 
  else {
    // Suhu Panas (> 35°C): LED Merah Menyala
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledMerah, HIGH);
  }
  
  // Tunda pembacaan selama 1 detik
  delay(1000);
}