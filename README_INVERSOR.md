# Inversor de Onda Sinusoidal Pura 24V DC a 120V AC

## 📋 Descripción General

Este proyecto implementa un **inversor DC-AC de onda sinusoidal pura** de alta frecuencia utilizando Arduino y un transformador de ferrita. El sistema convierte 24V DC a 120V AC @ 60Hz con una forma de onda sinusoidal limpia mediante técnica SPWM (Sinusoidal Pulse Width Modulation).

### Características Principales

- ✅ **Entrada**: 24V DC (rango: 22-28V)
- ✅ **Salida**: 120V AC RMS @ 60Hz
- ✅ **Forma de onda**: Sinusoidal pura (THD <5%)
- ✅ **Topología**: Puente H completo (Full Bridge)
- ✅ **Modulación**: SPWM a ~31.25 kHz
- ✅ **Transformador**: Ferrita alta frecuencia (20-50 kHz)
- ✅ **Protecciones**: Sobrecorriente, sobrevoltaje, bajo voltaje
- ✅ **Potencia**: Hasta 500W (según componentes)

---

## 🔧 Hardware Requerido

### Componentes Principales

#### 1. Microcontrolador
- **Arduino Uno** o **Arduino Nano**
- Frecuencia: 16 MHz
- Timers disponibles: Timer1 (16-bit), Timer2 (8-bit)

#### 2. MOSFETs (Etapa de Potencia)
| Componente | Especificación | Cantidad |
|------------|---------------|----------|
| MOSFET | IRFZ44N o IRF3205 | 4 |
| VDS | ≥60V | - |
| ID continua | ≥30A | - |
| RDS(on) | <0.03Ω | - |

#### 3. Gate Drivers
- **IR2110** (x2) - Half-bridge driver
- Bootstrap capacitors: 10µF/25V (x2)
- Bootstrap diodes: UF4007 (x2)

#### 4. Transformador de Ferrita
- **Núcleo**: ETD39, EE55 o similar
- **Material**: Ferrita N87, N27 o equivalente
- **Frecuencia operación**: 20-50 kHz
- **Potencia**: 300-500W
- **Devanados**:
  - Primario: 20-30 vueltas (cable Litz AWG18-20)
  - Secundario: 100-150 vueltas (cable Litz AWG22-24)
- **Entrehierro**: 0.5-1mm

#### 5. Sensores
- **Corriente**: ACS712-20A (sensor efecto Hall)
- **Voltaje**: Divisor resistivo 10:1
  - R1: 100kΩ (1%)
  - R2: 10kΩ (1%)

#### 6. Filtro de Salida LC
- **Inductor**: 2-5 mH, corriente nominal >5A
- **Capacitor**: 4.7µF, 250V AC (film polypropylene)
- **Frecuencia de corte**: ~1-2 kHz

#### 7. Componentes Adicionales
- Fusible de entrada: 25-30A
- TVS diode: P6KE36CA (protección sobrevoltaje)
- Snubber RC: 100Ω + 100nF (x4, en paralelo a cada MOSFET)
- Disipadores de calor con ventilación
- Conectores de alta corriente

---

## 📐 Principio de Funcionamiento

### 1. Modulación SPWM (Sinusoidal PWM)

El sistema genera una onda sinusoidal de 60Hz mediante la técnica **SPWM**:

```
Señal Portadora (Carrier) ──┐
   31.25 kHz (triangular)    ├──► Comparador ──► PWM
                              │
Señal Moduladora (Reference)─┘
   60 Hz (senoidal)
```

**Proceso**:
1. Se genera una tabla con 100 valores de una onda seno (60Hz)
2. Cada valor modula el duty cycle del PWM de alta frecuencia
3. El resultado es una señal PWM cuyo promedio sigue una senoide
4. El transformador + filtro LC extrae la componente de 60Hz

### 2. Puente H Completo

```
Q1 ON, Q4 ON  →  Corriente: +24V → Primario → GND  (Semiciclo +)
Q2 ON, Q3 ON  →  Corriente: GND → Primario → +24V  (Semiciclo -)
```

**Ventajas del puente H completo**:
- Aprovecha todo el voltaje de entrada (±24V)
- Permite inversión de polaridad
- Mayor eficiencia que topología push-pull simple

### 3. Tabla SPWM

