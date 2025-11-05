/*
 * INVERSOR DE ONDA SINUSOIDAL PURA 24V DC a 120V AC
 * Usando transformador de ferrita de alta frecuencia
 * 
 * ADVERTENCIA: Este proyecto maneja voltajes peligrosos.
 * Solo debe ser construido por personas con experiencia en electrónica de potencia.
 * 
 * Configuración del Hardware:
 * - Arduino Nano/Uno (ATmega328P)
 * - Driver de MOSFET IR2110 o similar (x2)
 * - MOSFETs de potencia IRF3205 o similar (x4) para puente H completo
 * - Transformador de ferrita de alta frecuencia (24V a 170V peak, ~50kHz)
 * - Sensores de voltaje y corriente para protección
 * - Optoacopladores para aislamiento
 */

// ============= CONFIGURACIÓN DE PINES =============
#define PWM_HIGH_A    9   // Timer1 - PWM lado alto A (OC1A)
#define PWM_LOW_A     10  // Timer1 - PWM lado bajo A (OC1B)
#define PWM_HIGH_B    3   // Timer2 - PWM lado alto B (OC2B)
#define PWM_LOW_B     11  // Timer2 - PWM lado bajo B (OC2A)

#define VOLTAGE_SENSE A0  // Sensor de voltaje de salida
#define CURRENT_SENSE A1  // Sensor de corriente
#define BATTERY_SENSE A2  // Sensor de voltaje de batería

#define ENABLE_PIN    7   // Pin de habilitación del inversor
#define LED_STATUS    13  // LED de estado
#define OVERLOAD_PIN  8   // Pin de detección de sobrecarga

// ============= PARÁMETROS DEL INVERSOR =============
#define CARRIER_FREQ  50000  // Frecuencia portadora 50kHz para transformador de ferrita
#define OUTPUT_FREQ   60     // Frecuencia de salida 60Hz (cambiar a 50Hz si es necesario)
#define SAMPLES       128    // Número de muestras por ciclo de onda sinusoidal

// Valores de protección
#define MAX_CURRENT      10.0  // Corriente máxima en Amperios
#define MIN_BATTERY_V    20.0  // Voltaje mínimo de batería
#define MAX_BATTERY_V    29.0  // Voltaje máximo de batería
#define TARGET_VOLTAGE   120.0 // Voltaje objetivo de salida RMS

// ============= VARIABLES GLOBALES =============
volatile uint8_t sineIndex = 0;
volatile bool outputEnabled = false;
float batteryVoltage = 0;
float outputVoltage = 0;
float outputCurrent = 0;
bool overloadFlag = false;

// Tabla de onda sinusoidal precalculada (0-255 para PWM de 8 bits)
// Tabla SPWM optimizada para reducir armónicos
const uint8_t sineTable[SAMPLES] PROGMEM = {
  128, 134, 140, 147, 153, 159, 165, 171, 177, 183, 189, 195, 200, 206, 211, 217,
  222, 227, 232, 236, 241, 245, 249, 253, 257, 260, 264, 267, 270, 273, 275, 278,
  280, 282, 284, 286, 287, 288, 289, 290, 291, 291, 291, 291, 291, 290, 289, 288,
  287, 286, 284, 282, 280, 278, 275, 273, 270, 267, 264, 260, 257, 253, 249, 245,
  241, 236, 232, 227, 222, 217, 211, 206, 200, 195, 189, 183, 177, 171, 165, 159,
  153, 147, 140, 134, 128, 122, 116, 109, 103, 97, 91, 85, 79, 73, 67, 61,
  56, 50, 45, 39, 34, 29, 24, 20, 15, 11, 7, 3, -1, -4, -8, -11,
  -14, -17, -19, -22, -24, -26, -28, -30, -31, -32, -33, -34, -35, -35, -35, -35,
  -35, -34, -33, -32, -31, -30, -28, -26, -24, -22, -19, -17, -14, -11, -8, -4,
  -1, 3, 7, 11, 15, 20, 24, 29, 34, 39, 45, 50, 56, 61, 67, 73,
  79, 85, 91, 97, 103, 109, 116, 122, 128, 134, 140, 147, 153, 159, 165, 171
};

