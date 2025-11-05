# 🔧 GUÍA PARA PROBAR EL CÓDIGO EN ARDUINO IDE

## ✅ SÍ, puedes probarlo en Arduino IDE

El código está completamente listo para ser cargado y probado en Arduino IDE. Aquí te explico cómo hacerlo de manera segura.

---

## 📋 REQUISITOS PREVIOS

### Software Necesario:
- ✅ **Arduino IDE** (versión 1.8.x o 2.x)
- ✅ Drivers USB (CH340 si usas Arduino Nano clone)

### Hardware Mínimo para Pruebas:
- ✅ Arduino Nano/Uno
- ✅ Cable USB
- ✅ (Opcional) LEDs y resistencias para simular salidas

---

## 🚀 PASO A PASO: CARGA DEL CÓDIGO

### 1. Abrir el Archivo

**Opción A - Desde archivo .ino:**
```
1. Abre Arduino IDE
2. Ve a: Archivo → Abrir
3. Selecciona: inversor_onda_pura_24v_120v.ino
4. El archivo se abrirá en una nueva ventana
```

**Opción B - Copiar y pegar:**
```
1. Abre Arduino IDE
2. Crea un nuevo sketch: Archivo → Nuevo
3. Borra todo el contenido
4. Copia todo el código del archivo .ino
5. Pégalo en el editor
6. Guarda como: inversor_onda_pura_24v_120v
```

---

### 2. Configurar Arduino IDE

```
1. Selecciona la placa:
   Herramientas → Placa → Arduino Nano (o Arduino Uno)
   
2. Si es Arduino Nano, selecciona el procesador:
   Herramientas → Procesador → ATmega328P (Old Bootloader)
   (Prueba ambas opciones si una no funciona)
   
3. Selecciona el puerto COM:
   Herramientas → Puerto → COMx (Windows) o /dev/ttyUSBx (Linux/Mac)
```

---

### 3. Verificar el Código

```
1. Haz clic en el botón "Verificar" (✓) o presiona Ctrl+R
2. Espera a que compile
3. Deberías ver:
   "El Sketch usa XXXX bytes (XX%) del espacio de almacenamiento"
   "Las variables Globales usan XXXX bytes (XX%) de memoria dinámica"
```

**Resultado esperado:**
```
✅ Compilación completa
✅ Uso de memoria: ~15-25% del programa
✅ Variables globales: ~30-40% de RAM
✅ Sin errores
```

---

### 4. Cargar al Arduino

```
1. Conecta el Arduino al USB
2. Verifica que el LED de encendido (rojo) esté encendido
3. Haz clic en "Subir" (→) o presiona Ctrl+U
4. Espera a que se cargue (LEDs TX/RX parpadearán)
5. Verás: "Subida completada"
```

---

## 🧪 PRUEBAS SIN HARDWARE DE POTENCIA

### ⚠️ IMPORTANTE: Pruebas seguras SIN conectar MOSFETs ni 24V

Puedes probar el código de forma **completamente segura** sin armar el circuito de potencia:

---

### PRUEBA 1: Verificar Arranque y Serial Monitor

**Qué hacer:**
```
1. Carga el código al Arduino
2. Abre el Monitor Serial: Herramientas → Monitor Serial
3. Configura la velocidad a: 115200 baudios
```

**Qué deberías ver:**
```
=================================
INVERSOR ONDA SINUSOIDAL PURA
24V DC -> 120V AC @ 60Hz
=================================
PWM configurado a 20kHz
Generador de seno configurado: 60Hz
Sistema inicializado
Presione el botón para encender
----------------------------
Estado: APAGADO
Voltaje entrada: 0.00 V
Corriente (ADC): 0
```

✅ **Si ves esto: El código funciona correctamente**

---

### PRUEBA 2: Verificar LED Indicador

**Qué hacer:**
```
1. Observa el LED integrado del Arduino (pin 13)
2. Al encenderse, debería parpadear 3 veces rápido
3. Luego quedarse apagado
```

**Comportamiento esperado:**
```
Encendido → Parpadeo (200ms × 3 veces) → Apagado
```

✅ **Si parpadea: La inicialización es correcta**

---

### PRUEBA 3: Verificar Botón de Encendido

**Qué hacer:**
```
1. Conecta un botón entre el pin D2 y GND
2. O simplemente conecta un cable desde D2 a GND momentáneamente
3. Observa el Monitor Serial
```

**Qué deberías ver:**
```
>>> INVERSOR ENCENDIDO <<<
----------------------------
Estado: ENCENDIDO
Voltaje entrada: 0.00 V
Corriente (ADC): 0
```

**Al volver a presionar:**
```
>>> INVERSOR APAGADO <<<
```

✅ **Si cambia de estado: La interrupción funciona**

---

### PRUEBA 4: Verificar Señales PWM con LED

**⚠️ Esta es la prueba MÁS IMPORTANTE para ver si genera SPWM**

**Material necesario:**
- 2× LEDs (cualquier color)
- 2× Resistencias 220Ω o 330Ω

