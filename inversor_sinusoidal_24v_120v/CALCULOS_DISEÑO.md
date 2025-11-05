# Cálculos de Diseño para Inversor Sinusoidal Pura

## 📐 Cálculos Básicos del Sistema

### 1. Potencia y Corrientes

Para un inversor de **500W**:

```
Potencia de salida: Pout = 500W
Eficiencia estimada: η = 85-90%
Potencia de entrada: Pin = Pout / η = 500W / 0.88 = 568W

Voltaje de entrada: Vin = 24V
Corriente de entrada: Iin = Pin / Vin = 568W / 24V = 23.7A

Voltaje de salida RMS: Vout = 120V
Corriente de salida RMS: Iout = Pout / Vout = 500W / 120V = 4.17A
Corriente pico salida: Ipk = Iout × √2 = 4.17A × 1.414 = 5.9A
```

### Tabla de Potencias Comunes

| Potencia | Vin | Iin @ 88% η | Vout | Iout | Ipk out | MOSFETs recomendados |
|----------|-----|-------------|------|------|---------|---------------------|
| 250W | 24V | 11.9A | 120V | 2.1A | 3.0A | IRFZ44N (49A) |
| 500W | 24V | 23.7A | 120V | 4.2A | 5.9A | IRF3205 (110A) |
| 1000W | 24V | 47.3A | 120V | 8.3A | 11.8A | IRF3205 en paralelo |
| 1500W | 24V | 71.0A | 120V | 12.5A | 17.7A | IRFP260N (50A) × 2 |

---

## 🧲 Diseño del Transformador de Ferrita

### Parámetros Iniciales

```
Frecuencia de operación: f = 31,250 Hz (31.25 kHz)
Voltaje primario: Vp = 24V
Voltaje secundario: Vs = 120V + 10% = 132V (margen para pérdidas)
Relación de transformación: n = Vs / Vp = 132 / 24 = 5.5:1
Potencia aparente: S = 500 VA
```

### Selección del Núcleo

**Área producto (AP method):**

```
AP = Ae × Aw = (Pout × 10⁴) / (K × f × Bmax × J × η)

Donde:
- Pout = 500W
- K = 4.44 (constante de forma de onda)
- f = 31,250 Hz
- Bmax = 0.25T (densidad de flujo máx. para ferrita N87 a 100°C)
- J = 3 A/mm² (densidad de corriente en conductores)
- η = 0.88 (eficiencia)

AP = (500 × 10⁴) / (4.44 × 31,250 × 0.25 × 3 × 0.88)
AP = 5,000,000 / 91,763
AP ≈ 54.5 cm⁴
```

**Núcleos recomendados:**

| Núcleo | Ae (cm²) | Aw (cm²) | AP (cm⁴) | Potencia max | Precio aprox |
|--------|----------|----------|----------|--------------|--------------|
| ETD39 | 1.25 | 1.13 | 1.41 | 300W | $8-12 |
| ETD44 | 1.74 | 1.41 | 2.45 | 500W | $10-15 |
| ETD49 | 2.11 | 1.83 | 3.86 | 800W | $12-18 |
| ETD54 | 2.80 | 2.20 | 6.16 | 1200W | $15-25 |

**Para 500W → Usar ETD49 con margen de seguridad**

---

### Cálculo de Espiras

**Espiras del primario (Np):**

```
Método 1 - Ley de Faraday:
Np = (Vp × 10⁴) / (4.44 × f × Bmax × Ae)

Np = (24 × 10⁴) / (4.44 × 31,250 × 0.25 × 2.11)
Np = 240,000 / 73,428
Np ≈ 3.27 espiras

Sin embargo, esto es muy bajo. Para reducir corriente magnetizante y mejorar
regulación, aumentar a:
Np = 8-10 espiras (recomendado)
```

**Usando Np = 9 espiras:**

```
Bmax real = (Vp × 10⁴) / (4.44 × f × Np × Ae)
Bmax = (24 × 10⁴) / (4.44 × 31,250 × 9 × 2.11)
Bmax = 0.091 T

Esto está bien dentro del límite (< 0.3T para ferrita)
```

**Espiras del secundario (Ns):**

```
Ns = Np × n = 9 × 5.5 = 49.5 espiras
Redondear a: Ns = 50 espiras
```