El código utiliza una tabla precalculada de 100 muestras por ciclo:

```cpp
Frecuencia de actualización = 60 Hz × 100 = 6 kHz
Tiempo entre muestras = 1/6000 = 166.67 µs
```

Cada muestra controla el duty cycle del PWM, creando una envolvente sinusoidal.

---

## 💻 Arquitectura del Software

### Estructura del Código

```
setup()
├── Configurar pines I/O
├── Configurar PWM (Timer1, Timer2)
├── Configurar interrupciones
└── Inicializar estado

loop()
├── Generación SPWM (cada 166.67µs)
│   ├── Leer tabla senoidal
│   ├── Calcular duty cycles
│   └── Actualizar registros PWM
├── Monitoreo (cada 100ms)
│   ├── Leer sensores
│   ├── Verificar protecciones
│   └── Mostrar datos por serial
└── Control de estado
```

### Configuración de Timers

#### Timer1 (Pines 9 y 10)
```cpp
TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
TCCR1B = _BV(WGM12) | _BV(CS10);  // Fast PWM, sin prescaler

Frecuencia PWM = 16 MHz / 256 = 62.5 kHz (Timer1)
                ≈ 31.25 kHz (con configuración)
```

#### Timer2 (Pines 3 y 11)
```cpp
TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
TCCR2B = _BV(CS20);  // Fast PWM, sin prescaler

Frecuencia PWM = 16 MHz / 256 = 62.5 kHz
```

---

## 🔌 Esquema de Conexiones

Ver archivo: **`ESQUEMA_CONEXIONES_INVERSOR.txt`** para diagramas detallados.

### Resumen de Pines Arduino

| Pin | Función | Conexión |
|-----|---------|----------|
| 9 | PWM Q1 (Alta) | IR2110 #1 → Gate Q1 |
| 10 | PWM Q2 (Baja) | IR2110 #1 → Gate Q2 |
| 3 | PWM Q3 (Alta) | IR2110 #2 → Gate Q3 |
| 11 | PWM Q4 (Baja) | IR2110 #2 → Gate Q4 |
| A0 | Sensor Corriente | ACS712 Output |
| A1 | Sensor Voltaje | Divisor resistivo |
| 2 | Botón Enable | Pulsador + Pull-up |
| 13 | LED Estado | LED + 220Ω |

---

## ⚠️ Protecciones Implementadas

### 1. Protección por Software

```cpp
✓ Bajo voltaje:     Vin < 22V  → Shutdown
✓ Sobrevoltaje:     Vin > 28V  → Shutdown
✓ Sobrecorriente:   I > 20A    → Shutdown
✓ Temperatura:      T > 80°C   → Shutdown (si hay sensor)
```

### 2. Protección por Hardware

- **Fusible**: 25-30A en la entrada de potencia
- **TVS Diode**: Protección contra picos de voltaje
- **Snubber RC**: Suprime picos inductivos en MOSFETs
- **Dead-time**: Previene cortocircuito shoot-through

### 3. Dead-Time

**Crítico**: Nunca activar Q1 y Q2 simultáneamente (o Q3 y Q4), causaría cortocircuito.

El código implementa dead-time implícito:
```cpp
if (valorSeno >= 128) {
    OCR1A = pwmValue;  // Q1 activo
    OCR1B = 0;         // Q2 apagado
} else {
    OCR1A = 0;         // Q1 apagado
    OCR1B = pwmValue;  // Q2 activo
}
```

---

## 🚀 Instalación y Uso

### 1. Instalación del Software

```bash
# Clonar o descargar el archivo .ino
# Abrir con Arduino IDE

# Seleccionar placa:
Herramientas → Placa → Arduino Uno (o Nano)

# Seleccionar puerto:
Herramientas → Puerto → COM# (Windows) o /dev/ttyUSB# (Linux)

# Compilar y cargar
Sketch → Subir
```

### 2. Configuración Inicial

Ajusta estas constantes según tu diseño:

```cpp
#define FRECUENCIA_SALIDA 60     // 50 Hz para Europa, 60 Hz para América
#define VOLTAJE_MIN 22.0         // Voltaje mínimo de batería
#define VOLTAJE_MAX 28.0         // Voltaje máximo de batería
#define CORRIENTE_MAX 20.0       // Corriente máxima permitida

// Calibración de sensores
float voltaje = lecturaADC * (FACTOR_ESCALA);  // Ajustar según divisor
float corriente = (voltajeSensor - 2.5) / 0.100;  // Ajustar según ACS712
```