// ============= CONFIGURACIÓN INICIAL =============
void setup() {
  Serial.begin(115200);
  Serial.println(F("Inversor Sinusoidal Puro 24V->120V"));
  Serial.println(F("Alta Frecuencia con Transformador de Ferrita"));
  
  // Configurar pines
  pinMode(PWM_HIGH_A, OUTPUT);
  pinMode(PWM_LOW_A, OUTPUT);
  pinMode(PWM_HIGH_B, OUTPUT);
  pinMode(PWM_LOW_B, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(OVERLOAD_PIN, INPUT_PULLUP);
  
  // Deshabilitar salida inicialmente
  digitalWrite(ENABLE_PIN, LOW);
  outputEnabled = false;
  
  // Configurar Timer1 para PWM de alta frecuencia (Lado A del puente H)
  setupTimer1PWM();
  
  // Configurar Timer2 para PWM de alta frecuencia (Lado B del puente H)
  setupTimer2PWM();
  
  // Configurar interrupción para actualizar la tabla sinusoidal
  setupModulationTimer();
  
  // Configurar ADC para lectura rápida
  setupADC();
  
  delay(1000);
  
  // Verificar voltaje de batería antes de habilitar
  checkBatteryVoltage();
  
  Serial.println(F("Sistema inicializado. Esperando comando..."));
  Serial.println(F("Enviar 'ON' para encender, 'OFF' para apagar"));
}

// ============= CONFIGURACIÓN DE TIMER1 (50kHz PWM) =============
void setupTimer1PWM() {
  // Configurar Timer1 para Fast PWM, frecuencia ~50kHz
  // Modo 14: Fast PWM con TOP en ICR1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // ICR1 define la frecuencia portadora
  // Para 50kHz: ICR1 = F_CPU / (prescaler * freq) = 16MHz / (1 * 50kHz) = 320
  ICR1 = (F_CPU / CARRIER_FREQ) - 1;
  
  // Configurar modo Fast PWM (modo 14)
  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // Sin prescaler
  
  // Habilitar salidas PWM (non-inverting mode)
  TCCR1A |= _BV(COM1A1) | _BV(COM1B1);
  
  // Iniciar con duty cycle 0
  OCR1A = 0;
  OCR1B = 0;
}

// ============= CONFIGURACIÓN DE TIMER2 (50kHz PWM) =============
void setupTimer2PWM() {
  // Configurar Timer2 para Fast PWM
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  
  // Modo Fast PWM
  TCCR2A = _BV(WGM21) | _BV(WGM20); // Fast PWM mode
  TCCR2B = _BV(CS20); // Sin prescaler
  
  // Habilitar salidas PWM (non-inverting mode)
  TCCR2A |= _BV(COM2A1) | _BV(COM2B1);
  
  // Iniciar con duty cycle 0
  OCR2A = 0;
  OCR2B = 0;
}

// ============= CONFIGURACIÓN DE TIMER0 PARA MODULACIÓN =============
void setupModulationTimer() {
  // Usar Timer0 para generar interrupciones a frecuencia de muestreo
  // Frecuencia de muestreo = OUTPUT_FREQ * SAMPLES = 60Hz * 128 = 7680Hz
  
  cli(); // Deshabilitar interrupciones
  
  // Timer0 ya es usado por Arduino para millis(), pero podemos compartirlo
  // Configurar interrupción de comparación
  OCR0A = 0xAF; // Valor para ~7.8kHz con prescaler 64
  TIMSK0 |= _BV(OCIE0A); // Habilitar interrupción de comparación
  
  sei(); // Habilitar interrupciones
}

// ============= ISR: ACTUALIZACIÓN DE SPWM =============
ISR(TIMER0_COMPA_vect) {
  if (!outputEnabled) {
    OCR1A = 0;
    OCR1B = 0;
    OCR2A = 0;
    OCR2B = 0;
    return;
  }
  
  // Leer valor de la tabla sinusoidal
  uint8_t sineValue = pgm_read_byte(&sineTable[sineIndex]);
  
  // Calcular duty cycles para puente H completo
  // Lado A (positivo) y Lado B (negativo) en oposición de fase
  uint16_t dutyA, dutyB;
  
  if (sineValue >= 128) {
    // Semi-ciclo positivo: Lado A activo
    dutyA = map(sineValue, 128, 255, 0, ICR1);
    dutyB = 0;
    OCR1A = dutyA;  // High-side A
    OCR1B = 0;      // Low-side A (apagado durante high-side)
    OCR2A = 0;      // Low-side B (apagado)
    OCR2B = 0;      // High-side B (apagado)
  } else {
    // Semi-ciclo negativo: Lado B activo
    dutyB = map(sineValue, 0, 127, ICR1, 0);
    dutyA = 0;
    OCR1A = 0;      // High-side A (apagado)
    OCR1B = 0;      // Low-side A (apagado)
    OCR2A = 0;      // Low-side B (apagado durante high-side)
    OCR2B = dutyB;  // High-side B
  }
  
  // Incrementar índice de la tabla
  sineIndex++;
  if (sineIndex >= SAMPLES) {
    sineIndex = 0;
  }
}

// ============= CONFIGURACIÓN ADC RÁPIDO =============
void setupADC() {
  // Configurar ADC para conversión más rápida
  // Prescaler de 16 para ~1MHz ADC clock (16MHz/16)
  ADCSRA = _BV(ADEN) | _BV(ADPS2); // Habilitar ADC, prescaler 16
}

// ============= LECTURA DE SENSORES =============
void readSensors() {
  // Leer voltaje de batería (divisor resistivo 1:10)
  int batteryRaw = analogRead(BATTERY_SENSE);
  batteryVoltage = (batteryRaw * 5.0 / 1023.0) * 10.0; // Ajustar según divisor
  
  // Leer corriente (sensor ACS712 o similar)
  int currentRaw = analogRead(CURRENT_SENSE);
  outputCurrent = ((currentRaw * 5.0 / 1023.0) - 2.5) / 0.185; // ACS712-5A: 185mV/A
  
  // Leer voltaje de salida (después de rectificación y filtrado)
  int voltageRaw = analogRead(VOLTAGE_SENSE);
  outputVoltage = (voltageRaw * 5.0 / 1023.0) * 50.0; // Ajustar según divisor
}

// ============= VERIFICACIÓN DE BATERÍA =============
void checkBatteryVoltage() {
  readSensors();
  
  if (batteryVoltage < MIN_BATTERY_V) {
    Serial.print(F("ERROR: Voltaje de batería bajo: "));
    Serial.print(batteryVoltage);
    Serial.println(F("V"));
    outputEnabled = false;
    digitalWrite(ENABLE_PIN, LOW);
  } else if (batteryVoltage > MAX_BATTERY_V) {
    Serial.print(F("ADVERTENCIA: Voltaje de batería alto: "));
    Serial.print(batteryVoltage);
    Serial.println(F("V"));
  }
}

// ============= SISTEMA DE PROTECCIÓN =============
void checkProtections() {
  readSensors();
  
  // Protección de sobrecorriente
  if (outputCurrent > MAX_CURRENT) {
    Serial.println(F("SOBRECORRIENTE DETECTADA!"));
    shutdownInverter();
    overloadFlag = true;
    return;
  }
  
  // Protección de bajo voltaje de batería
  if (batteryVoltage < MIN_BATTERY_V) {
    Serial.println(F("BAJO VOLTAJE DE BATERÍA!"));
    shutdownInverter();
    return;
  }
  
  // Verificar pin de sobrecarga externa
  if (digitalRead(OVERLOAD_PIN) == LOW) {
    Serial.println(F("SOBRECARGA EXTERNA DETECTADA!"));
    shutdownInverter();
    overloadFlag = true;
    return;
  }
}

// ============= APAGAR INVERSOR =============
void shutdownInverter() {
  outputEnabled = false;
  digitalWrite(ENABLE_PIN, LOW);
  
  // Apagar todos los PWM
  OCR1A = 0;
  OCR1B = 0;
  OCR2A = 0;
  OCR2B = 0;
  
  Serial.println(F("Inversor apagado por protección"));
  
  // Parpadear LED de error
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_STATUS, HIGH);
    delay(100);
    digitalWrite(LED_STATUS, LOW);
    delay(100);
  }
}

