const int analogPin = A0;  // Pin analógico utilizado
const int resolution = 256; // 8 bits de resolución (0-255)
const float referenceVoltage = 5.0; // Voltaje de referencia del Arduino (en V)
const int sampleRate = 5000; // Tasa de muestreo en muestras por segundo

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT); // Usar la referencia de voltaje por defecto (generalmente 5V)
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long samplingInterval = 1000000 / sampleRate; // Intervalo de tiempo entre muestras en microsegundos

  static unsigned long lastSampleTime = 0;

  if (currentTime - lastSampleTime >= samplingInterval) {
    lastSampleTime = currentTime;

    int rawValue = analogRead(analogPin); // Leer valor analógico
    float voltage = (float)rawValue / resolution * referenceVoltage; // Calcular voltaje

    Serial.println(voltage, 4); // Imprimir voltaje con 4 decimales
  }
}
