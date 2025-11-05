/*
 * INVERSOR DE ONDA SINUSOIDAL PURA 24V DC A 120V AC
 * Usando transformador de ferrita de alta frecuencia
 * 
 * Características:
 * - Entrada: 24V DC
 * - Salida: 120V AC RMS @ 60Hz
 * - Modulación: SPWM (Sinusoidal Pulse Width Modulation)
 * - Frecuencia PWM: ~31.25 kHz
 * - Topología: Puente H completo (Full Bridge)
 * 
 * Conexiones:
 * - Pin 9: PWM Fase A+ (MOSFET Q1)
 * - Pin 10: PWM Fase A- (MOSFET Q2)
 * - Pin 3: PWM Fase B+ (MOSFET Q3)
 * - Pin 11: PWM Fase B- (MOSFET Q4)
 * - Pin A0: Sensor de corriente
 * - Pin A1: Sensor de voltaje de entrada
 * - Pin 2: Botón de habilitación/deshabilitación
 * - Pin 13: LED de estado
 */

#include <Arduino.h>

// ============= CONFIGURACIÓN =============
#define FRECUENCIA_SALIDA 60      // Frecuencia de salida en Hz (60Hz para USA/México)
#define MUESTRAS_POR_CICLO 100    // Número de muestras por ciclo de 60Hz
#define TIEMPO_MUESTRA_US (1000000 / (FRECUENCIA_SALIDA * MUESTRAS_POR_CICLO))

// Pines de salida PWM para puente H completo
#define PWM_A_HIGH 9              // Lado alto fase A
#define PWM_A_LOW 10              // Lado bajo fase A
#define PWM_B_HIGH 3              // Lado alto fase B
#define PWM_B_LOW 11              // Lado bajo fase B

// Pines de entrada
#define PIN_CORRIENTE A0          // Sensor de corriente ACS712
#define PIN_VOLTAJE A1            // Divisor de voltaje de entrada
#define PIN_ENABLE 2              // Botón de habilitación
#define PIN_LED_ESTADO 13         // LED indicador

// Límites de protección
#define VOLTAJE_MIN 22.0          // Voltaje mínimo de entrada (V)
#define VOLTAJE_MAX 28.0          // Voltaje máximo de entrada (V)
#define CORRIENTE_MAX 20.0        // Corriente máxima (A)
#define TEMP_MAX 80               // Temperatura máxima (°C) - si tienes sensor

// Variables globales
volatile bool inversorHabilitado = false;
volatile uint8_t indiceMuestra = 0;
unsigned long ultimoTiempo = 0;

// Tabla SPWM: valores precalculados de seno (0-255 para PWM de 8 bits)
// 100 muestras por ciclo de 60Hz = 166.67 µs por muestra
const uint8_t tablaSpwm[MUESTRAS_POR_CICLO] PROGMEM = {
  128, 136, 144, 152, 160, 167, 175, 182, 189, 196,
  203, 209, 215, 221, 227, 232, 237, 241, 245, 249,
  252, 254, 256, 258, 259, 260, 260, 260, 259, 258,
  256, 254, 252, 249, 245, 241, 237, 232, 227, 221,
  215, 209, 203, 196, 189, 182, 175, 167, 160, 152,
  144, 136, 128, 120, 112, 104, 96, 89, 81, 74,
  67, 60, 53, 47, 41, 35, 29, 24, 19, 15,
  11, 7, 4, 2, 0, -2, -3, -4, -4, -4,
  -3, -2, 0, 2, 4, 7, 11, 15, 19, 24,
  29, 35, 41, 47, 53, 60, 67, 74, 81, 89
};

// ============= FUNCIONES DE CONFIGURACIÓN =============

