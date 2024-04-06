#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const String ssid = "< SSID >";
const String password = "< PASSWORD >";
const String token = "< TOKEN >"

ESP8266WebServer server(80);

const int led    = 2;
const int sensor = 3;
const int relay  = 0;
const int lockDuration = 14000; //default 14 second lockout

int requestedState = 1; // open=0, closed=1
int currentState = 1; // open=0, closed=1
int lockExpire = 0; 

void setup() {
  // Initialize Serial
  Serial.begin(115200);
     
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  // Connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Setup GPIO pin modes
  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(sensor, INPUT);
  // Initialize GPIO positions
  digitalWrite(led, HIGH);
  digitalWrite(relay, HIGH);

  server.on("/", []() {
    server.send(200, "text/plain", "This is Joe's Garage Door!\nSend a GET request to an endpoint below: \n\n`/<token>/toggle`\t\tToggle door state. Digital 'press' of the garage door button.\n`/<token>/status`\t\tGet JSON status of garage door.\n`/<token>/setState?value=1`\tSet state of garage door. Valid states are the two values, `open=0` and `closed=1`\n");
  });
  
  server.on("/"+token+"/toggle", []() {
    digitalWrite(led, LOW);
    toggleGarageDoorState();
    server.send(200, "text/plain", "Pressed garage door button. Door state will toggle.\n");
    digitalWrite(led, HIGH);
  });

  server.on("/"+token+"/status", []() {
    digitalWrite(led, LOW);
    server.send(200, "text/plain", "{\"currentState\":" + String(currentState) + ",\"requestedState\":" + String(requestedState) +"}\n");
    digitalWrite(led, HIGH);
  });

  server.on("/"+token+"/setState", []() {
    String argValue = server.arg("value");
    requestedState = argValue.toInt();
    if (requestedState != currentState) {
      if (millis() >= lockExpire) {
        toggleGarageDoorState();
        lockExpire = millis() + lockDuration;
      }
    }
          server.send(200, "text/plain", "{\"currentState\":" + String(currentState) + ",\"requestedState\":" + String(requestedState) +"}\n");
          server.send(200, "text/plain", "{\"currentState\":" + String(currentState) + ",\"requestedState\":" + String(requestedState) +"}\n");
      }
    }
    server.send(200, "text/plain", "{\"currentState\":" + String(currentState) + ",\"requestedState\":" + String(requestedState) +"}\n");
      }
    }
  });

  server.onNotFound([]() {
    digitalWrite(led, 1);
    String message = "Route Not Found\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  });

  server.begin();
  Serial.println("HTTP server started");

}

void toggleGarageDoorState() {
    digitalWrite(led, LOW);
    digitalWrite(relay, HIGH);
    delay(100);
    digitalWrite(relay, LOW);
    delay(100);
    digitalWrite(relay, HIGH);
    digitalWrite(led, HIGH);
}
 
void loop() { 
  currentState = digitalRead(sensor) ^ 1; // The XOR operator inverts the value read by the sensor.
  server.handleClient();
  MDNS.update();
  delay(2); //allow the cpu to switch to other tasks
}
