//*******************************************************************************************************************************
//*******************************************************************************************************************************
//                                                   SMART TOILET
//*******************************************************************************************************************************
//*******************************************************************************************************************************
// DESCRIÇÃO DO FUNCIONAMENO: ESSE PROGRAMA REALIZA O FUNCIONAMENTO AUTOMÁTICO DE UMA PRIVADA INTELIGENTE, QUE APRESENTAM DOIS 
// MODOS
//  Modo 1
//• Através de acionamento externo em um botão, abre as duas tampas;
//• Através de acionamento externo em um botão fecha as duas tampas;
//• Quando baixar as duas tampas o sistema de descarga deve ser acionado no modo “baixo de descarga”.
// Modo 2
//• Através de acionamento externo por um botão abre uma tampa;
//• Faz aquecimento da tampa à 26 ºC;
//• Após uso, através de acionamento de botão realiza o procedimento de higienização deve ser acionado por 3 segundos;
//• Ao levantar-se, a tampa deve fechar automaticamente;
//• Quando baixar uma tampa o sistema de descarga deve ser acionado em modo “turbo de descarga”.
//
// PROGRAMADOR: Marcos da Silva Paiva
// DATA: 20/05/2021
// VERSÃO: 1.0
//*******************************************************************************************************************************
//*******************************************************************************************************************************

#include <Arduino.h>

//DECLARAÇÃO DAS VARIÁVEIS DO SISTEMA
const int seatSensor = 2; //Sensor de assento
const int btnMode1 = 3; //Botão que ativa o Modo 1
const int btnMode2 = 4; //Botão que ativa o modo 2
const int cover1 = 5; //Tampa de cima
const int cover2 = 6; //Tampa de baixo
const int cover1r = 7; //Tampa de cima reversão do motor
const int cover2r = 8; //Tampa de baixo reversão do motor
const int dischargeLow = 9; //Discarga baixa
const int dischargeHigh = 10; //Discarga Turbo
const int heater = 11; //Aquecedor do assento
const int sanitation = 12; //Sistema de higienização

int stateCover1 = 0; //SE IGUAL A 1 TAMPA DE CIMA LEVANTADA SE IGUAL 0 TAMPA DE CIMA ABAIXADA
int stateCover2 = 0; //SE IGUAL A 1 TAMPA DE BAIXO ABERTA, SE IGUAL A 0 TAMPA DE BAIXO FECHADA
int stateSeatSensor = 0; //SE IGUAL 1 PESSOA SENTADA SE IGUAL A 0 PESSOA LEVANTADA
int mode2 = 0; //INDICA QUE O MODO 2 ESTÁ ATIVO

//CONFIGURAÇÕES DO SISTEMA
void setup() {
  //DECLARANDO AS ENTRADAS (BOTÕES)
  pinMode (seatSensor, INPUT);
  pinMode (btnMode1, INPUT);
  pinMode (btnMode2, INPUT);
  //DECLARANDO SAÍDAS (MOTORES E AQUECEDOR)
  pinMode(cover1, OUTPUT);
  pinMode(cover2, OUTPUT);
  pinMode(cover1r, OUTPUT);
  pinMode(cover2r, OUTPUT);
  pinMode (dischargeLow, OUTPUT);
  pinMode (dischargeHigh, OUTPUT);
  pinMode (heater, OUTPUT);
  pinMode (sanitation, OUTPUT);

  Serial.begin(115200); //ATIVANDO COMUNICAÇÃO SERIAL COM BAUD RATE DE 115200

  //INICAINDO SISTEMA COM TODAS AS SAÍDAS DESLIGADAS
  digitalWrite(cover1, LOW);
  digitalWrite(cover2, LOW);
  digitalWrite(cover1r, LOW);
  digitalWrite(cover2r, LOW);
  digitalWrite(dischargeLow, LOW);
  digitalWrite(dischargeHigh, LOW);
  digitalWrite(heater, LOW);
  digitalWrite(sanitation, LOW);
}

