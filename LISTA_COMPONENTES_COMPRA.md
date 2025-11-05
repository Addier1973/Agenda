# 🛒 LISTA DE COMPONENTES PARA COMPRAR
## Inversor de Onda Sinusoidal Pura 24V → 120V AC

---

## 📦 COMPONENTES PRINCIPALES

### 1. Microcontrolador
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Arduino Nano V3 | 1 | ATmega328P, USB-CH340 | $5 - $10 |
| **Alternativa:** Arduino Uno R3 | 1 | ATmega328P, USB oficial | $15 - $25 |

---

### 2. MOSFETs de Potencia (Elegir UNA opción)

#### Opción A - Económica:
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| IRFZ44N | 4 | 55V, 49A, RDS(on)=17.5mΩ | $1.50 c/u |

#### Opción B - Recomendada:
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| IRF3205 | 4 | 55V, 110A, RDS(on)=8mΩ | $2.50 c/u |

#### Opción C - Alto Rendimiento:
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| IRFB4110 | 4 | 100V, 180A, RDS(on)=3.7mΩ | $4.00 c/u |

---

### 3. Drivers de MOSFET
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| IR2110 | 2 | High/Low Side Driver | $2.50 c/u |
| **Alternativa:** IR2184 | 2 | Similar, más económico | $1.80 c/u |

---

### 4. Transformador de Ferrita

**OPCIÓN 1: Comprar núcleo y bobinar:**
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Núcleo Ferrita ETD49 | 1 | Material N87 o 3C90 | $8 - $15 |
| Bobbin ETD49 | 1 | Carrete para bobinado | $2 - $3 |
| Alambre esmaltado #14 AWG | 2m | Para primario (2.5mm²) | $2 |
| Alambre esmaltado #20 AWG | 10m | Para secundario (0.8mm²) | $3 |
| Cinta Kapton | 1 rollo | Aislamiento entre capas | $5 |

**OPCIÓN 2: Transformador comercial:**
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Transformador 24V→120V | 1 | 500W-1000W, alta frecuencia | $30 - $80 |
| (Buscar en proveedores especializados) | | | |

---

### 5. Componentes del Filtro LC
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Inductor 10mH | 1 | 5A corriente nominal, núcleo polvo hierro | $8 - $12 |
| Capacitor 10µF/250VAC | 1 | Tipo X2 (polipropileno) | $3 - $5 |
| **Alternativa capacitor:** | 1 | 2x 4.7µF en paralelo | $2 c/u |

---

## 🔌 COMPONENTES ELECTRÓNICOS

### 6. Diodos
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| UF4007 (ultrarrápido) | 4 | 1A, 1000V, para bootstrap | $0.15 c/u |
| MBR4045 (Schottky) | 1 | 40A, 45V, protección polaridad | $1.50 |

---

### 7. Capacitores
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Electrolítico 4700µF/35V | 1 | Filtro entrada principal | $3 - $5 |
| Electrolítico 10µF/50V | 4 | Bootstrap (2 por IR2110) | $0.30 c/u |
| Cerámico 100nF/50V | 10 | Desacoplo (varios puntos) | $0.10 c/u |
| Electrolítico 10µF/25V | 4 | Reguladores voltaje | $0.20 c/u |

---

### 8. Resistencias (1/4W o mayor)
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| 10Ω / 2W | 4 | Gate resistors para MOSFETs | $0.15 c/u |
| 68kΩ / 1% | 1 | Divisor voltaje (R1) | $0.20 |
| 10kΩ / 1% | 2 | Divisor voltaje (R2) + NTC | $0.20 c/u |
| 220Ω / 1/4W | 1 | LED indicador | $0.10 |

---

### 9. Reguladores de Voltaje
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| LM7812 | 1 | Regulador 12V / 1A (TO-220) | $0.50 |
| LM7805 | 1 | Regulador 5V / 1A (TO-220) | $0.50 |

**ALTERNATIVA EFICIENTE (Recomendada):**
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Módulo Buck XL4015 | 1 | 24V → 12V, 5A | $2 - $3 |
| Módulo Buck LM2596 | 1 | 24V → 5V, 3A | $1.50 - $2 |

---

## 📡 SENSORES

### 10. Sensores de Monitoreo
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| ACS712-30A | 1 | Sensor corriente Hall Effect | $2 - $3 |
| **Alternativa:** ACS712-20A | 1 | Mayor sensibilidad, menor rango | $2 - $3 |
| Termistor NTC 10kΩ | 1 | Sensor temperatura (opcional) | $0.50 |

