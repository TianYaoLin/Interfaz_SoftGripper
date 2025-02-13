#include <WiFiNINA.h>

#define PIN_STEP_ROT1_DIR 10
#define PIN_STEP_ROT1_STP 8
#define PIN_STEP_ROT2_DIR 9
#define PIN_STEP_ROT2_STP 7

const int maxSteps = 3200; // Maximo numero de pasos para rotacion y elevacion
const int stepDelay = 300; // Ajuste de velocidad del motor en microsegundos

int currentPosition = 0; // Posicion actual del motor
int lastCommand = 0; // Ultimo valor introducido

// Configuracion WiFi
const char* ssid = "INORDINE";
const char* password = "TrueFuturePatterns_@285";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_STEP_ROT1_STP, OUTPUT);
  pinMode(PIN_STEP_ROT1_DIR, OUTPUT);
  pinMode(PIN_STEP_ROT2_STP, OUTPUT);
  pinMode(PIN_STEP_ROT2_DIR, OUTPUT);
  
  Serial.print("Conectando a ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    if (request.indexOf("/action?") != -1) {
      int valueStart = request.indexOf("value=") + 6;
      int valueEnd = request.indexOf(" ", valueStart);
      if (valueEnd == -1) {
        valueEnd = request.length();
      }
      String valueStr = request.substring(valueStart, valueEnd);
      int value = valueStr.toInt();
      
      if (value == 0 || value == 50 || value == 75) {
        Serial.print("Valor recibido: ");
        if (value == 75) {
          Serial.println("Interpretado como 75% pero mostrado como 100%");
        } else {
          Serial.println(value);
        }
        moverMotores(value);
      } else {
        Serial.println("Valor fuera de rango. Ignorando comando.");
      }
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><title>Control del Gripper</title>");
    client.println("<style>");
    client.println("body { text-align: center; padding-top: 40px; font-family: Georgia, serif; background-color: #f0f8ff; color: #333; }");
    client.println(".button-container { margin-top: 30px; }");
    client.println("button { font-family: Georgia, serif; font-size: 1.2rem; padding: 10px 20px; border: none; border-radius: 5px; background-color: #4682b4; color: white; cursor: pointer; margin: 5px; }");
    client.println("button:hover { background-color: #315f86; }");
    client.println(".historial { background-color: #e6f2ff; padding: 10px; border-radius: 5px; margin-top: 50px; box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1); }");
    client.println(".icon { margin-right: 8px; vertical-align: middle; width: 30px; height: 30px; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>Control de Actuadores</h1>");
    client.println("<h3>Seleccione la posicion deseada</h3>");
    client.println("<div class='button-container'>");
    client.println("<form action='/action'>");
    client.println("<button type='submit' name='value' value='0'>0%</button>");
    client.println("<button type='submit' name='value' value='50'>50%</button>");
    client.println("<button type='submit' name='value' value='75'>100%</button>");
    client.println("</form>");
    client.println("</div>");

    // Historial en HTML
    client.println("<div class='historial'>");
    client.println("<h2><img src='https://icones.pro/wp-content/uploads/2022/03/historique-icone-de-l-historique-noir.png' class='icon'>Historial de Comandos</h2>");
    client.println("<p>Ultima posicion establecida: " + String(lastCommand == 75 ? "100%" : String(lastCommand) + "%") + "</p>");
    client.println("</div>");

    client.println("</body>");
    client.println("</html>");
    
    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void moverMotores(int value) {
  int steps = map(value, 0, 100, 0, maxSteps);
  int stepsToMove = steps - currentPosition;
  
  for (int i = 0; i < abs(stepsToMove); i++) {
    digitalWrite(PIN_STEP_ROT1_DIR, (stepsToMove > 0) ? HIGH : LOW);
    digitalWrite(PIN_STEP_ROT1_STP, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(PIN_STEP_ROT1_STP, LOW);
    delayMicroseconds(stepDelay);
    
    digitalWrite(PIN_STEP_ROT2_DIR, (stepsToMove > 0) ? HIGH : LOW);
    digitalWrite(PIN_STEP_ROT2_STP, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(PIN_STEP_ROT2_STP, LOW);
    delayMicroseconds(stepDelay);
  }
  
  currentPosition = steps;
  lastCommand = value;
}
