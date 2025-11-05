# Inversor de Onda Sinusoidal Pura 24V DC a 120V AC

## Descripción
Este proyecto implementa un inversor de onda sinusoidal pura de alta frecuencia usando Arduino y un transformador de ferrita. Convierte 24V DC a 120V AC a 60Hz mediante la técnica SPWM (Sinusoidal Pulse Width Modulation).

## Características
- **Entrada:** 24V DC
- **Salida:** 120V AC, 60Hz
- **Forma de onda:** Sinusoidal pura (THD < 5%)
- **Frecuencia PWM:** ~31.25 kHz (óptimo para transformador de ferrita)
- **Topología:** Puente H completo (Full-Bridge)
- **Control:** Modulación SPWM con 256 muestras por ciclo

## Componentes Necesarios

### Electrónica de Control
- 1x Arduino Nano/Uno (ATmega328P)
- 2x Driver de MOSFET (IR2110, IR2184, o IRS2092)
- Fuente de alimentación aislada 12-15V para drivers (bootstrap)

### Electrónica de Potencia
- 4x MOSFETs de potencia N-channel:
  - IRF3205 (55V, 110A, RDS(on)=8mΩ) **RECOMENDADO**
  - IRFZ44N (55V, 49A, RDS(on)=17.5mΩ)
  - O similar con Vds > 60V, Id > 20A
- 4x Diodos ultrarrápidos (UF5408 o similar, 3A, 1000V)
- Disipadores de calor para MOSFETs

### Transformador
- Transformador de ferrita de alta frecuencia:
  - Primario: 24V-0-24V (center-tap) o doble devanado 24V
  - Secundario: 120V-140V
  - Potencia: Según necesidad (500W, 1000W, etc.)
  - Núcleo: Ferrita ETD39, ETD49 o similar
  - Frecuencia de operación: 25-50 kHz

### Filtrado y Protección
- Condensadores de filtro de entrada:
  - 2-4x Electrolíticos 2200µF/50V (bajo ESR)
- Filtro LC de salida:
  - Inductor: 1-2 mH, corriente nominal según potencia
  - Condensador: 1-4.7µF/250V (film, X2 o similar)
- Fusibles:
  - Entrada: 20-30A (según potencia)
  - Salida: 2-5A (según potencia)
- Varistores (MOV) para protección de sobretensión

### Adicionales (Opcionales pero Recomendados)
- Sensor de voltaje de entrada (divisor resistivo)
- Sensor de voltaje de salida (transformador de voltaje o divisor con aislamiento)
- Sensor de corriente (ACS712 o similar)
- Ventilador de enfriamiento (12V)
- Display LCD 16x2 para monitoreo
- Botón de encendido/apagado

## Diagrama de Conexiones

```
                          ARDUINO NANO/UNO
                          ┌───────────┐
                     5V ──┤VIN     D13├── LED Status
                    GND ──┤GND     D12├──
                          │        D11├── PWM_B_LOW → Driver Q4 (Low-side B)
   Enable Button ────────┤D7      D10├── PWM_A_LOW → Driver Q2 (Low-side A)
                          │         D9├── PWM_A_HIGH → Driver Q1 (High-side A)
                          │         D3├── PWM_B_HIGH → Driver Q3 (High-side B)
                          └───────────┘

                      PUENTE H (H-BRIDGE)
                      
                 +24V ────────────────────────── +24V
                          │              │
                         Q1             Q3
                    (High-A)       (High-B)
                          │              │
                          ├──────┬───────┤
                          │      │       │
                    Primario     │   Primario
                          │      │       │
                          ├──────┴───────┤
                          │              │
                         Q2             Q4
                    (Low-A)        (Low-B)
                          │              │
                 GND ─────┴──────────────┴────── GND

        Transformador de Ferrita
        ┌─────────────┐
    ────┤ Primario    │
    ────┤   24V-0-24V │
        │             │
        │  Secundario ├──── 120V AC ──┐
        │     120V    ├──── (Salida)  │
        └─────────────┘                │
                                       │
                              ┌────────┴────────┐
                              │  Filtro LC      │
                              │  L=1mH          │
                              │  C=2.2µF/250V   │
                              └─────────────────┘
```

