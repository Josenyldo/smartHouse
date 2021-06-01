
#include <WiFi.h>
#include "FirebaseESP32.h"

#define WIFI_SSID "iMAXX-FiBRA-JOSENILDO"                   
#define WIFI_PASSWORD "sj@divinha"         
#define FIREBASE_HOST "https://smart-house-9d2e8-default-rtdb.firebaseio.com/"    
#define FIREBASE_AUTH "bB0CH5loxBbr7w6C4LZGHOAtduYWQGXdy1qn5jWG"
// Firebase Data object
FirebaseData firebaseData;
FirebaseJson json;
String nodo = "/Vazao";
bool estado;

//Porta de envia o comando para o relé
#define porta GPIO_NUM_25

//Porta que recebe dados do sensor de fluxo
const int portaVazao = GPIO_NUM_35;

//Função que irá fazer a atualizão da vazão
static void atualizaVazao();

//conta a quantidade de pulso para ser multiplicador pela a constante 5.5 e gerar o valor da vazão
volatile int pulsos_vazao = 0;

// Armazena a vazão
float vazao = 0;

void conexaoWifi(){
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectado al Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  
}

void conexaoFirebase(){
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}
  
// interrupção
void IRAM_ATTR gpio_isr_handler_up(void* arg)
{
  pulsos_vazao++;
  portYIELD_FROM_ISR();
}







//Função que contem a task a ser executada
void tarefa_pedir_vazao(void *parameter){
  while(1){
    
  vazao = pulsos_vazao/5.5;
  pulsos_vazao = 0;
 
  // Realizar o print da leitura no serial
  Serial.println("Leitura do Sensor de Vazao (L/s):");
  Serial.println(vazao);
  // realizar um delay e inicializar leitura daqui a 1 segundos
  delay(200);
  json.set("/Data", vazao);
  Firebase.updateNode(firebaseData,"/Vazao",json);
  delay(100);

  
   Firebase.getBool(firebaseData, nodo + "/Porta/switch");
   estado = firebaseData.boolData();
       
   if(!estado){
        digitalWrite(porta,HIGH);
    }else
       digitalWrite(porta,LOW);

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
  pinMode(porta,OUTPUT);
  iniciaVazao((gpio_num_t) portaVazao);
  conexaoWifi();
  conexaoFirebase();

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
