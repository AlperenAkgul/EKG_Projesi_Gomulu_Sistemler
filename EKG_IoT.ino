#include <WiFi.h>
#include <PubSubClient.h>
 
#define WIFISSID "Redmi" 
#define PASSWORD "123456789" 
#define TOKEN "BBUS-UlJ9JPoBQsmfbF8kGEww6bgWau81hY" 
#define MQTT_CLIENT_NAME "gomulusis" 

#define VARIABLE_LABEL "sensor" 
#define DEVICE_LABEL "esp32" 
 
#define SENSOR A0 
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char str_sensor[10];
 

WiFiClient ubidots;
PubSubClient client(ubidots);
 
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect() {
 
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      
      delay(2000);
    }
  }
}
 

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
 
  pinMode(SENSOR, INPUT);
 
  Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); 
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); 
  
  float sensor = analogRead(SENSOR); 
  

  dtostrf(sensor, 4, 2, str_sensor);//bir kayan nokta (float) değerini karakter dizisine (string) dönüştürmek için kullandık. 4 genişlik, 2 virgülden sonra basamak sayısı. 
  //str_sensor veriyi alır 
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); 
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(500);
}