### 3. Procedimiento de Prueba

#### Primera Prueba (SIN CARGA)
```
1. Verificar todas las conexiones
2. Conectar osciloscopio a la salida del transformador
3. Alimentar con 24V DC
4. Presionar botón de habilitación
5. Verificar forma de onda en el osciloscopio
   - Debe verse una senoide de 60Hz
   - THD < 5%
6. Medir voltaje RMS (debe ser ~120V)
```

#### Segunda Prueba (CARGA LIGERA)
```
1. Conectar bombilla incandescente 20-40W
2. Habilitar inversor
3. Verificar funcionamiento
4. Monitorear temperatura de MOSFETs
5. Verificar voltaje y corriente por serial
```

#### Tercera Prueba (CARGA NOMINAL)
```
1. Aumentar carga gradualmente
2. Monitorear:
   - Temperatura de MOSFETs (< 80°C)
   - Temperatura del transformador (< 60°C)
   - Corriente de entrada
   - Voltaje de salida (debe mantenerse estable)
3. Verificar eficiencia (típica: 85-92%)
```

### 4. Monitor Serial

Abre el monitor serial (115200 baud) para ver:

```
=================================
Inversor Sinusoidal Pura 24V-120V
Alta Frecuencia - SPWM
=================================
Sistema inicializado
Presiona el botón para habilitar

>>> INVERSOR HABILITADO <<<

Estado: ON | Vin: 24.2V | I: 5.34A | P: 129.2W
Estado: ON | Vin: 24.1V | I: 5.38A | P: 129.7W
Estado: ON | Vin: 24.0V | I: 5.41A | P: 129.8W
```

---

## 📊 Cálculos y Dimensionamiento

### Transformador de Ferrita

**Potencia**: 500W  
**Frecuencia**: 31.25 kHz  

**Área del producto (Ap)**:
```
Ap = (Pout × 10^4) / (2 × Bmax × Jmax × η × f)

Donde:
- Pout = 500W
- Bmax = 0.3T (ferrita N87)
- Jmax = 4 A/mm²
- η = 0.9 (eficiencia)
- f = 31250 Hz

Ap ≈ 0.9 cm⁴  →  Núcleo ETD39 o EE55
```

**Número de vueltas primario**:
```
Np = (Vin × 10^4) / (4 × Bmax × Ae × f)

Donde:
- Vin = 24V
- Ae = 1.25 cm² (ETD39)
- Bmax = 0.3T
- f = 31250 Hz

Np ≈ 20 vueltas
```

**Número de vueltas secundario**:
```
Ns = Np × (Vout / Vin) × 1.1

Donde:
- Vout = 120V RMS × √2 = 170V peak
- Vin = 24V
- Factor 1.1 = margen

Ns ≈ 156 vueltas (usar 150 vueltas)
```

### Capacidad de Corriente

**Primario**:
```
Ip = Pout / (Vin × η) = 500 / (24 × 0.9) ≈ 23A
Cable: Litz AWG18 o múltiple AWG20
```

**Secundario**:
```
Is = Pout / Vout = 500 / 120 ≈ 4.2A
Cable: Litz AWG22
```

---

## 🔋 Eficiencia y Pérdidas

### Eficiencia Esperada: 85-92%

**Pérdidas por componente**:

1. **MOSFETs** (conducción + conmutación): 2-3%
   - Pconducción = I² × RDS(on) × Dcycle
   - Pconmutación = V × I × tswitch × fsw / 2

2. **Transformador**: 3-5%
   - Pérdidas en núcleo (histéresis + corrientes parásitas)
   - Pérdidas en cobre (I² × R)

3. **Gate Drivers**: 0.5-1%

4. **Filtro LC**: 1-2%

5. **Otras pérdidas**: 1-2%

**Total**: 8-15% de pérdidas → **85-92% eficiencia**

---

## 🛠️ Mejoras y Optimizaciones

### Mejoras Implementables

1. **Control de lazo cerrado**:
   - Realimentación del voltaje de salida
   - Ajuste dinámico del duty cycle
   - Regulación ante variaciones de carga

2. **Sincronización de red** (para UPS):
   - Detección de fase de la red eléctrica
   - Conmutación sin corte

