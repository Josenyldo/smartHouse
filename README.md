# Smart House
# Objetivo

O obejtivo desse projeto é medir a vazão em tempo real a partir de uma rede local, além degit  poder enviar comandos para a solução em que ela posssa resolver outros problemas como abrir uma porta.
 
 # Materiais
 Para desenvolvimentos do projeto foi utilizado o microcontrolador ESP32, porque ele conta com suporte para WIFI, é necessário já que precisamos fazer um monitoramento por uma rede sem fio; módulo relé que faz a integração entre o microcontrolador e a solenoide que está porta que possamos fazer o acionamento via rede sem fio; fonte de alimentação para alimentar o sistema. Nesse caso utilizando duas: uma para alimentar o ESP32 e outra para fornecesse energia suficiente para acionar a solenoide; e fios jumpers para fazer algumas ligações.
 
# Firmware
Para implementação do código utilizamos o FreeRTOS  para priorizar algumas tarefas para que elas sejam resolvidas atendendo as restrições de tempo real. Além de podemos levar nossa aplicação para outro dispotivo que também implemente FreeRTOS.

# Testes
O vídeo que contempla a primeira parte do projeto é: [Acionamento de Solenoide](https://www.youtube.com/watch?v=opxVK3Q5iVM) 




O vídeo que contempla a segunda parte do projeto é: [Medido a vazão da água da torneira](https://www.youtube.com/watch?v=-18AyDJfE4k) 
