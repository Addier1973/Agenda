# Lista de Componentes Detallada

## 🔌 Electrónica de Control

### Microcontrolador
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| Arduino Nano | ATmega328P, 16MHz | 1 | $3-5 USD | Alternativa: Arduino Uno |
| Zócalo para Arduino | 2.54mm | 1 | $0.50 USD | Opcional |

### Drivers de MOSFET
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| IR2110 | High/Low Side Driver | 2 | $2-3 USD c/u | Necesario uno por brazo |
| IR2184 | Half-Bridge Driver | 2 | $2-3 USD c/u | Alternativa al IR2110 |
| IRS2092 | Class-D Driver | 2 | $3-4 USD c/u | Mejor para alta frecuencia |

### Componentes del Driver
| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| Capacitor Bootstrap | 10µF/25V Electrolítico | 2 | Bajo ESR |
| Diodo Bootstrap | UF4007 (1A, 1000V) | 2 | Ultrarrápido |
| Resistor Gate | 10Ω, 1/4W | 4 | Para limitación corriente gate |
| Capacitor Bypass VCC | 100nF Cerámico | 4 | Cerca de cada IC |
| Capacitor Bypass VDD | 10µF/25V | 2 | Electrolítico |

---

## ⚡ Electrónica de Potencia

### MOSFETs (Opción Recomendada: Alta Potencia)
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| IRF3205 | 55V, 110A, RDS(on)=8mΩ | 4 | $1.50 USD c/u | **RECOMENDADO** |
| Disipador TO-220 | 8-10°C/W | 4 | $0.50 USD c/u | Con pasta térmica |
| Aisladores TO-220 | Mica o silicona | 4 | $0.10 USD c/u | Con grasa térmica |

### MOSFETs (Opción Económica: Media Potencia)
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| IRFZ44N | 55V, 49A, RDS(on)=17.5mΩ | 4 | $0.80 USD c/u | Para potencias < 300W |

### Diodos de Protección
| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| UF5408 | 3A, 1000V Ultrarrápido | 4 | En paralelo con MOSFETs |
| MBR20200CT | 20A, 200V Schottky | 2 | Alternativa |

---

## 🔄 Transformador de Ferrita

### Núcleo de Ferrita
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| Núcleo ETD49 | Ferrita N87 o similar | 1 | $8-15 USD | Para 500-1000W |
| Núcleo ETD39 | Ferrita N87 o similar | 1 | $5-10 USD | Para 200-500W |
| Bobbin ETD49 | Porta-bobina | 1 | $1-2 USD | Compatible con núcleo |
| Clip de retención | Para núcleo | 1 | $0.50 USD | Sujeción del núcleo |

### Alambre para Bobinado
| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| Alambre Litz AWG18 | Multi-hebra, aislado | 50m | Para primario (alta corriente) |
| Alambre esmaltado AWG24 | Magneto | 100m | Para secundario |
| Cinta aislante | Kapton o similar | 1 rollo | Entre capas |

### Cálculo Aproximado de Espiras (ETD49, 31.25kHz)
```
Primario: 8-10 espiras (24V)
Secundario: 40-50 espiras (120V)
Relación: 1:5 aproximadamente
```

---

## 🔋 Filtrado y Almacenamiento

### Entrada (24V DC)
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| Capacitor electrolítico | 2200µF/50V Bajo ESR | 4 | $2-3 USD c/u | En paralelo |
| Capacitor cerámico | 100nF/50V | 4 | $0.10 USD c/u | Cerca de MOSFETs |
| Fusible entrada | 20-30A según potencia | 1 | $1-2 USD | Tipo automotriz |
| Portafusible | Para fusible ATO | 1 | $0.50 USD | - |

### Filtro LC de Salida
| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| Inductor toroidal | 1-2mH, 10A | 1 | $5-10 USD | Núcleo de polvo de hierro |
| Capacitor film | 2.2µF/250V X2 | 2 | $1.50 USD c/u | En paralelo |
| Capacitor film | 100nF/250V X2 | 2 | $0.50 USD c/u | Supresión alta frecuencia |

---

## 🛡️ Protección

### Protección de Sobretensión
| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| Varistor entrada | 36V (para 24V) | 1 | MOV, ejemplo: V36ZA2P |
| Varistor salida | 180V (para 120V) | 1 | MOV, ejemplo: V180ZA2P |

### Fusibles Adicionales
| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| Fusible rápido salida | 5A/250V | 1 | Protección carga |

---

## 🌡️ Enfriamiento

| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| Ventilador 12V | 80x80mm o 120x120mm | 1-2 | $3-5 USD c/u | Según potencia |
| Disipador principal | Aluminio, tamaño grande | 1 | $10-20 USD | Para montar todos los MOSFETs |
| Pasta térmica | Compuesto térmico | 1 tubo | $2-3 USD | Alta conductividad |
| Sensor temperatura | NTC 10K o DS18B20 | 1 | $0.50 USD | Monitoreo opcional |

---

## 🔌 Conectores y Terminales

