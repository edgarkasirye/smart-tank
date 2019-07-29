#include <NewPing.h>
#include <DS3231.h>
#include <SoftwareSerial.h>

int trig=9,j;
int echo=8;
float dist,distance,v,h,t,rate,cp;
float rates[1000];
const float ln=20.0,r=10.0;
String command,instruct;
 
//unsigned long  t=getRTCTime();

NewPing orla(trig, echo, 400);
DS3231 rtc(A4,A5);
SoftwareSerial gsm(11, 12); // RX, TX


void setup() {
   h=(ln-distance);
  rate=((distance-dist)/t);
  
  Serial.begin(9600);
  Serial.println("Taking inital mesurements...");

  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
   gsm.begin(9600);
   modem_init();
  data_init();
  internet_init();
 
  rtc.begin();
  rtc.setDOW(THURSDAY);
  rtc.setTime(0,0,0);
  rtc.setDate(7,12,2000);
  Serial.println("input critical point of choice:");

     while (Serial.available() == 0){ }
     float in = Serial.parseFloat();
      cp=in;
 
  
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
   dist= orla.ping_cm();
   
  Serial.print("Initial space length:");
  Serial.println(dist);
  Serial.print("Initial Water Height:");
  Serial.println(ln-dist);
  Serial.print("Initial Volume:");
  Serial.println(3.14*r*r*(ln-dist));
   if(h<=cp){
    SMS();
    }
   else {Serial.println("The tank is full");}
  Serial.println("Inital measurements done");
  Serial.println();
  Serial.println();
  delay(5000);
  
}

void loop() {
digitalWrite(trig,LOW);
delayMicroseconds(2);
digitalWrite(trig,HIGH);
delayMicroseconds(10);
  distance=orla.ping_cm();
   if(distance<dist){distance=dist;}
   h=(ln-distance);
   v=(3.14*r*r*h);
  Serial.print("Space Length:");
  Serial.println(distance);
  Serial.print("Water Height:");
  Serial.println(h);
  Serial.print("Volume:");
  Serial.println(v);
  
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  Serial.print(rtc.getDateStr());
  Serial.print(" at ");

  Serial.print(rtc.getTimeStr());
  Serial.println();
  Serial.print("InputTime: ");
    while(Serial.available() == 0){ }
      float tt= Serial.parseFloat();
        delay(2000); t=tt;
      Serial.println(t);
     Serial.print("Rate of dispense: ");
     Serial.print(rate);
      rates[j]=rate;
     Serial.println(" metres per second");
     if(h<=cp){
    SMS();
    }
   else {Serial.println("Water is available");}

 Serial.println("Send data to the cloud manually...");
 Serial.println();
  delay(100);
  for(int i=0;i<12;i++){function();}
 
      j++; 
      send_data();
}

void function(){
  
while(Serial.available() == 0){}
  command = Serial.readString();
  gsm.println(command);

  while(gsm.available() == 0){}
   instruct = gsm.readString();
   Serial.println(instruct);
 
  }
  
void SMS(){
    
      Serial.print("\r");
      delay(100);
      Serial.print("AT+CMGF=1\r");
      delay(100);
      Serial.print("AT+CMGS=\"0784799280\"\r");
      delay(100);
      Serial.print("YOUR WATER IS AT CRITICAL LEVEL");
      delay(100);
    
    }
  void modem_init(){
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}

void data_init(){
  Serial.println("Please wait.....");
  gsm.println("AT");
  delay(1000); delay(1000);
  gsm.println("AT+CPIN?");
  delay(1000); delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("contype");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("GPRS");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000); ;
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("APN");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  //------------APN------------//
  gsm.print("MTN-UGANDA"); //APN Here
  //--------------------------//
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("USER");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.write(',');
  gsm.write('"');
  gsm.print("PWD");
  gsm.write('"');
  gsm.write(',');
  gsm.write('"');
  gsm.print("  ");
  gsm.write('"');
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(2000);
  gsm.print("AT+SAPBR=1,1");
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(3000);
}

void internet_init(){
  Serial.println("Please wait.....");
  delay(1000);
  gsm.println("AT+HTTPINIT");
  delay(1000); delay(1000);
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("CID");
  gsm.print('"');
  gsm.print(',');
  gsm.println('1');
  delay(1000);
}

void send_data(){
  gsm.print("AT+HTTPPARA=");
  gsm.print('"');
  gsm.print("URL");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("http:");
  gsm.print('/');
  gsm.print('/');
  //-----------------------Your API Key Here----------------------//
  //Replace xxxxxxxxxxx with your write API key.
  gsm.print("api.thingspeak.com/update?api_key=WCUL6ADB9MCGM6GM&field1="); 
  //---------------------------------------------------------------//
  gsm.print(ln); //>>>>>>  variable 1 (temperature)
  //gsm.print("&field2=");
  //gsm.print(humi); //>>>>>> variable 2 (Humidity)
  gsm.write(0x0d);
  gsm.write(0x0a);
  delay(1000);
  gsm.println("AT+HTTPACTION=0");
  delay(1000);
}
