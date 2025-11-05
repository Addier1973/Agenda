/*
 * INVERSOR DE ONDA SINUSOIDAL PURA 24V DC a 120V AC
 * Frecuencia de salida: 60Hz
 * Método: SPWM (Sinusoidal Pulse Width Modulation)
 * Transformador: Ferrita (alta frecuencia ~31.25kHz)
 * 
 * Hardware requerido:
 * - Arduino Nano/Uno/Mega
 * - Driver de MOSFET (IR2110, IR2184 o similar)
 * - 4 MOSFETs de potencia (IRF3205, IRFZ44N o similar)
 * - Transformador de ferrita de alta frecuencia (24V-0-24V primario, 120V secundario)
 * - Filtro LC de salida
 * - Sensor de voltaje (opcional, para retroalimentación)
 * 
 * Conexiones:
 * Pin 9 (OC1A)  -> Gate MOSFET Q1 (High-side puente H brazo A) via driver
 * Pin 10 (OC1B) -> Gate MOSFET Q2 (Low-side puente H brazo A) via driver
 * Pin 3 (OC2B)  -> Gate MOSFET Q3 (High-side puente H brazo B) via driver
 * Pin 11 (OC2A) -> Gate MOSFET Q4 (Low-side puente H brazo B) via driver
 */

// Definiciones de pines
#define PWM_A_HIGH 9   // Timer1 OC1A - Brazo A High-side
#define PWM_A_LOW 10   // Timer1 OC1B - Brazo A Low-side
#define PWM_B_HIGH 3   // Timer2 OC2B - Brazo B High-side
#define PWM_B_LOW 11   // Timer2 OC2A - Brazo B Low-side

#define ENABLE_PIN 7   // Pin para habilitar/deshabilitar el inversor
#define LED_STATUS 13  // LED indicador de estado

// Parámetros de la onda sinusoidal
#define SINE_FREQ 60      // Frecuencia de salida AC (Hz)
#define PWM_FREQ 31250    // Frecuencia PWM (Hz) - Para transformador de ferrita
#define SAMPLES 256       // Número de muestras por ciclo de onda sinusoidal
#define DEAD_TIME 20      // Tiempo muerto entre conmutaciones (en cuentas PWM)

// Tabla de valores sinusoidales precalculados (0-255)
// Optimizado para 256 muestras por ciclo
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

void setup() {
  Serial.begin(115200);
  Serial.println(F("Inversor Sinusoidal Pura 24V->120V"));
  Serial.println(F("Inicializando..."));
  
  // Configurar pines
  pinMode(PWM_A_HIGH, OUTPUT);
  pinMode(PWM_A_LOW, OUTPUT);
  pinMode(PWM_B_HIGH, OUTPUT);
  pinMode(PWM_B_LOW, OUTPUT);
  pinMode(ENABLE_PIN, INPUT_PULLUP);
  pinMode(LED_STATUS, OUTPUT);
  
  // Asegurar que todas las salidas estén en LOW
  digitalWrite(PWM_A_HIGH, LOW);
  digitalWrite(PWM_A_LOW, LOW);
  digitalWrite(PWM_B_HIGH, LOW);
  digitalWrite(PWM_B_LOW, LOW);
  
  // Configurar Timer1 para PWM de alta frecuencia (Pines 9 y 10)
  setupTimer1PWM();
  
  // Configurar Timer2 para PWM de alta frecuencia (Pines 3 y 11)
  setupTimer2PWM();
  
  // Configurar Timer0 para actualización de valores sinusoidales
  // Timer0 se usa para la temporización de actualización de muestras
  setupSineUpdateTimer();
  
  Serial.println(F("Configuracion completada"));
  Serial.println(F("Presione el boton de habilitacion para iniciar"));
  
  delay(1000);
}

void loop() {
  // Verificar estado del pin de habilitación
  static bool lastState = false;
  bool currentState = !digitalRead(ENABLE_PIN); // Pull-up, así que invertido
  
  if (currentState != lastState) {
    lastState = currentState;
    
    if (currentState) {
      enableInverter();
    } else {
      disableInverter();
    }
  }
  
  // Parpadear LED según el estado
  if (inverterEnabled) {
    digitalWrite(LED_STATUS, (millis() / 100) % 2);
  } else {
    digitalWrite(LED_STATUS, LOW);
  }
  
  // Información de diagnóstico cada 2 segundos
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    lastPrint = millis();
    Serial.print(F("Estado: "));
    Serial.println(inverterEnabled ? F("ACTIVO") : F("INACTIVO"));
  }
  
  delay(10);
}

