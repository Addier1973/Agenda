# Inversor de Onda Sinusoidal Pura 24V DC a 120V AC
## Alta Frecuencia con Transformador de Ferrita

---

## ⚠️ ADVERTENCIAS DE SEGURIDAD

**PELIGRO: ALTO VOLTAJE**
- Este proyecto maneja voltajes potencialmente letales (120V AC)
- Solo debe ser construido por personas con experiencia en electrónica de potencia
- Utilice siempre prácticas de seguridad apropiadas
- Nunca trabaje en el circuito mientras esté energizado
- Use aislamiento adecuado y carcasa con conexión a tierra

---

## 📋 Descripción del Proyecto

Este inversor convierte 24V DC (de baterías) a 120V AC con onda sinusoidal pura utilizando:
- **Modulación SPWM** (Sinusoidal Pulse Width Modulation)
- **Transformador de ferrita** de alta frecuencia (50kHz)
- **Puente H completo** con MOSFETs de potencia
- **Protecciones integradas** (sobrecorriente, bajo voltaje, sobrecarga)

### Características Principales:
- Frecuencia de salida: 60Hz (configurable a 50Hz)
- Frecuencia de conmutación: 50kHz
- Onda sinusoidal pura con bajo contenido armónico
- Soft-start para proteger componentes
- Monitoreo en tiempo real vía Serial
- Sistema de protecciones múltiples

---

## 🔧 Lista de Componentes

### Componentes Principales:

#### Microcontrolador:
- **1x Arduino Nano o Arduino Uno** (ATmega328P)
- Puede usar cualquier Arduino compatible con 16MHz

#### Drivers de MOSFET:
- **2x IR2110** - Driver de medio puente para MOSFETs
- Alternativas: IR2113, IRS2092, IR21844
- Incluyen protección de dead-time

#### MOSFETs de Potencia:
- **4x IRF3205** - MOSFETs canal N (55V, 110A, 8mΩ)
- Alternativas: IRFB4115, IRFP250N, IRFZ44N
- Deben soportar al menos 60V y 20A cada uno

#### Transformador:
- **1x Transformador de ferrita de alta frecuencia**
  - Entrada: 24V (pico a pico)
  - Salida: 170V peak (120V RMS)
  - Frecuencia: 50kHz
  - Potencia: según necesidad (300W - 2000W)
  - Núcleo: Ferrita tipo ETD, EE o toroidal
  - Relación de transformación: aproximadamente 1:7

#### Sensores:
- **1x Sensor de corriente ACS712-20A** o ACS758
- **2x Divisores resistivos** para medición de voltaje
  - Resistencias de alta potencia y precisión
  - Relación 10:1 y 50:1

#### Componentes Pasivos:
- **4x Diodos rápidos** (UF4007 o similar) - para protección
- **4x Capacitores electrolíticos** 100µF 50V - filtrado
- **2x Capacitores electrolíticos** 2200µF 35V - entrada DC
- **Capacitores cerámicos** 100nF - desacoplo (varios)
- **4x Resistencias gate** 10Ω 1W - para MOSFETs
- **Resistencias para divisores** de voltaje (alta potencia)
- **Inductores de filtro** según diseño de salida

#### Optoacopladores (Recomendado):
- **4x PC817** o 6N137 - para aislamiento galvánico

#### Disipadores y Ventilación:
- **Disipadores térmicos** para MOSFETs e IR2110
- **Ventilador** 12V (opcional pero recomendado)
- **Pasta térmica** de calidad

#### PCB y Montaje:
- **PCB de doble cara** o diseño custom
- **Borneras de conexión** para entrada/salida
- **Fusibles** 30A para entrada DC
- **Interruptor** de potencia
- **Carcasa** con ventilación y tierra

#### Alimentación Auxiliar:
- **Fuente 12V aislada** para IR2110 (bootstrap o fuente dedicada)
- **Regulador 5V** para Arduino (puede usar el USB o un 7805)

---

## 📐 Diagrama de Conexión

