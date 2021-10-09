#include<WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// Wiring: SDA pin is connected to D21 and SCL pin to D22(ESP32).
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

int dry= 3100;
int wet=600-70;
int mot=35;
int msensor=34;
//int Led=12;
const int sendInterval = 5000;
//Wifi properties
const char* WIFI_NAME= "amarsource";
const char* WIFI_PASSWORD = "amarsourcewifi";
String GOOGLE_SCRIPT_ID= "AKfycbx_HgffEGNWjMG3ZYS7OXMRGbmDztbyKA8uVi5L2YhfAKw62NSVqu0WYP6DvxnhSb7s";

const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";//making the server object



WiFiClientSecure client;
void setup() {
  //sensor motor code
  pinMode(msensor,INPUT);
  //pinMode(Led,OUTPUT);
  pinMode(mot,OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(WIFI_NAME);  //connecting to wifi
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);  while (WiFi.status() != WL_CONNECTED) {
  delay(200);
  Serial.print("Trying to connect to Wifi Network");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println(WiFi.localIP());
  
  //LCD CODE
  Wire.begin();
  lcd.init();// initialize the lcd 
  lcd.backlight();
  }
  
  void loop() {
  // put your main code here, to run repeatedly:  
  int datas=analogRead(msensor);  
  Serial.println(datas);
  int percentageMoisture=map(datas,wet, dry,100, 0);  
  Serial.println(percentageMoisture);
  if(percentageMoisture<40)
  {
    digitalWrite(mot,HIGH);
    delay(1000);
    digitalWrite(mot,LOW);  }
  String percentageMoisture_s(percentageMoisture);
  sendData("tag=pin34&value="+percentageMoisture_s);
  delay(sendInterval);
  lcd.setCursor(0,0);
  lcd.print("Moisture:");
  lcd.setCursor(11,0);
  lcd.print(percentageMoisture_s+"%");
  //delay(300000);
  delay(5000);
}
void sendData(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   Serial.println(url);
    Serial.println("Making a request");
    http.begin(url); //Specify the URL and certificate
    int httpCode = http.GET();
    Serial.println("sending");
    http.end();
    Serial.println(": done "+httpCode);
}
