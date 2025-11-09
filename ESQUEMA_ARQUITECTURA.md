# 📱 ESQUEMA DE ARQUITECTURA - APLICACIÓN DE AGENDA

## 🏗️ ARQUITECTURA GENERAL

```
┌─────────────────────────────────────────────────────────────────────┐
│                        CAPA DE PRESENTACIÓN                          │
│                          (Activities)                                │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐          │
│  │  MainActivity │    │ NuevoActivity│    │  VerActivity │          │
│  │              │    │              │    │              │          │
│  │ - RecyclerView│   │ - EditText   │    │ - EditText   │          │
│  │ - SearchView │    │   (nombre)   │    │   (readonly) │          │
│  │ - FAB        │    │ - EditText   │    │ - FAB Editar │          │
│  │              │    │   (telefono) │    │ - FAB Eliminar│         │
│  │              │    │ - EditText   │    │              │          │
│  │              │    │   (correo)   │    │              │          │
│  │              │    │ - Button     │    │              │          │
│  └──────┬───────┘    └──────┬───────┘    └──────┬───────┘          │
│         │                   │                    │                  │
│         │                   │              ┌─────┴─────┐           │
│         │                   │              │           │           │
│         │                   │         ┌────▼─────┐     │           │
│         │                   │         │ Editar   │     │           │
│         │                   │         │ Activity │     │           │
│         │                   │         │          │     │           │
│         │                   │         │- EditText│     │           │
│         │                   │         │- Button  │     │           │
│         │                   │         │  Guardar │     │           │
│         │                   │         └────┬─────┘     │           │
│         │                   │              │           │           │
└─────────┼───────────────────┼──────────────┼───────────┼───────────┘
          │                   │              │           │
          │                   │              │           │
┌─────────┼───────────────────┼──────────────┼───────────┼───────────┐
│         │    CAPA DE ADAPTADORES           │           │           │
│         │                                  │           │           │
│    ┌────▼────────────────┐                │           │           │
│    │ListaContactosAdapter│                │           │           │
│    │                     │                │           │           │
│    │- listaContactos     │                │           │           │
│    │- listaOriginal      │                │           │           │
│    │                     │                │           │           │
│    │Métodos:             │                │           │           │
│    │+ filtrado()         │                │           │           │
│    │+ onBindViewHolder() │                │           │           │
│    └──────────┬──────────┘                │           │           │
│               │                           │           │           │
└───────────────┼───────────────────────────┼───────────┼───────────┘
                │                           │           │
                │                           │           │
┌───────────────┼───────────────────────────┼───────────┼───────────┐
│               │     CAPA DE DATOS         │           │           │
│               │                           │           │           │
│          ┌────▼───────────────────────────▼───────────▼────┐      │
│          │            DbContactos                           │      │
│          │                                                  │      │
│          │  Operaciones CRUD:                              │      │
│          │  + insertarContacto()                           │      │
│          │  + mostrarContactos()                           │      │
│          │  + verContacto(id)                              │      │
│          │  + editarContacto(id, ...)                      │      │
│          │  + eliminarContacto(id)                         │      │
│          │                                                  │      │
│          └───────────────────┬──────────────────────────────┘      │
│                              │                                     │
│                              │ extends                             │
│                              │                                     │
│                   ┌──────────▼────────────┐                        │
│                   │      DbHelper         │                        │
│                   │                       │                        │
│                   │ - DATABASE_NOMBRE     │                        │
│                   │ - DATABASE_VERSION    │                        │
│                   │ - TABLE_CONTACTOS     │                        │
│                   │                       │                        │
│                   │ + onCreate()          │                        │
│                   │ + onUpgrade()         │                        │
│                   └───────────┬───────────┘                        │
│                               │                                    │
│                               │                                    │
│                    ┌──────────▼──────────┐                         │
│                    │   SQLite Database   │                         │
│                    │                     │                         │
│                    │  Tabla: t_contactos │                         │
│                    │  - id (PK)          │                         │
│                    │  - nombre           │                         │
│                    │  - telefono         │                         │
│                    │  - correo_electronico│                        │
│                    └─────────────────────┘                         │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                         CAPA DE MODELO                               │
│                                                                      │
│                    ┌─────────────────────┐                          │
│                    │  Contactos (Entity) │                          │
│                    │                     │                          │
│                    │  - id: int          │                          │
│                    │  - nombre: String   │                          │
│                    │  - telefono: String │                          │
│                    │  - correoElectornico│                          │
│                    │                     │                          │
│                    │  + getters/setters  │                          │
│                    └─────────────────────┘                          │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 🔄 FLUJO DE NAVEGACIÓN

```
                    ┌──────────────────┐
                    │   MainActivity   │ ◄─┐
                    │  (Lista)         │   │
                    └────┬─────┬───────┘   │
                         │     │           │
           ┌─────────────┘     └──────────┐│
           │                              ││
           │ [FAB/Menu]            [Click]││
           │                              ││
    ┌──────▼──────┐              ┌───────▼▼─────┐
    │   Nuevo     │              │   Ver        │
    │   Activity  │              │   Activity   │
    │             │              │              │
    │  [Guardar]  │              │ [FAB Editar] │
    └─────────────┘              └──────┬───┬───┘
                                        │   │
                                        │   │[FAB Eliminar]
                                        │   │
                                  [Click]   └──────┐
                                        │          │
                                 ┌──────▼──────┐   │
                                 │   Editar    │   │
                                 │   Activity  │   │
                                 │             │   │
                                 │  [Guardar]  │   │
                                 └──────┬──────┘   │
                                        │          │
                                        └──────────┘
                                        Volver a MainActivity