## Esquema de Driver IR2110 (Para cada brazo del puente H)

```
              IR2110 (Driver High-side/Low-side)
              ┌─────────────────┐
        VCC ──┤VCC          VB  ├─┬── Bootstrap Cap (10µF) ──┐
        GND ──┤GND          VS  ├─┤                          │
              │                 │ │                          │
   PWM_HIGH ──┤HIN          HO  ├─┴─→ Gate High-side MOSFET │
   PWM_LOW  ──┤LIN          LO  ├───→ Gate Low-side MOSFET  │
              │                 │                            │
        VCC ──┤VDD          COM ├─── Source High-side ◄──────┘
              └─────────────────┘
              
   Bootstrap Diode: UF4007 desde VCC a VB
```

## Configuración y Calibración

### 1. Verificación Inicial (SIN CARGA)
```cpp
// Antes de conectar el transformador:
1. Verificar alimentación Arduino (5V estable)
2. Verificar alimentación drivers (12-15V)
3. Medir señales PWM con osciloscopio
4. Verificar tiempo muerto entre High/Low side (>200ns)
```

### 2. Prueba con Transformador (SIN CARGA DE SALIDA)
```cpp
1. Conectar transformador al puente H
2. Habilitar inversor brevemente (1-2 segundos)
3. Medir voltaje de salida con multímetro AC
4. Verificar forma de onda con osciloscopio
5. Verificar temperatura de MOSFETs
```

### 3. Prueba con Carga
```cpp
1. Comenzar con carga ligera (lámpara 25W)
2. Aumentar gradualmente la carga
3. Monitorear temperatura constantemente
4. Verificar estabilidad del voltaje de salida
```

### 4. Ajustes Disponibles en el Código

#### Frecuencia de salida AC
```cpp
#define SINE_FREQ 60    // Cambiar a 50 para Europa/Asia
```

#### Frecuencia PWM
```cpp
#define PWM_FREQ 31250  // Ajustar según transformador
// Valores típicos: 20kHz - 50kHz
```

#### Número de muestras por ciclo
```cpp
#define SAMPLES 256     // Más muestras = mejor resolución
                        // Pero mayor carga del procesador
```

#### Tiempo muerto
```cpp
#define DEAD_TIME 20    // Aumentar si hay problemas de
                        // conducción simultánea (shoot-through)
```

## Cálculos de Diseño

### Potencia Máxima (Ejemplo para 500W)
```
Corriente de entrada (24V): I = 500W / 24V = 20.8A
Corriente RMS salida (120V): I = 500W / 120V = 4.16A
Corriente pico salida: Ipk = 4.16A × √2 = 5.9A
```

### Selección de MOSFETs
```
Vds requerido: > 2 × Vin = 2 × 24V = 48V → Usar Vds ≥ 55V
Corriente: Id > 1.5 × Iin = 1.5 × 20.8A = 31.2A
RDS(on): Lo más bajo posible para reducir pérdidas
```

### Pérdidas en MOSFETs
```
Pérdidas de conducción: Pcond = I²rms × RDS(on)
Para IRF3205: Pcond = (20.8A)² × 0.008Ω = 3.46W por MOSFET
Pérdidas de conmutación: Psw ≈ 0.5 × Vds × Id × (tr + tf) × fsw
```

### Transformador de Ferrita
```
Área del núcleo: Ae = (Pout × 10⁴) / (4.44 × f × Bmax × N1)
Donde:
  Pout = Potencia de salida (VA)
  f = Frecuencia PWM (Hz)
  Bmax = Densidad de flujo máx. (0.2-0.3T para ferrita)
  N1 = Número de espiras primario
```

## Precauciones de Seguridad

### ⚠️ ADVERTENCIAS IMPORTANTES

1. **ALTO VOLTAJE:** La salida de 120V AC puede ser LETAL. Tomar todas las precauciones.

