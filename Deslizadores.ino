#include <WiFiNINA.h>

#define PIN_STEP_ROT1_DIR 10
#define PIN_STEP_ROT1_STP 8
#define PIN_STEP_ROT2_DIR 9
#define PIN_STEP_ROT2_STP 7

const int maxSteps = 3200; // Máximo número de pasos para rotación y elevación
const int stepDelay = 300; // Ajuste de velocidad del motor en microsegundos

int currentPositionRot1 = 0; // Posición actual del motor de rotación1 (1 dedo)
int currentPositionRot2 = 0; // Posición actual del motor de rotación2 (2 dedos)

//NUEVO
int lastCommandRot1 = 0; // Último valor introducido para motor de rotación
int lastCommandRot2 = 0; // Último valor introducido para motor de elevación

// Configuración WiFi
const char* ssid = "INORDINE";           // Nombre de tu red WiFi
const char* password = "TrueFuturePatterns_@285"; // Contraseña de tu red WiFi

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_STEP_ROT1_STP, OUTPUT);
  pinMode(PIN_STEP_ROT1_DIR, OUTPUT);
  pinMode(PIN_STEP_ROT2_STP, OUTPUT);
  pinMode(PIN_STEP_ROT2_DIR, OUTPUT);

  // Conexión a la red WiFi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Espera por clientes
  if (client) {
    Serial.println("Cliente conectado");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Ignora peticiones de favicon
    if (request.indexOf("/favicon.ico") != -1) {
      client.stop();
      Serial.println("Petión de favicon ignorada");
      return;
    }

    // Analiza la petición GET para extraer los valores de los deslizadores
    if (request.indexOf("/action?") != -1) {
      int rot1Start = request.indexOf("rot=") + 4;
      int rot1End = request.indexOf("&", rot1Start);
      int rot2Start = request.indexOf("elv=") + 4;
      int rot2End = request.indexOf(" ", rot2Start);
      if (rot2End == -1) {
        rot2End = request.length();
      }

      String rot1Str = request.substring(rot1Start, rot1End);
      String rot2Str = request.substring(rot2Start, rot2End);
      int rot1Value = rot1Str.toInt();
      int rot2Value = rot2Str.toInt();

      if ((rot1Value < 0 || rot1Value > 100) || (rot2Value < 0 || rot2Value > 100)) {
        Serial.println("Valores fuera de rango (0-100). Ignorando comando.");
        client.stop();
        return;
      }

      Serial.print("Valor 1 dedo recibido: ");
      Serial.println(rot1Value);
      Serial.print("Valor 2 dedos recibido: ");
      Serial.println(rot2Value);

      moverMotores(rot1Value, rot2Value);
    }

    // Genera y envía el HTML con dos deslizadores y un botón de enviar
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<title>Control del Gripper</title>");  // Aquí se agrega el título de la página
    client.println("<head><style>");
    client.println("body { text-align: center; padding-top: 40px; font-family: Georgia, serif; background-color: #f0f8ff; color: #333; }");
    client.println(".historial { background-color: #e6f2ff; padding: 10px; border-radius: 5px; margin-top: 50px; box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1); }");
    client.println("input[type='range'] { width: 300px; appearance: none; height: 10px; border-radius: 5px; background: #ddd; outline: none; transition: background 0.3s ease-in-out; }");
    client.println(".slider-container { display: inline-block; margin: 30px; }");
    client.println(".section-spacing { margin-bottom: 60px; }");
    client.println(".icon { margin-right: 8px; vertical-align: middle; width: 30px; height: 30px; }");
    client.println(".submit-button { font-family: Georgia, serif; font-size: 1.2rem; padding: 10px 20px; border: none; border-radius: 5px; background-color: #4682b4; color: white; cursor: pointer; margin-top: 20px; }");
    client.println(".submit-button:hover { background-color: #315f86; }");
    client.println(".spacer { margin-bottom: 60px; }");
    client.println("</style></head>");
    client.println("<body>");
    client.println("<h1>Control de Actuadores</h1>");
    client.println("<h3>Seleccione las acciones y ajuste los deslizadores</h3>");
    client.println("<form action='/action'>");

    client.println("  <div class='slider-container section-spacing'>");
    client.println("    <h3><img src='https://img.icons8.com/ios-glyphs/30/4682b4/gear.png' class='icon'>Apertura/Cierre del Dedo Unico</h3>");
    client.println("    <label for='rot'>Rango de apertura (0-100):</label><br>");
    client.println("    <input type='range' id='rot' name='rot' min='0' max='100' oninput=\"actualizarValor('rotValue', this.value); cambiarColor('rot', this.value)\">");
    client.println("    <span id='rotValue'>50</span>");
    client.println("  </div>");

    client.println("  <div class='slider-container section-spacing'>");
    client.println("    <h3><img src='https://img.icons8.com/ios-glyphs/30/4682b4/gear.png' class='icon'>Apertura/Cierre de los Dedos Conjuntos</h3>");
    client.println("    <label for='elv'>Rango de apertura (0-100):</label><br>");
    client.println("    <input type='range' id='elv' name='elv' min='0' max='100' oninput=\"actualizarValor('elvValue', this.value); cambiarColor('elv', this.value)\">");
    client.println("    <span id='elvValue'>50</span>");
    client.println("  </div>");

    client.println("  <br><br><input type='submit' value='Ejecutar cambios' class='submit-button'>");
    client.println("  <div class='spacer'></div>");
    client.println("</form>");

    // Historial en HTML
    client.println("<div class='historial'>");
    client.println("<h2><img src='https://icones.pro/wp-content/uploads/2022/03/historique-icone-de-l-historique-noir.png' class='icon'>Historial de Comandos</h2>");
    client.println("<p>Ultima accion del dedo unico: " + String(lastCommandRot1) + "</p>");
    client.println("<p>Ultima accion de los dedos conjuntos: " + String(lastCommandRot2) + "</p>");
    client.println("</div>");

    client.println("<script>");
    client.println("function actualizarValor(id, valor) {");
    client.println("  document.getElementById(id).innerText = valor;");
    client.println("}");

    client.println("function cambiarColor(id, valor) {");
    client.println("  const elemento = document.getElementById(id);");
    client.println("  if (valor < 33) {");
    client.println("    elemento.style.background = '#87CEFA';");
    client.println("  } else if (valor < 66) {");
    client.println("    elemento.style.background = '#DAA520';");
    client.println("  } else {");
    client.println("    elemento.style.background = '#6B8E23';");
    client.println("  }");
    client.println("}");
    client.println("</script>");

    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void moverMotores(int rot1Value, int rot2Value) {
  int stepsRot1 = map(rot1Value, 0, 100, 0, maxSteps);
  int stepsRot2 = map(rot2Value, 0, 100, 0, maxSteps);

  int stepsToMoveRot1 = stepsRot1 - currentPositionRot1;
  int stepsToMoveRot2 = stepsRot2 - currentPositionRot2;

  int maxSteps = max(abs(stepsToMoveRot1), abs(stepsToMoveRot2));

  for (int i = 0; i < maxSteps; i++) {
    if (i < abs(stepsToMoveRot1)) {
      digitalWrite(PIN_STEP_ROT1_DIR, (stepsToMoveRot1 > 0) ? HIGH : LOW);
      digitalWrite(PIN_STEP_ROT1_STP, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(PIN_STEP_ROT1_STP, LOW);
      delayMicroseconds(stepDelay);
    }
    if (i < abs(stepsToMoveRot2)) {
      digitalWrite(PIN_STEP_ROT2_DIR, (stepsToMoveRot2 > 0) ? HIGH : LOW);
      digitalWrite(PIN_STEP_ROT2_STP, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(PIN_STEP_ROT2_STP, LOW);
      delayMicroseconds(stepDelay);
    }
  }

  currentPositionRot1 = stepsRot1;
  currentPositionRot2 = stepsRot2;

  // Actualizar los últimos comandos
  lastCommandRot1 = rot1Value;
  lastCommandRot2 = rot2Value;
}
