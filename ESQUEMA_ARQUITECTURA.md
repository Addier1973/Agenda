# 📱 ESQUEMA DE ARQUITECTURA - APLICACIÓN AGENDA

## 📋 Índice
1. [Arquitectura General](#arquitectura-general)
2. [Flujo de Navegación](#flujo-de-navegación)
3. [Diagrama de Clases](#diagrama-de-clases)
4. [Estructura de Base de Datos](#estructura-de-base-de-datos)
5. [Componentes Principales](#componentes-principales)

---

## 🏗️ Arquitectura General

```
┌─────────────────────────────────────────────────────────────┐
│                    CAPA DE PRESENTACIÓN                       │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │ MainActivity│  │NuevoActivity│  │ VerActivity │         │
│  │  (Lista)    │  │  (Crear)    │  │ (Detalles)  │         │
│  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘         │
│         │                 │                 │                 │
│         │                 │         ┌───────┴──────┐         │
│         │                 │         │EditarActivity│         │
│         │                 │         │  (Modificar)  │         │
│         │                 │         └───────┬──────┘         │
│         │                 │                 │                 │
├─────────┼─────────────────┼─────────────────┼───────────────┤
│         │                 │                 │                 │
│  ┌──────▼─────────────────▼─────────────────▼──────┐        │
│  │      ListaContactosAdapter (RecyclerView)        │        │
│  │          - Muestra lista de contactos             │        │
│  │          - Implementa filtrado/búsqueda          │        │
│  └──────────────────────┬───────────────────────────┘        │
│                         │                                     │
├─────────────────────────┼─────────────────────────────────────┤
│                         │        CAPA DE DATOS                │
├─────────────────────────┼─────────────────────────────────────┤
│                         │                                     │
│  ┌──────────────────────▼───────────────────────────┐        │
│  │             DbContactos (DAO)                     │        │
│  │  - insertarContacto()                              │        │
│  │  - mostrarContactos()                              │        │
│  │  - verContacto()                                   │        │
│  │  - editarContacto()                                │        │
│  │  - eliminarContacto()                              │        │
│  └──────────────────────┬───────────────────────────┘        │
│                         │                                     │
│  ┌──────────────────────▼───────────────────────────┐        │
│  │          DbHelper (SQLiteOpenHelper)              │        │
│  │  - Gestión de base de datos SQLite                │        │
│  │  - Versión: 2                                      │        │
│  │  - DB: agenda.db                                   │        │
│  └──────────────────────┬───────────────────────────┘        │
│                         │                                     │
├─────────────────────────┼─────────────────────────────────────┤
│                         │      CAPA DE ENTIDADES              │
├─────────────────────────┼─────────────────────────────────────┤
│                         │                                     │
│  ┌──────────────────────▼───────────────────────────┐        │
│  │              Contactos (POJO)                     │        │
│  │  - id: int                                         │        │
│  │  - nombre: String                                  │        │
│  │  - telefono: String                                │        │
│  │  - correoElectornico: String                       │        │
│  └────────────────────────────────────────────────────┘        │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔀 Flujo de Navegación

```
                          ┌──────────────────┐
                          │   MainActivity   │
                          │   (Lista)        │
                          │                  │
                          │  • RecyclerView  │
                          │  • SearchView    │
                          │  • FAB +         │
                          └────────┬─────────┘
                                   │
                     ┌─────────────┼─────────────┐
                     │             │             │
            ┌────────▼───────┐    │    ┌────────▼───────┐
            │ NuevoActivity  │    │    │  VerActivity   │
            │   (Crear)      │    │    │  (Ver detalles)│
            │                │    │    │                │
            │ • txtNombre    │    │    │ • Modo lectura │
            │ • txtTelefono  │    │    │ • FAB editar   │
            │ • txtCorreo    │    │    │ • FAB eliminar │
            │ • btnGuardar   │    │    └────────┬───────┘
            └────────┬───────┘    │             │
                     │            │    ┌────────▼───────┐
                     │            │    │EditarActivity  │
                     │            │    │  (Modificar)   │
                     │            │    │                │
                     │            │    │ • Campos edit. │
                     │            │    │ • btnGuardar   │
                     │            │    └────────┬───────┘
                     │            │             │
                     └────────────┴─────────────┘
                                  │
                        ┌─────────▼──────────┐
                        │   Volver a lista   │
                        │   (MainActivity)   │
                        └────────────────────┘

LEYENDA:
────► Navegación directa
FAB: FloatingActionButton
```

---

## 📊 Diagrama de Clases

```
┌─────────────────────────────────┐
│        <<Activity>>             │
│       MainActivity              │
├─────────────────────────────────┤
│ - txtBuscar: SearchView         │
│ - listaContactos: RecyclerView  │
│ - listaArrayContactos: ArrayList│
│ - fabNuevo: FloatingActionButton│
│ - adapter: ListaContactosAdapter│
├─────────────────────────────────┤
│ + onCreate()                    │
│ + onCreateOptionsMenu()         │
│ + onOptionsItemSelected()       │
│ + onQueryTextChange()           │
│ - nuevoRegistro()               │
└─────────────┬───────────────────┘
              │ usa
              │
┌─────────────▼───────────────────┐
│    <<Adapter>>                  │
│  ListaContactosAdapter          │
├─────────────────────────────────┤
│ - listaContactos: ArrayList     │
│ - listaOriginal: ArrayList      │
├─────────────────────────────────┤
│ + onCreateViewHolder()          │
│ + onBindViewHolder()            │
│ + filtrado()                    │
│ + getItemCount()                │
│                                 │
│  [ContactoViewHolder]           │
│  - viewNombre: TextView         │
│  - viewTelefono: TextView       │
│  - viewCorreo: TextView         │
└─────────────┬───────────────────┘
              │ maneja
              │
┌─────────────▼───────────────────┐
│        <<Entity>>               │
│         Contactos               │
├─────────────────────────────────┤
│ - id: int                       │
│ - nombre: String                │
│ - telefono: String              │
│ - correoElectornico: String     │
├─────────────────────────────────┤
│ + getId()                       │
│ + setId()                       │
│ + getNombre()                   │
│ + setNombre()                   │
│ + getTelefono()                 │
│ + setTelefono()                 │
│ + getCorreoElectornico()        │
│ + setCorreoElectornico()        │
└─────────────▲───────────────────┘
              │
              │ usa
              │
┌─────────────┴───────────────────┐
│        <<DAO>>                  │
│       DbContactos               │
├─────────────────────────────────┤
│ - context: Context              │
├─────────────────────────────────┤
│ + insertarContacto(): long      │
│ + mostrarContactos(): ArrayList │
│ + verContacto(): Contactos      │
│ + editarContacto(): boolean     │
│ + eliminarContacto(): boolean   │
└─────────────┬───────────────────┘
              │ extiende
              │
┌─────────────▼───────────────────┐
│   <<SQLiteOpenHelper>>          │
│        DbHelper                 │
├─────────────────────────────────┤
│ - DATABASE_VERSION = 2          │
│ - DATABASE_NOMBRE = "agenda.db" │
│ + TABLE_CONTACTOS = "t_contactos"│
├─────────────────────────────────┤
│ + onCreate()                    │
│ + onUpgrade()                   │
└─────────────────────────────────┘


┌──────────────────────────────────────────────────────┐
│            OTRAS ACTIVITIES                          │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ┌─────────────────┐  ┌─────────────────┐          │
│  │ NuevoActivity   │  │  VerActivity    │          │
│  ├─────────────────┤  ├─────────────────┤          │
│  │ - txtNombre     │  │ - txtNombre     │          │
│  │ - txtTelefono   │  │ - txtTelefono   │          │
│  │ - txtCorreo     │  │ - txtCorreo     │          │
│  │ - btnGuarda     │  │ - fabEditar     │          │
│  ├─────────────────┤  │ - fabEliminar   │          │
│  │ + onCreate()    │  ├─────────────────┤          │
│  │ - limpiar()     │  │ + onCreate()    │          │
│  └─────────────────┘  │ - lista()       │          │
│                       └─────────────────┘          │
│                                                      │
│  ┌─────────────────┐                                │
│  │ EditarActivity  │                                │
│  ├─────────────────┤                                │
│  │ - txtNombre     │                                │
│  │ - txtTelefono   │                                │
│  │ - txtCorreo     │                                │
│  │ - btnGuarda     │                                │
│  │ - contacto      │                                │
│  ├─────────────────┤                                │
│  │ + onCreate()    │                                │
│  │ - verRegistro() │                                │
│  └─────────────────┘                                │
│                                                      │
│    Todas usan DbContactos para operaciones CRUD     │
└──────────────────────────────────────────────────────┘
```

---

## 🗄️ Estructura de Base de Datos

```
┌─────────────────────────────────────────┐
│        Base de Datos: agenda.db         │
│              Versión: 2                 │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│      Tabla: t_contactos                 │
├──────────────────┬──────────────────────┤
│  Campo           │  Tipo                │
├──────────────────┼──────────────────────┤
│  id              │  INTEGER (PK, AUTO)  │
│  nombre          │  TEXT (NOT NULL)     │
│  telefono        │  TEXT (NOT NULL)     │
│  correo_electronico │ TEXT (NULLABLE)   │
└──────────────────┴──────────────────────┘

Operaciones SQL:

CREATE:
INSERT INTO t_contactos (nombre, telefono, correo_electronico)
VALUES (?, ?, ?)

READ (Todos):
SELECT * FROM t_contactos ORDER BY nombre ASC

READ (Uno):
SELECT * FROM t_contactos WHERE id = ? LIMIT 1

UPDATE:
UPDATE t_contactos 
SET nombre = ?, telefono = ?, correo_electronico = ?
WHERE id = ?

DELETE:
DELETE FROM t_contactos WHERE id = ?
```

---

## 🔧 Componentes Principales

### 1️⃣ MainActivity
**Propósito:** Pantalla principal que muestra la lista de contactos
**Funcionalidades:**
- Muestra lista de contactos en RecyclerView
- Búsqueda/filtrado de contactos en tiempo real
- Navegación a pantalla de crear nuevo contacto
- Click en item para ver detalles

### 2️⃣ NuevoActivity
**Propósito:** Crear nuevos contactos
**Funcionalidades:**
- Formulario con campos: nombre, teléfono, correo
- Validación de campos obligatorios
- Inserción en base de datos
- Limpieza de formulario tras guardar

### 3️⃣ VerActivity
**Propósito:** Ver detalles de un contacto (modo lectura)
**Funcionalidades:**
- Muestra información del contacto
- Campos no editables
- Botón para editar contacto
- Botón para eliminar contacto (con diálogo de confirmación)

### 4️⃣ EditarActivity
**Propósito:** Modificar un contacto existente
**Funcionalidades:**
- Carga datos del contacto
- Permite editar campos
- Actualiza en base de datos
- Navega a ver contacto tras guardar

### 5️⃣ ListaContactosAdapter
**Propósito:** Adaptador para RecyclerView
**Funcionalidades:**
- Muestra lista de contactos
- Implementa filtrado por nombre
- Maneja clicks en items
- ViewHolder para optimización

### 6️⃣ DbHelper
**Propósito:** Gestión de base de datos SQLite
**Funcionalidades:**
- Creación de base de datos
- Creación de tablas
- Actualización de esquema

### 7️⃣ DbContactos
**Propósito:** Capa de acceso a datos (DAO)
**Funcionalidades:**
- CRUD completo de contactos
- Operaciones SQLite
- Conversión entre Cursor y objetos

### 8️⃣ Contactos
**Propósito:** Entidad/modelo de datos
**Funcionalidades:**
- Representa un contacto
- Getters y setters
- POJO (Plain Old Java Object)

---

## 📱 Layouts Asociados

```
┌─────────────────────────────────────────────────┐
│  Layout                  │  Activity/Adapter    │
├──────────────────────────┼──────────────────────┤
│  activity_main.xml       │  MainActivity        │
│  activity_nuevo.xml      │  NuevoActivity       │
│  activity_ver.xml        │  VerActivity +       │
│                          │  EditarActivity      │
│  lista_item_contacto.xml │  ListaContactosAdapter│
└──────────────────────────┴──────────────────────┘
```

---

## 🎨 Patrón de Diseño Utilizado

**Patrón:** MVC (Model-View-Controller) simplificado para Android

```
MODEL (Modelo)
  ├── Contactos (entidad)
  ├── DbHelper (gestor de BD)
  └── DbContactos (DAO)

VIEW (Vista)
  ├── activity_main.xml
  ├── activity_nuevo.xml
  ├── activity_ver.xml
  └── lista_item_contacto.xml

CONTROLLER (Controlador)
  ├── MainActivity
  ├── NuevoActivity
  ├── VerActivity
  ├── EditarActivity
  └── ListaContactosAdapter
```

---

## 🔄 Flujo de Datos

```
1. CREAR CONTACTO:
   Usuario → NuevoActivity → DbContactos.insertarContacto() 
   → SQLite → Éxito/Error → Toast

2. LISTAR CONTACTOS:
   MainActivity → DbContactos.mostrarContactos() 
   → SQLite → ArrayList<Contactos> → ListaContactosAdapter 
   → RecyclerView

3. VER CONTACTO:
   Click en item → VerActivity → DbContactos.verContacto(id) 
   → SQLite → Contactos → Mostrar datos

4. EDITAR CONTACTO:
   FAB editar → EditarActivity → Cargar datos → Usuario edita 
   → DbContactos.editarContacto() → SQLite → VerActivity

5. ELIMINAR CONTACTO:
   FAB eliminar → Diálogo confirmación → 
   DbContactos.eliminarContacto() → SQLite → MainActivity

6. BUSCAR CONTACTO:
   Usuario escribe → SearchView.onQueryTextChange() 
   → ListaContactosAdapter.filtrado() → notifyDataSetChanged()
```

---

## 📌 Resumen de Características

✅ **CRUD Completo:** Crear, Leer, Actualizar, Eliminar contactos
✅ **Búsqueda:** Filtrado en tiempo real por nombre
✅ **Validación:** Campos obligatorios (nombre y teléfono)
✅ **Persistencia:** Base de datos SQLite local
✅ **UI Moderna:** RecyclerView, FloatingActionButtons, SearchView
✅ **Confirmaciones:** Diálogo antes de eliminar
✅ **Navegación:** Flujo intuitivo entre pantallas
✅ **Ordenamiento:** Contactos ordenados alfabéticamente

---

## 🎯 Arquitectura en Capas

```
┌─────────────────────────────────────────┐
│     CAPA DE PRESENTACIÓN (UI)           │
│  Activities, Adapters, Layouts           │
└────────────────┬────────────────────────┘
                 │
┌────────────────▼────────────────────────┐
│     CAPA DE LÓGICA DE NEGOCIO           │
│  Validaciones, Navegación                │
└────────────────┬────────────────────────┘
                 │
┌────────────────▼────────────────────────┐
│     CAPA DE ACCESO A DATOS              │
│  DbContactos (DAO), DbHelper             │
└────────────────┬────────────────────────┘
                 │
┌────────────────▼────────────────────────┐
│     CAPA DE DATOS (PERSISTENCIA)        │
│  SQLite Database (agenda.db)             │
└─────────────────────────────────────────┘
                 │
┌────────────────▼────────────────────────┐
│     CAPA DE ENTIDADES                   │
│  Contactos (POJO)                        │
└─────────────────────────────────────────┘
```

---

**Generado:** $(date)
**Aplicación:** Agenda de Contactos Android
**Paquete:** com.cdp.agenda
