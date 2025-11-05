# INVERSOR DE ONDA SINUSOIDAL PURA 24V → 120V AC

## 📋 Descripción General

Este proyecto implementa un inversor de onda sinusoidal pura que convierte 24V DC a 120V AC usando un transformador de ferrita de alta frecuencia (20kHz) y modulación SPWM (Sinusoidal Pulse Width Modulation).

### Especificaciones Técnicas

- **Entrada:** 24V DC (rango: 22V - 29V)
- **Salida:** 120V AC RMS @ 60Hz (configurable a 50Hz)
- **Frecuencia de conmutación:** 20 kHz
- **Topología:** Puente completo (Full H-Bridge)
- **Distorsión armónica total (THD):** < 5% (con filtro LC adecuado)
- **Eficiencia estimada:** 85-92%

---

## 🛠️ Lista de Componentes

### Componentes Principales

1. **Microcontrolador**
   - Arduino Nano/Uno (ATmega328P)
   - o Arduino Mega (para más funcionalidades)

2. **MOSFETs de Potencia** (4 unidades)
   - IRFZ44N (55V, 49A, RDS(on) = 17.5mΩ) - Económico
   - IRF3205 (55V, 110A, RDS(on) = 8mΩ) - Recomendado
   - IRFB4110 (100V, 180A, RDS(on) = 3.7mΩ) - Alto rendimiento

3. **Drivers de MOSFET** (2 unidades mínimo)
   - IR2110 (driver de medio puente con bootstrap)
   - IR2184 (alternativa económica)
   - UCC27211 (opción moderna)

4. **Transformador de Ferrita**
   - Núcleo: ETD49, EE55 o similar
   - Material: N87, 3C90 o equivalente (para ~20kHz)
   - Relación de vueltas: 1:6 o 1:7 (24V → 144V o 168V antes del filtro)
   - Potencia: según necesidad (500W, 1000W, etc.)

5. **Componentes del Filtro LC**
   - Inductor: 5-10 mH, corriente nominal según carga
   - Capacitor: 10-20 µF, 250V AC (tipo X2 o capacitor de película)
   - Valores exactos dependen de la frecuencia de corte deseada

6. **Componentes Auxiliares**
   - 4x Diodos rápidos (UF4007 o similar) para bootstrap
   - 4x Capacitores bootstrap: 10µF/50V
   - Resistencias gate: 10-22Ω (4 unidades)
   - Optoacopladores para aislamiento (opcional pero recomendado)
   - Sensor de corriente: ACS712 (20A o 30A)
   - Divisor de voltaje para monitoreo (resistencias precisión 1%)

7. **Protección y Enfriamiento**
   - Disipador de calor con ventilador
   - Pasta térmica
   - Fusible 40A para entrada 24V
   - Protección térmica (termistor NTC)

---

## 🔌 Diagrama de Conexiones

### Conexiones del Arduino

```
Arduino          Conexión
────────────────────────────────────────
Pin 9     →     IR2110 #1 (IN para MOSFETs Q1/Q2)
Pin 10    →     IR2110 #2 (IN para MOSFETs Q3/Q4)
Pin 5     →     MOSFET Q2 Gate (lado bajo A)
Pin 6     →     MOSFET Q4 Gate (lado bajo B)

Pin A0    →     Sensor ACS712 OUT
Pin A1    →     Divisor de voltaje (24V → 5V)
Pin 2     →     Botón de encendido (con pull-up)
Pin 13    →     LED indicador

GND       →     GND común (tierra de señal)
5V        →     Alimentación sensores
```

### Esquema del H-Bridge

```
                    24V DC
                      |
          ┌───────────┴───────────┐
          |                       |
        [Q1]                    [Q3]
      (High-A)               (High-B)
          |                       |
          ├───────────┬───────────┤
          |           |           |
    ┌─────┴─────┐     |     ┌─────┴─────┐
    |  Primario |     |     | Primario  |
    |    TX     |─────┘     |    TX     |
    | Ferrita   |           | Ferrita   |
    └─────┬─────┘           └─────┬─────┘
          |                       |
          ├───────────┬───────────┤
          |           |           |
        [Q2]        GND         [Q4]
      (Low-A)                  (Low-B)
          |                       |
          └───────────┴───────────┘
                      |
                    GND

Q1, Q2 = Rama A del puente
Q3, Q4 = Rama B del puente

Secundario del transformador → Filtro LC → 120V AC
```

### Configuración de Drivers IR2110

**Para IR2110 #1 (controla Q1 y Q2):**
```
VCC (IR2110)  → 5V Arduino (o 12V para mejor rendimiento)
VB            → Bootstrap capacitor (10µF) + diodo desde 24V
VS            → Source de Q1 (punto medio rama A)
HIN           → Pin 9 Arduino
LIN           → Pin 5 Arduino (invertido en software si necesario)
HO            → Gate Q1
LO            → Gate Q2
COM           → GND
```

