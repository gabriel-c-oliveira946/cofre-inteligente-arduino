#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int trigPin = 9;
const int echoPin = 8;
const int botaoPin = 7;
const int ledPin = 6; // <-- NOVO: Avisamos onde está o LED

int saldo = 0;
bool moedaDetectada = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(botaoPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); // <-- NOVO: Prepara o pino para acender a luz

  atualizarDisplay();
  Serial.println("Cofre Iniciado!");
}

void loop() {
  long duracao, distancia;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Limite de tempo para evitar travamentos
  duracao = pulseIn(echoPin, HIGH, 30000); 
  
  if (duracao == 0) {
    distancia = 100; 
  } else {
    distancia = (duracao / 2) / 29.1;
  }

  // Depósito de Moeda
  if (distancia < 5 && distancia > 0 && !moedaDetectada) {
    saldo += 1;
    moedaDetectada = true;
    atualizarDisplay();
    Serial.println("Moeda depositada!");
    delay(50); 
  } else if (distancia >= 30) {
    moedaDetectada = false; 
  }

  // Lógica do Saque (Botão)
  if (digitalRead(botaoPin) == LOW) {
    if (saldo > 0) { 
      saldo = 0;
      atualizarDisplay();
      Serial.println("Saldo zerado!");
      
      // <-- NOVO: O show de luzes do saque!
      digitalWrite(ledPin, HIGH); // Acende o LED
      delay(1000);                // Deixa aceso por 1 segundo inteiro
      digitalWrite(ledPin, LOW);  // Apaga o LED
      
      delay(50); // Respiro de segurança
    }
  }
  
  delay(50);
}

void atualizarDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Meu Cofre"); 
  lcd.setCursor(0, 1);
  lcd.print("Saldo: R$ ");
  lcd.print(saldo);
}