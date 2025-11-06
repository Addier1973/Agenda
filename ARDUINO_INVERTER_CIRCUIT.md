# Esquema Eléctrico - Inversor con Arduino

## Descripción General
Este es un circuito inversor DC a AC controlado por Arduino que convierte 12V DC a aproximadamente 220V AC usando modulación PWM y un transformador elevador.

## Diagrama del Circuito

```
                                    CIRCUITO INVERSOR ARDUINO
                                    
                    +12V DC
                     │
                     ├─────────────┐
                     │             │
                     │            [C1]  100µF/25V (Filtro)
                     │             │
                     ├─────────────┴─────────┐
                     │                       │
                     │                       │
                 [R1] 10Ω                [R2] 10Ω
                     │                       │
                     │                       │
    Arduino Pin 9 ───┤                       ├─── Arduino Pin 10
    (PWM)            │                       │    (PWM)
                     │                       │
                 ┌───┴───┐               ┌───┴───┐
                 │ IRF540│               │ IRF540│
                 │ MOSFET│               │ MOSFET│
                 │  (Q1) │               │  (Q2) │
                 └───┬───┘               └───┬───┘
                     │                       │
                     ├───────┐   ┐───────────┤
                     │       │   │           │
                  [Pin 1]    │   │        [Pin 3]
                     │       │   │           │
                     │    ┌──┴───┴──┐        │
                     │    │         │        │
                     └────┤  TRANS- ├────────┘
                          │  FORMA- │
    Arduino Pin 11 ───────┤   DOR   │
    (PWM)                 │ 12V-0-12V│───── Salida 220V AC
                          │  Primario│      (Secundario)
    Arduino Pin 12 ───────┤         │───── 
    (PWM)            ┌────┤  Center │
                     │    │   Tap   │
                     │    └─────────┘
                     │       [Pin 2]
                     │         │
                 ┌───┴───┐ ┌───┴───┐
                 │ IRF540│ │ IRF540│
                 │ MOSFET│ │ MOSFET│
                 │  (Q3) │ │  (Q4) │
                 └───┬───┘ └───┬───┘
                     │         │
                 [R3] 10Ω  [R4] 10Ω
                     │         │
                     ├─────────┴─────────────┐
                     │                       │
                     │                      [C2] 100µF/25V
                     │                       │
                     └───────────────────────┤
                                            GND
                                             ⏚
```

## Componentes Necesarios

### Componentes Principales:
1. **Arduino Uno/Nano** - Microcontrolador para generar señales PWM
2. **MOSFETs IRF540N (x4)** - Transistores de potencia para conmutación
3. **Transformador 12V-0-12V a 220V** (centro tap en primario, 5A mínimo)
4. **Optoacopladores 4N25 (x4)** - Aislamiento óptico (no mostrados)

### Componentes Pasivos:
- **R1, R2, R3, R4**: Resistencias 10Ω 1W (limitación corriente gate)
- **R5, R6, R7, R8**: Resistencias 10kΩ (pull-down gates) - no mostradas
- **C1, C2**: Capacitores electrolíticos 100µF/25V (filtro fuente)
- **C3**: Capacitor 470µF/25V (filtro salida) - no mostrado
- **D1-D4**: Diodos 1N4007 (protección flyback) - no mostrados

### Protección:
- **Fusible**: 10A en entrada +12V
- **Diodos de protección**: 1N4007 en paralelo a cada MOSFET
- **Disipadores de calor** para MOSFETs

## Diagrama de Conexiones Arduino

```
Arduino Uno/Nano
┌─────────────────┐
│                 │
│  Digital Pin 9  ├─────> Gate Q1 (a través de R1 y optoacoplador)
│  Digital Pin 10 ├─────> Gate Q2 (a través de R2 y optoacoplador)
│  Digital Pin 11 ├─────> Gate Q3 (a través de R3 y optoacoplador)
│  Digital Pin 12 ├─────> Gate Q4 (a través de R4 y optoacoplador)
│                 │
│  5V             ├─────> Alimentación optoacopladores
│  GND            ├─────> Tierra común
│                 │
└─────────────────┘
```

## Funcionamiento

### Principio de Operación:
1. Arduino genera señales PWM a 50Hz en pines 9-12
2. Los pares Q1-Q2 y Q3-Q4 se activan alternativamente
3. Cuando Q1-Q2 conducen, corriente fluye en una dirección del primario
4. Cuando Q3-Q4 conducen, corriente fluye en dirección opuesta
5. El transformador eleva el voltaje a 220V AC en el secundario

