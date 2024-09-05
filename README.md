# Leitor de Encoder incremental com arduino

### Encoder:
  - Os canais A e B são utilizados como referência de cada passo de giro.
  - O canal Z é utilizado como referência para uma volta completa.
    
  - Fios:
      - Canal Z: Amarelo
      - Canal A: Verde
      - Canal B: Branco
      - Vcc: Vermelho
      - Gnd: Preto
      - Shield
        
  ### Microcontrolador:
   - Arduino Nano
   - Código: [encoder_io.ino](https://github.com/gmc-b/Arduino_Encoder_IO/blob/main/encoder_io/encoder_io.ino)
    
  ### Esquemático
  ![Esquemático](https://github.com/gmc-b/Arduino_Encoder_IO/blob/main/img/esquematico.png)

  ### Data Sheet 
  - As versões [original](https://github.com/gmc-b/Arduino_Encoder_IO/blob/main/datasheet/LPD3806-360BM_original_chin%C3%AAs.PDF) e [traduzida](https://github.com/gmc-b/Arduino_Encoder_IO/blob/main/datasheet/LPD3806-360BM_traduzido_portugues.pdf) do datasheet do encoder podem ser consultadas no folder [/datasheet](https://github.com/gmc-b/Arduino_Encoder_IO/tree/main/datasheet). Não foi possível encontrar a documentação específica para o modelo adquirido (com canal Z), porém o modelo descrito no data sheet segue as mesmas especificações técnicas.