void loop() {
  stateSeatSensor = digitalRead(seatSensor); //VERIFICA ESTADO DO SENSOR DE ASSENTO

  // MODO 1
  if ((digitalRead(btnMode1) == HIGH) && (stateCover1 == 0) && (stateCover2 == 0)) { //VERIFICA SE BOTÃO MODO 1 PRESSIONADO AND TAMPAs FECHADAS AND MODO 1 DESLIGADO
    //ROTINA REALIZADA QUANDO AS TAMPAS ESTÃO FECHADAS
    digitalWrite(cover1, HIGH); //ABRE TAMPA DE CIMA
    digitalWrite(cover2, HIGH); //ABRE TAMPA DE BAIXO
    delay(3000); //TEMPO PARA ABRIR AS TAMPAS
    digitalWrite(cover1, LOW); //ABRE TAMPA DE CIMA
    digitalWrite(cover2, LOW); //ABRE TAMPA DE BAIXO
    stateCover1 = 1; //TAMPA DE CIMA LEVANTADA
    stateCover2 = 1; //TAMPA DE BAIXO LEVANTADA
    Serial.println("TAMPAS ABERTAS");
    delay(100);
  } else if ((digitalRead(btnMode1) == HIGH) && (stateCover1 == 1) && (stateCover2 == 1)) { //VERIFICA SE BOTÃO MODO 1 PRESSIONADO AND TAMPAS ABERTAS AND MODO 1 LIGADO
    //ROTINA REALIZADA QUANDO AS TAMPAS ESTÃO ABERTAS NO MODO 1, PARA FECHAR A TAMPA É NECESSÁRIO AS DUAS SAÍDAS ATIVAS
    digitalWrite(cover1r, HIGH); //FECHA TAMPA DE CIMA
    digitalWrite(cover2r, HIGH); //FECHA TAMPA DE BAIXO
    digitalWrite(cover1, HIGH);
    digitalWrite(cover2, HIGH);
    delay(3000); //TEMPO PARA FECHAR AS TAMPAS
    digitalWrite(cover1, LOW);
    digitalWrite(cover2, LOW);
    digitalWrite(cover1r, LOW);
    digitalWrite(cover2r, LOW);
    stateCover1 = 0; //TAMPA DE CIMA ABAIXADA
    stateCover2 = 0; //TAMPA DE BAIXO ABAIXADA
    Serial.println("TAMPAS FECHADAS E ATIVANDO DESCARGA BAIXA");
    digitalWrite(dischargeLow, HIGH); //ATIVA DISCARGA BAIXA
    delay(500); //500 milissegundos acionada
    digitalWrite(dischargeLow, LOW); //DESATIVA DISCARGA BAIXA
    delay(100);
  }

  // MODO 2
  if ((digitalRead(btnMode2) == HIGH) && (stateCover1 == 0) && (stateCover2 == 0) && (mode2 == 0)) { //VERIFICA SE BOTÃO MODO 2 PRESSIONADO AND TAMPAS FECHADAS AND MODO 2 DESLIGADO
    //ROTINA REALIZADA QUANDO AS TAMPAS ESTÃO FECHADAS
    digitalWrite(cover1, HIGH); //ABRE TAMPA DE CIMA
    digitalWrite(heater, HIGH); //LIGA AQUECEDOR DO ASSENTO
    delay(3000); //TEMPO PARA ABRIR A TAMPA DE CIMA
    digitalWrite(cover1, LOW); //ABRE TAMPA DE CIMA
    Serial.println("TAMPA DE CIMA ABERTA E ACIONADO AQUECEDOR");
    stateCover1 = 1; //TAMPA DE CIMA LEVANTADA
    mode2 = 1; //MODO 1 ATIVO
    delay(100);
  } else if ((digitalRead(btnMode2) == HIGH) && (stateCover1 == 1) && (stateCover2 == 0) && (stateSeatSensor == 1) && (mode2 == 1)) { //VERIFICA SE BOTÃO MODO 2 PRESSIONADO AND TAMPA DE CIMA LEVANTADA AMD TAMPA DE BAIXO ABAIXADA AND PESSOA SENTADA AND MODO 2 LIGADO
    //ROTINA REALIZADA A HIGIENIZAÇÃO NO MODO 2
    digitalWrite(heater, LOW); //DESLIGA AQUECEDOR DO ASSENTO
    delay(100);
    Serial.println("LIGANDO HIGIENIZAÇÃO");
    digitalWrite(sanitation, HIGH); //ACIONA A HIGIENIZAÇÃO
    delay(3000); //ACIONADA POR 3 SEGUNDOS
    Serial.println("DESLIGANDO HIGIENIZAÇÃO");
    digitalWrite(sanitation, LOW); //DESACIONA A HIGIENIZAÇÃO
    mode2 = 2;
    delay(100);
  } else if ((stateSeatSensor == 0) && (stateCover1 == 1) && (stateCover2 == 0) && (mode2 == 2)) {
    //ROTINA REALIZADA APÓS A HIGIENIZAÇÃO NO MODO 2, FECHAMENTO DA TAMPA, É NECESSÁRIO AS DUAS SAÍDAS ATIVAS
    delay(3000); //TEMPO PARA A PESSOA LEVANTAR DO ASSENTO
    digitalWrite(cover1r, HIGH); //FECHA TAMPA DE CIMA
    digitalWrite(cover1, HIGH);
    delay(3000); //TEMPO PARA FECHAR AS TAMPAS
    digitalWrite(cover1r, LOW); //FECHA TAMPA DE CIMA
    mode2 = 0; //MODO 1 ATIVO
    stateCover1 = 0; //TAMPA DE CIMA ABAIXADA
    Serial.println("TAMPA FECHADA E ATIVANDO DESCARGA TURBO");
    digitalWrite(dischargeHigh, HIGH); //ACIONA DESCARGA NO MODO TURBO
    delay(500);
    digitalWrite(dischargeHigh, LOW); //DESACIONA DESCARGA NO MODO TURBO
    delay(100);
  }
}