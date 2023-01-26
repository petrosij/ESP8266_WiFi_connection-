  /*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <String.h>

#define ENDOFLINE '\n'
#define STARTOFREQUEST "GET /"

#define PINBIKOAG 4
#define PINMONOKOAG 5
#define PINMONOCUT 6

#define SWITCHBIKOAG "switchBikoag="
#define SWITCHMONOKOAG "switchMonokoag="
#define SWITCHMONOCUT "switchMonocut="

#define SWITCHBIKOAGLEN 13
#define SWITCHMONOKOAGLEN 15
#define SWITCHMONOCUTLEN 14

String str = "";
String str2 = "";
long bikoagState = 0;
long monokoagState = 0;
long monocutState = 0;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x, 0x, 0x, 0x, 0x, 0x
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {

  pinMode(PINBIKOAG,OUTPUT);
  pinMode(PINMONOKOAG,OUTPUT);
  pinMode(PINMONOCUT,OUTPUT);
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  digitalWrite(PINBIKOAG,!bikoagState);
  digitalWrite(PINMONOKOAG,!monokoagState);
  digitalWrite(PINMONOCUT,!monocutState);
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        str += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
  
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
         // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println(" <head>");
          client.println("  <title>Тег FORM</title>");
          client.println(" </head>");
          client.println(" <body>");
          
          client.println(" <h1> <form action=\"\">");
          client.println("  bikoagulation <button type=\"submit\" name=\"switchBikoag\" value=\"1\"> <h2> Switch ON </h2> </button>     ");
          client.println("  <button type=\"submit\" name=\"switchBikoag\" value=\"0\"> <h2> Switch OFF </h2> </button>     ");
          client.println(" </form>");
          client.println(" <form action=\"\">");
          client.println("  monokoagulation <button type=\"submit\" name=\"switchMonokoag\" value=\"1\"> <h2> Switch ON </h2> </button>     ");
          client.println("  <button type=\"submit\" name=\"switchMonokoag\" value=\"0\"> <h2> Switch OFF </h2> </button>     ");
          client.println(" </form>");
          client.println(" <form action=\"\">");
          client.println("  mono cutting <button type=\"submit\" name=\"switchMonocut\" value=\"1\"> <h2> Switch ON </h2> </button>     ");
          client.println("  <button type=\"submit\" name=\"switchMonocut\" value=\"0\"> <h2> Switch OFF </h2> </button>     ");
          client.println(" </form> </h1>");

          client.println(" </body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
 
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    //------------------------------------
    

 int endPosition = 0;
  
  // put your main code here, to run repeatedly:
  for (int i = 0;i<100;i++){  // check for 1000???   <-----------------------------
    if (str[i] == ENDOFLINE){
      str2 = str.substring(0,i);
      endPosition = i;
      Serial.print("endPosition is ");
      Serial.println(endPosition);
      break; 
    }
  }
   // Bikoagulation case
  if (str2.startsWith(STARTOFREQUEST)){
    Serial.print("GET OK\n");
    for (int i = 0;i<=endPosition;i++){
      if(str2.substring(i,endPosition).startsWith(SWITCHBIKOAG)){
       Serial.print("str2 is ");
       Serial.println(str2);
       if (monokoagState == 0 && monocutState == 0){
        bikoagState = str2.substring(i+SWITCHBIKOAGLEN,i+SWITCHBIKOAGLEN+1).toInt();
       }else{
        Serial.println("Other channel is already working");
       }
       Serial.print("substring to int is: ");
       Serial.println(str2.substring(i+SWITCHBIKOAGLEN,i+SWITCHBIKOAGLEN+1));
      }
    }
  }else{
    Serial.print("Not line of a get request\n");
    Serial.print(str2);
  }

  // Monokoagulation state
    if (str2.startsWith(STARTOFREQUEST)){
    Serial.print("GET OK\n");
    for (int i = 0;i<=endPosition;i++){
      if(str2.substring(i,endPosition).startsWith(SWITCHMONOKOAG)){
       Serial.print("str2 is ");
       Serial.println(str2);
       if (bikoagState == 0 && monocutState == 0){
       monokoagState = str2.substring(i+SWITCHMONOKOAGLEN,i+SWITCHMONOKOAGLEN+1).toInt();
       }else{
        Serial.println("Other channel is already working");
       }
       Serial.print("substring to int is: ");
       Serial.println(str2.substring(i+SWITCHMONOKOAGLEN,i+SWITCHMONOKOAGLEN+1));
      }
    }
  }else{
    Serial.print("Not line of a get request\n");
    Serial.print(str2);
  }

  //Monocutting state
    if (str2.startsWith(STARTOFREQUEST)){
    Serial.print("GET OK\n");
    for (int i = 0;i<=endPosition;i++){
      if(str2.substring(i,endPosition).startsWith(SWITCHMONOCUT)){
       Serial.print("str2 is ");
       Serial.println(str2);
       if (bikoagState == 0 && monokoagState == 0){
       monocutState = str2.substring(i+SWITCHMONOCUTLEN,i+SWITCHMONOCUTLEN+1).toInt();
       }else{
        Serial.println("Other channel is already working");
       }
       Serial.print("substring to int is: ");
       Serial.println(str2.substring(i+SWITCHMONOCUTLEN,i+SWITCHMONOCUTLEN+1));
      }
    }
  }else{
    Serial.print("Not line of a get request\n");
    Serial.print(str2);
  }
  
  Serial.print("bikoagState is ");
  Serial.println(bikoagState); 
  Serial.print("monokoagState is ");
  Serial.println(monokoagState); 
  Serial.print("monocutSatate is ");
  Serial.println(monocutState); 

    
    //------------------------------------
    str = "";
    
  }
}