**Verificación de relación:**

```
Relación real: n = Ns / Np = 50 / 9 = 5.56
Voltaje secundario sin carga: Vs = Vp × n = 24V × 5.56 = 133.4V ✓
```

---

### Calibre de Conductores

**Corriente RMS en devanados:**

```
Primario:
Irms_p = Pin / Vp = 568W / 24V = 23.7A

Con densidad de corriente J = 3-4 A/mm²:
Área conductora = Irms_p / J = 23.7A / 3.5A/mm² = 6.77 mm²

AWG equivalente: AWG 10 (5.26 mm²) o AWG 9 (6.63 mm²)

RECOMENDACIÓN: Usar alambre Litz AWG 18 (0.82 mm²) × 10 hilos en paralelo
               = 8.2 mm² efectiva (reduce pérdidas por efecto piel)
```

```
Secundario:
Irms_s = Pout / Vs = 500W / 120V = 4.17A

Área conductora = 4.17A / 4A/mm² = 1.04 mm²
AWG equivalente: AWG 18 (0.82 mm²) o AWG 17 (1.04 mm²)

RECOMENDACIÓN: Usar alambre esmaltado AWG 16 (1.31 mm²)
```

---

### Pérdidas del Transformador

**Pérdidas en el núcleo (Core losses):**

```
Material: Ferrita N87
A 31.25 kHz y Bmax = 0.1T, de datasheet:
Pérdidas volumétricas ≈ 20 mW/cm³

Volumen del núcleo ETD49 ≈ 14 cm³
Pcore = 20 mW/cm³ × 14 cm³ = 0.28W
```

**Pérdidas en devanados (Copper losses):**

```
Primario:
Resistencia por espira (con Litz): rp ≈ 0.5 mΩ/espira
Rp = 9 espiras × 0.5 mΩ = 4.5 mΩ
Pérdidas: Pp = Irms_p² × Rp = (23.7A)² × 0.0045Ω = 2.53W

Secundario:
rs ≈ 2 mΩ/espira (más largo, menor calibre)
Rs = 50 espiras × 2 mΩ = 100 mΩ
Pérdidas: Ps = Irms_s² × Rs = (4.17A)² × 0.1Ω = 1.74W

Pérdidas totales cobre: Pcu = Pp + Ps = 4.27W
```

**Pérdidas totales del transformador:**

```
Ptotal = Pcore + Pcu = 0.28W + 4.27W = 4.55W
Eficiencia transformador: ηt = 1 - (Ptotal / Pin) = 1 - (4.55/568) = 99.2% ✓
```

---

## 💨 Disipación Térmica en MOSFETs

### Pérdidas en MOSFETs IRF3205

**Especificaciones IRF3205:**
- Vds(max) = 55V
- Id(max) = 110A
- RDS(on) @ 25°C = 8 mΩ
- RDS(on) @ 100°C ≈ 12 mΩ (factor de temperatura)

**Corriente RMS por MOSFET:**

```
En configuración puente H, cada MOSFET conduce aprox. 50% del tiempo
Irms_mosfet = Iin / √2 = 23.7A / 1.414 = 16.8A
```

**Pérdidas por conducción:**

```
Pcond = Irms² × RDS(on) × duty_cycle
Pcond = (16.8A)² × 0.012Ω × 0.5
Pcond = 1.69W por MOSFET

Para 4 MOSFETs: Ptotal_cond = 4 × 1.69W = 6.76W
```

**Pérdidas por conmutación (Switching losses):**

```
Psw = 0.5 × Vds × Id × (tr + tf) × fsw

Donde:
- Vds = 24V
- Id = 23.7A
- tr + tf ≈ 50ns (de datasheet)
- fsw = 31,250 Hz

Psw = 0.5 × 24V × 23.7A × 50×10⁻⁹s × 31,250Hz
Psw = 0.44W por MOSFET

Para 4 MOSFETs: Ptotal_sw = 4 × 0.44W = 1.76W
```

**Pérdidas totales:**

```
Ptotal_mosfets = Ptotal_cond + Ptotal_sw = 6.76W + 1.76W = 8.52W
```

---

### Requerimientos de Disipación

**Cálculo de temperatura:**