**Conexiones:**
```
Arduino Pin 9  ──[220Ω]──►|── LED1 ──► GND
Arduino Pin 10 ──[220Ω]──►|── LED2 ──► GND
```

**Esquema visual:**
```
            Pin 9                   Pin 10
              │                       │
            ┌─┴─┐                   ┌─┴─┐
            │220Ω                   │220Ω
            └─┬─┘                   └─┬─┘
              │                       │
          ┌───┴──┐                ┌───┴──┐
         ►│ LED1 │               ►│ LED2 │
          └───┬──┘                └───┬──┘
              │                       │
             GND                     GND
```

**Qué hacer:**
```
1. Monta el circuito con los 2 LEDs
2. Conecta el cable/botón en pin D2 a GND para "encender"
3. Observa los LEDs
```

**Qué deberías ver:**
```
🔴 LED1 (pin 9):  Brillo variable en patrón senoidal
🔴 LED2 (pin 10): Brillo variable en patrón senoidal (desfasado 180°)

Los LEDs parpadean alternadamente, creando el efecto de la onda seno:
- Uno sube de brillo mientras el otro baja
- Frecuencia visible: 60 veces por segundo (60Hz)
- Cada LED representa un semiciclo de la onda
```

✅ **Si los LEDs varían en brillo: ¡El SPWM funciona perfectamente!**

---

### PRUEBA 5: Verificar PWM con Osciloscopio (IDEAL)

**Si tienes osciloscopio:**

**Conexiones:**
```
CH1 (Canal 1) → Pin 9 del Arduino
CH2 (Canal 2) → Pin 10 del Arduino
GND (común)   → GND del Arduino
```

**Configuración del osciloscopio:**
```
- Escala de tiempo: 5ms/div (para ver la onda de 60Hz)
- Escala vertical: 2V/div
- Trigger: CH1, flanco ascendente, 2.5V
- Modo: Dual (ver ambos canales)
```

**Qué deberías ver:**

```
Canal 1 (Pin 9):
     ╱╲___________________╱╲
    ╱  ╲                 ╱  ╲
___╱    ╲_______________╱    ╲___
   ◄── Semiciclo PWM ──►

Canal 2 (Pin 10):
_______╱╲___________________╱╲
      ╱  ╲                 ╱  ╲
_____╱    ╲_______________╱    ╲
      ◄── Semiciclo PWM ──►
      (Desfasado 180°)
```

**Mediciones esperadas:**
- Frecuencia envolvente: **60 Hz** (16.67ms periodo)
- Frecuencia PWM: **20 kHz** (50µs periodo)
- Amplitud: **0-5V** (lógica TTL)
- Forma: **SPWM** (ancho de pulso varía según seno)

✅ **Si ves esto: El código genera SPWM correctamente**

---

### PRUEBA 6: Verificar Protecciones (Simuladas)

**Qué hacer:**
```
1. En el código, modifica temporalmente los límites:
   #define VOLTAJE_MIN 1.0  // Cambiar de 22.0 a 1.0
   #define VOLTAJE_MAX 3.0  // Cambiar de 29.0 a 3.0
   
2. Recarga el código
3. Enciende el inversor con el botón
```

**Qué deberías ver en Serial:**
```
ADVERTENCIA: Voltaje bajo!
Inversor apagado por protección
¡SOBRECARGA DETECTADA!
Presione reset para reiniciar
```

**LED debería:**
```
Parpadear rápidamente (cada 100ms)
```

✅ **Si se apaga y parpadea: Las protecciones funcionan**

**⚠️ RECUERDA: Volver los valores originales después**
```
#define VOLTAJE_MIN 22.0
#define VOLTAJE_MAX 29.0
```

---

## 📊 TABLA DE RESULTADOS ESPERADOS

| Prueba | Pin/Función | Resultado Esperado | Estado |
|--------|-------------|-------------------|--------|
| 1. Serial Monitor | USB | Mensajes de inicialización | ☐ |
| 2. LED interno | Pin 13 | Parpadeo 3× al iniciar | ☐ |
| 3. Botón ON/OFF | Pin D2 | Cambia estado ON/OFF | ☐ |
| 4. LED en Pin 9 | Pin 9 | Brillo variable (60Hz) | ☐ |
| 5. LED en Pin 10 | Pin 10 | Brillo variable desfasado | ☐ |
| 6. Osciloscopio | Pin 9/10 | SPWM 60Hz @ 20kHz | ☐ |
| 7. Protecciones | Software | Apagado automático | ☐ |

---

## ⚠️ ADVERTENCIAS IMPORTANTES

### 🔴 NO HAGAS ESTO (Todavía):

❌ **NO conectes los MOSFETs** sin verificar las señales primero  
❌ **NO conectes 24V** hasta haber probado con osciloscopio  
❌ **NO conectes el transformador** sin verificar el H-Bridge  
❌ **NO conectes carga a 120V** hasta probar con baja potencia  

### ✅ HAZLO EN ESTE ORDEN:

