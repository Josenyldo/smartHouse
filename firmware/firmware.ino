void MostrarInfor(void *pvParameters){
  while(true)
    Serial.println("OI");
  
  
  }



void setup() {
  Serial.begin(9600);
xTaskCreatePinnedToCore(
  MostrarInfor,
  "MostrarInfor",
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