| Componente | Especificación | Cantidad | Notas |
|------------|----------------|----------|-------|
| Terminal tornillo 24V | 10mm, 30A | 2 | Entrada positivo/negativo |
| Terminal AC salida | Tipo IEC C13 hembra | 1 | Estándar 120V |
| Cable AWG10 | Multihebra, rojo | 1m | Entrada positiva |
| Cable AWG10 | Multihebra, negro | 1m | Entrada negativa/tierra |
| Cable AWG14 | Para salida AC | 2m | Vivo/neutro |

---

## 🎛️ Interfaz de Usuario (Opcional)

| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| LCD 16x2 | Con I2C | 1 | $3-4 USD | Display información |
| Pulsador momentáneo | 12mm, SPST | 1 | $0.50 USD | Enable/Disable |
| LED 5mm Verde | Alta luminosidad | 1 | $0.10 USD | Estado ON |
| LED 5mm Rojo | Alta luminosidad | 1 | $0.10 USD | Estado OFF/Error |
| Resistor LED | 220Ω, 1/4W | 2 | $0.05 USD c/u | Limitación corriente |
| Buzzer 5V | Activo | 1 | $0.50 USD | Alarma opcional |

---

## 📊 Sensores (Opcional pero Recomendado)

| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| ACS712-20A | Sensor corriente | 1 | $2-3 USD | Monitoreo entrada |
| Divisor resistivo | Alta tensión | 1 | DIY | Medición voltaje salida |
| Resistor 1MΩ | 1/2W, alta precisión | 4 | $0.20 USD c/u | Para divisor voltaje |
| Optoacoplador | PC817 | 2 | $0.20 USD c/u | Aislamiento señal |

---

## 🏗️ PCB y Montaje

| Componente | Especificación | Cantidad | Precio Aprox. | Notas |
|------------|----------------|----------|---------------|-------|
| PCB FR4 | 100x150mm, 2oz cobre | 1 | $10-15 USD | Fabricación PCB |
| Espaciadores M3 | 10mm, nylon/metal | 8 | $0.10 USD c/u | Montaje |
| Tornillos M3 | 10mm | 16 | $0.05 USD c/u | - |
| Caja/gabinete | Plástico/metal | 1 | $10-20 USD | Protección |
| Borneras | 5.08mm paso | 5 | $0.30 USD c/u | Conexiones |

---

## 💰 Resumen de Costos

### Configuración Básica (500W)
| Categoría | Costo Estimado |
|-----------|----------------|
| Control (Arduino + Drivers) | $15-20 USD |
| Potencia (MOSFETs + Diodos) | $10-15 USD |
| Transformador (Núcleo + Alambre) | $15-25 USD |
| Filtrado y Protección | $15-20 USD |
| Enfriamiento | $5-10 USD |
| PCB y Montaje | $15-25 USD |
| Conectores y Cables | $10-15 USD |
| **TOTAL BÁSICO** | **$85-130 USD** |

### Configuración Completa (Con monitoreo y display)
| **TOTAL CON OPCIONALES** | **$110-160 USD** |

---

## 🛒 Dónde Comprar

### Proveedores Internacionales
- **AliExpress:** MOSFETs, drivers, componentes pasivos (económico)
- **Mouser/Digikey:** Componentes de calidad garantizada
- **eBay:** Núcleos de ferrita, disipadores

### Proveedores Locales (Ejemplo México/Latinoamérica)
- **Steren/RadioShack:** Componentes básicos
- **Distrelec/Electrónica Steren:** Componentes de potencia
- **Mercado Libre:** Variedad de componentes

---

## ⚙️ Herramientas Necesarias

| Herramienta | Uso |
|-------------|-----|
| Multímetro | Mediciones voltaje/corriente |
| Osciloscopio | Verificación forma de onda (recomendado) |
| Soldador 60W+ | Soldadura componentes grandes |
| Cautín 30W | Soldadura componentes pequeños |
| Estaño 60/40 | Soldadura |
| Pasta de soldar | Facilita soldadura |
| Pinzas | Manipulación componentes |
| Cortaalambres | Corte de cables |
| Desarmadores | Montaje |
| Taladro | Perforaciones en PCB/gabinete |
| Lima/Dremel | Ajustes mecánicos |

---

## 📝 Notas Importantes

1. **Calidad de Componentes:** Para aplicaciones críticas, usar componentes de marcas reconocidas (Infineon, International Rectifier, Vishay).

2. **Tolerancias:** Los capacitores electrolíticos deben ser de bajo ESR (<100mΩ) para alta frecuencia.

3. **MOSFETs Falsos:** Cuidado con MOSFETs falsificados en mercados asiáticos. Verificar con probador de componentes.

4. **Núcleo de Transformador:** La ferrita debe ser adecuada para la frecuencia (N87, N97 para 20-100kHz).

5. **Disipación Térmica:** No escatimar en enfriamiento. Los MOSFETs pueden destruirse fácilmente por sobrecalentamiento.

6. **Pruebas:** Comprar componentes adicionales (10-20% extra) para pruebas y reemplazos.

---

**Última actualización:** Noviembre 2025
