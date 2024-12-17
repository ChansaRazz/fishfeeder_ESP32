#include <ESP32Servo.h>
#include "RTClib.h"
#include <Wire.h>
//#include <esp_wifi.h>
//#include <LiquidCrystal.h>  // libray lcd
//LiquidCrystal lcd(0x27, 16, 2);

RTC_DS3231 rtc;
char dataHari[7][12] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
String hari;
int tanggal, bulan, tahun, jam, menit, detik;
float suhu;

//ambil template_id, device_id, dan token blynk
#define BLYNK_TEMPLATE_ID "TMPL6kcHhtIl3"
#define BLYNK_TEMPLATE_NAME "Pakan IKAN"
#define BLYNK_AUTH_TOKEN "T38UGkwRJWmGcuJ86esddwSfxeArJ5V4"

//include library blynk
#include <BlynkSimpleEsp32.h>

//buat sebuah objek untuk servo
Servo myservo;

//variabel menampung datastream status pakan
int StatusPakan;

void setup() {
  Serial.begin(9600);
  //pin digunakan untuk servo = D2 = 4
  myservo.attach(2);
  //posisi servo awal
  myservo.write(0);
  //  lcd.begin();

   if (!rtc.begin()) {
     Serial.println("RTC Tidak Ditemukan");
     Serial.flush();
     abort();
    }

  //Atur Waktu
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2023, 11, 28, 17, 29, 25));

  //koneksi ke Wifi
  WiFi.begin("Kelompok 1", "12345678");

  //uji koneksi wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi Terkoneksi");

  //jika konek wifi, koneksikan ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, "Kelompok 1", "12345678");
  Serial.println("Blynk Terkoneksi");
}

void BeriPakan() {
  //proses perputaran servo secara halus
  for (int posisi = 0; posisi <= 180; posisi++) {
    //set posisi servo berdasarkan variable posisi
    myservo.write(posisi);
    delay(10);
  }
  //proses perputaran servo kembali posisi awal
  for (int posisi = 180; posisi >= 0; posisi--) {
    myservo.write(posisi);
    delay(10);
  }
}

void PakanOtomatis() {
  if (jam == 17 & menit == 30 & detik == 10) {
    for (int posisi = 0; posisi <= 180; posisi++) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);
    }

    for (int posisi = 180; posisi >= 0; posisi--) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);

      //      lcd.clear();
      //      lcd.setCursor(0, 0);
      //      lcd.print("Ikan Sudah Kenyang");
      //      lcd.setCursor(0, 1);
      //      lcd.print("                ");
      //      delay(1000);
    }
  }

  if (jam == 17 & menit == 34 & detik == 40) {
    for (int posisi = 0; posisi <= 180; posisi++) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);
    }

    for (int posisi = 180; posisi >= 0; posisi--) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);

      //      lcd.clear();
      //      lcd.setCursor(0, 0);
      //      lcd.print("Ikan Sudah Kenyang");
      //      lcd.setCursor(0, 1);
      //     lcd.print("                ");
      //     delay(1000);
    }
  }

  if (jam == 17 & menit == 35 & detik == 00) {
    for (int posisi = 0; posisi <= 180; posisi++) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);
    }

    for (int posisi = 180; posisi >= 0; posisi--) {
      //set posisi servo berdasarkan variable posisi
      myservo.write(posisi);
      delay(10);

      //    lcd.clear();
      //     lcd.setCursor(0, 0);
      //    lcd.print("Ikan Sudah Kenyang");
      //    lcd.setCursor(0, 1);
      //    lcd.print("                ");
      //    delay(1000);
    }
  }
}

void loop() {
  //jalankan aplikasi Blynk dari ESP32
  Blynk.run();

  //tampilkan waktu di LCD
  //DateTime now = rtc.now();
 // hari = dataHari[now.dayOfTheWeek()];
 // tanggal = now.day(), DEC;
//  bulan = now.month(), DEC;
//  tahun = now.year(), DEC;
 // jam = now.hour(), DEC;
//  menit = now.minute(), DEC;
 // detik = now.second(), DEC;
 // suhu = rtc.getTemperature();

  //  lcd.setCursor(0, 0);
  //  lcd.print(String() + hari + "," + tanggal + "-" + bulan + "-" + tahun);
  //  lcd.setCursor(0, 1);
  //  lcd.print(String() + jam + ":" + menit + ":" + detik + " " + suhu + ".C ");
  //  delay(1000);

  //  Serial.println(String() + hari + ", " + tanggal + "-" + bulan + "-" + tahun);
  //  Serial.println(String() + jam + ":" + menit + ":" + detik);
  //  Serial.println(String() + "Suhu: " + suhu + " C");
  //  Serial.println();

  //tampilkan StatusPakan di serial monitor
  Serial.println("Status Pakan : " + String(StatusPakan));

  //jika StatusPakan = 1 maka beri pakan
  if (StatusPakan == 1) {
    //panggil void BeriPakan, 1 siklus perputaran
    BeriPakan();
    //kembalikan StatusPakan menjadi 0 setelah 1 siklus beri pakan
    Blynk.virtualWrite(V0, 0);
    StatusPakan = 0;
    delay(2000);
  }

  if (StatusPakan == 0) {
    PakanOtomatis();
  }
}

//baca datastream StatusPakan
BLYNK_WRITE(V0) {
  //baca datastream kemudian tampung kedalam variable StatusPakan
  StatusPakan = param.asInt();
}
