//////////////////////////////////// Leitor de Encoder ////////////////////////////////////
// Leitor de encoder incremental com canais A,B e Z.
// Os canais A e B são utilizados como referência de cada passo de giro.
// O canal Z é utilizado como referência para uma volta completa.
//
// Placa: Arduino UNO
// Interrupções:
//    Para uma frequência de 50 Hz (T = 20 ms)
//    Clock sistema: 16 MHz
//    Prescalar escolhido: 64
//    Frequência timer 1: 16Mhz/64 = 250 KHz
//    Pulse time = 1/250 Khz =  4 us 
//    Contar até: 20 ms / 4 us = 5.000 (f = 50 Hz)





//////////////// Constantes ////////////////
const short INPUT_PIN_ENCODER_Z = 2; // Sinal de entrada de referência de volta   // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar  
const short INPUT_PIN_ENCODER_A = 3;   // Sinal de entrada do canal A do encoder    // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar      
const short INPUT_PIN_ENCODER_B = 4;   // Sinal de entrada do canal B do encoder   

const int  ENCODER_RESOLUTION = 600;
const int  TIMER_COUNTER      = 5000;



//////////////// Variáveis ////////////////
volatile char  direction = 0;
volatile char  pin_state = 0;
volatile int   counter   = 0;

char serial_buffer     = 0;



//////////////// Funções adicionais ////////////////
void set_state(){   

  direction = ( ( PIND + 0b00001000) & 0b00010000); // direction > 0 para Sentido positivo, direction = 0 para sentido negativo 

  if (direction){
    counter --; // Sentido anti-horário adotado como negativo
    if (counter < 0) counter = ENCODER_RESOLUTION-1;
  }
  else{
    counter ++; // Sentido horário adotado como positivo
    if (counter >= ENCODER_RESOLUTION) counter = 0;
    
  }
}


ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  // Reseta o contador para a próxima interrupção
  Serial.println(counter );
}


void tare(){
  counter = 0;
}


//////////////////////////////// Setup ////////////////////////////////


void setup() {

  //////////////// Timer interrupt ////////////////
  cli();                      // Desativa interrupções para configuração
  
  TCCR1A = 0;                 // Atribui valor 0 para o registrador TCCR1A para desativar modo PWM
  TCCR1B = 0;                 // Atribui valor 0 para o registrador TCCR1B 

  TCCR1B |= 0b00000011;        // Atribuição de valor 011 (prescalar: 64) para os bits responsáveis pela definição de prescalar
  TIMSK1 |= 0b00000010;        // Atribuição de valor 10 para habilitar o modo comparador
  OCR1A   = TIMER_COUNTER;     // Define o valor de comparação para ativação da interrupção

  sei();                      // Ativa novamente as interrupções


  //////////////// Definição de pinos ////////////////
  Serial.begin(9600); 

  //pinMode(INPUT_PIN_ENCODER_Z, INPUT);
  pinMode(INPUT_PIN_ENCODER_A, INPUT);
  pinMode(INPUT_PIN_ENCODER_B, INPUT);
  
  //attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_Z), tare, FALLING);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_A), set_state, FALLING);

}

void loop() {}


