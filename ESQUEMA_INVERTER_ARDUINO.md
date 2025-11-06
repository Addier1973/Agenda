# Esquema Eléctrico - Inverter Arduino (DC a AC)

## Descripción General
Este documento presenta el esquema eléctrico de un inverter controlado por Arduino que convierte corriente continua (DC) a corriente alterna (AC).

## Componentes Principales

### Hardware Requerido:
1. **Arduino UNO/NANO** (Microcontrolador)
2. **MOSFETs IRF540N** (x4) - Para el puente H
3. **Transformador** 12V-0-12V a 220V (Centro tap)
4. **Resistencias** 220Ω (x4) - Para los gates de los MOSFETs
5. **Optoacopladores 4N25** (x4) - Aislamiento óptico
6. **Diodos 1N4007** (x4) - Protección
7. **Capacitores** 1000µF/25V (x2) - Filtrado
8. **Batería** 12V (Fuente DC)
9. **Disipadores de calor** para MOSFETs

## Esquema del Circuito

```
                         INVERTER ARDUINO - ESQUEMA ELÉCTRICO
                         ====================================

                              SECCIÓN DE CONTROL (Arduino)
                              ============================

                                    +5V
                                     |
                    Arduino UNO      |
                  ┌─────────────┐    |
                  │             │    |
            Pin 9 │○           ○│ VCC├─────┐
                  │             │    │     │
           Pin 10 │○           ○│ GND├──┐  │
                  │             │    │  │  │
           Pin 11 │○            │    │  │  │
                  │             │    │  │  │
                  └─────────────┘    │  │  │
                    │  │  │  │       │  │  │
                    │  │  │  │       │  │  │
                    │  │  │  └───────┼──┼──┼─── PWM4 (Pin 11)
                    │  │  └──────────┼──┼──┼─── PWM3 (Pin 10)
                    │  └─────────────┼──┼──┼─── PWM2 (Pin 9)
                    └────────────────┼──┼──┼─── PWM1 (Pin 8)
                                     │  │  │
                                     │  │  │
                    SECCIÓN DE OPTOACOPLADORES (Aislamiento)
                    ========================================

    PWM1 ──220Ω──┬──────┐
                 │      │  4N25
              ┌──┴──┐   │ ┌───┐      
              │ LED │   └─┤1  6├─── Q1_Gate
              └──┬──┘     │   │
                GND       └───┘
                             5├─── GND
                             
    PWM2 ──220Ω──┬──────┐
                 │      │  4N25
              ┌──┴──┐   │ ┌───┐      
              │ LED │   └─┤1  6├─── Q2_Gate
              └──┬──┘     │   │
                GND       └───┘
                             5├─── GND

    PWM3 ──220Ω──┬──────┐
                 │      │  4N25
              ┌──┴──┐   │ ┌───┐      
              │ LED │   └─┤1  6├─── Q3_Gate
              └──┬──┘     │   │
                GND       └───┘
                             5├─── GND

    PWM4 ──220Ω──┬──────┐
                 │      │  4N25
              ┌──┴──┐   │ ┌───┐      
              │ LED │   └─┤1  6├─── Q4_Gate
              └──┬──┘     │   │
                GND       └───┘
                             5├─── GND


                    SECCIÓN DE POTENCIA (Puente H)
                    ==============================

                          BATERÍA 12V
                              │
                         ┌────┴────┐
                         │    +    │
                    C1 ═══          C2 ═══
                   1000µF│          │1000µF
                         │    -    │
                         └────┬────┘
                              │
                    ┌─────────┴─────────┐
                    │                   │
              ┌─────┴─────┐       ┌─────┴─────┐
              │   Q1      │       │   Q3      │
              │ IRF540N   │       │ IRF540N   │
              │  (MOSFET) │       │  (MOSFET) │
              │ D ┌───┐ S │       │ D ┌───┐ S │
              ├───┤   ├───┤       ├───┤   ├───┤
              │ G └───┘   │       │ G └───┘   │
              │     ▲     │       │     ▲     │
              │     │220Ω │       │     │220Ω │
              │   Q1_Gate │       │   Q3_Gate │
              └─────┬─────┘       └─────┬─────┘
                    │                   │
                    ├───────┬───────────┤
                    │       │           │
                    │   ┌───┴───┐       │
                    │   │ TRANS │       │
                    │   │FORMA- │       │
                    │   │  DOR  │       │
                    │   │12V-0- │       │
                    │   │ 12V   │       │
                    │   │  →    │       │
                    │   │ 220V  │       │
                    │   │  AC   │       │
                    │   └───┬───┘       │
                    │       │           │
              ┌─────┴─────┐ │     ┌─────┴─────┐
              │   Q2      │ │     │   Q4      │
              │ IRF540N   │ │     │ IRF540N   │
              │  (MOSFET) │ │     │  (MOSFET) │
              │ D ┌───┐ S │ │     │ D ┌───┐ S │
              ├───┤   ├───┤ │     ├───┤   ├───┤
              │ G └───┘   │ │     │ G └───┘   │
              │     ▲     │ │     │     ▲     │
              │     │220Ω │ │     │     │220Ω │
              │   Q2_Gate │ │     │   Q4_Gate │
              └─────┬─────┘ │     └─────┬─────┘
                    │       │           │
                  GND    Centro       GND
                          Tap


                    Salida AC 220V (Del transformador)
                    ==================================
                              │
                         ┌────┴────┐
                         │ 220V AC │ ──→ CARGA
                         │ 50/60Hz │
                         └─────────┘
```