---

## 🛡️ PROTECCIONES

### 11. Componentes de Protección
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Fusible 40A | 2 | Tipo automotriz o cerámico | $1 c/u |
| Portafusible | 2 | Para fusibles de entrada/salida | $1 c/u |
| Varistor (MOV) 150V | 1 | 20mm, tipo 14D151K | $1 |
| Interruptor ON/OFF | 1 | 40A, tipo rocker switch | $2 - $3 |
| Botón pulsador | 1 | NO (normalmente abierto) | $0.50 |
| LED rojo 5mm | 1 | Indicador de encendido | $0.10 |

---

## 🌡️ DISIPACIÓN TÉRMICA

### 12. Enfriamiento
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Disipador TO-220 | 1 | Grande (para 4 MOSFETs) | $5 - $10 |
| Ventilador 12V | 1 | 80mm o 120mm | $3 - $5 |
| Pasta térmica | 1 tubo | Tipo Arctic MX-4 o similar | $3 - $5 |
| Aisladores térmicos TO-220 | 4 | Mica o silicona | $0.20 c/u |
| Tornillos M3 | 8 | Para fijar MOSFETs | $0.05 c/u |
| Tuercas M3 | 8 | | $0.05 c/u |
| Disipador TO-220 pequeño | 2 | Para LM7812/7805 | $0.50 c/u |

---

## 🔌 CABLES Y CONECTORES

### 13. Cableado
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| Cable 10 AWG rojo | 1m | Para entrada +24V | $2/metro |
| Cable 10 AWG negro | 1m | Para entrada GND | $2/metro |
| Cable 14 AWG (varios colores) | 3m | Conexiones internas | $1/metro |
| Cable 22 AWG (varios colores) | 5m | Señales de control | $0.50/metro |
| Cable verde/amarillo 14 AWG | 1m | Tierra de seguridad | $1/metro |
| Terminal ojo 10 AWG | 4 | Para conexiones principales | $0.30 c/u |
| Terminal faston | 10 | Conectores rápidos | $0.15 c/u |
| Termocontraíble (surtido) | 1 set | Aislamiento conexiones | $3 - $5 |
| Conector AC hembra | 1 | Para salida 120V (tipo NEMA) | $2 - $3 |

---

## 📐 MATERIAL DE MONTAJE

### 14. PCB y Estructura
| Componente | Cantidad | Especificación | Precio Aprox. |
|------------|----------|----------------|---------------|
| PCB perforado | 1 | 10x15cm o similar | $2 - $3 |
| Protoboard grande | 1 | Para pruebas iniciales (opcional) | $5 - $8 |
| Caja plástica/metálica | 1 | 20x15x10cm mínimo | $8 - $15 |
| Estaño para soldar | 1 rollo | 60/40 o lead-free | $5 - $8 |
| Flux para soldar | 1 | Pasta o líquido | $3 |
| Soporte Arduino | 1 | Espaciadores M3 | $1 |
| Cable ties (bridas) | 20 | Organización cables | $0.05 c/u |

---

## 🔧 HERRAMIENTAS NECESARIAS (Si no las tienes)

### 15. Herramientas Básicas
| Herramienta | Necesidad | Precio Aprox. |
|-------------|-----------|---------------|
| Multímetro digital | **ESENCIAL** | $15 - $30 |
| Cautín 60W | **ESENCIAL** | $10 - $20 |
| Pinzas de punta | Recomendado | $5 - $10 |
| Pinzas de corte | Recomendado | $5 - $10 |
| Destornilladores | **ESENCIAL** | $5 - $10 |
| Pelacables | Recomendado | $5 - $8 |
| Extractor de estaño | Útil | $2 - $5 |

### 16. Herramientas Avanzadas (Muy recomendadas)
| Herramienta | Necesidad | Precio Aprox. |
|-------------|-----------|---------------|
| Osciloscopio | **MUY RECOMENDADO** | $50 - $300+ |
| Fuente regulable 0-30V | Recomendado para pruebas | $30 - $100 |
| Pinza amperimétrica | Útil para mediciones | $20 - $50 |

---

## 💰 RESUMEN DE COSTOS

### Costo Estimado por Sección:

