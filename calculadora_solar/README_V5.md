# 📊 CALCULADORA SOLAR V5 - METODOLOGÍA PROFESIONAL

## Sistema Fotovoltaico con Parámetros Técnicos Completos según NEC/IEC

**Email:** apolivera1973@gmail.com  
**Versión:** 5.0 - Metodología Profesional Completa  
**Fecha:** Diciembre 2024

---

## 🆕 VERSIÓN 5.0 - DISEÑO PROFESIONAL

### ⚡ **METODOLOGÍA COMPLETA IMPLEMENTADA:**

La Versión 5 incluye TODOS los parámetros técnicos necesarios para un diseño fotovoltaico profesional según estándares internacionales (NEC, IEC):

| Categoría | Parámetros Incluidos |
|-----------|---------------------|
| **Panel Solar** | Pmax, Vmp, Imp, **Voc**, **Isc**, Coef. temp. Voc/Isc/Pmax |
| **Inversor** | MPPT min/max, Vmax absoluto, Imax DC, Num. MPPT, Potencia AC |
| **Ambiente** | Temp. mín/máx, NOCT, HSP anual/crítico, Factor pérdidas |
| **Correcciones** | Voc temp. mín, Vmp operación, Isc temp. máx (automáticas) |
| **Baterías** | Tipo, DoD automático, Capacidad requerida |
| **Strings** | **EDITABLES por usuario**: Paneles/string, V, I, P |
| **Validaciones** | 4 validaciones NEC automáticas con resultado visual |

---

## 📋 ¿QUÉ HAY DE NUEVO EN V5?

### ✅ **1. ESPECIFICACIONES COMPLETAS DEL PANEL**
**Antes (V4.1):**
- Pmax, Vmp, Imp

**Ahora (V5):**
- Pmax, Vmp, Imp
- **+ Voc (Voltaje circuito abierto)** ⚠️ CRÍTICO
- **+ Isc (Corriente cortocircuito)** ⚠️ CRÍTICO
- **+ Coef. temp. Voc** (%/°C)
- **+ Coef. temp. Isc** (%/°C)
- **+ Coef. temp. Pmax** (%/°C)

### ✅ **2. ESPECIFICACIONES COMPLETAS DEL INVERSOR**
**Antes (V4.1):**
- Potencia AC

**Ahora (V5):**
- Potencia AC
- **+ Voltaje MPPT mínimo**
- **+ Voltaje MPPT máximo**
- **+ Voltaje DC máximo absoluto** ⚠️ NO EXCEDER
- **+ Corriente DC máxima por MPPT**
- **+ Número de MPPT**
- **+ Frecuencia** (50/60 Hz)

### ✅ **3. DATOS AMBIENTALES DEL SITIO**
**Nuevo en V5:**
- Temperatura mínima histórica (para Voc máximo)
- Temperatura máxima promedio (para corrientes)
- Temp. operación célula (NOCT)
- HSP promedio anual
- HSP mes crítico
- Factor de pérdidas del sistema (0.75-0.85)

### ✅ **4. CORRECCIONES POR TEMPERATURA (AUTOMÁTICAS)**
**Nuevo en V5:**

La calculadora realiza automáticamente las correcciones según normativa:

```
Voc_min = Voc_STC × [1 + (Coef_Voc/100) × (Tmin - 25)]
Vmp_op = Vmp_STC × [1 + (Coef_Pmax/100) × (Top - 25)]
Isc_max = Isc_STC × [1 + (Coef_Isc/100) × (Tmax - 25)]
```

### ✅ **5. DISEÑO DE STRINGS FLEXIBLE**
**Antes (V4.1):**
- Valores calculados pero no editables correctamente

**Ahora (V5):**
- **⚠️ CAMPOS TOTALMENTE EDITABLES:**
  - Paneles por string (serie)
  - Voltaje del string (V)
  - Corriente del string (A)
  - Potencia del string (W)

**¿Por qué editables?** Porque el diseño del string depende del modelo específico del inversor y la configuración real de los paneles. El instalador DEBE ingresar estos valores según su diseño.

### ✅ **6. VALIDACIONES DE SEGURIDAD NEC**
**Nuevo en V5:**