```

---

## 📊 DIAGRAMA DE CLASES

```
┌─────────────────────────┐
│   AppCompatActivity     │
└───────────┬─────────────┘
            │
            │ extends
            │
    ┌───────┴────────┬──────────┬──────────┐
    │                │          │          │
┌───▼────────┐  ┌───▼──────┐ ┌─▼────────┐ ┌▼────────────┐
│ MainActivity│  │  Nuevo  │ │   Ver    │ │   Editar    │
│             │  │ Activity │ │ Activity │ │  Activity   │
├─────────────┤  ├──────────┤ ├──────────┤ ├─────────────┤
│-txtBuscar   │  │-txtNombre│ │-txtNombre│ │-txtNombre   │
│-listaContactos││-txtTelef.│ │-txtTelef.│ │-txtTelefono │
│-fabNuevo    │  │-txtCorreo│ │-txtCorreo│ │-txtCorreo   │
│-adapter     │  │-btnGuarda│ │-fabEditar│ │-btnGuarda   │
│             │  │          │ │-fabElimin│ │-contacto    │
├─────────────┤  ├──────────┤ ├──────────┤ ├─────────────┤
│+onCreate()  │  │+onCreate()│ │+onCreate()│ │+onCreate()  │
│+nuevoReg()  │  │-limpiar()│ │-lista()  │ │-verRegistro()│
│+onQueryText.│  │          │ │          │ │             │
└─────┬───────┘  └────┬─────┘ └────┬─────┘ └──────┬──────┘
      │               │            │              │
      │ usa           │ usa        │ usa          │ usa
      │               │            │              │
      │         ┌─────┴────────────┴──────────────┘
      │         │
      │    ┌────▼──────────┐
      │    │  DbContactos  │
      │    ├───────────────┤
      │    │-context       │
      │    ├───────────────┤
      │    │+insertarContacto()
      │    │+mostrarContactos()
      │    │+verContacto(id)
      │    │+editarContacto(...)
      │    │+eliminarContacto(id)
      │    └───────┬───────┘
      │            │ extends
      │            │
      │    ┌───────▼───────┐
      │    │   DbHelper    │
      │    ├───────────────┤
      │    │+onCreate()    │
      │    │+onUpgrade()   │
      │    └───────────────┘
      │
      │ usa
      │
┌─────▼──────────────────┐
│ListaContactosAdapter   │
├────────────────────────┤
│-listaContactos         │
│-listaOriginal          │
├────────────────────────┤
│+onCreateViewHolder()   │
│+onBindViewHolder()     │
│+filtrado(String)       │
│+getItemCount()         │
│                        │
│  [Inner Class]         │
│  ContactoViewHolder    │
└────────┬───────────────┘
         │ usa
         │
    ┌────▼────────┐
    │  Contactos  │
    ├─────────────┤
    │-id          │
    │-nombre      │
    │-telefono    │
    │-correoElec..│
    ├─────────────┤
    │+getId()     │
    │+setId()     │
    │+getNombre() │
    │+setNombre() │
    │+getTelefono()│
    │+setTelefono()│
    │+getCorreoE..()│
    │+setCorreoE..()│
    └─────────────┘