```
ΔT = Rθ × P

Donde Rθ es la resistencia térmica total:
Rθ_jc (junction-case) = 0.75 °C/W (de datasheet)
Rθ_cs (case-sink) = 0.5 °C/W (con pasta térmica)
Rθ_sa (sink-ambient) = ? (depende del disipador)

Para mantener Tj < 100°C con Ta = 40°C:
ΔT_permitido = 100°C - 40°C = 60°C

Rθ_total = ΔT / P = 60°C / 8.52W = 7.04 °C/W

Rθ_sa = Rθ_total - Rθ_jc - Rθ_cs
Rθ_sa = 7.04 - 0.75 - 0.5 = 5.79 °C/W
```

**Disipador recomendado:**

```
Se requiere disipador con Rθ_sa < 5.79 °C/W

Con ventilación forzada (ventilador 12V):
- Disipador de 2-3 °C/W es adecuado
- Dimensiones típicas: 150mm × 100mm × 25mm (aletas)

Sin ventilación (convección natural):
- Disipador más grande: 1-2 °C/W
- Dimensiones típicas: 200mm × 150mm × 40mm
```

---

## 🔌 Filtro LC de Salida

### Diseño del Filtro

**Objetivos:**
- Frecuencia de corte: fc ≈ 1-3 kHz (entre 60Hz y 31.25kHz)
- Atenuación de frecuencia PWM (31.25kHz): >40dB
- THD < 5%

**Frecuencia de corte:**

```
fc = 1 / (2π√(L×C))

Seleccionando fc = 2 kHz:
2000 = 1 / (2π√(L×C))
L×C = 1 / (4π² × 4×10⁶)
L×C = 6.33 × 10⁻⁹
```

**Selección de valores:**

```
Opción 1: L = 1 mH, C = 6.33 µF
         fc = 2.0 kHz ✓

Opción 2: L = 2 mH, C = 3.2 µF
         fc = 1.99 kHz ✓ (más común, mejor disponibilidad)

Opción 3: L = 1.5 mH, C = 4.7 µF
         fc = 1.89 kHz ✓
```

**RECOMENDACIÓN: L = 2 mH, C = 2.2 µF (serie 2 × 1.1 µF)**

---

### Atenuación del Filtro

**Filtro LC es de 2do orden → -40dB/década**

```
Atenuación a 31.25 kHz:

Ratio de frecuencias: r = 31.25kHz / 2kHz = 15.625
En décadas: log₁₀(15.625) = 1.194 décadas

Atenuación = -40 dB/década × 1.194 = -47.8 dB

Factor de reducción: 10^(47.8/20) = 245×

Si voltaje PWM pico-pico = 48V:
Voltaje residual = 48V / 245 = 0.196V (insignificante) ✓
```

---

### Especificaciones de Componentes

**Inductor:**

```
Valor: L = 2 mH
Corriente RMS: Irms = 5A (mínimo)
Corriente pico: Ipk = 6A (con margen)
Tipo: Toroidal, núcleo de polvo de hierro (Iron Powder)
Material: -26 mix o -52 mix (baja pérdida a 2kHz)

Construcción DIY:
Núcleo: T106-26 (2.69 cm OD)
Espiras: N = 100√(L_mH / AL) = 100√(2 / 0.088) ≈ 48 espiras
Alambre: AWG 16 (1.31 mm²)
```

**Capacitor:**

```
Valor: C = 2.2 µF (2 × 1.1 µF en paralelo)
Voltaje: Mínimo 250V AC, recomendado 310V o 630V
Tipo: Film (Polipropileno), clase X2
Ripple current: > 1A RMS

Ejemplo: Vishay MKP1848S22 (2.2µF, 310V AC)
```

---

## 📊 Eficiencia Total del Sistema

### Desglose de Pérdidas

```
Componente              | Pérdidas | % de Pin
─────────────────────────┼──────────┼─────────
MOSFETs (4x)           |   8.52W  |  1.50%
Drivers IR2110 (2x)    |   1.00W  |  0.18%
Transformador (núcleo) |   0.28W  |  0.05%
Transformador (cobre)  |   4.27W  |  0.75%
Filtro LC (inductor)   |   1.50W  |  0.26%
Capacitores ESR        |   0.50W  |  0.09%
Cableado/conexiones    |   2.00W  |  0.35%
─────────────────────────┼──────────┼─────────
Total de pérdidas      |  18.07W  |  3.18%

Potencia de salida     | 500.00W  | 88.01%
Eficiencia total       |   η = 88.0% ✓
```

