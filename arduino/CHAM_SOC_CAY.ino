#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  125 
#define SERVOMAX  575 
int set_pulse[3] = {130,0,90};

#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#define RX_PIN 16  // Chân RX của ESP32 kết nối với TX của DFPlayer Mini
#define TX_PIN 17  // Chân TX của ESP32 kết nối với RX của DFPlayer Mini
HardwareSerial mySerial(2);  // Khởi tạo UART2

DFRobotDFPlayerMini myDFPlayer;

const int trig = 27;     // chân trig của HC-SR04
const int echo = 14;     // chân echo của HC-SR04

#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B 23  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 16   // The number of LEDs (pixels) on WS2812B LED strip

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

#include <DHT.h>

// Define pins
#define ANALOG_PIN 32
#define DIGITAL_PIN 18
#define DHT_PIN 19
#define RELAY_PIN1 26
#define RELAY_PIN2 25
#define RELAY_PIN3 33

// DHT sensor setup
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);


#include <BH1750.h>

BH1750 lightMeter;

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

int mua, khoang_cach, do_am_dat;
float anh_sang, nhiet_do, do_am_kk;
float nguong_ndc, nguong_ndt, nguong_as;
int nguong_do_am_dat;
int tu_dong,tat_tieng,r,g,b,cd,nhactt,dieu_hoa_khi,suoi_khi,may_bom,bon_chua,den,nhacbt;
String command,ai;

const char* ssid = "plant";
const char* password = "12345678";

// Google Sheets URL
String sheetUrl = "https://script.google.com/macros/s/AKfycbwqxSzIz8deUSGsz61ZUkTtxIb1f-n3n3x12ufhUKFuyfcIiHpgXRBLAUM3McpCddatOg/exec";

