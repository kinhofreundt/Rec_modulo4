//Bibliotecas
#include <WiFi.h>

//Diretivas de pinagem
#define LedVm 15
#define LedVd 16
#define LedAz 17
#define LedAm 18
#define buzzer 1
#define buttonVm 4
#define buttonVd 5
#define buttonAz 6
#define buttonAm 7

//Variáveis globais
int sequencia[32] = {};
int botoes[4] = { buttonVm, buttonVd, buttonAz, buttonAm };
int leds[4] = { LedVm, LedVd, LedAz, LedAm };
int sons[4] = { 262, 294, 330, 349 };
int rodada = 0;
int passo = 0;
int botao_pressionado = 0;
bool game_over = false;
bool iniciar_jogo = false;
int pontuacao = 0;
const char* ssid = "Inteli-COLLEGE";
const char* password = "QazWsx@123";
WiFiServer server(80);

void setup() {
  // Leds
  pinMode(LedVm, OUTPUT);
  pinMode(LedVd, OUTPUT);
  pinMode(LedAz, OUTPUT);
  pinMode(LedAm, OUTPUT);
  //Buzzer
  pinMode(buzzer, OUTPUT);
  //Botões
  pinMode(buttonVm, INPUT);
  pinMode(buttonVd, INPUT);
  pinMode(buttonAz, INPUT);
  pinMode(buttonAm, INPUT);
  Serial.begin(115200);
  //Conexão com o server
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  if (iniciar_jogo == true) {
    proximaRodada();
    cliente();
    pontuacao = rodada;
    Serial.print(pontuacao);
    reproduzirSequencia();
    aguardarJogador();
    //Estrutura para reiniciar as variaveis
    if (game_over == true) {

      for (int i = 0; i < 32; i++) { sequencia[i] = 0; };
      rodada = 0;
      passo = 0;
      game_over = false;
      iniciar_jogo = false;
    }
    delay(1000);
  }
  cliente();
}

//Funções
void proximaRodada() {
  // Essa função sorteia um valor entre 0 e 3 e
  //prenche o vetor sequencia com esse valor
  int sorteio = random(4);
  sequencia[rodada] = sorteio;
  rodada = rodada + 1;
}
void reproduzirSequencia() {
  //Essa função reproduz a sequencia que foi atribuida
  //ao vetor na função anterior e correlaciona os valores com
  //os leds e sons a partir dos idices dos vetores
  for (int i; i < rodada; i++) {
    tone(buzzer, sons[sequencia[i]]);
    digitalWrite(leds[sequencia[i]], HIGH);
    delay(500);
    noTone(buzzer);
    digitalWrite(leds[sequencia[i]], LOW);
    delay(100);
  }
}
void aguardarJogador() {
  //Função que verifica e aguarda a jogada
  // da interação
  for (int i = 0; i < rodada; i++) {
    bool jogada_efetuada = false;
    while (!jogada_efetuada) {
      for (int j = 0; j <= 3; j++) {
        if (digitalRead(botoes[j]) == HIGH) {
          botao_pressionado = j;
          tone(buzzer, sons[j]);
          digitalWrite(leds[j], HIGH);
          delay(300);
          digitalWrite(leds[j], LOW);
          noTone(buzzer);
          jogada_efetuada = true;
        }
      }
    }
    //Verifica a jogada
    if (sequencia[passo] != botao_pressionado) {
      //Efeito de game over
      for (int i = 0; i <= 3; i++) {
        tone(buzzer, 70);
        digitalWrite(leds[i], HIGH);
        delay(100);
        noTone(buzzer);
        digitalWrite(leds[i], LOW);
      }
      game_over = true;
      break;
    }
    passo = passo + 1;
  }
  passo = 0;
}
void cliente() {
  //Código simple serve arduino example
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //Conteudo html:
            client.print("<head><meta charset=utf-8>");
            client.print("<meta http-equiv=\"refresh\" content=\"1, http://10.128.65.191\">");
            client.print("</head>");
            client.print("<body>");
            client.print("Click <a href=\"/H\">here</a> Para iniciar o jogo.<br>");
            client.print("Pontuação:");
            client.print(pontuacao);

            client.print("</body>");
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Requisição para iniciar o jogo
        if (currentLine.endsWith("GET /H")) {
          iniciar_jogo = true;
        }
      }
    }
    // Fecha a conexão
    client.stop();
    Serial.println("Client Disconnected.");
  }
}