### Secuencia de Conmutación:
```
Ciclo Positivo (10ms):  Q1 y Q2 ON,  Q3 y Q4 OFF
Ciclo Negativo (10ms):  Q3 y Q4 ON,  Q1 y Q2 OFF

Frecuencia: 50Hz (periodo 20ms)
```

## Código Arduino Básico

```cpp
// Pines de control
#define PIN_Q1 9
#define PIN_Q2 10
#define PIN_Q3 11
#define PIN_Q4 12

void setup() {
  pinMode(PIN_Q1, OUTPUT);
  pinMode(PIN_Q2, OUTPUT);
  pinMode(PIN_Q3, OUTPUT);
  pinMode(PIN_Q4, OUTPUT);
  
  // Configurar Timer1 para PWM a 50Hz
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
  ICR1 = 20000; // 50Hz con prescaler 8
  OCR1A = 10000;
  OCR1B = 10000;
}

void loop() {
  // Semiciclo positivo
  digitalWrite(PIN_Q1, HIGH);
  digitalWrite(PIN_Q2, HIGH);
  digitalWrite(PIN_Q3, LOW);
  digitalWrite(PIN_Q4, LOW);
  delay(10); // 10ms = medio ciclo de 50Hz
  
  // Semiciclo negativo
  digitalWrite(PIN_Q1, LOW);
  digitalWrite(PIN_Q2, LOW);
  digitalWrite(PIN_Q3, HIGH);
  digitalWrite(PIN_Q4, HIGH);
  delay(10); // 10ms = medio ciclo de 50Hz
}
```

## Esquema de Circuito Driver (Detalle por MOSFET)

```
                          +12V
                           │
                           ├──── [VCC Pin 4N25]
                           │
Arduino Pin ──[220Ω]──[LED 4N25]
                           │
                          GND
                           
                          +12V
                           │
                       [10kΩ] (Pull-up)
                           │
    [Pin Fototransistor]───┴──── Gate MOSFET
           4N25                        │
                                   [10kΩ] (Pull-down)
                                       │
                                      GND
```

## Especificaciones Técnicas

| Parámetro              | Valor          |
|------------------------|----------------|
| Tensión de entrada     | 12V DC         |
| Tensión de salida      | 220V AC ±10%   |
| Frecuencia salida      | 50Hz           |
| Potencia máxima        | 500W           |
| Forma de onda          | Onda cuadrada modificada |
| Eficiencia estimada    | 75-85%         |
| Corriente entrada max  | 45A @ 500W     |

## Consideraciones de Seguridad

⚠️ **ADVERTENCIAS IMPORTANTES:**

1. **Alto Voltaje**: Este circuito genera 220V AC que puede ser MORTAL
2. **Aislamiento**: Usar transformador con aislamiento galvánico adecuado
3. **Disipación**: Los MOSFETs requieren disipadores térmicos grandes
4. **Fusibles**: SIEMPRE usar fusibles en entrada y salida
5. **Ventilación**: El circuito genera calor, requiere ventilación
6. **Pruebas**: Probar primero con carga resistiva (bombilla)
7. **No tocar**: Nunca tocar el circuito con alimentación conectada

## Mejoras Recomendadas

1. **PWM Sinusoidal (SPWM)**: Mejorar forma de onda con modulación sinusoidal
2. **Control de retroalimentación**: Sensor de voltaje para regulación
3. **Protección de sobrecorriente**: Sensor ACS712 + código protección
4. **Display LCD**: Mostrar voltaje y corriente de salida
5. **Arranque suave**: Implementar soft-start para evitar picos

## Notas Adicionales

- Este es un diseño de **onda cuadrada modificada**
- Para onda sinusoidal pura, se requiere filtro LC y SPWM
- Los tiempos muertos entre conmutaciones previenen cortocircuitos
- Considerar usar driver de gate dedicado (IR2110) para mejor rendimiento
- El transformador es el componente más crítico y costoso

## Referencias

- IRF540N Datasheet
- Arduino Timer Programming
- Inverter Design Principles
- Safety Standards IEC 60950

---
**Fecha de creación**: 2025-11-06  
**Versión**: 1.0  
**Licencia**: Open Source / Educational Use
