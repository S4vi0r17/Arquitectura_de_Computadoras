void enviarImagen() {
  HTTPClient http;

  WiFiClient client;
  if (!client.connect(serverAddress, 4000)) {
    Serial.println("Error de conexión");
    return;
  }

  // Leer la imagen del sistema de archivos (ejemplo)
  File file = SPIFFS.open("/imagen.jpg", "r");
  if (!file) {
    Serial.println("Error al abrir el archivo de imagen");
    return;
  }

  // Enviar una solicitud HTTP POST con la imagen como carga útil
  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/json");

  // Codificar la imagen a base64
  size_t fileSize = file.size();
  char *buffer = (char *)malloc(fileSize);
  if (buffer == NULL) {
    Serial.println("Error de memoria");
    return;
  }
  file.readBytes(buffer, fileSize);
  file.close();
  String encodedImage = base64::encode(buffer, fileSize);

  // Crear el JSON con la imagen codificada
  String jsonPayload = "{\"imagen\": \"" + encodedImage + "\"}";

  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Respuesta del servidor: " + response);
  } else {
    Serial.println("Error en la solicitud HTTP");
  }

  http.end();
  free(buffer);
}