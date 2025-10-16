// Gerekli Kütüphane: I2C LCD için
#include <LiquidCrystal_I2C.h>

// --- Pin Tanımlamaları ---
const int nemSensorPin = A0;
const int uyariLedPin = 8;
const int buzzerPin = 9;

// --- Kalibrasyon ve Eşik Ayarları ---
// BU DEĞERLERİ KENDİ SENSÖRÜNÜZE GÖRE AYARLAYIN!
const int HAVADA_DEGER = 950; // Sensör havadayken okunan değer
const int SUDA_DEGER = 350;  // Sensör suya dalmışken okunan değer

// UYARI EŞİKLERİ
const int KURU_YUZDE_ESIK = 30; // %30'un altı kuru kabul edilsin
const int ISLAK_YUZDE_ESIK = 85; // %85'in üstü aşırı nemli kabul edilsin

// LCD Objesi (Adres 0x27, çalışmazsa 0x3F dene)
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  // Pin modlarını ayarla
  pinMode(uyariLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // LCD'yi Başlat
  lcd.init();
  lcd.backlight();

 
  lcd.setCursor(3, 0); // Yazıyı ortalamak için
  lcd.print("ta2cay");
  lcd.setCursor(0, 1);
  lcd.print("Dijital Sulama"); // 

  Serial.begin(9600);
  delay(3000); // Mesajın 3 saniye görünmesini sağla
}


void loop() {
  // Nem Sensöründen Ham Değeri Oku
  int nemHamDeger = analogRead(nemSensorPin);

  // Ham değeri 0-100 arası yüzdeye çevir
  int nemYuzde = map(nemHamDeger, HAVADA_DEGER, SUDA_DEGER, 0, 100);
  nemYuzde = constrain(nemYuzde, 0, 100); // Değeri 0-100 aralığında tut

  lcd.clear();

  
  lcd.setCursor(0, 0);
  lcd.print("Toprak Nem: %");
  // Fotoğraftaki gibi tek haneli sayıların başında boşluk bırakmak için:
  if (nemYuzde < 10) {
    lcd.print(" ");
  }
  lcd.print(nemYuzde);


  
  lcd.setCursor(0, 1);

  if (nemYuzde < KURU_YUZDE_ESIK) {
    // Durum: Kuru 
    lcd.print("ACIL SULAMA GEREK");
    uyariVer(); // LED ve Buzzer ile 3 kez kesikli uyarı ver
  }
  else if (nemYuzde > ISLAK_YUZDE_ESIK) {
    // Durum: Aşırı Nemli 
    lcd.print("ASIRI SULANMIS!");
    digitalWrite(uyariLedPin, LOW);
    noTone(buzzerPin);
  }
  else {
   
    lcd.print("I Y I  D U R U M"); 
    digitalWrite(uyariLedPin, LOW);
    noTone(buzzerPin);
  }

  // Seri Monitörde Kontrol
  Serial.print("Ham Deger: ");
  Serial.print(nemHamDeger);
  Serial.print(" | Yuzde Deger: ");
  Serial.println(nemYuzde);

  delay(2000);
}

// LED ve Buzzer ile 3 kez kesik kesik uyarı veren fonksiyon 
void uyariVer() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(uyariLedPin, HIGH);
    tone(buzzerPin, 1000);
    delay(200);
    digitalWrite(uyariLedPin, LOW);
    noTone(buzzerPin);
    delay(200);
  }
}