4 validaciones automáticas con resultado visual:

| Validación | Norma | Estado |
|------------|-------|--------|
| Voc string × 1.25 < Vmax inversor | NEC 690.7 | ✅ OK / ❌ FALLA |
| Isc string × 1.25 < Imax inversor | NEC 690.8 | ✅ OK / ❌ FALLA |
| Potencia arreglo ≤ Pinv × 1.2 | Buenas prácticas | ✅ OK / ⚠️ SOBRECARGA |
| Voltaje batería = Voltaje inversor | Compatibilidad | ✅ OK / ❌ VERIF |

---

## 📊 ESTRUCTURA DE LA CALCULADORA V5

### **HOJA 1: CALCULADORA** (Principal)

#### **Sección 1: ☀️ Especificaciones del Panel Solar**
- 8 parámetros técnicos (TODOS EDITABLES)
- Incluye parámetros críticos: Voc, Isc, coeficientes de temperatura

#### **Sección 2: 🔌 Especificaciones del Inversor**
- 9 parámetros técnicos (TODOS EDITABLES)
- Incluye rangos MPPT, corrientes, voltajes máximos

#### **Sección 3: 🌡️ Datos Ambientales del Sitio**
- 6 parámetros climáticos (TODOS EDITABLES)
- Temperaturas, HSP, factor de pérdidas

#### **Sección 4: ⚡ Correcciones por Temperatura**
- 3 cálculos automáticos según temperatura
- Correcciones de Voc, Vmp, Isc

#### **Sección 5: 💡 Cálculo de Consumo Eléctrico**
- Lista de equipos con potencia y horas de uso
- Consumo total diario calculado

#### **Sección 6: 🔋 Dimensionamiento de Baterías**
- Tipo de batería (dropdown: Plomo-ácido/AGM/Gel/Litio)
- Profundidad de descarga automática según tipo
- Capacidad requerida (Ah) calculada

#### **Sección 7: ☀️ Dimensionamiento del Arreglo Fotovoltaico**
- Potencia FV requerida (calculada)
- Paneles necesarios teóricos (calculados)
- **DISEÑO DE STRING (EDITABLE por usuario):**
  - Paneles por string (serie)
  - Número de strings (paralelo)
  - Paneles totales a instalar
- **PARÁMETROS DEL STRING (EDITABLES):**
  - Voltaje del string (V)
  - Corriente del string (A)
  - Potencia del string (W)
  - Potencia total del arreglo

#### **Sección 8: ⚠️ Validaciones de Seguridad (NEC 690)**
- 4 validaciones automáticas con resultado visual

### **HOJA 2: LISTA DE MATERIALES**
- Desglose completo de materiales por categoría
- Cantidades calculadas automáticamente
- Precios unitarios editables
- Subtotales por categoría
- Costo de mano de obra editable
- INVERSIÓN TOTAL DEL PROYECTO

### **HOJA 3: INFORME CLIENTE**
- Resumen ejecutivo para el cliente
- Especificaciones del sistema
- Generación eléctrica estimada
- Inversión del proyecto
- Formato listo para exportar a PDF

---

## 🎨 IDENTIFICACIÓN VISUAL DE CAMPOS

### 🟨 **FONDO AMARILLO = CAMPO EDITABLE**
Puedes y DEBES modificar estos valores según tus datos específicos.

### 🟩 **FONDO VERDE = RESULTADO CALCULADO**
Se actualiza automáticamente según los valores editables.

### 🔴 **TEXTO ROJO = ADVERTENCIA IMPORTANTE**
Indica parámetros críticos o que requieren atención especial.

### ✅ **VERDE + Checkmark = Validación OK**
La validación de seguridad pasó correctamente.

### ❌ **ROJO + X = Validación FALLIDA**
La validación de seguridad NO pasó, ajustar diseño.

---

## 📖 GUÍA DE USO RÁPIDO

### **PASO 1: DATOS DEL PANEL SOLAR**
1. Consulta la **hoja técnica** del panel que vas a utilizar
2. Ingresa TODOS los parámetros: Pmax, Vmp, Imp, **Voc**, **Isc**, coeficientes
3. ⚠️ Los **coeficientes de temperatura son CRÍTICOS**