```
                                    +24V BATERÍA
                                        |
                                        |
                    +-------------------+-------------------+
                    |                   |                   |
                   [FUSIBLE 30A]    [C1: 2200µF]       [Sensor Corriente]
                    |                   |                   |
                    |                   GND                 A1 (Arduino)
                    |
      +-------------+-------------+
      |                           |
   [MOSFET Q1]              [MOSFET Q3]
   (High Side A)            (High Side B)
      |                           |
      |         TRANSFORMADOR     |
      +-------[  PRIMARIO   ]-----+
      |         DE FERRITA        |
      |         (50kHz)           |
   [MOSFET Q2]              [MOSFET Q4]
   (Low Side A)             (Low Side B)
      |                           |
      +-------------+-------------+
                    |
                   GND

    
    SECUNDARIO DEL TRANSFORMADOR
           |           |
           +-[Rectif.]-+--[Filtro]---> 120V AC OUT
                       |
                      GND


    CONTROL (Arduino):
    
    Pin 9  (OC1A) --[R 10Ω]---> IR2110 #1 (IN) --> Q1 Gate
    Pin 10 (OC1B) --[R 10Ω]---> IR2110 #1 (IN) --> Q2 Gate
    Pin 3  (OC2B) --[R 10Ω]---> IR2110 #2 (IN) --> Q3 Gate
    Pin 11 (OC2A) --[R 10Ω]---> IR2110 #2 (IN) --> Q4 Gate
    
    Pin 7 --> ENABLE (control de drivers)
    Pin 8 <-- OVERLOAD (entrada de protección)
    Pin 13 --> LED Status
    
    Pin A0 <-- Divisor Voltaje Salida (50:1)
    Pin A1 <-- Sensor Corriente (ACS712)
    Pin A2 <-- Divisor Voltaje Batería (10:1)
```

### Configuración del Puente H:

```
    Q1 y Q2 forman el Lado A (semi-puente)
    Q3 y Q4 forman el Lado B (semi-puente)
    
    Operación:
    - Semi-ciclo positivo: Q1 conduce (PWM), Q4 conduce (complementario)
    - Semi-ciclo negativo: Q3 conduce (PWM), Q2 conduce (complementario)
    - NUNCA Q1 y Q2 conducen simultáneamente (cortocircuito)
    - NUNCA Q3 y Q4 conducen simultáneamente (cortocircuito)
```

---

## 🔌 Conexiones Detalladas

### Arduino a Drivers IR2110:

```
Driver IR2110 #1 (Lado A):
  Pin 1 (VCC)   --> 12V
  Pin 2 (HIN)   --> Arduino Pin 9 (a través de R 10Ω)
  Pin 3 (LIN)   --> Arduino Pin 10 (a través de R 10Ω)
  Pin 4 (COM)   --> GND
  Pin 5 (LO)    --> Gate Q2 + Resistencia 10Ω
  Pin 6 (VS)    --> Source Q1 + Capacitor Bootstrap
  Pin 7 (HO)    --> Gate Q1 + Resistencia 10Ω
  Pin 8 (VB)    --> +12V Bootstrap (a través de diodo rápido)

Driver IR2110 #2 (Lado B):
  (Conexión similar para Q3 y Q4)
```

### Sensores:

```
Sensor de Corriente (ACS712):
  VCC --> 5V Arduino
  GND --> GND
  OUT --> A1 Arduino
  IP+ --> Línea positiva principal
  IP- --> A carga

Divisor Voltaje Batería:
  +24V --[R1: 90kΩ]--+--[R2: 10kΩ]-- GND
                     |
                     +----> A2 Arduino
                     
Divisor Voltaje Salida:
  120V --[R1: 490kΩ]--+--[R2: 10kΩ]-- GND
                      |
                      +----> A0 Arduino
  (¡Usar resistencias de alta potencia!)
```

---

## 💻 Instalación y Uso

### 1. Cargar el Código:
```bash
# Abrir Arduino IDE
# Cargar el archivo: inversor_sinusoidal_24v_120v.ino
# Seleccionar placa: Arduino Nano/Uno
# Seleccionar puerto COM correcto
# Compilar y subir
```

### 2. Configuración Inicial:
- Verificar todas las conexiones antes de energizar
- Conectar batería de 24V (desconectada de carga)
- Abrir Monitor Serial (115200 baud)
- El sistema iniciará en modo apagado

### 3. Comandos Seriales:
- `ON` - Encender inversor
- `OFF` - Apagar inversor
- `STATUS` - Ver estado del sistema
- `RESET` - Resetear flags de error

### 4. Monitoreo:
El sistema muestra cada segundo:
```
Batería: 24.5V | Salida: 118.2V | Corriente: 2.34A
```

---

## ⚙️ Ajustes y Calibración

### Cambiar frecuencia de salida (50Hz o 60Hz):
```cpp
#define OUTPUT_FREQ   60  // Cambiar a 50 para países con 50Hz
```

### Ajustar voltaje objetivo:
```cpp
#define TARGET_VOLTAGE   120.0  // Cambiar según necesidad
```

### Ajustar protecciones:
```cpp
#define MAX_CURRENT      10.0   // Corriente máxima en Amperios
#define MIN_BATTERY_V    20.0   // Voltaje mínimo de batería
#define MAX_BATTERY_V    29.0   // Voltaje máximo de batería
```

### Calibrar sensores:
Ajustar factores de conversión en la función `readSensors()`:
```cpp
batteryVoltage = (batteryRaw * 5.0 / 1023.0) * 10.0; // Ajustar multiplicador
outputCurrent = ((currentRaw * 5.0 / 1023.0) - 2.5) / 0.185; // Ajustar según sensor
```

