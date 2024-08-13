//////////////////////////////////// Leitor de Encoder ////////////////////////////////////
// Leitor de encoder incremental com canais A,B e Z.
// Os canais A e B são utilizados como referência de cada passo de giro.
// O canal Z é utilizado como referência para uma volta completa.
//
// Placa: Arduino UNO
// 


//////////////// Constantes ////////////////
const short INPUT_PIN_ENCODER_Z = 2; // Sinal de entrada do canal Z do encoder // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar 
const short INPUT_PIN_ENCODER_A = 3; // Sinal de entrada do canal A do encoder // Apenas os pinos 2 e 3 do arduino UNO permitem interrupções, não alterar
const short INPUT_PIN_ENCODER_B = 4; // Sinal de entrada do canal B do encoder

const int ENCODER_RESOLUTION = 600;
const float STEP             = (float) 360/ENCODER_RESOLUTION;

//////////////// Variáveis ////////////////
float angle            = 0;

volatile short state_A = 0;
volatile short state_B = 0;
volatile int   counter = 0;

//////////////// Funções adicionais ////////////////
void set_state(){                             // É importante manter a leitura do sensor ótico e atualização das variáveis em uma ISR e não no loop. Dessa forma é garantida a captura e processamento das mudanças de estado
  state_A = digitalRead(INPUT_PIN_ENCODER_A);
  state_B = digitalRead(INPUT_PIN_ENCODER_B);
  if (state_A == state_B){
    counter = (counter - 1 + ENCODER_RESOLUTION )%ENCODER_RESOLUTION; // Sentido anti-horário adotado como negativo
  }
  else{
    counter = (counter + 1 + ENCODER_RESOLUTION )%ENCODER_RESOLUTION; // Sentido horário adotado como positivo
  }
}


void tare(){
  counter = 0;
}

//////////////// Funções básicas ////////////////
void setup() {
  Serial.begin(9600); 

  pinMode(INPUT_PIN_ENCODER_Z, INPUT);
  pinMode(INPUT_PIN_ENCODER_A, INPUT);
  pinMode(INPUT_PIN_ENCODER_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_Z), tare, FALLING);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN_ENCODER_A), set_state, FALLING);

}

void loop() {

  angle = counter*STEP;
  Serial.println(angle);

}


