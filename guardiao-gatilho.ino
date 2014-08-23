#include <SPI.h>
#include <Ethernet.h>
 
 
#define SENSORPIN 7 //porta conectada o sensor
static char apiKey[] = "xxxxxxxx-yyyy-zzzz-wwww-kkkkkkkkkkkk"; //@todo Alterar para a API Key do Projeto.
static char serial[] = "GUC0003"; //@todo Alterar para a Serial do Dispositivo.
 
unsigned long frequencyTime = 1000; //Definicao do tempo que devera ser lido o estado do atuador.
static byte mac[] = { 
  0xDE, 0xAD, 0x09, 0xE0, 0xFE, 0xED };
char serverName[] = "guardiao.cl";
/**
 * //Caso queira definir o IP manualmente, descomente.
 * IPAddress ip(10,10,1,10);
 * IPAddress dnsserver(10,10,1,1);
 * IPAddress gateway(10,10,1,1);
 * IPAddress subnet(255,255,255,0);
 **/
 
EthernetClient client;
 
void setup(){
  Serial.begin(9600);
  pinMode(SENSORPIN,INPUT);
  Ethernet.begin(mac); //IP Automático
  //Ethernet.begin(mac, ip, dnsserver, gateway, subnet); //IP Manual
  Serial.print(F("IP Atual: "));
  Serial.println(Ethernet.localIP());
}
 
void loop(){
  int stateSensor = digitalRead(SENSORPIN);
  if(stateSensor==1){
    Serial.println("Presenca Detectada");
    activateState();
  }
  else{
    Serial.println("Sem Presenca Detectada");
    desactivateState(); //Caso deseje, comente essa linha para que apenas seja ativado em caso de presença
  }
 
  delay(frequencyTime); //Tempo de esperar para nova leitura.
}
 
void activateState(){
  char outBuf[128];
  char host[64];
  if(client.connect(serverName, 80)) {
    sprintf(outBuf,"GET /trigger/%s/on/?apiKey=%s HTTP/1.1",serial,apiKey);
    client.println(outBuf);
    sprintf(host,"Host: %s",serverName);
    client.println(host);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    client.stop();
  } 
  else {
    Serial.println("Falha de conexao");
  }
}
 
void desactivateState(){
  char outBuf[128];
  char host[64];
  if(client.connect(serverName, 80)) {
    sprintf(outBuf,"GET /trigger/%s/off/?apiKey=%s HTTP/1.1",serial,apiKey);
    client.println(outBuf);
    sprintf(host,"Host: %s",serverName);
    client.println(host);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    client.stop();
  } 
  else {
    Serial.println("Falha de conexao");
  }
}