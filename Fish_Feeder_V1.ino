#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
//#include <Wire.h>

//ambil template_id, device_id, dan token blynk
#define BLYNK_TEMPLATE_ID "TMPL6kcHhtIl3"
#define BLYNK_TEMPLATE_NAME "Pakan IKAN"
#define BLYNK_AUTH_TOKEN "T38UGkwRJWmGcuJ86esddwSfxeArJ5V4"

//deklarasi pin ultrasonik
#define TRIGGERPIN 5
#define ECHOPIN 4
//include library blynk
#include <BlynkSimpleEsp32.h>
//lcd
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Alamat I2C dan ukuran LCD 
//buat sebuah objek untuk servo
Servo myservo;

//variabel menampung datastream status pakan
int StatusPakan;
//variabel menampung datastream banyak pakan
int level;
int habis;
void setup() {
  Serial.begin(115200);
  //pin digunakan untuk servo = D2 
  myservo.attach(2);
  //posisi servo awal
  myservo.write(0);
  
  lcd.begin();   // Inisialisasi LCD
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Mangan Iwak");

  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  //koneksi ke Wifi
  WiFi.begin("Kelompok 1", "12345678");

  //uji koneksi wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi Terkoneksi");
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("WiFi Nyambung");

  //jika konek wifi, koneksikan ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, "Kelompok 1", "12345678");
  Serial.println("Blynk Terkoneksi");
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Blynk Nyambung");
  delay(2000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Ayo Pakan!");
}
//fungsi banyakpakan
void BanyakPakan(){
  long duration, jarak, fix;
  digitalWrite(TRIGGERPIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn (ECHOPIN, HIGH);
  //jarak = (duration / 2) / 12.2;
  jarak = (duration * 0.034) / 2;
  fix = 14 - jarak;
  level = (fix * 100) / 14;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sisa Pakan: ");
  lcd.print(level);
  lcd.print("%");
    
  Serial.print(level);
  Serial.println("%");
  delay(1000);
}

void BeriPakan() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ikan Lagi Makan");
  //proses perputaran servo secara halus
  for (int posisi = 0; posisi <= 90; posisi++) {
    //set posisi servo berdasarkan variable posisi
    myservo.write(posisi);
    delay(5);
  }
  //proses perputaran servo kembali posisi awal
  for (int posisi = 90; posisi >= 0; posisi--) {
    myservo.write(posisi);
    delay(5);
  }
}
void Peringatan(){
  if (level <= 14){
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Pakan");
    lcd.setCursor(2, 1);
    lcd.print("Habis");
    Blynk.virtualWrite(V2, 1);
  }
  if (level >= 15){
    Blynk.virtualWrite(V2, 0);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Mangan");
    lcd.setCursor(3, 1);
    lcd.print("Iwak");
  }
}
//void Print(){
//  lcd.clear();
 // lcd.setCursor(3, 0);
 // lcd.print("Mangan");
 // lcd.setCursor(3, 1);
 // lcd.print("Iwak");
//}
void loop() {
  //jalankan aplikasi Blynk dari ESP32
  Blynk.run();

  //tampilkan StatusPakan di serial monitor
  //Serial.println("Status Pakan : " + String(StatusPakan));

  //jika StatusPakan = 1 maka beri pakan
  if (StatusPakan == 1) {
    //panggil void BeriPakan, 1 siklus perputaran
    BeriPakan();
    //kembalikan StatusPakan menjadi 0 setelah 1 siklus beri pakan
    Blynk.virtualWrite(V0, 0);
    StatusPakan = 0;
    //TAMPILKAN BANYAK PAKAN DI SERIAL,LCD, DAN BLYNK GAUGE
    BanyakPakan();
    Blynk.virtualWrite(V1, level);
    delay(5000);
    Peringatan();
    
  }
  
  //Print();
  //Print();
}

//baca datastream StatusPakan
BLYNK_WRITE(V0) {
  //baca datastream kemudian tampung kedalam variable StatusPakan
  StatusPakan = param.asInt();
}
BLYNK_WRITE(V1) {
  level = param.asInt();
}
BLYNK_WRITE(V2){
  habis = param.asInt();
}