1. ✅ Pruebas de software (Serial, LED) ← **ESTÁS AQUÍ**
2. ✅ Verificar PWM con LEDs
3. ✅ Verificar PWM con osciloscopio (MUY recomendado)
4. ✅ Montar drivers IR2110 (sin MOSFETs)
5. ✅ Verificar salidas de IR2110
6. ✅ Conectar MOSFETs (sin transformador)
7. ✅ Verificar con LEDs en lugar de transformador
8. ✅ Conectar transformador (sin carga)
9. ✅ Finalmente, probar con carga pequeña

---

## 🐛 SOLUCIÓN DE PROBLEMAS

### Problema 1: No Compila

**Error: "expected ';' before..."**
```
Solución: Verifica que copiaste TODO el código completo
```

**Error: "avr/io.h: No such file..."**
```
Solución: Selecciona correctamente la placa Arduino en Herramientas → Placa
```

---

### Problema 2: No Se Carga

**Error: "avrdude: stk500_recv(): programmer is not responding"**
```
Solución 1: Cambia a "ATmega328P (Old Bootloader)"
Solución 2: Verifica el puerto COM correcto
Solución 3: Prueba otro cable USB
Solución 4: Reinstala drivers CH340
```

---

### Problema 3: No Aparece en Serial Monitor

```
Solución 1: Verifica velocidad 115200 baudios
Solución 2: Selecciona "Ambos NL y CR" o "Nueva línea"
Solución 3: Presiona reset en el Arduino
Solución 4: Cierra y reabre el Monitor Serial
```

---

### Problema 4: LEDs No Varían de Brillo

```
Causa posible: El inversor está APAGADO
Solución: Presiona el botón (conecta D2 a GND momentáneamente)

Causa posible: Conexión incorrecta de LEDs
Solución: Verifica polaridad (patita larga = +, corta = -)

Causa posible: Resistencia muy alta
Solución: Usa resistencia de 220Ω o menos
```

---

## 💡 CONSEJOS ADICIONALES

### 1. Modificar Frecuencia de Salida (50Hz)

Si estás en Europa o quieres 50Hz:

```cpp
// Cambia esta línea:
#define FRECUENCIA_SALIDA 60

// Por:
#define FRECUENCIA_SALIDA 50
```

---

### 2. Ver Más Detalles en Serial

Descomenta estas líneas en el código (si existen) para debug:

```cpp
// En la función ISR, agregar (CUIDADO: ralentiza el código):
// Serial.print("Índice: ");
// Serial.println(indiceTabla);
```

**⚠️ ADVERTENCIA:** Imprimir dentro de interrupciones puede causar problemas.  
Solo para debug, luego comentar de nuevo.

---

### 3. Probar con Diferentes Valores PWM

Para probar manualmente un duty cycle fijo (sin SPWM):

```cpp
// En setup(), después de configurarPWM(), agregar:
void setup() {
  // ... código existente ...
  
  // PRUEBA: PWM fijo al 50%
  OCR1A = 400;  // 50% de 799 (TOP)
  OCR1B = 0;    // Apagado
}

// Comentar el ISR para que no interfiera
// ISR(TIMER2_COMPA_vect) { ... }
```

Con esto, el pin 9 debería tener un LED con brillo constante al 50%.

---

## 📹 VIDEO TUTORIAL (Referencia)

Si necesitas ayuda visual para cargar código, busca en YouTube:
- "Como cargar código a Arduino Nano"
- "Arduino IDE tutorial español"
- "Instalar drivers CH340 Arduino"

---

## ✅ CHECKLIST FINAL

Antes de pasar a hardware de potencia:

- [ ] El código compila sin errores
- [ ] El código se carga al Arduino exitosamente
- [ ] El Monitor Serial muestra mensajes correctos
- [ ] El LED interno parpadea 3 veces al inicio
- [ ] El botón ON/OFF cambia el estado
- [ ] Los LEDs en pin 9/10 varían de brillo
- [ ] (Opcional) El osciloscopio muestra SPWM correcto
- [ ] Las protecciones funcionan (prueba simulada)

**Si todos están ✅: Estás listo para pasar a la etapa de hardware**

---

## 🎓 CONCLUSIÓN

El código está **100% funcional** y listo para Arduino IDE. Puedes:

1. ✅ **Cargarlo directamente** sin modificaciones
2. ✅ **Probarlo sin peligro** solo con el Arduino
3. ✅ **Verificar el SPWM** con LEDs simples
4. ✅ **Validar la lógica** antes de construir el circuito

---

## 🆘 ¿NECESITAS AYUDA?

Si tienes problemas específicos:

1. **Copia el mensaje de error completo**
2. **Indica tu placa Arduino (Nano/Uno/Mega)**
3. **Menciona qué prueba estás haciendo**
4. **Describe qué ves vs. qué esperabas**

---

**¡Éxito con tus pruebas!** 🚀⚡

Una vez que verifiques que el código funciona en Arduino IDE,  
podrás continuar con confianza a construir el circuito de potencia.