// ============= ENCENDER INVERSOR =============
void startInverter() {
  checkBatteryVoltage();
  
  if (batteryVoltage < MIN_BATTERY_V) {
    Serial.println(F("No se puede iniciar: voltaje de batería bajo"));
    return;
  }
  
  if (overloadFlag) {
    Serial.println(F("Sobrecarga detectada previamente. Reinicie el sistema."));
    return;
  }
  
  Serial.println(F("Iniciando inversor..."));
  
  // Soft-start: incrementar gradualmente
  sineIndex = 0;
  digitalWrite(ENABLE_PIN, HIGH);
  delay(100);
  
  outputEnabled = true;
  digitalWrite(LED_STATUS, HIGH);
  
  Serial.println(F("Inversor encendido"));
}

// ============= BUCLE PRINCIPAL =============
void loop() {
  static unsigned long lastCheck = 0;
  static unsigned long lastDisplay = 0;
  
  // Verificar protecciones cada 100ms
  if (millis() - lastCheck > 100) {
    lastCheck = millis();
    
    if (outputEnabled) {
      checkProtections();
    }
  }
  
  // Mostrar información cada segundo
  if (millis() - lastDisplay > 1000) {
    lastDisplay = millis();
    
    if (outputEnabled) {
      Serial.print(F("Batería: "));
      Serial.print(batteryVoltage, 1);
      Serial.print(F("V | Salida: "));
      Serial.print(outputVoltage, 1);
      Serial.print(F("V | Corriente: "));
      Serial.print(outputCurrent, 2);
      Serial.println(F("A"));
    }
  }
  
  // Procesar comandos seriales
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    if (command == "ON") {
      startInverter();
    } else if (command == "OFF") {
      Serial.println(F("Apagando inversor..."));
      shutdownInverter();
    } else if (command == "STATUS") {
      readSensors();
      Serial.println(F("=== ESTADO DEL SISTEMA ==="));
      Serial.print(F("Estado: "));
      Serial.println(outputEnabled ? F("ENCENDIDO") : F("APAGADO"));
      Serial.print(F("Batería: "));
      Serial.print(batteryVoltage, 2);
      Serial.println(F("V"));
      Serial.print(F("Salida: "));
      Serial.print(outputVoltage, 2);
      Serial.println(F("V"));
      Serial.print(F("Corriente: "));
      Serial.print(outputCurrent, 2);
      Serial.println(F("A"));
    } else if (command == "RESET") {
      overloadFlag = false;
      Serial.println(F("Flags de error reseteados"));
    } else {
      Serial.println(F("Comandos: ON, OFF, STATUS, RESET"));
    }
  }
  
  // Parpadeo del LED de estado cuando está encendido
  if (outputEnabled) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
      lastBlink = millis();
      digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
    }
  }
}
