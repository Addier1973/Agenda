# Esquema Eléctrico: Inverter DC-AC con Arduino

## Descripción General
Este documento describe el esquema eléctrico de un inverter DC a AC de 12V a 220V utilizando Arduino para el control PWM.

## Componentes Principales

### Lista de Materiales
- Arduino UNO (o similar)
- MOSFETs IRF540N x4 (Puente H)
- Transformador 12V-0-12V / 220V (5A o más)
- Optoacopladores 4N25 x4
- Resistencias 220Ω x4
- Resistencias 10KΩ x4
- Diodos 1N4007 x4
- Capacitores 1000µF 25V x2
- Batería 12V (o fuente DC)
- Disipadores de calor
- Ventilador 12V (opcional)

## Esquema Eléctrico

```
                                    INVERTER DC-AC CON ARDUINO
                                    ==========================

                            +12V Batería
                              |
                              +----[Fusible 10A]----+
                              |                     |
                    [C1 1000µF]                     |
                              |                     |
                             GND                    |
                                                    |
                                                    |
   Arduino UNO                                      |
   +----------+                                     |
   |          |                                     |
   |      D9  |---[220Ω]---+                        |
   |          |            |                        |
   |      D10 |---[220Ω]---|-+                      |
   |          |            | |                      |
   |      D11 |---[220Ω]---|--+                     |
   |          |            | | |                    |
   |      D12 |---[220Ω]---|--|--+                  |
   |          |            | | | |                  |
   |      GND |---+        | | | |                  |
   |          |   |        | | | |                  |
   |      5V  |---|-+      | | | |                  |
   +----------+   | |      | | | |                  |
                  | |      | | | |                  |
                 GND |     | | | |                  |
                     |     | | | |                  |
                     |     | | | |                  |
        PUENTE H (MOSFETS)| | | |                  |
        ==================| | | |                  |
                          | | | |                  |
        Optoacoplador 1   | | | |                  |
        +------+          | | | |                  |
      --| 1  6 |--[10K]---|-|-|-|--+5V             |
        |      |          | | | |                  |
      --| 2  4 |----------|-|-|-|--GND             |
        |      |          | | | |                  |
        | 4N25 |          | | | |                  |
        |      |          | | | |                  |
        | 3  5 |---+      | | | |                  |
        +------+   |      | | | |                  |
     LED   Fototr.|      | | | |                  |
      1     2   | |      | | | |                  |
            +---|-|------+ | | |                  |
            |   | +--------+ | |     Q1 (IRF540N) |
            |   |            | |     +----------+ |
            |   |            +-|-----| G      D |-+----+
            |   |              +-----| MOSFET   |      |
            |   |                    |        S |---+  |
            |   |                    +----------+   |  |
            |   |                                   |  |
            |   |   Optoacoplador 2   Q2 (IRF540N) |  |
            |   |   +------+          +----------+ |  |
            |   +---| 1  6 |--[10K]---| G      D |-+  |
            |       |      |          | MOSFET   |    |
            +-------| 2  4 |----------| S        |----+----> Centro Transformador (CT)
            |       | 4N25 |          +----------+    |
            |       | 3  5 |---+                      |
            |       +------+   |                      |
            |       LED Fototr.|                      |
            |        3    4  | |                      |
            |            +---|-|----------------------+
            |            |   | |
            |            |   | |      Q3 (IRF540N)   |
            |            |   +-|-----+----------+     |
            |            |     +-----| G      D |-----+
            |            |           | MOSFET   |
            |            |           |        S |---+
            |            |           +----------+   |
            |            |                          |
            |   Optoacoplador 3   Q4 (IRF540N)     |
            |   +------+          +----------+     |
            |---| 1  6 |--[10K]---| G      D |-----+
            |   |      |          | MOSFET   |
            +---| 2  4 |----------| S        |----------> Terminal Negativo (-12V)
                | 4N25 |          +----------+
                | 3  5 |---+
                +------+   |
                LED Fototr.|
                 5    6  | |
                         | |
                         +-+

        Optoacoplador 4
        Similar al 3...

   TRANSFORMADOR 12V-0-12V a 220V
   ==============================
   
   Primario (Baja Tensión)        Secundario (Alta Tensión)
   
   +12V ----+                     +---- 220V AC (Fase)
            |                     |
   CT (0V)--+                     |
            |                     |
   -12V ----+                     +---- 220V AC (Neutro)


   [C2 1000µF]
       |
      GND


   SALIDA 220V AC
   ==============
   
   +---- Fase (220V AC, 50Hz)
   |
   +---- Neutro
   |
   +---- Tierra (Chassis)

```

## Conexiones Detalladas

### Arduino → Optoacopladores
| Pin Arduino | Resistencia | Optoacoplador | LED Pin |
|-------------|-------------|---------------|---------|
| D9          | 220Ω        | OPT1          | Pin 1   |
| D10         | 220Ω        | OPT2          | Pin 1   |
| D11         | 220Ω        | OPT3          | Pin 1   |
| D12         | 220Ω        | OPT4          | Pin 1   |

