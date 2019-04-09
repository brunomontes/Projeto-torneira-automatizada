
#include <Ultrasonic.h>

#define TRIGGER_PIN  11
#define ECHO_PIN     12
#define rele          7
#define verde         3
#define amarelo       4
#define vermelho      5

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

unsigned long AtualMillis;
long AnteriorMillis = 0;
long Intervalo = 0; //Intervalo de desligamento
int limite = 0;
unsigned long SomaIntervalo = 0;

float vazao; //Variável para armazenar o valor em L/min
int contaPulso; //Variável para a quantidade de pulsos
float QtdLtitros = 0; //Variável para Quantidade de agua
float MiliLitros = 0; //Variavel para Conversão

/*float Parcial = 0;
float vazao2; //Variável para armazenar o valor em L/min
float Litros2;
float MiliLitros2 = 0; //Variavel para Conversão*/

void setup(){
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
  
  pinMode(2, INPUT);
  attachInterrupt(0, incpulso, RISING); //Configura o pino 2(Interrupção 0) para trabalhar como interrupção
}

void loop(){
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

  contaPulso = 0;   //Zera a variável para contar os giros por segundos
  sei();      //Habilita interrupção
  delay (1000); //Aguarda 1 segundo
  cli();      //Desabilita interrupção
  
  vazao = contaPulso / 5.5; //Converte para L/min
  MiliLitros = vazao / 60;
  QtdLtitros = QtdLtitros + MiliLitros;
  
  AtualMillis = millis();
  Intervalo = AtualMillis - AnteriorMillis;
  AnteriorMillis = AtualMillis;
  SomaIntervalo = SomaIntervalo + Intervalo;

  if(cmMsec >  10){
    digitalWrite(rele,HIGH);
    digitalWrite(verde,LOW);
    digitalWrite(amarelo,LOW);
    digitalWrite(vermelho,LOW);
    
    //Serial.println("Torneira OFF");
    //delay(500);
    SomaIntervalo = 0;
    limite = 0; 
    //Parcial = 0;
    
    Serial.print("Gasto total em L: ");
    Serial.println(QtdLtitros); 
    Serial.println(microsec);
    Serial.println(cmMsec);
  }
  else{
    
    digitalWrite(rele,LOW);
    digitalWrite(verde,HIGH);
    Serial.println("Torneira ON");
    Serial.println(SomaIntervalo); //intervalo de tempo de uso em milisegundos
    //delay(500);
    //Serial.println(Parcial);
    //Serial.print(", CM: ");
    //Serial.println(cmMsec);

    if(SomaIntervalo > 0){
       digitalWrite(verde,HIGH);
    }

    if(SomaIntervalo > 3000){    
      digitalWrite(amarelo, HIGH);
    }

    if(SomaIntervalo > 6000){
      digitalWrite(vermelho, HIGH);    
    }
    
    if(SomaIntervalo > 9000){
       while (limite == 0){
          digitalWrite(rele,HIGH);
          digitalWrite(verde,LOW);
          digitalWrite(amarelo,LOW);
          digitalWrite(vermelho,LOW);
          Serial.println("%%%%LIMITE DE USO%%%%");
          delay(500);
          digitalWrite(vermelho,HIGH);
          delay(500);
          
          microsec = ultrasonic.timing();
          cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
          if(cmMsec >  10){
            limite = 1;
            SomaIntervalo = 0;
          }
       }
    }       
  }
  
    
}

void incpulso ()
{
 contaPulso++; //Incrementa a variável de pulsos
}