## Diagrama de Bloques Simplificado

```
┌──────────────┐     ┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│   Batería    │────→│   Arduino    │────→│Optoacoplador │────→│   Puente H   │
│   12V DC     │     │  (Control    │     │ (Aislamiento)│     │  (MOSFETs)   │
└──────────────┘     │   PWM)       │     └──────────────┘     └──────┬───────┘
                     └──────────────┘                                  │
                                                                        ↓
                                                              ┌──────────────────┐
                                                              │  Transformador   │
                                                              │   12V → 220V     │
                                                              └─────────┬────────┘
                                                                        │
                                                                        ↓
                                                              ┌──────────────────┐
                                                              │  Salida AC 220V  │
                                                              │     (CARGA)      │
                                                              └──────────────────┘
```

## Descripción del Funcionamiento

### 1. Generación de PWM (Arduino)
- El Arduino genera señales PWM en los pines 8, 9, 10 y 11
- Frecuencia de salida: 50Hz o 60Hz (configurable)
- Modulación SPWM (Sine PWM) para generar onda senoidal

### 2. Aislamiento Óptico
- Los optoacopladores 4N25 aíslan la sección de control (5V) de la potencia (12V)
- Protegen el Arduino de picos de tensión
- Las resistencias de 220Ω limitan la corriente del LED interno

### 3. Puente H (Inversión)
- **Semiciclo Positivo**: Q1 y Q4 conducen (Q2 y Q3 apagados)
- **Semiciclo Negativo**: Q2 y Q3 conducen (Q1 y Q4 apagados)
- Conmutación a 50/60 Hz para generar AC
- Los diodos de protección previenen corrientes inversas

### 4. Transformador Elevador
- Eleva la tensión de 12V AC a 220V AC
- Relación de transformación: ~18:1
- Centro tap conectado a tierra común

### 5. Filtrado
- Capacitores C1 y C2 (1000µF) filtran la tensión DC de entrada
- Reducen el riple y estabilizan la alimentación

## Especificaciones Técnicas

| Parámetro | Valor |
|-----------|-------|
| Tensión de entrada | 12V DC |
| Tensión de salida | 220V AC |
| Frecuencia de salida | 50/60 Hz |
| Potencia máxima | ~100-500W (según MOSFETs) |
| Forma de onda | Senoidal modificada/SPWM |
| Eficiencia | ~70-85% |

## Código Arduino (Ejemplo Básico)

```cpp
// Pines de salida PWM
const int pin1 = 8;  // Q1
const int pin2 = 9;  // Q2
const int pin3 = 10; // Q3
const int pin4 = 11; // Q4

// Frecuencia de la onda (50Hz)
const int frecuencia = 50;
const int periodo = 1000000 / frecuencia; // microsegundos

void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
}

void loop() {
  // Semiciclo positivo
  digitalWrite(pin1, HIGH); // Q1 ON
  digitalWrite(pin4, HIGH); // Q4 ON
  digitalWrite(pin2, LOW);  // Q2 OFF
  digitalWrite(pin3, LOW);  // Q3 OFF
  delayMicroseconds(periodo / 2);
  
  // Semiciclo negativo
  digitalWrite(pin1, LOW);  // Q1 OFF
  digitalWrite(pin4, LOW);  // Q4 OFF
  digitalWrite(pin2, HIGH); // Q2 ON
  digitalWrite(pin3, HIGH); // Q3 ON
  delayMicroseconds(periodo / 2);
}
```

## Notas de Seguridad ⚠️

1. **PELIGRO DE ELECTROCUCIÓN**: La salida de 220V AC puede ser MORTAL
2. Usar disipadores de calor adecuados en los MOSFETs
3. Nunca tocar el circuito mientras está energizado
4. Verificar todas las conexiones antes de energizar
5. Usar fusibles de protección en la entrada y salida
6. Implementar dead-time entre conmutaciones para evitar cortocircuitos
7. Probar primero con cargas pequeñas (bombilla de 40W)
8. Mantener buena ventilación del sistema

## Mejoras Posibles

1. **SPWM (Sine PWM)**: Implementar modulación senoidal pura
2. **Control de Frecuencia**: Permitir ajuste 50/60 Hz
3. **Protección de sobrecarga**: Sensor de corriente ACS712
4. **Display LCD**: Mostrar voltaje, corriente y potencia
5. **Control de ventilador**: Activar según temperatura
6. **Protección de batería baja**: Apagado automático < 10.5V
7. **Arranque suave**: Rampa de inicio progresivo

## Referencias y Materiales

- Hoja de datos IRF540N MOSFET
- Hoja de datos optoacoplador 4N25
- Tutorial Arduino PWM avanzado
- Cálculos de transformadores de potencia

---

**ADVERTENCIA**: Este esquema es para fines educativos. La construcción de inversores de potencia requiere conocimientos avanzados de electrónica y puede ser peligroso. Consulte con un profesional antes de construir este circuito.

**Autor**: Generado para proyecto Arduino Inverter  
**Fecha**: 2025-11-06  
**Versión**: 1.0