### Puente H (Configuración)
- **Q1 + Q4**: Par diagonal 1 (alimenta +12V al primario)
- **Q2 + Q3**: Par diagonal 2 (alimenta -12V al primario)
- Nunca activar Q1+Q2 o Q3+Q4 simultáneamente (cortocircuito)

### Transformador
- **Primario**: Centro: 0V, Extremos: +12V y -12V
- **Secundario**: 220V AC, 50Hz
- **Potencia**: Mínimo 300W (2.5A en 12V)

## Código Arduino (PWM SPWM)

```cpp
// Inverter DC-AC con modulación SPWM
// Frecuencia de salida: 50Hz

const int MOSFET1 = 9;   // Par 1
const int MOSFET2 = 10;  // Par 1
const int MOSFET3 = 11;  // Par 2
const int MOSFET4 = 12;  // Par 2

const int FREQ = 50;           // Frecuencia de salida (Hz)
const int PWM_FREQ = 20000;    // Frecuencia PWM (Hz)
const int SAMPLES = 100;       // Muestras por ciclo

int sineTable[SAMPLES];

void setup() {
  pinMode(MOSFET1, OUTPUT);
  pinMode(MOSFET2, OUTPUT);
  pinMode(MOSFET3, OUTPUT);
  pinMode(MOSFET4, OUTPUT);
  
  // Generar tabla seno
  for(int i = 0; i < SAMPLES; i++) {
    sineTable[i] = 127 + 127 * sin(2 * PI * i / SAMPLES);
  }
  
  // Configurar Timer1 para PWM rápido
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  ICR1 = 400; // TOP para ~20kHz PWM
}

void loop() {
  for(int i = 0; i < SAMPLES; i++) {
    int pwmValue = sineTable[i];
    
    if(pwmValue > 127) {
      // Semiciclo positivo: Activar Q1 y Q4
      analogWrite(MOSFET1, pwmValue);
      analogWrite(MOSFET4, pwmValue);
      analogWrite(MOSFET2, 0);
      analogWrite(MOSFET3, 0);
    } else {
      // Semiciclo negativo: Activar Q2 y Q3
      analogWrite(MOSFET2, 255 - pwmValue);
      analogWrite(MOSFET3, 255 - pwmValue);
      analogWrite(MOSFET1, 0);
      analogWrite(MOSFET4, 0);
    }
    
    delayMicroseconds(200); // Delay para 50Hz
  }
}
```

## Diagrama de Bloques

```
+----------+     +-------------+     +----------+     +---------------+     +--------+
| Batería  |---->| Arduino     |---->| Opto-    |---->| Puente H     |---->| Trans- |----> 220V AC
| 12V DC   |     | (Control    |     | acopla-  |     | (4 MOSFETs)  |     | forma- |
|          |     | PWM SPWM)   |     | dores    |     |              |     | dor    |
+----------+     +-------------+     +----------+     +---------------+     +--------+
```

## Funcionamiento

1. **Arduino genera señal SPWM** (Sine PWM) a ~20kHz
2. **Optoacopladores aíslan** el circuito de control del de potencia
3. **Puente H conmuta** la corriente DC en ambas direcciones
4. **Transformador eleva** de 12V AC a 220V AC
5. **Salida**: Onda sinusoidal modificada de 220V, 50Hz

## Precauciones de Seguridad

⚠️ **ADVERTENCIAS IMPORTANTES**:

1. **Alto Voltaje**: 220V AC es PELIGROSO y potencialmente MORTAL
2. **Aislamiento**: Usar transformador con aislamiento galvánico
3. **Disipación**: MOSFETs requieren disipadores y ventilación
4. **Protecciones**:
   - Fusibles en entrada y salida
   - Protección contra sobrecorriente
   - Protección térmica
5. **Carcasa**: Debe estar conectada a tierra
6. **Pruebas**: Realizar pruebas con carga resistiva primero
7. **No usar con**: Equipos médicos o sensibles sin onda sinusoidal pura

## Mejoras Posibles

- **Filtro LC** en salida para suavizar la onda
- **Sensor de corriente** (ACS712) para protección
- **Sensor de temperatura** (LM35) para apagado térmico
- **Display LCD** para monitoreo
- **Arranque suave** (soft-start)
- **SPWM mejorado** con más puntos de muestreo

## Notas Técnicas

- **Eficiencia esperada**: 75-85%
- **Forma de onda**: Sinusoidal modificada (cuasi-cuadrada con PWM)
- **THD**: ~15-25% (moderado)
- **Potencia máxima**: Limitada por transformador y MOSFETs
- **Tiempo dead-time**: Necesario 1-2µs entre conmutaciones

## Referencias

- Datasheet IRF540N MOSFET
- Datasheet 4N25 Optocoupler
- Arduino Timer PWM Reference
- Inverter Design Principles

---

**PRECAUCIÓN**: Este esquema es educativo. Para aplicaciones reales, consultar un ingeniero eléctrico certificado y cumplir normativas locales de seguridad eléctrica.

**Fecha**: 2025-11-06  
**Versión**: 1.0
