
#include <WiFi.h>

//Credecias
const char* ssid = "Sua Rede";
const char* password = "Sua Senha da Rede";



//Inciando Servidor na porta 80
WiFiServer server(80);

//Porta de envia o comando para o relé
#define Porta GPIO_NUM_25
//Porta que recebe dados do sensor de fluxo
const int portaVazao = GPIO_NUM_35;

//Função que irá fazer a atualizão da vazão
static void atualizaVazao();

//conta a quantidade de pulso para ser multiplicador pela a constante 5.5 e gerar o valor da vazão
volatile int pulsos_vazao = 0;

// Armazena a vazão
float vazao = 0;

// interrupção
void IRAM_ATTR gpio_isr_handler_up(void* arg)
{
  pulsos_vazao++;
  portYIELD_FROM_ISR();
}

//Função que contem toda a infraestruta da página web
void WiFiLocalWebPageCtrl(void)
{
  WiFiClient client = server.available();                              
    String currentLine = "";                
    while (client.connected()) {          
      if (client.available()) {             
        
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<head>");
            client.print("<meta charset=\"UTF-8\">");
            client.println("<meta http-equiv=\"refresh\" content=\"0.8;URL='/L'\"/>");
            client.print("</head>");
            client.print("<h1>Vazão nesse instante: </h1> ");
            client.print(vazao);
            client.print("  <h1>(L/s)<h1>");
            client.print("<br>");             
             client.print("<h1><a href=\"/H\">Liga LED</a></h1>");
          } 
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");

}





//Função que faz a conexão com a rede wifi
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

//Função que contem a task a ser executada
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


  //reponsável pela a interrupção no ESP32
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
  pinMode(Porta,OUTPUT);

  iniciaVazao((gpio_num_t) portaVazao);
  connectWiFi();



//Criando Task do RTOS
xTaskCreatePinnedToCore(
  tarefa_pedir_vazao, //task
  "tarefa_pedir_vazao", //nome da task
  10000,// tamanho da pilha de memória resevada para essa tarefa
  NULL, // Parametras passados a task
  1,  //proridade na excecução dessa task
  NULL, //Cabeçalho da tarefa
  1 // núcleo onde task será executada
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