**Análisis:**
- Pérdidas principales están en MOSFETs (47% de pérdidas totales)
- Transformador bien diseñado (25% de pérdidas)
- Cableado y conexiones significativos (11% de pérdidas)

---

## 🔧 Mejoras para Alta Eficiencia

### 1. Reducir Pérdidas en MOSFETs

```
Opción A: Usar MOSFETs de menor RDS(on)
- IRFB4110 (100V, 180A, RDS(on)=3.7mΩ)
  Pérdidas: 1.69W × (3.7/12) = 0.52W por MOSFET
  Ahorro: 1.17W × 4 = 4.68W
  Nueva eficiencia: 89.5%

Opción B: Sincronización óptima (dead-time mínimo)
  Reducir DEAD_TIME de 20 a 10 (si drivers lo permiten)
  Reducción pérdidas de conmutación: ~20%
  Ahorro: ~0.35W
```

### 2. Optimizar Transformador

```
- Usar alambre Litz de mejor calidad
- Aumentar sección de ventana (Aw)
- Entrelazar devanados (interleaved windings)
  Reducción pérdidas: 30-40%
  Ahorro: 1.3-1.7W
```

### 3. Filtro Optimizado

```
- Usar inductor con núcleo de sendust (menor pérdida)
- Capacitores de film con ESR ultra-bajo
  Ahorro: 0.5-1.0W
```

**Eficiencia potencial máxima: 90-92%**

---

## 📈 Curvas de Rendimiento

### Eficiencia vs. Carga

```
Carga (%)  | Potencia | Eficiencia | Pérdidas
───────────┼──────────┼────────────┼──────────
   10%     |   50W    |   75%      |   16.7W
   25%     |  125W    |   82%      |   27.4W
   50%     |  250W    |   87%      |   37.4W
   75%     |  375W    |   88%      |   51.1W
  100%     |  500W    |   88%      |   68.2W
  110%     |  550W    |   87%      |   82.2W (sobrecarga)
```

**Punto óptimo:** 75-100% de carga

---

## ⚡ Dimensionamiento de Batería

### Autonomía

```
Batería 24V, 100Ah:
Energía almacenada: E = 24V × 100Ah = 2400 Wh

Con carga de 500W y eficiencia 88%:
Tiempo de autonomía: t = E / P = 2400Wh / 500W = 4.8 horas

Considerando descarga máxima 80% (para vida útil):
Tiempo real: t = 4.8h × 0.8 = 3.84 horas ≈ 3h 50min
```

### Selección de Batería

| Capacidad | Voltaje | Tipo | Tiempo @ 500W | Costo aprox |
|-----------|---------|------|---------------|-------------|
| 50Ah | 24V | AGM | 1h 55min | $150-200 |
| 75Ah | 24V | AGM | 2h 52min | $200-280 |
| 100Ah | 24V | Litio | 3h 50min | $400-600 |
| 200Ah | 24V | AGM | 7h 40min | $500-700 |

---

## 🧪 Fórmulas de Referencia Rápida

```
Potencia de entrada:       Pin = Pout / η
Corriente de entrada:      Iin = Pin / Vin
Corriente RMS de salida:   Iout_rms = Pout / Vout
Corriente pico:            Ipk = Irms × √2

Relación transformador:    n = Ns / Np = Vs / Vp
Espiras primario:          Np = (Vp × 10⁴) / (4.44 × f × Bmax × Ae)
Espiras secundario:        Ns = Np × n

Frecuencia corte LC:       fc = 1 / (2π√(L×C))
Impedancia inductor:       XL = 2πfL
Impedancia capacitor:      XC = 1 / (2πfC)

Pérdidas conducción:       Pcond = Irms² × RDS(on)
Pérdidas conmutación:      Psw = 0.5 × V × I × (tr+tf) × fsw

Resistencia térmica:       Rθ = ΔT / P
Temperatura junction:      Tj = Ta + (Rθ × P)

Eficiencia:                η = Pout / Pin × 100%
THD:                       THD = √(Σ(Vn²)) / V1 × 100%
```

---

**Última actualización:** Noviembre 2025
