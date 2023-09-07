#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Configura los pines para el LCD

const int numSamples = 2000;  // 2000 muestras para cada señal
const int analogPin1 = A0;    // Pin analógico para la señal 1
const int analogPin2 = A1;    // Pin analógico para la señal 2
const float offsetVoltage = 2.0;  // Offset de 1V

void setup() {
  lcd.begin(16, 2);  // Inicializa el LCD con 16 columnas y 2 filas
  lcd.print("Senoidal 1:");
  lcd.setCursor(0, 1);
  lcd.print("Senoidal 2:");
  delay(1000);
  lcd.clear();
}

void loop() {
  for (int i = 0; i < numSamples; i++) {
    // Lee las señales senoidales de las entradas analógicas
    int rawValue1 = analogRead(analogPin1);
    int rawValue2 = analogRead(analogPin2);

    // Convierte los valores en voltaje
    float voltage1 = (rawValue1 * 5.0 / 1023.0); // Suponiendo una referencia de 5V
    float voltage2 = (rawValue2 * 5.0 / 1023.0);

    // Resta el offset de 1V
    voltage1 -= offsetVoltage;
    voltage2 -= offsetVoltage;

    // Muestra los datos en el LCD
    lcd.setCursor(12, 0);
    lcd.print(voltage1, 4);  // Mostrar 4 decimales
    lcd.setCursor(12, 1);
    lcd.print(voltage2, 4);

    // Espera para mantener la velocidad de muestreo
    delayMicroseconds(500);  // Ajusta según la velocidad deseada
  }
}

