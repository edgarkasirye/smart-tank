#include <NewPing.h>
#include <DS3231.h>
#include <SoftwareSerial.h>

int j,a=1;
float dist,sl,v,h,cp,rates[50];
const float ln=20.0,r=10.0;

NewPing orla(9,8,400); // trig, echo
DS3231 rtc(A4,A5);
SoftwareSerial gsm(11, 12); // RX, TX

void setup() {
   //float dist;
    pinMode(9,OUTPUT);  //trig
    pinMode(8,INPUT);   //echo
  
  Serial.begin(9600);
  Serial.println("Taking inital mesurements...");
  rtc.begin();
  rtc.setDOW(THURSDAY);
  rtc.setTime(0,0,0);
  rtc.setDate(7,12,2000);
  
   gsm.begin(9600);
   modem_init();
   data_init();
   internet_init();
 
  Serial.println("input critical point of choice:");
    while (Serial.available() == 0){ }
     float in = Serial.parseFloat();
      cp=in;
 
    digitalWrite(9,LOW);
    delayMicroseconds(2);
    digitalWrite(9,HIGH);
    delayMicroseconds(10);
   dist= orla.ping_cm();
   
  Serial.print("Initial space length:");
  Serial.println(dist);
  Serial.print("Initial Water Height:");
  Serial.println(ln-dist);
  Serial.print("Initial Volume:");
  Serial.println(3.14*r*r*(ln-dist));
   if((ln-dist)<=cp){
    SMS();
    }
   else {Serial.println("The tank is full");}
  Serial.println("Inital measurements done");
  Serial.println();
  Serial.println();
  delay(5000);
  
}

void loop() {
   call();
digitalWrite(9,LOW);
delayMicroseconds(2);
digitalWrite(9,HIGH);
delayMicroseconds(10);
  sl=orla.ping_cm();
   if(sl<dist){sl=dist;}
   h=(ln-sl);
   v=(3.14*r*r*h);
  Serial.print("Space length:");
  Serial.println(sl);
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
    delay(5000);
     float t= Serial.parseFloat();
         
      Serial.println(t);
     Serial.print("Rate of dispense: ");
     Serial.print((sl-dist)/t);
      float rate;
      rates[j]=rate=((sl-dist)/t);
     Serial.println(" cm per second");
     if((sl-dist)<=cp){
    SMS();
    }
   else {Serial.println("Water is available");}
    send_data();
 //Serial.println("Send data to the cloud manually...");
 Serial.println();
 // delay(100);
  //for(int i=0;i<12;i++){function();}
       j++; 
     delay(10000);
}

/*void function(){
  String command,instruct;
while(Serial.available() == 0){}
  command = Serial.readString();
  gsm.println(command);

  while(gsm.available() == 0){}
   instruct = gsm.readString();
   Serial.println(instruct);
 
  }*/
  
void SMS(){
    
      gsm.print("\r");
      delay(100);
      gsm.println("AT+CMGF=1\r");
      delay(100);
      gsm.println("AT+CMGS=\"0784799280\"\r");
      delay(100);
      gsm.println("YOUR WATER IS AT CRITICAL LEVEL");
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
  gsm.print(',');
  gsm.print('"');
  gsm.print("Contype");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("GPRS");
  gsm.print('"');
  gsm.print(0x0d);
  gsm.print(0x0a);
  delay(1000); ;
  gsm.print("AT+SAPBR=3,1");
  gsm.print(',');
  gsm.print('"');
  gsm.print("APN");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  //------------APN------------//
  gsm.print("mtn.co.ug"); //APN Here
  //--------------------------//
  gsm.print('"');
  gsm.print(0x0d);
  gsm.print(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.print(',');
  gsm.print('"');
  gsm.print("USER");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("  ");
  gsm.print('"');
  gsm.print(0x0d);
  gsm.print(0x0a);
  delay(1000);
  gsm.print("AT+SAPBR=3,1");
  gsm.print(',');
  gsm.print('"');
  gsm.print("PWD");
  gsm.print('"');
  gsm.print(',');
  gsm.print('"');
  gsm.print("  ");
  gsm.print('"');
  gsm.print(0x0d);
  gsm.print(0x0a);
  delay(2000);
  gsm.print("AT+SAPBR=1,1");
  gsm.print(0x0d);
  gsm.print(0x0a);
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
  gsm.print(h); //>>>>>>  variable 1 
  gsm.print("&field2=");
  gsm.print(rates[j]); //>>>>>> variable 2 
  gsm.print(0x0d);
  gsm.print(0x0a);
  delay(1000);
  gsm.println("AT+HTTPACTION=0");
  delay(1000);
}
 void call(){
  //int ;
  if (a==1){
    Serial.print(a);
    Serial.println("st Iteration");}
  else if (a==2){
    Serial.print(a);
    Serial.println("nd Iteration");}
  else if (a==3){
    Serial.print(a);
    Serial.println("rd Iteration");}
  else {
    Serial.print(a);
    Serial.println("th Iteration");}
    a++;
  }