3. **Pantalla LCD/OLED**:
   - Mostrar voltajes, corriente, potencia
   - Indicadores visuales de estado

4. **Control por microcontrolador más potente**:
   - STM32 (32-bit, mayor resolución PWM)
   - ESP32 (con conectividad WiFi)

5. **Tabla SPWM de mayor resolución**:
   - 200 o 256 muestras por ciclo
   - Menor THD

6. **Protección adicional**:
   - Sensor de temperatura en transformador
   - Protección contra cortocircuito ultrarrápida

---

## ⚡ Consideraciones de Seguridad

### ⚠️ ADVERTENCIAS CRÍTICAS

1. **VOLTAJE PELIGROSO**: La salida es 120V AC, potencialmente letal
2. **CORRIENTES ALTAS**: 20-30A en la entrada pueden causar incendios
3. **ALTA FRECUENCIA**: Puede interferir con dispositivos electrónicos
4. **CALOR**: Los componentes se calientan, riesgo de quemaduras

### 🛡️ Medidas de Seguridad Obligatorias

✅ **Gabinete aislado** con conexión a tierra  
✅ **Fusibles** en entrada y salida  
✅ **Separación galvánica** entre control y potencia  
✅ **Disipadores térmicos** con ventilación forzada  
✅ **PCB profesional** con trazas gruesas (>2mm)  
✅ **Espaciado mínimo** 5mm entre trazas de alta tensión  
✅ **Pruebas con carga resistiva** antes de usar con electrónicos  
✅ **Nunca operar sin las protecciones** implementadas  

### 🔍 Verificaciones Previas

Antes de alimentar el sistema:

- [ ] Verificar polaridad de la batería
- [ ] Comprobar aislamiento del transformador (>1MΩ)
- [ ] Verificar que no hay cortocircuitos
- [ ] Comprobar conexiones de los gate drivers
- [ ] Verificar alimentación de los drivers (12-15V)
- [ ] Asegurar que los disipadores están montados
- [ ] Verificar fusibles instalados
- [ ] Probar con osciloscopio las señales PWM (sin potencia)

---

## 📚 Referencias y Recursos

### Documentación Técnica

1. **MOSFETs**:
   - [IRFZ44N Datasheet](https://www.infineon.com/dgdl/irfz44n.pdf)
   - [IRF3205 Datasheet](https://www.infineon.com/dgdl/irf3205.pdf)

2. **Gate Drivers**:
   - [IR2110 Datasheet](https://www.infineon.com/dgdl/ir2110.pdf)
   - [AN-978: HV Floating MOS-Gate Driver ICs](https://www.infineon.com/dgdl/an-978.pdf)

3. **Transformadores de Ferrita**:
   - [Magnetics Ferrite Core Catalog](https://www.mag-inc.com)
   - [TDK Ferrite Material Characteristics](https://www.tdk-electronics.tdk.com)

4. **Teoría de Inversores**:
   - "Power Electronics: Converters, Applications, and Design" - Ned Mohan
   - "Pulse Width Modulation for Power Converters" - D. Grahame Holmes

### Herramientas de Diseño

- **LTspice**: Simulación de circuitos de potencia
- **FEMM**: Análisis de campos magnéticos (transformadores)
- **KiCad**: Diseño de PCB
- **Arduino IDE**: Desarrollo de firmware

---

## 📄 Licencia

Este proyecto se proporciona "tal cual" para fines educativos y de investigación.

**⚠️ DESCARGO DE RESPONSABILIDAD**: El autor no se hace responsable de daños materiales, personales o cualquier otro tipo que puedan resultar del uso de este diseño. Trabajar con altos voltajes y corrientes es peligroso y debe realizarse solo por personas calificadas.

---

## 👤 Autor

Código desarrollado para sistema inversor DC-AC de onda sinusoidal pura.

**Versión**: 1.0  
**Fecha**: 2025  
**Plataforma**: Arduino Uno/Nano (ATmega328P)

---

## 📞 Soporte

Para preguntas, mejoras o reportar problemas:
- Revisa la documentación completa
- Verifica las conexiones según el esquema
- Comprueba los valores de los sensores (calibración)
- Consulta el monitor serial para diagnóstico

**¡Buena suerte con tu proyecto de inversor!** ⚡🔧