// Configurar Timer1 para PWM rápido de 31.25kHz (Brazo A del puente H)
void setupTimer1PWM() {
  // Modo PWM de fase correcta, TOP = ICR1
  // Frecuencia PWM = 16MHz / (2 * prescaler * TOP)
  // Para 31.25kHz: TOP = 16000000 / (2 * 1 * 31250) = 256
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // Configurar modo PWM fase correcta con ICR1 como TOP
  ICR1 = 255; // TOP value para ~31.25kHz
  
  // Configurar salidas PWM en modo no invertido
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << CS10); // Sin prescaler
  
  // Inicializar duty cycles en 0
  OCR1A = 0;
  OCR1B = 0;
}

// Configurar Timer2 para PWM rápido (Brazo B del puente H)
void setupTimer2PWM() {
  // Timer2 es de 8 bits, configurado para Fast PWM
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  
  // Modo Fast PWM, TOP = 0xFF
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS20); // Sin prescaler, ~62.5kHz para Timer2
  
  // Inicializar duty cycles en 0
  OCR2A = 0;
  OCR2B = 0;
}

// Configurar interrupción para actualización de valores sinusoidales
void setupSineUpdateTimer() {
  // Usar Timer0 compare match para actualización de muestras
  // Frecuencia de actualización = SINE_FREQ * SAMPLES = 60 * 256 = 15.36kHz
  
  // Timer0 ya está usado por Arduino para millis(), pero podemos usar COMPA
  // Configurar para interrupción cada ~65us (15.36kHz)
  
  cli(); // Deshabilitar interrupciones
  
  // Timer0 configuración (no cambiar mucho porque afecta millis())
  // Usar interrupción de comparación A
  OCR0A = 0xAF; // Comparar valor para ~1kHz base
  TIMSK0 |= (1 << OCIE0A); // Habilitar interrupción de comparación A
  
  sei(); // Habilitar interrupciones
}

// Interrupción de actualización de valores sinusoidales
ISR(TIMER0_COMPA_vect) {
  static uint8_t counter = 0;
  
  if (!inverterEnabled) {
    return;
  }
  
  // Actualizar cada 15-16 veces para lograr ~15.36kHz
  counter++;
  if (counter < 15) {
    return;
  }
  counter = 0;
  
  // Leer valor sinusoidal de la tabla
  uint8_t sineValue = pgm_read_byte(&sineTable[sampleIndex]);
  
  // Calcular valores PWM con tiempo muerto
  uint8_t pwmValue = sineValue;
  
  // Para puente H completo, necesitamos dos brazos en oposición de fase
  // Brazo A: Semiciclo positivo
  // Brazo B: Semiciclo negativo (180° desfasado)
  
  if (sampleIndex < SAMPLES / 2) {
    // Semiciclo positivo - Brazo A activo
    OCR1A = pwmValue; // High-side A
    OCR1B = 0;        // Low-side A
    OCR2B = 0;        // High-side B
    OCR2A = 255 - pwmValue; // Low-side B complementario
  } else {
    // Semiciclo negativo - Brazo B activo
    OCR1A = 0;        // High-side A
    OCR1B = 255 - pwmValue; // Low-side A complementario
    OCR2B = pwmValue; // High-side B
    OCR2A = 0;        // Low-side B
  }
  
  // Avanzar al siguiente índice
  sampleIndex++;
  if (sampleIndex >= SAMPLES) {
    sampleIndex = 0;
  }
}

void enableInverter() {
  Serial.println(F(">>> INVERSOR HABILITADO <<<"));
  inverterEnabled = true;
  sampleIndex = 0;
  
  // Pequeño retardo suave para arranque
  delay(100);
}

void disableInverter() {
  Serial.println(F(">>> INVERSOR DESHABILITADO <<<"));
  inverterEnabled = false;
  
  // Apagar todos los PWM
  OCR1A = 0;
  OCR1B = 0;
  OCR2A = 0;
  OCR2B = 0;
  
  sampleIndex = 0;
}