void setup() {
  Serial.begin(115200);
  Serial.println(F("================================="));
  Serial.println(F("Inversor Sinusoidal Pura 24V-120V"));
  Serial.println(F("Alta Frecuencia - SPWM"));
  Serial.println(F("================================="));
  
  // Configurar pines de salida PWM
  pinMode(PWM_A_HIGH, OUTPUT);
  pinMode(PWM_A_LOW, OUTPUT);
  pinMode(PWM_B_HIGH, OUTPUT);
  pinMode(PWM_B_LOW, OUTPUT);
  
  // Configurar pines de entrada
  pinMode(PIN_ENABLE, INPUT_PULLUP);
  pinMode(PIN_LED_ESTADO, OUTPUT);
  pinMode(PIN_CORRIENTE, INPUT);
  pinMode(PIN_VOLTAJE, INPUT);
  
  // Inicializar salidas en bajo
  digitalWrite(PWM_A_HIGH, LOW);
  digitalWrite(PWM_A_LOW, LOW);
  digitalWrite(PWM_B_HIGH, LOW);
  digitalWrite(PWM_B_LOW, LOW);
  
  // Configurar Timer1 y Timer2 para PWM de alta frecuencia
  configurarPWM();
  
  // Configurar interrupción para el botón de habilitación
  attachInterrupt(digitalPinToInterrupt(PIN_ENABLE), toggleInversor, FALLING);
  
  Serial.println(F("Sistema inicializado"));
  Serial.println(F("Presiona el botón para habilitar"));
  Serial.println(F(""));
}

void configurarPWM() {
  // Configurar Timer1 (Pines 9 y 10) para PWM a ~31.25 kHz
  // Modo Fast PWM de 8 bits
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10); // Sin prescaler
  
  // Configurar Timer2 (Pines 3 y 11) para PWM a ~31.25 kHz
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
  TCCR2B = _BV(CS20); // Sin prescaler
  
  // Inicializar duty cycle a 0
  OCR1A = 0;  // Pin 9
  OCR1B = 0;  // Pin 10
  OCR2A = 0;  // Pin 11
  OCR2B = 0;  // Pin 3
}

// ============= FUNCIÓN PRINCIPAL =============

void loop() {
  unsigned long tiempoActual = micros();
  
  // Generar onda SPWM a 60Hz
  if (tiempoActual - ultimoTiempo >= TIEMPO_MUESTRA_US) {
    ultimoTiempo = tiempoActual;
    
    if (inversorHabilitado) {
      // Leer valor de la tabla SPWM
      uint8_t valorSeno = pgm_read_byte(&tablaSpwm[indiceMuestra]);
      
      // Generar señales complementarias para el puente H completo
      // Fase A: señal directa
      // Fase B: señal invertida (desfasada 180°)
      
      if (valorSeno >= 128) {
        // Semiciclo positivo
        uint8_t pwmValue = (valorSeno - 128) * 2;
        OCR1A = pwmValue;           // Pin 9: PWM variable
        OCR1B = 0;                  // Pin 10: OFF
        OCR2B = 0;                  // Pin 3: OFF
        OCR2A = pwmValue;           // Pin 11: PWM variable
      } else {
        // Semiciclo negativo
        uint8_t pwmValue = (128 - valorSeno) * 2;
        OCR1A = 0;                  // Pin 9: OFF
        OCR1B = pwmValue;           // Pin 10: PWM variable
        OCR2B = pwmValue;           // Pin 3: PWM variable
        OCR2A = 0;                  // Pin 11: OFF
      }
      
      // Avanzar al siguiente punto de la tabla
      indiceMuestra++;
      if (indiceMuestra >= MUESTRAS_POR_CICLO) {
        indiceMuestra = 0;
      }
    } else {
      // Inversor deshabilitado - apagar todas las salidas
      OCR1A = 0;
      OCR1B = 0;
      OCR2A = 0;
      OCR2B = 0;
    }
  }
  
  // Monitoreo y protecciones (cada 100ms)
  static unsigned long ultimoMonitoreo = 0;
  if (millis() - ultimoMonitoreo >= 100) {
    ultimoMonitoreo = millis();
    
    float voltajeEntrada = leerVoltajeEntrada();
    float corriente = leerCorriente();
    
    // Mostrar estado por serial
    if (inversorHabilitado) {
      Serial.print(F("Estado: ON | Vin: "));
      Serial.print(voltajeEntrada, 1);
      Serial.print(F("V | I: "));
      Serial.print(corriente, 2);
      Serial.print(F("A | P: "));
      Serial.print(voltajeEntrada * corriente, 1);
      Serial.println(F("W"));
    }
    
    // Protecciones
    verificarProtecciones(voltajeEntrada, corriente);
    
    // LED de estado
    digitalWrite(PIN_LED_ESTADO, inversorHabilitado ? HIGH : LOW);
  }
}

