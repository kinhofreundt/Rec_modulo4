int led[4] = {16,17,18,8};
int ldr = 4;
int botao1 = 6;
int botao2 = 5;
int valores[300] = {};
int contador = 0;
int buzzer = 46;

float converterValor(float valorLdr, float valmin, float valmax){
  float valor = 15*(valorLdr-valmin)/(valmax-valmin);
  return valor;
}


void setup() {
  Serial.begin(115200);
  for(int i=0;i<=4; i++){
    pinMode(led[i], OUTPUT);
  }
  pinMode(ldr, INPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  int valorLdr=analogRead(ldr);
  int valor = converterValor(valorLdr,8126,64);

  int botao_on = digitalRead(botao1);
  int botao_on2 = digitalRead(botao2);

  if(botao_on == LOW){
    valores[contador] = valor;
    Serial.println(valores[contador]);
    contador += 1;
    Serial.println(valorLdr);
    delay(400);

    for(int i=0; i<4; i++){
      digitalWrite(led[i], (1&valor>>i));
    } 
    tone(buzzer,valor*500,200);
  }

  if(botao_on2 == LOW){
    for(int i=0; i<=contador-1;i++){
      for(int j=0; j<4; j++){
        digitalWrite(led[j],1&valores[i]>>j);
      }
      tone(buzzer,valores[i]*500,200);
      delay(1000);
      for(int j=0; j<4; j++){
        digitalWrite(led[j],LOW);
      }        
      Serial.println(valores[i]);
      valores[i]=0;
    }
  delay(400);
  }

}