### **PASO 2: DATOS DEL INVERSOR**
1. Consulta la **hoja técnica** del inversor
2. Ingresa rangos MPPT (mínimo y máximo)
3. Ingresa voltaje DC máximo absoluto
4. Ingresa corriente DC máxima por MPPT
5. ⚠️ El **voltaje DC máximo NO DEBE EXCEDERSE**

### **PASO 3: DATOS AMBIENTALES**
1. Consulta el **atlas solar** de tu región para HSP
2. Ingresa temperaturas mínima y máxima históricas
3. Factor de pérdidas típico: **0.80** (puedes ajustarlo entre 0.75-0.85)

### **PASO 4: CONSUMOS**
1. Ingresa todos los equipos eléctricos
2. Especifica potencia (W) y horas de uso diario
3. La calculadora sumará el consumo total automáticamente

### **PASO 5: BATERÍAS**
1. Selecciona tipo de batería del dropdown
2. La profundidad de descarga se ajusta automáticamente:
   - Plomo-ácido: 50%
   - AGM: 70%
   - Gel: 60%
   - Litio: 90%
3. Ajusta días de autonomía según necesidad (típico: 1-3 días)

### **PASO 6: DISEÑO DE STRINGS** ⚠️ **IMPORTANTE**
1. La calculadora te dará el número **TEÓRICO** de paneles
2. **TÚ DEBES DECIDIR** la configuración de strings:
   - ¿Cuántos paneles en **serie** por string?
   - ¿Cuántos strings en **paralelo**?
3. **INGRESA MANUALMENTE:**
   - Voltaje del string según tu diseño
   - Corriente del string según tu diseño
   - Potencia del string según tu diseño

### **PASO 7: VALIDACIONES**
1. Revisa la sección de **validaciones de seguridad**
2. Si ves **❌ FALLA**, ajusta tu diseño de strings
3. **TODAS** las validaciones deben estar en **✅ OK**

### **PASO 8: LISTA DE MATERIALES Y PRESUPUESTO**
1. Revisa la hoja "Lista de Materiales"
2. Ajusta precios unitarios según tu mercado local
3. Ingresa costo de mano de obra
4. Obtén la inversión total del proyecto

### **PASO 9: INFORME CLIENTE**
1. Revisa la hoja "Informe Cliente"
2. Exporta a PDF para presentar al cliente

---

## 🔍 COMPARACIÓN DE VERSIONES

| Característica | V4.1 | V5 PROFESIONAL |
|----------------|------|----------------|
| Parámetros panel | 3 básicos | **8 completos** |
| Voc/Isc incluidos | ❌ No | ✅ **Sí** |
| Coef. temperatura | ❌ No | ✅ **Sí** |
| Rangos MPPT | ❌ No | ✅ **Sí** |
| Datos ambientales | ❌ No | ✅ **6 parámetros** |
| Correcciones temp. | ❌ No | ✅ **Automáticas** |
| Strings editables | ⚠️ Parcial | ✅ **Completamente** |
| Validaciones NEC | ❌ No | ✅ **4 automáticas** |
| Metodología | Básica | **Profesional completa** |
| Cumplimiento normas | No verificable | **NEC/IEC** |

---

## ⚠️ ADVERTENCIAS IMPORTANTES

### **1. PARÁMETROS DE STRING EDITABLES**
Los parámetros de STRING (voltaje, corriente, potencia) **SON EDITABLES** porque dependen del modelo específico de inversor y la configuración real de los paneles. **NO confíes en cálculos automáticos** para esto. Debes conocer tu sistema.

### **2. COEFICIENTES DE TEMPERATURA CRÍTICOS**
Las correcciones por temperatura **SON CRÍTICAS** para seguridad. **NO omitas** los coeficientes de temperatura del panel. Se encuentran en la hoja técnica del fabricante.

### **3. VALIDACIONES NEC OBLIGATORIAS**
Las validaciones NEC **SON OBLIGATORIAS** para cumplir con normativa. Si alguna validación falla (❌), **DEBES ajustar el diseño**. No instales un sistema que no cumpla las validaciones.

