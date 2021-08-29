#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "DHT.h"

#define SS_PIN 5
#define RST_PIN 22
#define servo1 2
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN) ;
#define DHTTYPE DHT11 
const char* ssid     = "winwinklao";
const char* password = "Klaowiner1942.";
 
WiFiServer server(80);
Servo myservo;
Servo servo;




const int DHTPin = 15;
DHT dht(DHTPin, DHTTYPE);

static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

 
void setup ()
{
        Serial.begin(115200);
 
        SPI.begin();
        mfrc522.PCD_Init();
        Serial.println("Put your card to the reader...");
        Serial.println();
        servo.attach(servo1);
        servo.write(110);   
              
              myservo.attach(21);
              pinMode(12, OUTPUT);      // set the LED pin mode
              delay(10);
              dht.begin();
            
             
              // We start by connecting to a WiFi network
             
              Serial.println();
              Serial.println();
              Serial.print("Connecting to ");
              Serial.println(ssid);
             
              WiFi.begin(ssid, password);
             
              while (WiFi.status() != WL_CONNECTED) 
              {
                      delay(500);
                      Serial.print(".");
              }
             
              Serial.println("");
              Serial.println("WiFi connected.");
              Serial.println("IP address: ");
              Serial.println(WiFi.localIP());
             
              server.begin();
              
 
}
 
