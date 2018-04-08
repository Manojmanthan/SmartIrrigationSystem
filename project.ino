#include <SoftwareSerial.h>

int sensor;
int map_data;
int pump_status;

#define SSID ""
#define PASSWD ""

#define READ_KEY "F696HD0RG3ZUXFN2"
#define WRITE_KEY "LA0C83IWTZGKC0U5"

SoftwareSerial esp(0,1);

void updateTS(String T,String P = "0");

void sendAT(String cmd){
  Serial.println("Sending AT commands:");
  esp.println(cmd);
  Serial.println(cmd);
  }

boolean connectWifi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  Serial.println("AT+CWLAP");
  String cmd = "AT+JAP=\""SSID"\",\""PASSWD"\"";
  sendAT(cmd);
  delay(5000);

  if(Serial.find("Error")){
    Serial.println("Error: The connection can't be established");
    return false;
  }else{
    Serial.println("Connection established successfully!!!");
    }

    cmd = "AT+CWMUX=0";
    sendAT(cmd);
    if(Serial.find("Error")){
          esp.print("Received:Error");
      }
  }

void setup(){
  Serial.begin(115200);
  esp.begin(115200);
  sendAT("AT");
  delay(5000);
  if(Serial.find("OK")){
    Serial.println("Connection to arduino and esp8266 successfully established");
    connectWifi();
    }
  }

void loop(){
  
  sensor = analogRead(A0);
  map_data = map(sensor,0,1023,100,0);
  String data = String(map_data);
  Serial.println("SOIL MOISTURE :");
  Serial.println(map_data);

  if(map_data<76){
    digitalWrite(8,HIGH);
    //digitalWrite(7,LOW);
    pump_status = 100;
    }
  else{
    digitalWrite(8,LOW);
    //digitalWrite(7,HIGH);
    pump_status = 0;
    }

  String pump = String(pump_status);
  updateTS(data,pump);
  delay(3000);
  }

void updateTS(String T,String P){
  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";
  sendAT(cmd);
  delay(3000);
  if(Serial.find("Error")){
    esp.print("Received: Error \n EXIT");
    return;
  }
  cmd = "GET /update?key=LA0C83IWTZGKC0U5&field1="+T+"&field2="+P+"\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
    esp.print(">");
    esp.print(cmd);
    Serial.print(cmd);
    

  if(Serial.find("Error")){
    esp.print("Received: Error \n EXIT2");
    return;
  }

  Serial.println("End of loop");
  
  }