**Para IR2110 #2 (controla Q3 y Q4):**
```
VCC (IR2110)  → 5V Arduino
VB            → Bootstrap capacitor (10µF) + diodo desde 24V
VS            → Source de Q3 (punto medio rama B)
HIN           → Pin 10 Arduino
LIN           → Pin 6 Arduino
HO            → Gate Q3
LO            → Gate Q4
COM           → GND
```

### Filtro LC de Salida

```
Secundario TX → [L: 10mH] → [C: 10µF] → 120V AC Salida
                               |
                              GND
```

**Cálculo del filtro:**
- Frecuencia de corte: fc = 1 / (2π√(LC))
- Para L=10mH y C=10µF: fc ≈ 500Hz
- Debe estar entre la frecuencia de salida (60Hz) y la de PWM (20kHz)

---

## ⚙️ Configuración del Software

### Parámetros Configurables en el Código

```cpp
#define FRECUENCIA_SALIDA 60        // 60Hz para América, 50Hz para Europa
#define FRECUENCIA_PWM 20000        // 20kHz (ajustar según núcleo)
#define TIEMPO_MUERTO_US 2          // Dead time: 1-5µs típico
#define MUESTRAS_SENO 100           // Resolución SPWM

// Protecciones
#define VOLTAJE_MIN 22.0            // Apagado por batería baja
#define VOLTAJE_MAX 29.0            // Apagado por sobrevoltaje
#define CORRIENTE_MAX 800           // Límite de corriente (calibrar)
```

### Calibración de Sensores

**Sensor de Voltaje (divisor resistivo):**
```cpp
// Ajustar el factor multiplicador según tu divisor
// Ejemplo: R1=68kΩ, R2=10kΩ → Factor = (R1+R2)/R2 = 7.8
voltajeEntrada = (analogRead(PIN_VOLTAJE) * 5.0 / 1023.0) * 7.8;
```

**Sensor de Corriente ACS712:**
```cpp
// ACS712-20A: 100mV/A, offset 2.5V
// ACS712-30A: 66mV/A, offset 2.5V
int valor = analogRead(PIN_CORRIENTE);
float voltaje = (valor * 5.0) / 1023.0;
float corriente = (voltaje - 2.5) / 0.100; // Para ACS712-20A
```

---

## 🚀 Procedimiento de Puesta en Marcha

### Fase 1: Verificación Sin Potencia

1. **Verificar todas las conexiones** con multímetro
2. **Comprobar continuidad** de gates a Arduino
3. **Verificar aislamiento** entre 24V y circuito de control
4. **Inspeccionar soldaduras** y posibles cortocircuitos

### Fase 2: Primera Alimentación

1. Alimentar **solo el Arduino** (sin conectar 24V de potencia)
2. Cargar el código y verificar mensajes por Serial Monitor
3. Verificar que los pines PWM están en LOW con inversor apagado
4. Usar osciloscopio en pins 9 y 10 con inversor encendido

### Fase 3: Prueba con Baja Potencia

1. Conectar fuente de 12V (en lugar de 24V) al H-Bridge
2. **No conectar aún el transformador**
3. Conectar LEDs en serie con resistencias (1kΩ) en lugar del primario
4. Encender inversor y verificar parpadeo alternado de LEDs
5. Medir con osciloscopio la señal SPWM

### Fase 4: Prueba con Transformador

1. Conectar el transformador de ferrita
2. **NO conectar carga aún en el secundario**
3. Alimentar con 24V (con limitador de corriente si es posible)
4. Medir forma de onda en el secundario con osciloscopio
5. **Debe verse una onda sinusoidal de ~144-168V pico**

### Fase 5: Prueba con Carga

1. Conectar el filtro LC en el secundario
2. Comenzar con carga pequeña: **bombilla incandescente de 25W**
3. Verificar forma de onda con carga
4. Medir voltaje RMS (debe ser ~120V)
5. Monitorear temperatura de MOSFETs
6. Aumentar gradualmente la carga

---

## 📊 Pruebas y Mediciones

### Verificaciones Críticas

✅ **Tiempo muerto (Dead Time)**
- Usar osciloscopio de 2 canales
- Medir Q1 y Q2 simultáneamente
- Verificar que NUNCA se solapan
- Ajustar `TIEMPO_MUERTO_US` si es necesario

✅ **Forma de onda de salida**
- Debe ser sinusoidal limpia
- Frecuencia: exactamente 60Hz (o 50Hz)
- Voltaje RMS: 120V ±5%
- THD < 5% (medible con analizador de espectro)

✅ **Eficiencia**
- Medir potencia de entrada: P_in = V_in × I_in
- Medir potencia de salida: P_out = V_out × I_out × cos(φ)
- Eficiencia = (P_out / P_in) × 100%
- Objetivo: > 85%

✅ **Temperatura**
- MOSFETs: < 80°C en operación continua
- Transformador: < 60°C
- Si excede, mejorar ventilación o reducir carga

---

## ⚠️ ADVERTENCIAS DE SEGURIDAD

### 🔴 PELIGROS ELÉCTRICOS

