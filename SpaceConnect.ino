//bibliotecas utilizadas

#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//definição das entradas e valores das variaveis:
int pinoSensor = A0; 
int valorForca = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int pinoTemp = A1;
int valorT = 0;
#define LDR A2
int valorLuz = 0;
float Temp = 0;
float Temperatura = 0;
Servo servo1;
Servo servo2;
int som = 7; 

//definindo se são saidas ou entradas e ligando a tela de LCD:
void setup(){
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(som,OUTPUT);
  lcd.init();                
  lcd.backlight();           
  lcd.print("LCD acesa!");    
  delay(2000);
  lcd.clear(); 
  servo1.attach(5);
  servo2.attach(6);
  
}

void loop() {
  // 1. Leitura dos Sensores e calculo da temperatura 
  valorForca = analogRead(pinoSensor); 
  valorT = analogRead(pinoTemp);
  Temp = (valorT * 5.0) / 1024.0;    
  Temperatura = (Temp - 0.5) * 100;
  valorLuz = analogRead(LDR);
  
  
  
  // --- VERIFICAÇÃO DOS ALERTAS ---
  // Usamos IFs separados para que uma emergência não anule a outra
  
  if (valorForca >= 850) {//aviso de impacto
    lcd.clear();
    digitalWrite(2, HIGH); 
    lcd.print("ALERTA ALERTA");
    lcd.setCursor(0, 1);
    lcd.print("IMPACTO");
    tone(som, 1000); 
    delay(1000);
    noTone(som);
    
  }
  else if (Temperatura >= 70.0) {//aviso de alta temperatura
    lcd.clear();
    digitalWrite(3, HIGH); 
    lcd.print("ALTA TEMPERATURA");
    lcd.setCursor(0, 1);
    lcd.print("PERIGO");
    tone(som, 1000); 
    delay(1000);
    noTone(som);
   
  }
  else if (valorLuz >= 512) { //aviso de luz intensa
    lcd.clear();
    digitalWrite(4, HIGH); 
    lcd.print("Brilho intenso");
    lcd.setCursor(0, 1);
    lcd.print("PERIGO");
    tone(som, 1000); 
    delay(1000);
    noTone(som);
  }
  // --- MODO NORMAL ---
  else {
    // Desliga TODOS os alarmes quando tudo estiver seguro
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW); 
    
    // Mostra Força na Linha 0, Esquerda
    lcd.setCursor(0, 0); 
    lcd.print("F:");
    lcd.print(valorForca);
	lcd.print("N");
    lcd.print("    "); // Espaços para limpar fantasmas de números antigos
    
    // Mostra Luz na Linha 0, Direita (Coluna 9)
    lcd.setCursor(9, 0);
    lcd.print("L:");
    lcd.print(valorLuz);
    lcd.print("    "); // Espaços para limpar fantasmas de números antigos
    
    // Mostra Temperatura na Linha 1
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(Temperatura, 1); 
    lcd.print("C   ");  
    delay(500); 
    
    //Faz os servos de demonstração se movimentarem
    servo1.write(0);
    servo2.write(0);
   	delay(0700);
   	servo1.write(90);
    servo2.write(90);
   	delay(0700);

   
    
  }
}
