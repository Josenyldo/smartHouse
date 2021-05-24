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

void tarefa_pedir_vazao(void *parameter){
  while(1){
    
      vazao = pulsos_vazao/5.5;
  pulsos_vazao = 0;
  // Realizar o print da leitura no serial
  Serial.println("Leitura do Sensor de Vazao (L/s):");
  Serial.println(vazao);
  // realizar um delay e inicializar leitura daqui a 1 segundos
  delay(1000);
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
