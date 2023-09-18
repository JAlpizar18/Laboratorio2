#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // Inicializa el objeto LCD

const int analogPinPos = A1; // Pin analógico donde está conectado el punto positivo del puente
const int analogPinNeg = A2; // Pin analógico donde está conectado el punto negativo del puente
const int termo1 = A4;
const int termo2 = A5;
  
const int numSamples = 2000; // Número de muestras para promediar en el nuevo pin
const int maxTemperatureDifference = 1000; // Umbral para filtrar valores atípicos

const float referenceVoltage1 = 1.35; // Referencia de voltaje en voltios (0.82V en este caso)
const float referenceVoltage2 = 1.46;


const float temperatureIncrement = 0.008; // 5 mV por grado Celsius

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial
  lcd.begin(16, 2);   // Inicializa el LCD de 16x2
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
}

void loop() {
  static float referenceTempC = -999.0; // Valor de referencia inicial
  float voltageSum = 0.0;
  float termo1VoltageSum = 0.0;
  float termo2VoltageSum = 0.0;

  // Realiza un promedio de varias muestras en el primer pin
  for (int i = 0; i < numSamples; i++) {
    int rawValuePos = analogRead(analogPinPos);
    int rawValueNeg = analogRead(analogPinNeg);
    float voltageDiff = ((float)rawValuePos - (float)rawValueNeg) * (5.0 / 1023.0);
    voltageSum += voltageDiff;

    float termo1RawValue = analogRead(termo1);
    float termo2RawValue = analogRead(termo2);

    float termo1Voltage = (termo1RawValue / 1023.0) * 5;
    float termo2Voltage = (termo2RawValue / 1023.0) * 5;


    termo1VoltageSum += termo1Voltage;
    termo2VoltageSum += termo2Voltage;

    delayMicroseconds(500);  // Espera para muestrear a 2000 Hz
  }

  float voltageAvg = voltageSum / numSamples;
  float resistance = (voltageAvg * 2000.0) / (10.0 - voltageAvg);
  float inv_T = 1.0 / 296.15 + (1.0 / 3950.0) * log(resistance / 2000.0);
  float tempK = 1.0 / inv_T;
  int tempC = int(tempK - 273.15);

  float termo1VoltageAvg = termo1VoltageSum / numSamples;
  float termo2VoltageAvg = termo2VoltageSum / numSamples;


  int Temperature1 = int((termo1VoltageAvg - referenceVoltage1) / temperatureIncrement);
  int Temperature2 = int((termo2VoltageAvg - referenceVoltage2) / temperatureIncrement);
  
  int temp_final_1 = tempC + Temperature1;
  int temp_final_2 = tempC + Temperature2;

  // Si no se ha capturado la referencia, hazlo ahora
  if (referenceTempC == -999.0) {
    referenceTempC = tempC;
  } else {
    // Filtra valores atípicos en función del umbral
    if ((abs(temp_final_1 - referenceTempC) <= maxTemperatureDifference) & (abs(temp_final_2 - referenceTempC) <= maxTemperatureDifference)) {
      // Muestra la temperatura en grados Celsius como un número entero en el LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp 1:");
      lcd.print(temp_final_1);
      lcd.print("C");

      // Muestra la temperatura del optoacoplador en el LCD
      lcd.setCursor(0, 1);
      lcd.print("Temp 2:");
      lcd.print(temp_final_2);
      lcd.print("C");

      // Muestra la temperatura final en el Monitor Serie
      Serial.print("Temperatura Final 1 (°C): ");
      Serial.println(temp_final_1);

      Serial.print("Temperatura Final 2 (°C): ");
      Serial.println(temp_final_2);

      Serial.print("termo1voltageAvg: ");
      Serial.println(termo1VoltageAvg, 2);

      Serial.print("termo2voltageAvg: ");
      Serial.println(termo2VoltageAvg, 2);

      
      Serial.print("interna (°C): ");
      Serial.println(tempC); // Imprime un decimal
    }
  }
}