int value = 0;
void loop() 
{
            WiFiClient client = server.available();   // listen for incoming clients

           
            if (client) 
            {                             // if you get a client,
                      Serial.println("New Client.");           // print a message out the serial port
                      String currentLine = "";                                                                  //led variable
                      boolean blank_line = true;               //temp variable
                      String curtain = "" ;
              
                      while (client.connected()) // loop while the client's connected
                      {            
                              if (client.available()) 
                              {             // if there's bytes to read from the client,
                                      char c = client.read();             // read a byte, then
                                      
                                    
                                              if (c == '\n' && blank_line) // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)         //start temp
                                              {
                                                    float h = dht.readHumidity();  // Read temperature as Celsius (the default)
                                                    
                                                    float t = dht.readTemperature();   // Read temperature as Fahrenheit (isFahrenheit = true)
                                                   
                                                    float f = dht.readTemperature(true);   // Check if any reads failed and exit early (to try again).
                                                   
                                                    if (isnan(h) || isnan(t) || isnan(f)) 
                                                    {
                                                          Serial.println("Failed to read from DHT sensor!");
                                                          strcpy(celsiusTemp, "Failed");
                                                          strcpy(fahrenheitTemp, "Failed");
                                                          strcpy(humidityTemp, "Failed");
                                                    }
                                                    else 
                                                    {
                                                          // Computes temperature values in Celsius + Fahrenheit and Humidity
                                                          float hic = dht.computeHeatIndex(t, h, false);
                                                          dtostrf(hic, 6, 2, celsiusTemp);
                                                          float hif = dht.computeHeatIndex(f, h);
                                                          dtostrf(hif, 6, 2, fahrenheitTemp);
                                                          dtostrf(h, 6, 2, humidityTemp);
                                                          // You can delete the following Serial.print's, it's just for debugging purposes
                                                          Serial.print("Humidity: ");
                                                          Serial.print(h);
                                                          Serial.print(" %\t Temperature: ");
                                                          Serial.print(t);
                                                          Serial.print(" *C ");
                                                          Serial.print(f);
                                                          Serial.print(" *F\t Heat index: ");
                                                          Serial.print(hic);
                                                          Serial.print(" *C ");
                                                          Serial.print(hif);
                                                          Serial.print(" *F");
                                                          Serial.print("Humidity: ");
                                                          Serial.print(h);
                                                          Serial.print(" %\t Temperature: ");
                                                          Serial.print(t);
                                                          Serial.print(" *C ");
                                                          Serial.print(f);
                                                          Serial.print(" *F\t Heat index: ");
                                                          Serial.print(hic);
                                                          Serial.print(" *C ");
                                                          Serial.print(hif);
                                                          Serial.println(" *F");
                                                    }
                                                    client.println("HTTP/1.1 200 OK");
                                                    client.println("Content-Type: text/html");
                                                    client.println("Connection: close");
                                                    client.println();
                                                    // your actual web page that displays temperature and humidity
                                                    client.println("<!DOCTYPE HTML>");
                                                    client.println("<html>");
                                                    client.println("<head></head><body><font size =3><h1>Temperature in Celsius:    ");
                                                    client.println(celsiusTemp);
                                                    client.println("*C</h3><h3><head></head><body><h1>Temperature in Fahrenheit: ");
                                                    client.println(fahrenheitTemp);
                                                    client.println("*F</h3><h3><head></head><body><h1>Humidity: ");
                                                    client.println(humidityTemp);
                                                    client.println("%</h3><h3>");
                                                    client.println("</font></body></html>");
                                                    break;
                                            }
                                            if (c == '\n') // when starts reading a new line
                                            {
                                               blank_line = true;
                                            }
                                            else if (c != '\r')      // when finds a character on the current line

                                            {
                                                blank_line = false;
                                            }                       // if the byte is a newline character                                         // end temp
                                    
                                      
                                      
                                      Serial.write(c);                    // print it out the serial monitor                                      // start led
                                      if (c == '\n') 
                                      {  
                                              // if the current line is blank, you got two newline characters in a row.
                                              // that's the end of the client HTTP request, so send a response:
                                              if (currentLine.length() == 0)                                                                                           
                                              {
                                                      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                                                      // and a content-type so the client knows what's coming, then a blank line:
                                                      client.println("HTTP/1.1 200 OK");
                                                      client.println("Content-type:text/html");
                                                      client.println();
                                           
                                                      // the content of the HTTP response follows the header:
                                                      client.print("Click <a href=\"/12H\">on</a> to turn the LED on pin 12 on.<br>");
                                                      client.print("Click <a href=\"/12L\">off</a> to turn the LED on pin 12 off.<br>");
                                                      client.print("<br>");
                                           
                                                      // The HTTP response ends with another blank line:
                                                      client.println();
                                                      // break out of the while loop:
                                                      break;
                                              } 
                                              else  // if you got a newline, then clear currentLine:
                                              {    
                                                    currentLine = "";
                                              }
                                      } 
                                      
                                      else if (c != '\r')  // if you got anything else but a carriage return character,
                                      {  
                                          currentLine += c;      // add it to the end of the currentLine
                                          
                                      }
                               
                                      // Check to see if the client request was "GET /xH" or "GET /xL":
                                      if (currentLine.endsWith("GET /12H")) 
                                      {
                                            digitalWrite(12, HIGH);               // GET /H turns the LED on
                                      }
                                      if (currentLine.endsWith("GET /12L")) 
                                      {
                                            digitalWrite(12, LOW);                // GET /L turns the LED off
                                      }                                                                                                             // end led

                                      

                                      if (c == '\n') 
                                      {  
                                              // if the current line is blank, you got two newline characters in a row.
                                              // that's the end of the client HTTP request, so send a response:
                                              if (curtain.length() == 0)                                                                                           
                                              {
                                                      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                                                      // and a content-type so the client knows what's coming, then a blank line:
                                                      client.println("HTTP/1.1 200 OK");
                                                      client.println("Content-type:text/html");
                                                      client.println();
                                           
                                                      // the content of the HTTP response follows the header:
                                                      client.print("Click <a href=\"/start\">Start</a> to turn the Servo on pin 19 on.<br>");
                                                      client.print("Click <a href=\"/stop\">Stop</a> to turn the Servo on pin 19 off.<br>");
                                                      client.print("<br>");
                                           
                                                      // The HTTP response ends with another blank line:
                                                      client.println();
                                                      // break out of the while loop:
                                                      break;
                                              } 
                                              else  // if you got a newline, then clear currentLine:
                                              {    
                                                    curtain = "";
                                              }
                                      } 
                                      
                                      else if (c != '\r')  // if you got anything else but a carriage return character,
                                      {  
                                          curtain += c;      // add it to the end of the currentLine
                                      }
                               
                                      // Check to see if the client request was "GET /xH" or "GET /xL":
                                      if (curtain.endsWith("GET /start")) 
                                      {
                                            myservo.writeMicroseconds(0); // สั่งให้ Servo หมุนวนขวา
                                            delay(1000);               // GET /H turns the LED on
                                           
                                      }
                                      if (curtain.endsWith("GET /stop")) 
                                      {
                                            myservo.writeMicroseconds(1450); // สั่งให้ Servo หมุนวนขวาซ้าย
                                            delay(1000); // หน่วงเวลา 2000ms    
                                                 
                                      }                                                                                                             // end led
                              
                              }
                             
                              }
                      
                      // close the connection:
                      delay(1);
                      client.stop();
                      Serial.println("Client Disconnected.");
            }
            if(! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if(! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i< mfrc522.uid.size; i++)
  {
    Serial.println(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " ");
    Serial.println(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); 
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "71 D7 EA 2E","F6 73 4B 73")
  {
    Serial.println("Authorial access");
    Serial.println();
    delay(500);
    servo.write(0);
    delay(5000);
    servo.write(110); 
  }

  else {
    Serial.println(" Access denied");
    servo.write(110);
  }


  
}