| Sección | Costo Mínimo | Costo Típico | Costo Alto |
|---------|--------------|--------------|------------|
| Microcontrolador | $5 | $10 | $25 |
| MOSFETs (x4) | $6 | $10 | $16 |
| Drivers IR2110 (x2) | $3.60 | $5 | $5 |
| Transformador | $20 | $40 | $80 |
| Filtro LC | $11 | $17 | $17 |
| Diodos | $1.10 | $2 | $2 |
| Capacitores | $8 | $12 | $12 |
| Resistencias | $1.50 | $2 | $2 |
| Reguladores | $3 | $5 | $7 |
| Sensores | $2.50 | $4 | $6 |
| Protecciones | $5 | $8 | $10 |
| Enfriamiento | $8.40 | $15 | $25 |
| Cables y conectores | $10 | $18 | $25 |
| Montaje y PCB | $15 | $25 | $35 |
| **TOTAL** | **$99.10** | **$173** | **$267** |

### Herramientas (si no las tienes):
- Básicas: $40 - $80
- Con osciloscopio: $90 - $380

---

## 🌐 DÓNDE COMPRAR

### Proveedores Internacionales:
- **AliExpress** - Precios muy económicos, envío lento (30-60 días)
- **Amazon** - Precios medios, envío rápido (2-5 días)
- **eBay** - Variedad de vendedores
- **Mouser** - Componentes profesionales, precios altos
- **DigiKey** - Componentes profesionales, gran inventario
- **Newark/Farnell** - Distribuidores oficiales

### Proveedores Locales (según país):
- **México:** Steren, ElectronicaSierra, Sic Distribuciones
- **Argentina:** Electrocomponentes, DigiKey Argentina
- **Chile:** Victron, DigiKey Chile
- **Colombia:** Sigma Electrónica, Suconel
- **España:** RS Components, Farnell España
- **USA:** Jameco, SparkFun, Adafruit

### Búsqueda de Transformador:
- Buscar: "high frequency transformer 24V to 120V"
- Contactar fabricantes: TDK, Ferroxcube, EPCOS
- Foros: EEVblog, EDN, All About Circuits

---

## 📝 NOTAS DE COMPRA

### ⚠️ IMPORTANTE:

1. **MOSFETs**: Comprar 1-2 extras por si se dañan durante pruebas
2. **Transformador**: La opción más crítica y cara. Considerar:
   - Bobinar el tuyo: más barato pero requiere conocimiento
   - Comprar hecho: más caro pero confiable
   - Alternativa: buscar transformadores de UPS viejos y rebobinar
3. **Osciloscopio**: Aunque opcional, es MUY recomendado para verificar:
   - Señales PWM
   - Tiempo muerto
   - Forma de onda de salida
4. **Sensores**: El ACS712 es económico pero no muy preciso. Para aplicaciones críticas, considerar sensores de corriente tipo "shunt" o Hall de mayor calidad.

### 💡 CONSEJOS PARA AHORRAR:

- Comprar en lote (kits de Arduino, resistencias, capacitores)
- Buscar componentes en marketplaces locales (mercado libre, etc.)
- Reutilizar disipadores de computadoras viejas
- Comprar cables en ferreterías (más barato que tiendas electrónicas)
- Usar caja de proyecto de plástico genérica
- Para pruebas, usar protoboard antes de soldar PCB definitivo

### 🎯 PRIORIDAD DE COMPRA:

**FASE 1 - Esenciales para empezar:**
- Arduino + cable USB
- MOSFETs (x4) + drivers IR2110
- Componentes básicos (resistencias, capacitores)
- Protoboard para pruebas
- Multímetro

**FASE 2 - Para construcción:**
- Transformador
- Filtro LC
- Reguladores de voltaje
- Sensores
- PCB y materiales de montaje

**FASE 3 - Acabado:**
- Caja
- Enfriamiento (disipador + ventilador)
- Protecciones finales
- Conectores profesionales

---

## 📦 EJEMPLO DE LISTA DE COMPRA EN ALIEXPRESS

### Búsqueda sugerida por producto:

1. "Arduino Nano V3 CH340"
2. "IRF3205 MOSFET TO-220"
3. "IR2110 high low side driver"
4. "ETD49 ferrite core transformer"
5. "ACS712 30A current sensor module"
6. "10mH inductor power 5A"
7. "X2 capacitor 10uF 250VAC"
8. "TO-220 heatsink aluminum"
9. "12V cooling fan 80mm"
10. "LM7812 LM7805 voltage regulator kit"

---

**¡Buena suerte con tu proyecto!** 🎉⚡

Si tienes alguna duda sobre dónde comprar algún componente específico o necesitas alternativas, consulta los foros de electrónica o comunidades locales de makers.