2. **AISLAMIENTO:** Usar transformador con aislamiento adecuado (mínimo 3kV).

3. **PROTECCIONES:**
   - Instalar fusibles en entrada y salida
   - Implementar protección de sobrecorriente
   - Implementar protección de sobrevoltaje
   - Implementar protección térmica

4. **VENTILACIÓN:** Asegurar enfriamiento adecuado de todos los componentes de potencia.

5. **PRUEBAS:** 
   - Siempre probar primero sin carga
   - Usar carga resistiva para pruebas iniciales
   - No usar cargas inductivas sin filtrado adicional

6. **CONEXIONES:**
   - Verificar todas las conexiones antes de energizar
   - Usar cables de calibre adecuado (AWG 10-12 para entrada de alta corriente)
   - Conexiones a tierra apropiadas

## Mejoras Posibles

### 1. Control de Voltaje en Lazo Cerrado
```cpp
// Agregar sensor de voltaje y ajuste automático de amplitud PWM
int sensorVoltaje = analogRead(A0);
float voltajeSalida = sensorVoltaje * factor_calibracion;
// Ajustar amplitud según error
```

### 2. Protección de Sobrecorriente
```cpp
// Agregar sensor de corriente ACS712
int sensorCorriente = analogRead(A1);
if (sensorCorriente > LIMITE_CORRIENTE) {
  disableInverter();
  // Activar alarma
}
```

### 3. Control de Frecuencia Programable
```cpp
// Permitir cambio de frecuencia 50/60Hz
void setOutputFrequency(int freq) {
  SINE_FREQ = freq;
  // Recalcular parámetros
}
```

### 4. Arranque Suave (Soft-start)
```cpp
// Incrementar gradualmente la amplitud PWM
void softStart() {
  for(int i = 0; i < 100; i++) {
    amplitud = i;
    delay(10);
  }
}
```

### 5. Display LCD de Información
```cpp
// Mostrar voltaje, corriente, potencia, temperatura
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void displayInfo() {
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltaje);
  lcd.print("V I:");
  lcd.print(corriente);
  lcd.print("A");
}
```

## Solución de Problemas

| Problema | Posible Causa | Solución |
|----------|---------------|----------|
| No hay salida de voltaje | MOSFETs no conmutan | Verificar señales PWM y drivers |
| Voltaje de salida bajo | Duty cycle insuficiente | Ajustar tabla sinusoidal |
| MOSFETs se calientan mucho | RDS(on) alto o frecuencia muy alta | Usar MOSFETs de menor RDS(on), mejorar enfriamiento |
| Forma de onda distorsionada | Filtro LC inadecuado | Ajustar valores L y C del filtro |
| Arduino se resetea | Interferencia electromagnética | Agregar filtros, mejorar layout PCB |
| Shoot-through (cortocircuito) | Tiempo muerto insuficiente | Aumentar DEAD_TIME en código |

## Referencias y Recursos

1. **Application Notes:**
   - AN-978: HV Floating MOS-Gate Driver ICs (International Rectifier)
   - AN-6005: Driving and Layout Design for Fast Switching Super Junction MOSFETs

2. **Diseño de Transformadores:**
   - "Transformer and Inductor Design Handbook" - Colonel Wm. T. McLyman
   - Ferroxcube Soft Ferrites Data Handbook

3. **SPWM:**
   - "Power Electronics" - Daniel W. Hart
   - "Pulse Width Modulation for Power Converters" - Holmes & Lipo

## Licencia
Este proyecto es de código abierto. Úsalo bajo tu propio riesgo.

## Disclaimer
El autor no se hace responsable por daños a personas o equipos derivados del uso de este código. Este es un proyecto educativo que involucra altos voltajes y corrientes. Solo debe ser construido por personas con experiencia en electrónica de potencia y tomando todas las precauciones de seguridad necesarias.

---
**Fecha:** Noviembre 2025  
**Versión:** 1.0  
**Autor:** Generado para proyecto de inversor sinusoidal