---

## 🛡️ Protecciones Implementadas

1. **Sobrecorriente**: Apagado inmediato si corriente > MAX_CURRENT
2. **Bajo voltaje de batería**: Previene descarga profunda
3. **Sobrecarga externa**: Pin de entrada para protección adicional
4. **Soft-start**: Inicio gradual para proteger componentes
5. **Dead-time**: Los drivers IR2110 previenen conducción simultánea

---

## 🔍 Solución de Problemas

### El inversor no enciende:
- Verificar voltaje de batería (debe ser > 20V)
- Verificar comando "ON" vía Serial
- Comprobar conexión del pin ENABLE

### Voltaje de salida bajo:
- Revisar relación de transformación
- Verificar duty cycle del PWM
- Comprobar carga del transformador

### Sobrecorriente frecuente:
- Reducir carga conectada
- Verificar cortocircuitos
- Comprobar MOSFETs dañados

### Ruido en la salida:
- Mejorar filtrado de salida
- Verificar conexiones a tierra
- Agregar capacitores de snubber

### Calentamiento excesivo:
- Mejorar disipación térmica
- Verificar ventilación
- Reducir frecuencia de conmutación

---

## 📊 Especificaciones Técnicas

| Parámetro | Valor |
|-----------|-------|
| Voltaje de entrada | 24V DC (20-29V) |
| Voltaje de salida | 120V AC RMS |
| Frecuencia de salida | 60Hz (50Hz opcional) |
| Frecuencia de conmutación | 50kHz |
| Tipo de onda | Sinusoidal pura |
| THD (Distorsión armónica) | < 3% (teórico) |
| Eficiencia esperada | 85-92% |
| Potencia | Depende de componentes (500W-2000W) |
| Protecciones | Sobrecorriente, bajo voltaje |

---

## 🧮 Cálculos del Transformador

### Relación de Transformación:
```
Vpeak_salida = 120V × √2 = 170V
Vpeak_entrada = 24V

N = Vpeak_salida / Vpeak_entrada = 170 / 24 ≈ 7:1
```

### Número de Vueltas (ejemplo para núcleo ETD39):
```
Primario: N1 = 15 vueltas (cable AWG 12-14)
Secundario: N2 = 105 vueltas (cable AWG 18-20)
```

### Área del Núcleo:
```
Para 1000W a 50kHz:
Ae × Aw ≥ P / (4.44 × f × Bmax × J × η)

Donde:
- P = Potencia (W)
- f = Frecuencia (Hz)
- Bmax = Densidad de flujo máxima (0.2-0.3T para ferrita)
- J = Densidad de corriente (3-5 A/mm²)
```

---

## 🔄 Mejoras Futuras

1. **Regulación de voltaje**: Implementar feedback para mantener 120V constantes
2. **Control PID**: Para mejor respuesta ante cambios de carga
3. **Display LCD**: Mostrar información sin necesidad de PC
4. **WiFi/Bluetooth**: Control remoto del inversor
5. **Medición de THD**: Análisis de calidad de onda en tiempo real
6. **Múltiples modos**: Eco, Normal, Boost
7. **Sincronización de fase**: Para conexión a red (grid-tie)

---

## 📚 Referencias y Recursos

### Hojas de datos:
- [IR2110 Datasheet](https://www.infineon.com/dgdl/ir2110.pdf)
- [IRF3205 Datasheet](https://www.infineon.com/dgdl/irf3205.pdf)
- [ACS712 Datasheet](https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712)

### Lecturas recomendadas:
- "Power Electronics: Converters, Applications and Design" - Mohan, Undeland, Robbins
- Application Notes de International Rectifier (Infineon)
- Arduino PWM Secrets

### Comunidades:
- Arduino Forum
- EEVblog Forum
- DIY Audio Forum

---

## 📜 Licencia

Este proyecto se proporciona "TAL CUAL" sin garantías de ningún tipo.
El autor no se hace responsable de daños a equipos o lesiones personales.

**Use bajo su propio riesgo y responsabilidad.**

---

## ✍️ Notas del Desarrollador

Este código es un punto de partida para un inversor de onda sinusoidal pura.
Requiere ajustes según los componentes específicos utilizados.

**Recomendaciones importantes:**
1. Probar primero con cargas resistivas pequeñas (bombillas)
2. Usar osciloscopio para verificar forma de onda
3. Medir THD con analizador de espectro
4. No conectar equipos sensibles hasta verificar calidad de onda
5. Implementar todas las protecciones de hardware adicionales

**¡Buena suerte con tu proyecto!**

---

*Última actualización: 2025-11-05*
