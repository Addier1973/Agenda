/*
 * MEJORAS AVANZADAS PARA INVERSOR SINUSOIDAL PURA
 * Código con funciones adicionales:
 * - Control de voltaje en lazo cerrado
 * - Protección de sobrecorriente
 * - Display LCD con información en tiempo real
 * - Soft-start
 * - Monitoreo de temperatura
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración LCD (dirección I2C 0x27 o 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines PWM (igual que versión básica)
#define PWM_A_HIGH 9
#define PWM_A_LOW 10
#define PWM_B_HIGH 3
#define PWM_B_LOW 11

// Pines de control y sensores
#define ENABLE_PIN 7
#define LED_STATUS 13
#define VOLTAGE_SENSOR A0
#define CURRENT_SENSOR A1
#define TEMP_SENSOR A2
#define BUZZER_PIN 8

// Parámetros SPWM
#define SINE_FREQ 60
#define SAMPLES 256
#define DEAD_TIME 20

// Límites y protecciones
#define MAX_CURRENT 20.0      // Amperios (ajustar según diseño)
#define MAX_TEMP 75.0         // Grados Celsius
#define TARGET_VOLTAGE 120.0  // Voltaje objetivo
#define MIN_INPUT_VOLTAGE 22.0 // Voltaje mínimo batería
#define MAX_INPUT_VOLTAGE 28.0 // Voltaje máximo batería

// Tabla sinusoidal (mismo que versión básica)
const uint8_t sineTable[SAMPLES] PROGMEM = {
  127, 130, 133, 136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173,
  176, 179, 182, 185, 188, 191, 193, 196, 199, 201, 204, 206, 209, 211, 213, 216,
  218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 237, 239, 240, 242, 243, 245,
  246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 254, 254, 253, 252, 252, 251, 250, 249, 248, 247,
  246, 245, 243, 242, 240, 239, 237, 236, 234, 232, 230, 228, 226, 224, 222, 220,
  218, 216, 213, 211, 209, 206, 204, 201, 199, 196, 193, 191, 188, 185, 182, 179,
  176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 139, 136, 133, 130,
  127, 124, 121, 118, 115, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81,
  78, 75, 72, 69, 66, 63, 61, 58, 55, 53, 50, 48, 45, 43, 41, 38,
  36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 17, 15, 14, 12, 11, 9,
  8, 7, 6, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 4, 5, 6, 7,
  8, 9, 11, 12, 14, 15, 17, 18, 20, 22, 24, 26, 28, 30, 32, 34,
  36, 38, 41, 43, 45, 48, 50, 53, 55, 58, 61, 63, 66, 69, 72, 75,
  78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124
};

// Variables globales
volatile uint16_t sampleIndex = 0;
volatile bool inverterEnabled = false;
volatile uint8_t amplitudeScale = 100; // 0-100% para control de voltaje

// Variables de medición
float outputVoltage = 0.0;
float inputCurrent = 0.0;
float temperature = 0.0;
float inputVoltage = 0.0;
float outputPower = 0.0;

// Estado del sistema
enum SystemState {
  STATE_IDLE,
  STATE_STARTING,
  STATE_RUNNING,
  STATE_ERROR,
  STATE_SHUTDOWN
};

SystemState systemState = STATE_IDLE;
String errorMessage = "";

// PID para control de voltaje
struct PIDController {
  float kp = 0.5;
  float ki = 0.1;
  float kd = 0.05;
  float integral = 0.0;
  float lastError = 0.0;
  
  float compute(float error, float dt) {
    integral += error * dt;
    integral = constrain(integral, -100, 100); // Anti-windup
    
    float derivative = (error - lastError) / dt;
    lastError = error;
    
    return kp * error + ki * integral + kd * derivative;
  }
  
  void reset() {
    integral = 0.0;
    lastError = 0.0;
  }
};

PIDController voltagePID;

void setup() {
  Serial.begin(115200);
  Serial.println(F("================================="));
  Serial.println(F("Inversor Avanzado 24V -> 120V"));
  Serial.println(F("Version 2.0 con protecciones"));
  Serial.println(F("================================="));
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inversor 24V-120V");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  
  // Configurar pines
  pinMode(PWM_A_HIGH, OUTPUT);
  pinMode(PWM_A_LOW, OUTPUT);
  pinMode(PWM_B_HIGH, OUTPUT);
  pinMode(PWM_B_LOW, OUTPUT);
  pinMode(ENABLE_PIN, INPUT_PULLUP);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VOLTAGE_SENSOR, INPUT);
  pinMode(CURRENT_SENSOR, INPUT);
  pinMode(TEMP_SENSOR, INPUT);
  
  // Apagar todas las salidas
  digitalWrite(PWM_A_HIGH, LOW);
  digitalWrite(PWM_A_LOW, LOW);
  digitalWrite(PWM_B_HIGH, LOW);
  digitalWrite(PWM_B_LOW, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Configurar timers
  setupTimer1PWM();
  setupTimer2PWM();
  setupSineUpdateTimer();
  
  // Beep de inicio
  tone(BUZZER_PIN, 2000, 200);
  delay(300);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Listo");
  lcd.setCursor(0, 1);
  lcd.print("Pulse Enable");
  
  Serial.println(F("Sistema listo. Presione Enable para iniciar."));
}

void loop() {
  // Actualizar sensores
  readSensors();
  
  // Máquina de estados
  switch (systemState) {
    case STATE_IDLE:
      handleIdleState();
      break;
      
    case STATE_STARTING:
      handleStartingState();
      break;
      
    case STATE_RUNNING:
      handleRunningState();
      break;
      
    case STATE_ERROR:
      handleErrorState();
      break;
      
    case STATE_SHUTDOWN:
      handleShutdownState();
      break;
  }
  
  // Actualizar display
  updateDisplay();
  
  // Actualizar LED de estado
  updateStatusLED();
  
  delay(100); // 10Hz loop rate
}

void handleIdleState() {
  static bool lastEnableState = false;
  bool currentEnableState = !digitalRead(ENABLE_PIN);
  
  if (currentEnableState && !lastEnableState) {
    // Verificar condiciones antes de arrancar
    if (checkPreStartConditions()) {
      systemState = STATE_STARTING;
      Serial.println(F(">>> Iniciando arranque suave..."));
    }
  }
  
  lastEnableState = currentEnableState;
}

void handleStartingState() {
  // Soft-start: incrementar amplitud gradualmente
  static uint8_t softStartAmplitude = 0;
  static unsigned long lastIncrement = 0;
  
  if (millis() - lastIncrement > 50) { // Incrementar cada 50ms
    softStartAmplitude += 2;
    
    if (softStartAmplitude >= 100) {
      softStartAmplitude = 100;
      systemState = STATE_RUNNING;
      Serial.println(F(">>> Sistema en operación normal"));
    }
    
    amplitudeScale = softStartAmplitude;
    inverterEnabled = true;
    lastIncrement = millis();
  }
}

void handleRunningState() {
  // Verificar botón de deshabilitación
  if (digitalRead(ENABLE_PIN)) { // HIGH = deshabilitado (pull-up)
    systemState = STATE_SHUTDOWN;
    return;
  }
  
  // Control de voltaje con PID
  static unsigned long lastPIDUpdate = 0;
  if (millis() - lastPIDUpdate > 100) { // 10Hz PID rate
    float error = TARGET_VOLTAGE - outputVoltage;
    float correction = voltagePID.compute(error, 0.1);
    
    amplitudeScale = constrain(amplitudeScale + correction, 50, 100);
    lastPIDUpdate = millis();
  }
  
  // Verificar protecciones
  if (!checkProtections()) {
    systemState = STATE_ERROR;
  }
  
  // Imprimir estadísticas cada 2 segundos
  static unsigned long lastStats = 0;
  if (millis() - lastStats > 2000) {
    printStatistics();
    lastStats = millis();
  }
}

void handleErrorState() {
  // Apagar inversor inmediatamente
  disableInverter();
  
  // Alarma sonora
  static unsigned long lastBeep = 0;
  if (millis() - lastBeep > 500) {
    tone(BUZZER_PIN, 1000, 100);
    lastBeep = millis();
  }
  
  // Mostrar error en LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR:");
  lcd.setCursor(0, 1);
  lcd.print(errorMessage.substring(0, 16));
  
  Serial.print(F("ERROR: "));
  Serial.println(errorMessage);
  
  // Esperar que se suelte el botón de enable
  if (digitalRead(ENABLE_PIN)) {
    delay(2000); // Esperar 2 segundos
    systemState = STATE_IDLE;
    errorMessage = "";
    voltagePID.reset();
    Serial.println(F("Error reseteado. Sistema en espera."));
  }
}

void handleShutdownState() {
  // Apagado suave
  static uint8_t shutdownAmplitude = amplitudeScale;
  static unsigned long lastDecrement = 0;
  
  if (millis() - lastDecrement > 50) {
    if (shutdownAmplitude > 0) {
      shutdownAmplitude -= 5;
      amplitudeScale = shutdownAmplitude;
    } else {
      disableInverter();
      systemState = STATE_IDLE;
      voltagePID.reset();
      Serial.println(F(">>> Sistema apagado"));
    }
    lastDecrement = millis();
  }
}

bool checkPreStartConditions() {
  // Verificar voltaje de entrada
  if (inputVoltage < MIN_INPUT_VOLTAGE) {
    errorMessage = "Vin bajo";
    systemState = STATE_ERROR;
    return false;
  }
  
  if (inputVoltage > MAX_INPUT_VOLTAGE) {
    errorMessage = "Vin alto";
    systemState = STATE_ERROR;
    return false;
  }
  
  // Verificar temperatura
  if (temperature > MAX_TEMP) {
    errorMessage = "Temp alta";
    systemState = STATE_ERROR;
    return false;
  }
  
  return true;
}

bool checkProtections() {
  // Protección de sobrecorriente
  if (inputCurrent > MAX_CURRENT) {
    errorMessage = "Sobrecorriente";
    return false;
  }
  
  // Protección térmica
  if (temperature > MAX_TEMP) {
    errorMessage = "Sobretemperatura";
    return false;
  }
  
  // Protección de bajo voltaje
  if (inputVoltage < MIN_INPUT_VOLTAGE) {
    errorMessage = "Bateria baja";
    return false;
  }
  
  return true;
}

void readSensors() {
  // Leer voltaje de entrada (divisor resistivo 10:1)
  int adcValue = analogRead(VOLTAGE_SENSOR);
  inputVoltage = (adcValue / 1023.0) * 5.0 * 10.0; // Ajustar factor según divisor
  
  // Leer corriente (ACS712-20A: 2.5V=0A, 100mV/A)
  adcValue = analogRead(CURRENT_SENSOR);
  float voltage = (adcValue / 1023.0) * 5.0;
  inputCurrent = abs((voltage - 2.5) / 0.1); // 100mV/A = 0.1V/A
  
  // Leer temperatura (NTC con divisor resistivo)
  adcValue = analogRead(TEMP_SENSOR);
  // Conversión simplificada (ajustar según NTC específico)
  temperature = map(adcValue, 0, 1023, 0, 100);
  
  // Calcular voltaje de salida (simulado, requiere sensor real)
  // En implementación real, usar transformador de voltaje o divisor aislado
  outputVoltage = TARGET_VOLTAGE; // Placeholder
  
  // Calcular potencia de salida
  outputPower = inputVoltage * inputCurrent * 0.90; // Asumiendo 90% eficiencia
}

void printStatistics() {
  Serial.println(F("─────────────────────────────────"));
  Serial.print(F("Voltaje entrada: ")); Serial.print(inputVoltage); Serial.println(F("V"));
  Serial.print(F("Corriente entrada: ")); Serial.print(inputCurrent); Serial.println(F("A"));
  Serial.print(F("Potencia salida: ")); Serial.print(outputPower); Serial.println(F("W"));
  Serial.print(F("Temperatura: ")); Serial.print(temperature); Serial.println(F("°C"));
  Serial.print(F("Amplitud: ")); Serial.print(amplitudeScale); Serial.println(F("%"));
  Serial.println(F("─────────────────────────────────"));
}

void updateDisplay() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < 500) return; // Actualizar cada 500ms
  lastUpdate = millis();
  
  lcd.clear();
  
  switch (systemState) {
    case STATE_IDLE:
      lcd.setCursor(0, 0);
      lcd.print("Vin:");
      lcd.print(inputVoltage, 1);
      lcd.print("V ");
      lcd.print(temperature, 0);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Presione Enable");
      break;
      
    case STATE_STARTING:
      lcd.setCursor(0, 0);
      lcd.print("Arrancando...");
      lcd.setCursor(0, 1);
      lcd.print("Amp: ");
      lcd.print(amplitudeScale);
      lcd.print("%");
      break;
      
    case STATE_RUNNING:
      lcd.setCursor(0, 0);
      lcd.print("OUT:");
      lcd.print(outputVoltage, 0);
      lcd.print("V ");
      lcd.print(outputPower, 0);
      lcd.print("W");
      lcd.setCursor(0, 1);
      lcd.print("IN:");
      lcd.print(inputCurrent, 1);
      lcd.print("A ");
      lcd.print(temperature, 0);
      lcd.print("C");
      break;
      
    case STATE_ERROR:
      // Manejado en handleErrorState
      break;
      
    case STATE_SHUTDOWN:
      lcd.setCursor(0, 0);
      lcd.print("Apagando...");
      lcd.setCursor(0, 1);
      lcd.print("Amp: ");
      lcd.print(amplitudeScale);
      lcd.print("%");
      break;
  }
}

void updateStatusLED() {
  switch (systemState) {
    case STATE_IDLE:
      digitalWrite(LED_STATUS, LOW);
      break;
      
    case STATE_STARTING:
    case STATE_SHUTDOWN:
      // Parpadeo rápido
      digitalWrite(LED_STATUS, (millis() / 100) % 2);
      break;
      
    case STATE_RUNNING:
      // Parpadeo lento
      digitalWrite(LED_STATUS, (millis() / 500) % 2);
      break;
      
    case STATE_ERROR:
      // Parpadeo muy rápido
      digitalWrite(LED_STATUS, (millis() / 50) % 2);
      break;
  }
}

// ═══════════════════════════════════════════════════════════════════
// FUNCIONES DE CONFIGURACIÓN DE TIMERS (Igual que versión básica)
// ═══════════════════════════════════════════════════════════════════

void setupTimer1PWM() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  ICR1 = 255;
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << CS10);
  OCR1A = 0;
  OCR1B = 0;
}

void setupTimer2PWM() {
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS20);
  OCR2A = 0;
  OCR2B = 0;
}

void setupSineUpdateTimer() {
  cli();
  OCR0A = 0xAF;
  TIMSK0 |= (1 << OCIE0A);
  sei();
}

// ISR de actualización sinusoidal con control de amplitud
ISR(TIMER0_COMPA_vect) {
  static uint8_t counter = 0;
  
  if (!inverterEnabled) {
    return;
  }
  
  counter++;
  if (counter < 15) {
    return;
  }
  counter = 0;
  
  // Leer valor sinusoidal
  uint8_t sineValue = pgm_read_byte(&sineTable[sampleIndex]);
  
  // Aplicar escalado de amplitud (0-100%)
  uint16_t scaledValue = ((uint16_t)sineValue * amplitudeScale) / 100;
  uint8_t pwmValue = (uint8_t)scaledValue;
  
  // Generar señales PWM para puente H
  if (sampleIndex < SAMPLES / 2) {
    OCR1A = pwmValue;
    OCR1B = 0;
    OCR2B = 0;
    OCR2A = 255 - pwmValue;
  } else {
    OCR1A = 0;
    OCR1B = 255 - pwmValue;
    OCR2B = pwmValue;
    OCR2A = 0;
  }
  
  sampleIndex++;
  if (sampleIndex >= SAMPLES) {
    sampleIndex = 0;
  }
}

void disableInverter() {
  inverterEnabled = false;
  OCR1A = 0;
  OCR1B = 0;
  OCR2A = 0;
  OCR2B = 0;
  amplitudeScale = 0;
}