// ============= FUNCIONES DE MEDICIÓN =============

float leerVoltajeEntrada() {
  // Divisor de voltaje: R1=10k, R2=2k (ajusta según tu diseño)
  // Voltaje máximo ADC: 5V
  // Factor de escala: (R1+R2)/R2 * 5V / 1023
  int lecturaADC = analogRead(PIN_VOLTAJE);
  float voltaje = lecturaADC * (30.0 / 1023.0); // Ajusta este factor según tu divisor
  return voltaje;
}

float leerCorriente() {
  // Sensor ACS712 20A: 100mV/A, centrado en 2.5V
  // o ACS712 30A: 66mV/A
  int lecturaADC = analogRead(PIN_CORRIENTE);
  float voltajeSensor = lecturaADC * (5.0 / 1023.0);
  float corriente = (voltajeSensor - 2.5) / 0.100; // Para ACS712 20A
  return abs(corriente);
}

// ============= PROTECCIONES =============

void verificarProtecciones(float voltaje, float corriente) {
  static bool proteccionActiva = false;
  
  // Protección por bajo voltaje
  if (voltaje < VOLTAJE_MIN && inversorHabilitado) {
    Serial.println(F("¡ALERTA! Voltaje bajo - Deshabilitando inversor"));
    deshabilitarInversor();
    proteccionActiva = true;
  }
  
  // Protección por sobrevoltaje
  if (voltaje > VOLTAJE_MAX && inversorHabilitado) {
    Serial.println(F("¡ALERTA! Sobrevoltaje - Deshabilitando inversor"));
    deshabilitarInversor();
    proteccionActiva = true;
  }
  
  // Protección por sobrecorriente
  if (corriente > CORRIENTE_MAX && inversorHabilitado) {
    Serial.println(F("¡ALERTA! Sobrecorriente - Deshabilitando inversor"));
    deshabilitarInversor();
    proteccionActiva = true;
  }
  
  // Indicar visualmente protección activa
  if (proteccionActiva) {
    // Parpadear LED rápidamente
    static unsigned long ultimoParpadeo = 0;
    if (millis() - ultimoParpadeo >= 200) {
      ultimoParpadeo = millis();
      digitalWrite(PIN_LED_ESTADO, !digitalRead(PIN_LED_ESTADO));
    }
  }
}

// ============= CONTROL DEL INVERSOR =============

void toggleInversor() {
  static unsigned long ultimaInterrupcion = 0;
  unsigned long tiempoActual = millis();
  
  // Debounce: ignorar interrupciones muy seguidas
  if (tiempoActual - ultimaInterrupcion > 200) {
    if (inversorHabilitado) {
      deshabilitarInversor();
    } else {
      habilitarInversor();
    }
    ultimaInterrupcion = tiempoActual;
  }
}

void habilitarInversor() {
  // Verificar voltaje antes de habilitar
  float voltaje = leerVoltajeEntrada();
  
  if (voltaje >= VOLTAJE_MIN && voltaje <= VOLTAJE_MAX) {
    inversorHabilitado = true;
    indiceMuestra = 0;
    ultimoTiempo = micros();
    Serial.println(F(""));
    Serial.println(F(">>> INVERSOR HABILITADO <<<"));
    Serial.println(F(""));
  } else {
    Serial.println(F("No se puede habilitar: voltaje fuera de rango"));
    Serial.print(F("Voltaje actual: "));
    Serial.print(voltaje, 1);
    Serial.print(F("V (Rango: "));
    Serial.print(VOLTAJE_MIN, 1);
    Serial.print(F("-"));
    Serial.print(VOLTAJE_MAX, 1);
    Serial.println(F("V)"));
  }
}

void deshabilitarInversor() {
  inversorHabilitado = false;
  
  // Apagar todas las salidas PWM inmediatamente
  OCR1A = 0;
  OCR1B = 0;
  OCR2A = 0;
  OCR2B = 0;
  
  Serial.println(F(""));
  Serial.println(F(">>> INVERSOR DESHABILITADO <<<"));
  Serial.println(F(""));
}
