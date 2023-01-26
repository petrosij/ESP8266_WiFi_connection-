#include <ESP8266WiFi.h>
 
const char* ssid = "";  // для SSID точки доступа
const char* password = "";  // для пароля к точке доступа
 
int ledPin = 2; // контакт GPIO2 на ESP8266 (d4)
int buttonPin = 14; // контакт GPIO14 (d5)
bool rele_state = 0; // 0 - light off , 1 -light on
int wait_time = 100; // время ожидания повторного нажатия на кнопку (для устранения дребезга)
int last_push = 0; //  время последнего нажатия на кнопку 


WiFiServer server(80);  //  порт веб-сервера
 
void setup() {
  Serial.begin(115200);
  delay(10);
  last_push = millis();
 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
   
  // подключаемся к WiFi-сети:
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");  //  "Подключение к "
  Serial.println(ssid);
   
  WiFi.begin(ssid, password); 
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); //  "Подключение к WiFi
                                        //  выполнено"
   
  // запускаем сервер:
  server.begin();
  Serial.println("Server started");  //  "Сервер запущен"
 
  // печатаем IP-адрес:
  Serial.print("Use this URL to connect: ");  //  "Используем этот URL 
                                              //  для подключения: "
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  // проверяем, подключен ли клиент:
  WiFiClient client = server.available();
  if (client) {
       // ждем, когда клиент отправит какие-нибудь данные:
    Serial.println("new client");  //  "новый клиент"
    while(!client.available()){
      delay(1);
    }
     
    // считываем первую строчку запроса:
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
     
     // обрабатываем запрос:
    int value = LOW;                         
    if (request.indexOf("/LED=ON") != -1) {
      rele_state = 1;
    } 
    if (request.indexOf("/LED=OFF") != -1){
      rele_state = 0;
    }
          
    // выставляем значение на ledPin в соответствии с запросом:
    digitalWrite(ledPin, rele_state);
  
    // возвращаем ответ:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");  //  "Тип контента: 
                                                //  text/html "
    client.println("");  //  не забываем это...
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
     
    client.print("<h1>Led pin is now: </h1>");  //  "Контакт светодиода теперь 
                                       //  в состоянии: "
     
    if(rele_state == HIGH) {
      client.print("<font size=\"50\" color=\"green\" face=\"Arial\">ON</font>");   //  "Вкл"
    } else {
      client.print("<font size=\"50\" color=\"red\" face=\"Arial\">OFF</font>");  //  "Выкл"
    }
    client.println("<br><br>");
    client.println("<h1> Click <a href=\"/LED=ON\">here</a> to turn the LED on pin 2 ON<br> </h1>");  //  "Кликните тут, чтобы включить светодиод
                   //  на контакте 2"
    client.println("<h1> Click <a href=\"/LED=OFF\">here</a> to turn the LED on pin 2 OFF<br> </h1>");     //  "Кликните тут, чтобы выключить светодиод
                   //  на контакте 2"
  
    client.println("</html>");
   
    delay(1);
    Serial.println("Client disconnected");  //  "Клиент отключен"
    Serial.println("");
    }
    
    if (digitalRead(buttonPin) == 1 && (millis() - last_push > wait_time )){ // Проверка нажатия кнопки
      rele_state = !rele_state;
      last_push = millis();
      
    }

    digitalWrite(ledPin, rele_state);
 
  
  
}
