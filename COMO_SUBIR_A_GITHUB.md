# 🚀 CÓMO SUBIR LOS ARCHIVOS A GITHUB

## 📍 OPCIÓN 1: DESDE CURSOR/VS CODE

### Paso a paso:

1. **Abre la carpeta Arduino-inverter** en Cursor/VS Code

2. **En el panel izquierdo, busca el ícono de Git** (parece una rama o "Y"):
   ```
   ┌─────────────────┐
   │  📁 Archivos    │
   │  🔍 Buscar      │
   │  🔀 Git         │  ← CLICK AQUÍ
   │  🐛 Debug       │
   │  🧩 Extensiones │
   └─────────────────┘
   ```

3. **Verás un mensaje:**
   ```
   "Tu rama está adelantada por 1 commit"
   ```

4. **Busca el botón "Sincronizar cambios" o los tres puntos (⋮)**

5. **Click en los tres puntos (⋮)** arriba en el panel de Git

6. **Selecciona: "Push" o "Enviar"**

---

## 📍 OPCIÓN 2: DESDE TERMINAL (MÁS FÁCIL)

### En Cursor/VS Code:

1. **Presiona:** `Ctrl + Ñ` (o `Ctrl + ~` en teclado inglés)
   - O ve al menú: **Terminal → Nueva Terminal**

2. **Copia y pega estos comandos:**

```bash
cd /workspace/Arduino-inverter
git push origin main
```

3. **Presiona Enter**

4. Si te pide usuario/contraseña:
   - Usuario: tu nombre de GitHub
   - Contraseña: usar un **Personal Access Token** (no tu contraseña normal)

---

## 📍 OPCIÓN 3: DESDE GITHUB DESKTOP

Si tienes GitHub Desktop instalado:

1. Abre **GitHub Desktop**
2. En la barra superior: Selecciona **"Arduino-inverter"**
3. Verás: **"Push origin"** (botón azul arriba a la derecha)
4. **Click en "Push origin"**

---

## 🔑 SI TE PIDE AUTENTICACIÓN:

### Crear Personal Access Token:

1. Ve a: https://github.com/settings/tokens
2. Click en **"Generate new token"** → **"Generate new token (classic)"**
3. Dale un nombre: "Arduino-inverter-push"
4. Marca: ✅ **repo** (todos los permisos de repositorio)
5. Click en **"Generate token"**
6. **COPIA EL TOKEN** (solo se muestra una vez)
7. Usa este token como "contraseña" cuando Git te lo pida

---

## ✅ VERIFICA QUE FUNCIONÓ:

Después del push, abre en tu teléfono:
```
https://github.com/Tecno-Plus/Arduino-inverter
```

Deberías ver 3 archivos nuevos:
- ✅ ESQUEMA_ARQUITECTURA.md
- ✅ SCHEMATIC_VISUAL.md
- ✅ inverter_easyeda.json

---

## 🆘 SI NO FUNCIONA:

Dime qué mensaje de error ves y te ayudo a resolverlo.
