#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define IN_TOPIC   "FANSINMQTT"
#define OUT_TOPIC  "FANSOUTMQTT"

const char* ssid = "SII-Cikunir";
const char* password = "admin.admin";
const char* mqtt_server = "104.248.156.51";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long LastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  // put your setup code here, to run once:
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];

  }
  Serial.println("data");
  Serial.println(msg);
  if ((char)payload[0] == '0') {
    digitalWrite(BUILTIN_LED, HIGH);
    digitalWrite(4, LOW);
  } else {
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(4, HIGH);
  }
}


void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT Connection...");
    String clientId = "node-redxx1";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), "ali", "1234")) {
      Serial.println("connected");
      client.publish("outTopic", "Hello World");
      client.subscribe("inTopic");
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected ()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - LastMsg > 2000) {
    LastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "Hello World #%ld", value);
    Serial.print("Publish Message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
void charToString(char S[], String &D)
{
  String rc(S);
  D = rc;
}
