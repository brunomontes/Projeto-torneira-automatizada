
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

void setup(){
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
}

void loop(){
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

  AtualMillis = millis();
  Intervalo = AtualMillis - AnteriorMillis;
  AnteriorMillis = AtualMillis;
  SomaIntervalo = SomaIntervalo + Intervalo;

  if(cmMsec >  10){
    digitalWrite(rele,HIGH);
    digitalWrite(verde,LOW);
    digitalWrite(amarelo,LOW);
    digitalWrite(vermelho,LOW);
    
    Serial.println("Torneira OFF");
    delay(500);
    SomaIntervalo = 0;
    limite = 0; 
  }
  else{
    digitalWrite(rele,LOW);
    digitalWrite(verde,HIGH);
    Serial.println("Torneira ON");
    Serial.println(SomaIntervalo);
    delay(500);
   
    Serial.print(", CM: ");
    Serial.println(cmMsec);

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
