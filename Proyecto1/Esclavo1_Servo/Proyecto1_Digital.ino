#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <HardwareSerial.h>

// Configuración de la red WiFi
#define WIFI_SSID "iPhone Ever (2)"
#define WIFI_PASS "internet12"

// Configuración de Adafruit IO
#define IO_USERNAME "everyes"
#define IO_KEY "aio_LSUJ98fgXfbnoWd2nNjXo0Z8fl1E"

// Configuración del feed para recibir datos
String feedURL = "https://io.adafruit.com/api/v2/everyes/feeds/control/data?x-aio-key=" + String(IO_KEY);

// Configuración de los feeds para enviar datos
#define FEED_HORA "Hora"
#define FEED_PUERTA "Puerta"

// Pines para UART
#define RXD2 16  // Pin RX2 del ESP32
#define TXD2 17  // Pin TX2 del ESP32

// Instancias de WiFi y cliente MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, IO_USERNAME, IO_KEY);

// Feeds en Adafruit IO para enviar datos
Adafruit_MQTT_Publish Hora = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" FEED_HORA);
Adafruit_MQTT_Publish Puerta = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" FEED_PUERTA);

// Variable para almacenar el último valor enviado
String lastValueSent = "";

void setup() {
  Serial.begin(115200);

  // Inicializa Serial2 para UART
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);

  Serial.println("ESP32 listo para recibir y enviar datos.");

  // Conectar a la red WiFi
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Conectar a Adafruit IO
  connectToMQTT();
}

void loop() {
  // Asegurarse de que está conectado a Adafruit IO
  if (mqtt.connected()) {
    mqtt.processPackets(10000);  // Procesar paquetes MQTT
    if (!mqtt.ping()) {
      mqtt.disconnect();
    }
  } else {
    connectToMQTT();
  }

  // Recepción de datos desde Serial2 y envío a Adafruit IO
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');  // Leer hasta encontrar un '\n'
    data.trim(); // Elimina espacios en blanco al principio y al final
    Serial.print("Recibido: ");
    Serial.println(data);  // Mostrar el dato recibido en el monitor serial

    // Procesar los datos recibidos
    if (data.startsWith("Hora:")) {
      String hora = data.substring(5);
      Serial.print("Hora recibida: ");
      Serial.println(hora);  // Mostrar la hora

      // Publicar la hora en Adafruit IO
      if (!Hora.publish(hora.c_str())) {
        Serial.println("Fallo al enviar la Hora a Adafruit IO");
      } else {
        Serial.println("Hora enviada a Adafruit IO");
      }
    } else if (data.startsWith("Puerta:")) {
      String puerta = data.substring(7);
      Serial.print("Estado de la puerta: ");
      Serial.println(puerta);  // Mostrar el estado de la puerta

      // Publicar el estado de la puerta en Adafruit IO con reintento
      int estadoPuerta = (puerta == "open") ? 1 : 0;
      bool success = false;
      for (int attempts = 0; attempts < 3 && !success; attempts++) {
        if (!Puerta.publish((uint32_t)estadoPuerta)) {  // Usar el tipo correcto
          Serial.println("Fallo al enviar el estado de la Puerta a Adafruit IO, reintentando...");
          delay(100); // Retraso antes de reintentar
        } else {
          Serial.println("Estado de la Puerta enviado a Adafruit IO");
          success = true;
        }
      }

      if (!success) {
        Serial.println("Error: No se pudo enviar el estado de la Puerta después de varios intentos.");
      }
    } else {
      Serial.println("Dato desconocido recibido.");
    }
  }

  // Recepción de datos desde Adafruit IO
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(feedURL); // Comienza la solicitud

    int httpCode = http.GET(); // Realiza la solicitud GET

    if (httpCode > 0) {
      Serial.print("Código de respuesta HTTP: ");
      Serial.println(httpCode);

      // Chequear si la solicitud fue exitosa
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString(); // Obtener la respuesta en formato JSON
        Serial.println("Procesando datos...");

        // Crear un buffer para almacenar el JSON
        const size_t capacity = JSON_ARRAY_SIZE(20) + JSON_OBJECT_SIZE(3) + 512;
        DynamicJsonDocument doc(capacity);

        // Parsear el JSON
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          // Obtener el último objeto de la lista
          JsonObject lastEntry = doc.as<JsonArray>()[0];

          // Extraer el valor del campo "value"
          String value = lastEntry["value"];
          
          // Mostrar solo el valor
          Serial.print("Último valor: ");
          Serial.println(value);

          // Solo enviar el valor si ha cambiado
          if (value != lastValueSent) {
            Serial2.print(value);
            Serial2.print("\n"); // Enviar una nueva línea para terminar el envío
            lastValueSent = value; // Actualizar el último valor enviado
            Serial.println("Valor enviado al maestro porque cambió.");
          } else {
            Serial.println("Valor no ha cambiado, no se envía al maestro.");
          }

        } else {
          Serial.print("Error al parsear JSON: ");
          Serial.println(error.c_str());
        }
      } else {
        Serial.println("Respuesta no fue HTTP_CODE_OK");
      }
    } else {
      Serial.print("Error en la solicitud, código: ");
      Serial.println(httpCode);
    }

    http.end(); // Finalizar la conexión
  } else {
    Serial.println("No conectado a WiFi");
  }

  delay(10000); // Esperar 10 segundos antes de realizar la siguiente solicitud o procesamiento
}

void connectToMQTT() {
  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Reconectando en 5 segundos...");
    mqtt.disconnect();
    delay(5000);  // Esperar 5 segundos antes de intentar reconectar
  }

  Serial.println("Conectado a Adafruit IO!");
}


