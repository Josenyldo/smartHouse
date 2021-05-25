#include <WiFi.h>
const char* ssid = "iMAXX-FiBRA-JOSENILDO";
const char* password = "sj@divinha";
WiFiServer server(80);

/* LED */
#define LED_PIN 2
const int portaVazao = GPIO_NUM_35;

static void atualizaVazao();
volatile int pulsos_vazao = 0;
float vazao = 0;
// interrupção
void IRAM_ATTR gpio_isr_handler_up(void* arg)
{
  pulsos_vazao++;
  portYIELD_FROM_ISR();
}

//Página web

void WiFiLocalWebPageCtrl(void)
{
  WiFiClient client = server.available();   // listen for incoming clients
  //client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            
            client.println();

            // the content of the HTTP response follows the header:
            //WiFiLocalWebPageCtrl(); 
              client.print("<head>");
              client.print("<meta charset=\"UTF-8\">");
              client.println("<meta http-equiv=\"refresh\" content=\"2\"/>");
              client.print("</head>");
              client.print("Vazão nesse instante: ");
              client.print(vazao);
              client.print("  (L/s)<br>");
              client.print("<br>");        

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}





/***************************************************
* Connecting to a WiFi network
****************************************************/
void connectWiFi(void)
{
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

void tarefa_pedir_vazao(void *parameter){
  while(1){
    
      vazao = pulsos_vazao/5.5;
  pulsos_vazao = 0;
   WiFiLocalWebPageCtrl();
  // Realizar o print da leitura no serial
  Serial.println("Leitura do Sensor de Vazao (L/s):");
  Serial.println(vazao);
  // realizar um delay e inicializar leitura daqui a 1 segundos
  delay(200);
    }
     
  }
void iniciaVazao(gpio_num_t Port){
  gpio_set_direction(Port, GPIO_MODE_INPUT);
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);
  gpio_set_pull_mode(Port, GPIO_PULLUP_ONLY);
  gpio_intr_enable(Port);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(Port, gpio_isr_handler_up, (void*) Port);
}


void setup() {
  Serial.begin(115200);
  // definir porta do sensor de gas como entrada
  iniciaVazao((gpio_num_t) portaVazao);
  connectWiFi();




xTaskCreatePinnedToCore(
  tarefa_pedir_vazao,
  "tarefa_pedir_vazao",
  10000,
  NULL,
  1,
  NULL,
  1
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