int a_dieu_hoa, a_suoi, a_bom,a_bon, a_den, a_nhac, a_tu_dong, a_tay, a_tieng;
unsigned long timer;
unsigned long timerr;
void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  if (!myDFPlayer.begin(mySerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  //myDFPlayer.play(1);
  else {
    Serial.println(F("MP3 Ready!"));
  }
  
  myDFPlayer.volume(30);

  Wire.begin();
  lightMeter.begin();

  pwm.begin();
  pwm.setPWMFreq(60); 
  for(int i =0;i<3;i++){
      pwm.setPWM(i, 0, angleToPulse(set_pulse[i]) );
  }
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
  ws2812b.begin(); 

   dht.begin();
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(ANALOG_PIN, INPUT);
   lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("XIN CHAO BAN!");
  lcd.setCursor(2,1);
  lcd.print("MAY CHAM SOC CAY");
   lcd.setCursor(2,2);
  lcd.print("THONG MINH - AI");
   lcd.setCursor(1,3);
  lcd.print("Dang ket noi wifi!");
  myDFPlayer.play(11);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối đến WiFi...");
  }
  myDFPlayer.play(10);
  Serial.println("Đã kết nối WiFi!");
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("XIN CHAO BAN!");
  lcd.setCursor(2,1);
  lcd.print("MAY CHAM SOC CAY");
   lcd.setCursor(2,2);
  lcd.print("THONG MINH - AI");
   lcd.setCursor(2,3);
  lcd.print("Da ket noi wifi!");
  delay(3000);
  
  myDFPlayer.play(12);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("XIN CHAO BAN!");
  lcd.setCursor(2,1);
  lcd.print("MAY CHAM SOC CAY");
   lcd.setCursor(2,2);
  lcd.print("THONG MINH - AI");
   lcd.setCursor(1,3);
  lcd.print("Dang khoi dong...");
  timer=millis();
  timerr=millis();
}
void loop() {
  get_sensor();
  lcd.init(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MODE:");
  lcd.print(tu_dong ? "AUTO ":"MANUAL ");
  lcd.print(" MUTE:");
  lcd.print(tat_tieng ? "Y":"N");
  lcd.setCursor(0,1);
  lcd.print("BON:");
  lcd.print(khoang_cach);
  lcd.print("cm ");
  lcd.print("RAIN:");
  lcd.print(mua ? "N":"Y");
  lcd.print(" ");
  lcd.print("AI:");
  if(ai=="CAY BI BENH") lcd.print("Y");
  else lcd.print("N");
  lcd.setCursor(0,2);
  lcd.print("AS:");
  lcd.print(anh_sang,1);
  lcd.print("lx ");
  lcd.print("T:");
  lcd.print(nhiet_do,1);
  lcd.print("*C");
  lcd.setCursor(0,3);
  lcd.print("H-KK:");
  lcd.print(do_am_kk,1);
  lcd.print("% ");
  lcd.print("H-D:");
  lcd.print(do_am_dat);
  sendToSheet(khoang_cach, mua, anh_sang, nhiet_do, do_am_kk, do_am_dat);
  if(tu_dong==0){
    
    a_tu_dong=0;
    if(a_tay==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CHE DO THU CONG - ON!");
      myDFPlayer.play(21);
      a_tay=1;
      a_dieu_hoa=0, a_suoi=0, a_bom=0,a_bon=0, a_den=0, a_nhac=0, a_tu_dong=0, a_tieng=0;
    }
    if(dieu_hoa_khi==1&&a_dieu_hoa==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIEU HOA - ON!");
      myDFPlayer.play(19);
      a_dieu_hoa=1;
      digitalWrite(25,HIGH);
      pwm.setPWM(1, 0, angleToPulse(90));
      pwm.setPWM(2, 0, angleToPulse(0));
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(dieu_hoa_khi==0&&a_dieu_hoa==1){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIEU HOA - OFF!");
      myDFPlayer.play(9);
      a_dieu_hoa=0;
      digitalWrite(25,LOW);
      pwm.setPWM(1, 0, angleToPulse(0));
      pwm.setPWM(2, 0, angleToPulse(90));
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(suoi_khi==1&&a_suoi==0){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SUOI KHI - ON!");
      myDFPlayer.play(18);
      a_suoi=1;
      digitalWrite(25,HIGH);
      digitalWrite(26,HIGH);
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }

    }
    else if(suoi_khi==0&&a_suoi==1){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SUOI KHI - OFF!");
      myDFPlayer.play(8);
      a_suoi=0;
      digitalWrite(25,LOW);
      digitalWrite(26,LOW);
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(may_bom==1&&a_bom==0){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MAY BOM - ON!");
      if(khoang_cach<7||bon_chua==1){
        myDFPlayer.play(16);
        a_bom=1;
        digitalWrite(33,HIGH);
        if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
        lcd.init();                      // initialize the lcd 
        lcd.backlight();
      }
      else {
         lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HET NUOC!");
        myDFPlayer.play(23);
        a_bom=0;
        if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
      }
    }
    else if(may_bom==0&&a_bom==1){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MAY BOM - OFF!");
      myDFPlayer.play(7);
      a_bom=0;
      digitalWrite(33,LOW);
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
      lcd.init();                      

  lcd.backlight();
    }
    if(bon_chua==1&&a_bon==0){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MO NAP BON!");
      myDFPlayer.play(20);
      a_bon=1;
      pwm.setPWM(0, 0, angleToPulse(0));
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(bon_chua==0&&a_bon==1){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DONG NAP BON!");
      myDFPlayer.play(6);
      a_bon=0;
      pwm.setPWM(0, 0, angleToPulse(130));
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(den==1&&a_den==0){
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEN SINH HOC - ON!");
      myDFPlayer.play(17);
      a_den=1;
      ws2812b.setBrightness(cd);
      for(int i=0; i<NUM_PIXELS; i++) { 
    ws2812b.setPixelColor(i, ws2812b.Color(r, g, b));
    ws2812b.show();
    }
    if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(den==0&&a_den==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEN SINH HOC - OFF!");
      myDFPlayer.play(2);
      a_den=0;
      ws2812b.clear();
      ws2812b.show();
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(nhacbt==1&&a_nhac==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAT NHAC!");
      myDFPlayer.play(4);
      a_nhac=1;
      delay(3000);
      myDFPlayer.play(1);
    }
    else if(nhacbt==0&&a_nhac==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAT NHAC!");
      myDFPlayer.play(3);
      a_nhac=0;
    }
    if(tat_tieng==1&&a_tieng==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAT TIENG!");
      myDFPlayer.play(15);
      delay(3000);
      myDFPlayer.volume(0);
      a_tieng=1;
    }
    else if(tat_tieng==0&&a_tieng==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAT TIENG!");
      myDFPlayer.volume(30);
      myDFPlayer.play(14);
      a_tieng=0;
      if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(ai=="CAY BI BENH"&&millis()-timer>12000){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CAY BI BENH!");
        myDFPlayer.play(24);
        timer=millis();
        if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(khoang_cach>7&&millis()-timerr>10000&&bon_chua!=1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HET NUOC!");
        myDFPlayer.play(23);
        timerr=millis();
          if(nhacbt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
  }
  else {
    a_tay=0;
    if(a_tu_dong==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CHE DO TU DONG - ON!");
      myDFPlayer.play(22);
      a_tu_dong=1;
      a_dieu_hoa=0, a_suoi=0, a_bom=0,a_bon=0, a_den=0, a_nhac=0, a_tay=0, a_tieng=0;
    }
    if(mua==0&&a_bon==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MO NAP BON!");
      myDFPlayer.play(20);
      a_bon=1;
      pwm.setPWM(0, 0, angleToPulse(0));
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(mua==1&&a_bon==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DONG NAP BON!");
      myDFPlayer.play(6);
      a_bon=0;
      pwm.setPWM(0, 0, angleToPulse(130));
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
     if(nhiet_do>nguong_ndc&&a_dieu_hoa==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIEU HOA - ON!");
      myDFPlayer.play(19);
      a_dieu_hoa=1;
      digitalWrite(25,HIGH);
      pwm.setPWM(1, 0, angleToPulse(90));
      pwm.setPWM(2, 0, angleToPulse(0));
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(nhiet_do<nguong_ndc&&a_dieu_hoa==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIEU HOA - OFF!");
      myDFPlayer.play(9);
      a_dieu_hoa=0;
      digitalWrite(25,LOW);
      pwm.setPWM(1, 0, angleToPulse(0));
      pwm.setPWM(2, 0, angleToPulse(90));
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(nhiet_do<nguong_ndt&&a_suoi==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SUOI - ON!");
      myDFPlayer.play(18);
      a_suoi=1;
      digitalWrite(25,HIGH);
      digitalWrite(26,HIGH);
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }

    }
    else if(nhiet_do>nguong_ndt&&a_suoi==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SUOI - OFF!");
      myDFPlayer.play(8);
      a_suoi=0;
      digitalWrite(25,LOW);
      digitalWrite(26,LOW);
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(do_am_dat>nguong_do_am_dat&&a_bom==0){
        lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MAY BOM - ON!");
      if(khoang_cach<7||bon_chua==1){
        myDFPlayer.play(16);
        a_bom=1;
        digitalWrite(33,HIGH);
          if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
        lcd.init();                      // initialize the lcd 
        lcd.backlight();
      }
      else {
        lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HET NUOC!");
        myDFPlayer.play(23);
        a_bom=0;
      }
    }
    else if(do_am_dat<nguong_do_am_dat&&a_bom==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MAY BOM - OFF!");
      myDFPlayer.play(7);
      a_bom=0;
      digitalWrite(33,LOW);
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
      lcd.init();                      

  lcd.backlight();
    }
    if(anh_sang<nguong_as&&a_den==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEN SINH HOC - ON!");
      myDFPlayer.play(17);
      a_den=1;
      ws2812b.setBrightness(cd);
      for(int i=0; i<NUM_PIXELS; i++) { 
    ws2812b.setPixelColor(i, ws2812b.Color(r, g, b));
    ws2812b.show();
    }
      if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    else if(anh_sang>nguong_as&&a_den==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEN SINH HOC - OFF!");
      myDFPlayer.play(2);
      a_den=0;
      ws2812b.clear();
      ws2812b.show();
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(nhactt==1&&a_nhac==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAT NHAC!");
      myDFPlayer.play(4);
      a_nhac=1;
      delay(3000);
      myDFPlayer.play(1);
    }
    else if(nhactt==0&&a_nhac==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAT NHAC!");
      myDFPlayer.play(3);
      a_nhac=0;
    }
    if(tat_tieng==1&&a_tieng==0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAT TIENG!");
      myDFPlayer.play(15);
      delay(3000);
      myDFPlayer.volume(0);
      a_tieng=1;
    }
    else if(tat_tieng==0&&a_tieng==1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAT TIENG!");
      myDFPlayer.volume(30);
      myDFPlayer.play(14);
      a_tieng=0;
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(ai=="CAY BI BENH"&&millis()-timer>12000){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CAY BI BENH!");

        myDFPlayer.play(24);
        timer=millis();
          if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
    if(khoang_cach>7&&millis()-timerr>10000&&bon_chua!=1){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HET NUOC!");
        myDFPlayer.play(23);
        timerr=millis();
        if(nhactt==1){
          delay(3000);
          myDFPlayer.play(1);
    }
    }
  }
}
int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

int distance(){
  unsigned long duration; // biến đo thời gian
    int distances;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distances = int(duration/2/29.412);
    Serial.print(distances);
    Serial.println("cm");
    return distances;
}
void get_sensor(){
  do_am_dat = analogRead(ANALOG_PIN);
  Serial.print("Độ ẩm đất: ");
  Serial.println(do_am_dat);

  // Read digital value from pin 18
  mua = digitalRead(DIGITAL_PIN);
  Serial.print("mưa: ");
  Serial.println(mua);
  
  // Read temperature and humidity from DHT11
  do_am_kk = dht.readHumidity();
  nhiet_do = dht.readTemperature();
  khoang_cach = distance();
  anh_sang = lightMeter.readLightLevel();
  if (isnan(do_am_kk) || isnan(nhiet_do)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(nhiet_do);
    Serial.print(" °C");
    Serial.print(" Humidity: ");
    Serial.print(do_am_kk);
    Serial.println(" %");
  }
  
}
void sendToSheet(int mnuoc, int muaa, float as, float nt, int dakk, int dadat) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // Thiết lập URL để gửi dữ liệu
    String url = sheetUrl + "?muc_nuoc=" + String(mnuoc) + 
                 "&mua=" + String(muaa) + 
                 "&anh_sang=" + String(as) + 
                 "&nhiet_do=" + String(nt) + 
                 "&do_am_kk=" + String(dakk) + 
                 "&do_am_dat=" + String(dadat);

    Serial.println(url);
    http.begin(url);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS); 
    int httpResponseCode = http.GET();

    // Kiểm tra kết quả
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Data received: ");
      Serial.println(payload);

      // Chuyển dữ liệu JSON nhận được thành các giá trị
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (error) {
        Serial.println("Failed to parse JSON");
        return;
      }

      // Trích xuất giá trị từ JSON
      command = doc["command"].as<String>();
      ai = doc["ai"].as<String>();

      // In ra các giá trị
      Serial.print("command: ");
      Serial.println(command);
      Serial.print("ai: ");
      Serial.println(ai);
      splitString(command);
    } else {
      Serial.println("Error getting data from Google Sheets, HTTP Code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("Mất kết nối WiFi!");
  }
}

void splitString(String input) {
  // Tạo mảng để lưu các phần tử sau khi tách
  String parts[17];  // Dựa trên số lượng phần tử trong chuỗi
  int index = 0;

  // Dùng hàm split() để tách chuỗi
  int start = 0;
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) == ':') {
      parts[index] = input.substring(start, i);
      index++;
      start = i + 1;
    }
  }
  parts[index] = input.substring(start); // Thêm phần tử cuối cùng
  // Lấy từng phần tử và in ra (hoặc sử dụng cho các biến khác)
  tu_dong = parts[0].toInt();
  tat_tieng = parts[1].toInt();
  r = parts[2].toInt();
  g = parts[3].toInt();
  b = parts[4].toInt();
  cd = parts[5].toInt();
  nguong_ndc = parts[6].toFloat();
  nguong_ndt = parts[7].toFloat();
  nguong_do_am_dat = parts[8].toInt();
  nguong_as= parts[9].toInt();
  nhactt = parts[10].toInt();
  dieu_hoa_khi = parts[11].toInt();
  suoi_khi = parts[12].toInt();
  may_bom = parts[13].toInt();
  bon_chua = parts[14].toInt();
  den = parts[15].toInt();
  nhacbt = parts[16].toInt();

  // In ra các giá trị (hoặc xử lý chúng theo nhu cầu)
  Serial.print("tu_dong: "); Serial.println(tu_dong);
  Serial.print("tat_tieng: "); Serial.println(tat_tieng);
  Serial.print("r: "); Serial.println(r);
  Serial.print("g: "); Serial.println(g);
  Serial.print("b: "); Serial.println(b);
  Serial.print("cd: "); Serial.println(cd);
  Serial.print("nguong_ndc: "); Serial.println(nguong_ndc);
  Serial.print("nguong_ndt: "); Serial.println(nguong_ndt);
  Serial.print("nguong_do_am_dat: "); Serial.println(nguong_do_am_dat);
  Serial.print("nguong_as: "); Serial.println(nguong_as);
  Serial.print("nhactt: "); Serial.println(nhactt);
  Serial.print("dieu_hoa_khi: "); Serial.println(dieu_hoa_khi);
  Serial.print("suoi_khi: "); Serial.println(suoi_khi);
  Serial.print("may_bom: "); Serial.println(may_bom);
  Serial.print("bon_chua: "); Serial.println(bon_chua);
  Serial.print("den: "); Serial.println(den);
  Serial.print("nhacbt: "); Serial.println(nhacbt);
}