### **4. FACTOR DE PÉRDIDAS DEL SISTEMA**
El factor de pérdidas del sistema (típico **0.80**) considera:
- Pérdidas por cableado (1-3%)
- Pérdidas por suciedad (2-5%)
- Pérdidas por temperatura (5-15%)
- Pérdidas por sombras (0-10%)
- Pérdidas del inversor (3-5%)

### **5. PROFUNDIDAD DE DESCARGA (DoD)**
La profundidad de descarga varía según tipo de batería:
- **Plomo-ácido:** 50% (vida útil limitada si se excede)
- **AGM:** 70% (mejor rendimiento)
- **Gel:** 60% (buena durabilidad)
- **Litio:** 90% (mayor eficiencia y vida útil)

---

## 📚 REFERENCIAS Y NORMATIVAS

- **NEC** (National Electrical Code) Artículo 690 - Sistemas Fotovoltaicos
- **IEC 61215** - Cualificación del diseño de módulos fotovoltaicos
- **IEC 61730** - Seguridad de módulos fotovoltaicos
- **IEEE 1547** - Interconexión de recursos energéticos distribuidos

---

## 📥 ARCHIVOS DISPONIBLES

### ⭐ **Calculadora_Solar_V5_Profesional.xlsx** - VERSIÓN ACTUAL
**METODOLOGÍA PROFESIONAL COMPLETA:**
- ☀️ **8 parámetros del panel solar** (incluye Voc, Isc, coeficientes temp.)
- 🔌 **9 parámetros del inversor** (incluye rangos MPPT, corrientes)
- 🌡️ **6 datos ambientales** (temperaturas, HSP, factor pérdidas)
- ⚡ **Correcciones automáticas** por temperatura
- 🔋 **Dimensionamiento baterías** con DoD automático
- ⚙️ **Strings editables** por el usuario
- ⚠️ **4 validaciones NEC** automáticas
- 💰 **Lista de materiales** y presupuesto
- 📄 **Informe cliente** listo para PDF

### 5️⃣ **Calculadora_Solar_V4.1_Completa.xlsx**
- Versión anterior con lista de materiales
- Strings parcialmente editables

### 4️⃣ **Calculadora_Solar_V4_Listas_Informe.xlsx**
- Incluye listas desplegables
- Hoja de informe al cliente

### 3️⃣ **Calculadora_Solar_V3_Personalizable.xlsx**
- Sistema totalmente configurable
- Voltaje ajustable (12V/24V/48V/96V)
- Frecuencia seleccionable (50Hz/60Hz)
- Tipo de red editable

### 2️⃣ **Calculadora_Solar_V2_Completa.xlsx**
- Selector de tipo de baterías
- Tabla completa de costos

### 1️⃣ **Calculadora_Autoconsumo_Solar.xlsx**
- Versión básica original

---

## 💡 SOPORTE Y CONSULTAS

Para consultas sobre:
- ✅ Interpretación de resultados
- ✅ Ajuste de parámetros
- ✅ Validaciones de seguridad
- ✅ Dimensionamiento específico

**Contacto:** apolivera1973@gmail.com

⚠️ **NOTA:** Para diseños críticos o instalaciones comerciales/industriales, consulta con un **ingeniero electricista certificado** en sistemas fotovoltaicos.

---

## 📝 HISTORIAL DE VERSIONES

- **V5.0** (Dic 2024) - Metodología profesional completa con validaciones NEC
- **V4.1** (Dic 2024) - Lista de materiales y correcciones de cálculos
- **V4.0** (Dic 2024) - Listas desplegables e informe cliente
- **V3.0** (Dic 2024) - Sistema totalmente personalizable
- **V2.0** (Dic 2024) - Selector de baterías y tabla de costos
- **V1.0** (Dic 2024) - Versión inicial básica

---

## 📄 LICENCIA

Este software se proporciona "tal cual" sin garantías de ningún tipo. El usuario es responsable de verificar todos los cálculos y cumplir con las normativas locales aplicables.

**© 2024 - Todos los derechos reservados**

---

**⚡ ¡Diseña tu sistema fotovoltaico de forma profesional! ⚡**
