//////////////////////////////////// Leitor de Encoder ////////////////////////////////////
// Leitor de encoder incremental com canais A,B e Z.
// Os canais A e B são utilizados como referência de cada passo de giro.
// O canal Z é utilizado como referência para uma volta completa.
//
// Placa: Arduino UNO
// Interrupções:
//    Para uma frequência de 100 Hz (T = 10 ms)
//    Clock sistema: 16 MHz
//    Prescalar escolhido: 64
//    Frequência timer 1: 16Mhz/64 = 250 KHz
//    Pulse time = 1/250 Khz =  4 us 
//    Contar até: 10 ms / 4 us = 2500





//////////////// Constantes ////////////////
//const short INPUT_PIN_ENCODER_Z = 2; // Sinal de entrada de referência de volta (sensor hall) // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar 
const short INPUT_PIN_ENCODER_A = 3;   // Sinal de entrada do canal A do encoder                  // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar
const short INPUT_PIN_ENCODER_B = 4;   // Sinal de entrada do canal B do encoder

const int ENCODER_RESOLUTION = 600;
const float STEP             = (float) 360/ENCODER_RESOLUTION;
const float PI_2             = 6.2832;     // PI = 3.1416
const char  TARE_BYTE        = 0b00100101; // Byte de verificação para processo de tara definido arbitrariamente (decimal: 37/ ascii: "%")

const char BIT_MASK[8] = {0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};
//////////////// Variáveis ////////////////
volatile char  state_A = 0;
volatile char  state_B = 0;
volatile int   counter = 0;


char serial_buffer     = 0;




//////////////// Funções adicionais ////////////////
void set_state(){                                         // É importante manter a leitura do sensor ótico e atualização das variáveis em uma ISR e não no loop. Dessa forma é garantida a captura e processamento das mudanças de estado
  state_A = (PORTD & BIT_MASK[INPUT_PIN_ENCODER_A]) > 0 ; // & lógico com bit_mask para cada pino, garantindo leitura mais rápida  
  state_B = (PORTD & BIT_MASK[INPUT_PIN_ENCODER_B]) > 0 ; // & lógico com bit_mask para cada pino, garantindo leitura mais rápida 

  if (state_A == state_B){
    counter = (counter + 1 + ENCODER_RESOLUTION )%ENCODER_RESOLUTION; // Sentido anti-horário adotado como negativo
  }
  else{
    counter = (counter - 1 + ENCODER_RESOLUTION )%ENCODER_RESOLUTION; // Sentido horário adotado como positivo
  }
  }


ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  // Reseta o contador para a próxima interrupção
  Serial.println((counter * (PI_2/ENCODER_RESOLUTION)) ,3 );
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
  OCR1A   = 2500;               // Define o valor de comparação para ativação da interrupção

  sei();                      // Ativa novamente as interrupções


  //////////////// Definição de pinos ////////////////
  Serial.begin(9600); 

  //pinMode(INPUT_PIN_ENCODER_Z, INPUT);
  pinMode(INPUT_PIN_ENCODER_A, INPUT);
  pinMode(INPUT_PIN_ENCODER_B, INPUT);
  
  //attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_Z), tare, FALLING);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_A), set_state, FALLING);

}

void loop() {
  if (Serial.available() > 0) {
    serial_buffer = Serial.read();
    
    if (serial_buffer == TARE_BYTE){
      tare();
    }
  }
}


