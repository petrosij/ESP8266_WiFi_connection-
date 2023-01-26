#include <String.h>

#define ENDOFLINE '\n'
#define STARTOFREQUEST "GET /"
#define SWITCHSTATE "switchState="
#define SWITCHSTATELEN 12

#define LINESNUM 1000

#define PINBIKOAGUL 3
#define PINMONOKOAG 4
#define PINMONOCUT 5

String str = "";
String str2 = "";
long switchState = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

  str += "GET /?switchState=1 HTTP/1.1\n";
  str += "Host: 192.168.1.177\n";
  str += "Connection: keep-alive\n";
  str += "Upgrade-Insecure-Requests: 1\n";
  str += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36\n";
  str += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n";
  str += "Referer: http://192.168.1.177/?switchState7=0\n";
  str += "Accept-Encoding: gzip, deflate\n";
  str += "Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\n";
  str += "DNT: 1\n";

}

void loop() {
  
  int endPosition = 0;
  
  // put your main code here, to run repeatedly:
  for (int i = 0;i<1000;i++){  // check for 1000???   <-----------------------------
    if (str[i] == ENDOFLINE){
      str2 = str.substring(0,i);
      endPosition = i;
      Serial.print("endPosition is ");
      Serial.println(endPosition);
      break; 
    }
  }

  if (str2.startsWith(STARTOFREQUEST)){
    Serial.print("GET OK\n");
    for (int i = 0;i<=endPosition;i++){
      if(str2.substring(i,endPosition).startsWith(SWITCHSTATE)){
       Serial.print("str2 is ");
       Serial.println(str2);
       switchState = str2.substring(i+SWITCHSTATELEN,i+SWITCHSTATELEN+1).toInt();
       Serial.print("substring to int is: ");
       Serial.println(str2.substring(i+SWITCHSTATELEN,i+SWITCHSTATELEN+1));
       
      }
      
    }
    
  }else{
    Serial.print("Not line of a get request\n");
    Serial.print(str2);
  }
  
  Serial.print("Switch state is ");
  Serial.println(switchState); 
  delay(2000);


  
}