```

---

## 🔧 FUNCIONALIDADES POR COMPONENTE

### 📱 MainActivity
**Responsabilidades:**
- Mostrar lista de contactos en RecyclerView
- Búsqueda en tiempo real de contactos
- Navegar a crear nuevo contacto (FAB + menú)
- Navegar a ver detalle de contacto (click en item)

**Interacciones:**
- `DbContactos.mostrarContactos()` → Obtener lista completa
- `ListaContactosAdapter.filtrado()` → Filtrar búsqueda
- `Intent → NuevoActivity` → Crear contacto
- `Intent → VerActivity` → Ver detalle

---

### ➕ NuevoActivity
**Responsabilidades:**
- Capturar datos del nuevo contacto
- Validar campos obligatorios
- Insertar contacto en base de datos

**Interacciones:**
- `DbContactos.insertarContacto()` → Guardar en BD
- Toast para feedback al usuario

---

### 👁️ VerActivity
**Responsabilidades:**
- Mostrar detalles del contacto (solo lectura)
- Permitir editar contacto
- Permitir eliminar contacto con confirmación

**Interacciones:**
- `DbContactos.verContacto(id)` → Obtener datos
- `Intent → EditarActivity` → Editar
- `DbContactos.eliminarContacto(id)` → Eliminar
- `AlertDialog` → Confirmar eliminación

---

### ✏️ EditarActivity
**Responsabilidades:**
- Mostrar datos actuales del contacto
- Permitir modificar información
- Validar y actualizar en base de datos

**Interacciones:**
- `DbContactos.verContacto(id)` → Cargar datos
- `DbContactos.editarContacto()` → Actualizar
- `Intent → VerActivity` → Volver después de guardar

---

### 🗄️ DbHelper
**Responsabilidades:**
- Crear base de datos SQLite
- Gestionar versiones de BD
- Definir esquema de tabla

**Esquema:**
```sql
CREATE TABLE t_contactos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    telefono TEXT NOT NULL,
    correo_electronico TEXT
);
```

---

### 💾 DbContactos
**Responsabilidades:**
- Operaciones CRUD sobre contactos
- Consultas SQL
- Gestión de cursor

**Métodos:**
- `insertarContacto()` → INSERT
- `mostrarContactos()` → SELECT ALL + ORDER BY
- `verContacto(id)` → SELECT WHERE id
- `editarContacto()` → UPDATE
- `eliminarContacto()` → DELETE

---

### 📋 ListaContactosAdapter
**Responsabilidades:**
- Adaptador para RecyclerView
- Vincular datos con vistas
- Filtrado de lista
- Manejo de clicks

**Características:**
- Mantiene lista original y filtrada
- Filtrado por nombre (case-insensitive)
- Click listener para navegación

---

### 📦 Contactos (Entidad)
**Responsabilidades:**
- Modelo de datos
- Encapsulación de atributos
- Getters y setters

**Atributos:**
- `id`: Identificador único
- `nombre`: Nombre del contacto
- `telefono`: Número telefónico
- `correoElectornico`: Email

---

## 🎨 CAPA DE UI (Layouts)

```
┌─────────────────────────┐
│  activity_main.xml      │
│  - SearchView           │
│  - RecyclerView         │
│  - FloatingActionButton │
└─────────────────────────┘

┌─────────────────────────┐
│ lista_item_contacto.xml │
│  - TextView (nombre)    │
│  - TextView (telefono)  │
│  - TextView (correo)    │
└─────────────────────────┘

┌─────────────────────────┐
│  activity_nuevo.xml     │
│  - EditText (nombre)    │
│  - EditText (telefono)  │
│  - EditText (correo)    │
│  - Button (guardar)     │
└─────────────────────────┘

┌─────────────────────────┐
│  activity_ver.xml       │
│  - EditText (readonly)  │
│  - FAB (editar)         │
│  - FAB (eliminar)       │
└─────────────────────────┘
```

---

## 🔐 PATRONES DE DISEÑO UTILIZADOS

1. **Singleton Pattern** (implícito)
   - DbHelper mantiene instancia de base de datos

2. **Adapter Pattern**
   - ListaContactosAdapter adapta datos a RecyclerView

3. **View Holder Pattern**
   - ContactoViewHolder para optimizar RecyclerView

4. **DAO Pattern** (implícito)
   - DbContactos actúa como Data Access Object

---

## 📝 RESUMEN DE TECNOLOGÍAS

- **Lenguaje:** Java
- **UI:** RecyclerView, SearchView, FloatingActionButton
- **Base de datos:** SQLite
- **Arquitectura:** Model-Activity-Adapter
- **Componentes Android:**
  - AppCompatActivity
  - SQLiteOpenHelper
  - RecyclerView.Adapter
  - AlertDialog
  - Intent

---

## 🚀 FLUJO DE DATOS

```
Usuario → Activity → DbContactos → DbHelper → SQLite
                ↓
        ListaContactosAdapter
                ↓
          Contactos (Entity)
                ↓
          RecyclerView → Usuario
```