- **120V AC ES POTENCIALMENTE LETAL** - Tomar todas las precauciones
- **Aislar completamente** las partes de alta tensión
- **No tocar** el circuito con el inversor encendido
- **Usar caja aislante** para el circuito completo
- **Conexión a tierra** adecuada en la carcasa metálica

### 🔴 RIESGO DE INCENDIO

- **Fusible obligatorio** en la entrada de 24V
- **Verificar dimensionamiento** de cables (mínimo 10 AWG para 40A)
- **Monitoreo térmico** de componentes de potencia
- **Ventilación adecuada** del gabinete

### 🔴 PROTECCIONES REQUERIDAS

1. **Protección contra cortocircuito** en salida
2. **Protección contra sobrecarga** (implementada en código)
3. **Protección térmica** con termistor en disipador
4. **Protección contra inversión de polaridad** en entrada

---

## 🔧 Solución de Problemas

### Problema: No hay salida en el secundario

**Posibles causas:**
- Transformador conectado incorrectamente
- MOSFETs no conmutan (verificar drivers)
- Tiempo muerto demasiado largo
- Frecuencia PWM incorrecta para el núcleo

**Solución:**
1. Verificar señales PWM con osciloscopio
2. Comprobar alimentación de drivers IR2110
3. Verificar bootstrap capacitors cargados
4. Revisar conexiones del transformador

### Problema: MOSFETs se calientan excesivamente

**Posibles causas:**
- Resistencia gate muy alta (lenta conmutación)
- RDS(on) muy alto (usar MOSFETs de mejor calidad)
- Frecuencia PWM muy alta
- Shoot-through (conducción simultánea)

**Solución:**
1. Reducir resistencias gate a 10Ω
2. Aumentar tiempo muerto a 3-5µs
3. Mejorar disipador y ventilación
4. Usar MOSFETs con menor RDS(on)

### Problema: Forma de onda distorsionada

**Posibles causas:**
- Filtro LC mal dimensionado
- Tabla de seno con pocos puntos
- Carga no lineal conectada
- Transformador saturado

**Solución:**
1. Ajustar valores de L y C del filtro
2. Aumentar MUESTRAS_SENO a 200
3. Verificar que carga sea resistiva pura
4. Reducir nivel de potencia

### Problema: El inversor se apaga solo

**Posibles causas:**
- Protección por bajo voltaje activada
- Protección por sobrecorriente activada
- Batería insuficiente

**Solución:**
1. Verificar voltaje de batería (> 22V)
2. Calibrar sensor de corriente
3. Ajustar umbrales de protección
4. Revisar mensajes en Serial Monitor

---

## 📈 Mejoras Futuras

### Software
- [ ] Control PID para regulación de voltaje
- [ ] Compensación de carga (mantener 120V con distintas cargas)
- [ ] Arranque suave (soft-start) para evitar picos
- [ ] Pantalla LCD para mostrar parámetros
- [ ] Comunicación WiFi/Bluetooth para monitoreo remoto

### Hardware
- [ ] Sensores de voltaje/corriente de mayor precisión
- [ ] Aislamiento óptico completo (optoacopladores)
- [ ] Fuente auxiliar aislada para drivers
- [ ] Protección contra sobretensión con MOV
- [ ] Ventilador controlado por temperatura

### Topología
- [ ] Inversor multinivel para mejor THD
- [ ] Duplicación de MOSFETs en paralelo para mayor corriente
- [ ] Transformador con taps para diferentes voltajes

---

## 📚 Referencias y Recursos

### Documentación Técnica
- [Datasheet IR2110](https://www.infineon.com/dgdl/ir2110.pdf)
- [Application Note AN-978: HV Floating MOS-Gate Driver ICs](https://www.infineon.com/an978)
- [Texas Instruments: SPWM Inverter Design](https://www.ti.com/lit/an/slaa619/slaa619.pdf)

### Cálculo de Transformadores
- [Ferroxcube Design Tool](https://www.ferroxcube.com/en-global/design_tools)
- [TDK Ferrite Core Selection Guide](https://www.tdk-electronics.tdk.com/en/529404/tech-library)

### Tutoriales Recomendados
- YouTube: "Pure Sine Wave Inverter Design"
- EEVblog Forum: Inverter Projects
- EDABOARD.com: Power Electronics Section

---

## 📝 Notas del Desarrollador

Este diseño está optimizado para:
- Uso educativo y experimental
- Aplicaciones de energía solar/batería
- Prototipado rápido

**No está certificado para:**
- Uso comercial sin pruebas adicionales
- Equipos médicos o críticos
- Conexión directa a la red eléctrica

---

## 📄 Licencia

Este proyecto se proporciona "tal cual" sin garantías de ningún tipo. El usuario asume toda la responsabilidad por el uso de este diseño.

**Desarrollado con fines educativos - Usar bajo su propio riesgo**

---

## 👤 Soporte

Para preguntas o mejoras, consulte:
- La documentación del código fuente (.ino)
- Foros de Arduino y electrónica de potencia
- Comunidades de energía renovable

**¡Buena suerte con tu proyecto de inversor!** ⚡🔧
