#include <LiquidCrystal.h>

const int analogPin = A0;  // Pin analógico utilizado
const int resolution = 256; // 8 bits de resolución (0-255)
const float referenceVoltage = 1; // Voltaje de referencia del Arduino (en V)
const int sampleRate = 5000; // Tasa de muestreo en muestras por segundo

// Inicializa la biblioteca LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Inicializa el LCD con 16 columnas y 2 filas
  lcd.begin(16, 2);

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

    // Limpiar la pantalla LCD
    lcd.clear();

    // Imprimir el voltaje en la primera fila y segunda columna
    lcd.setCursor(0, 1);
    lcd.print("Voltage: ");
    lcd.print(voltage, 4);

  }
}
