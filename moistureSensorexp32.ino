#include<WiFi.h>
int dry= 3400;
int wet=600-70;
int mot=27;

int msensor=32;
int Led=12;

//Wifi properties
//const char* WIFI_NAME= "ENTER WIFI NAME";
//const char* WIFI_PASSWORD = "ENTER WIFI PASSWORD";


//making the server object
WiFiServer server(80);

//local machine(where the python is situated) properties
const uint16_t port = 80;
const char * host = "ENTER IP ADDRESS";

WiFiClient client;
void setup() {
  pinMode(msensor,INPUT);
  pinMode(Led,OUTPUT);
  pinMode(mot,OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);

  //connecting to wifi
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(200);
  Serial.print("Trying to connect to Wifi Network");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println(WiFi.localIP());
  server.begin();

  
 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int datas=analogRead(msensor);
  
  int percentageMoisture=map(datas,wet, dry,100, 0);
 
 
  Serial.println(percentageMoisture);
  if(percentageMoisture<40)
  {
    digitalWrite(mot,HIGH);
    delay(30000);
    digitalWrite(mot,LOW);
    
  }

  if(client.connect(host,port))
  {
    client.print(percentageMoisture);
  }
  
  delay(10000);
}
