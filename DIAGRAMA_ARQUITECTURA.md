# 📱 Diagrama de Arquitectura - Aplicación Agenda de Contactos

## 🏗️ Diagrama de Clases y Relaciones

```mermaid
classDiagram
    class MainActivity {
        -SearchView txtBuscar
        -RecyclerView listaContactos
        -ArrayList~Contactos~ listaArrayContactos
        -FloatingActionButton fabNuevo
        -ListaContactosAdapter adapter
        +onCreate()
        +onCreateOptionsMenu()
        +onOptionsItemSelected()
        +onQueryTextChange()
        -nuevoRegistro()
    }

    class NuevoActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreoElectronico
        -Button btnGuarda
        +onCreate()
        -limpiar()
    }

    class EditarActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreo
        -Button btnGuarda
        -FloatingActionButton fabEditar
        -FloatingActionButton fabEliminar
        -boolean correcto
        -Contactos contacto
        -int id
        +onCreate()
        -verRegistro()
    }

    class VerActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreo
        -Button btnGuarda
        -FloatingActionButton fabEditar
        -FloatingActionButton fabEliminar
        -Contactos contacto
        -int id
        +onCreate()
        -lista()
    }

    class ListaContactosAdapter {
        -ArrayList~Contactos~ listaContactos
        -ArrayList~Contactos~ listaOriginal
        +onCreateViewHolder()
        +onBindViewHolder()
        +filtrado(String)
        +getItemCount()
    }

    class ContactoViewHolder {
        -TextView viewNombre
        -TextView viewTelefono
        -TextView viewCorreo
    }

    class DbHelper {
        -static int DATABASE_VERSION
        -static String DATABASE_NOMBRE
        +static String TABLE_CONTACTOS
        +onCreate(SQLiteDatabase)
        +onUpgrade(SQLiteDatabase, int, int)
    }

    class DbContactos {
        -Context context
        +insertarContacto(String, String, String) long
        +mostrarContactos() ArrayList~Contactos~
        +verContacto(int) Contactos
        +editarContacto(int, String, String, String) boolean
        +eliminarContacto(int) boolean
    }

    class Contactos {
        -int id
        -String nombre
        -String telefono
        -String correoElectornico
        +getId()
        +setId(int)
        +getNombre()
        +setNombre(String)
        +getTelefono()
        +setTelefono(String)
        +getCorreoElectornico()
        +setCorreoElectornico(String)
    }

    %% Relaciones de herencia
    DbContactos --|> DbHelper : extiende
    ListaContactosAdapter *-- ContactoViewHolder : contiene

    %% Relaciones de uso
    MainActivity --> DbContactos : usa
    MainActivity --> ListaContactosAdapter : usa
    MainActivity --> Contactos : usa
    MainActivity --> NuevoActivity : navega a

    NuevoActivity --> DbContactos : usa

    EditarActivity --> DbContactos : usa
    EditarActivity --> Contactos : usa
    EditarActivity --> VerActivity : navega a

    VerActivity --> DbContactos : usa
    VerActivity --> Contactos : usa
    VerActivity --> MainActivity : navega a
    VerActivity --> EditarActivity : navega a

    ListaContactosAdapter --> Contactos : usa
    ListaContactosAdapter --> VerActivity : navega a

    DbContactos --> Contactos : retorna
    DbHelper --> SQLiteDatabase : gestiona
```

## 📊 Diagrama de Flujo de Navegación

```mermaid
graph TD
    A[MainActivity<br/>Lista de Contactos] -->|FAB + / Menú| B[NuevoActivity<br/>Crear Contacto]
    A -->|Click en item| C[VerActivity<br/>Ver Detalles]
    A -->|SearchView| A1[Filtrar Lista]
    
    B -->|Guardar| A
    
    C -->|FAB Editar| D[EditarActivity<br/>Editar Contacto]
    C -->|FAB Eliminar| A
    
    D -->|Guardar| C
    
    style A fill:#e1f5ff
    style B fill:#fff9e1
    style C fill:#e8f5e9
    style D fill:#fff3e0
```

## 🗄️ Diagrama de Base de Datos

```mermaid
erDiagram
    t_contactos {
        INTEGER id PK "PRIMARY KEY AUTOINCREMENT"
        TEXT nombre "NOT NULL"
        TEXT telefono "NOT NULL"
        TEXT correo_electronico "NULL"
    }
```

## 🔄 Diagrama de Secuencia - Crear Contacto

```mermaid
sequenceDiagram
    participant U as Usuario
    participant M as MainActivity
    participant N as NuevoActivity
    participant DB as DbContactos
    participant H as DbHelper
    participant SQL as SQLiteDatabase

    U->>M: Click FAB/Menú Nuevo
    M->>N: Intent → NuevoActivity
    N->>U: Muestra formulario
    U->>N: Ingresa datos y presiona Guardar
    N->>DB: insertarContacto(nombre, tel, email)
    DB->>H: getWritableDatabase()
    H->>SQL: Abre conexión BD
    DB->>SQL: insert(TABLE_CONTACTOS, values)
    SQL-->>DB: retorna ID
    DB-->>N: retorna ID
    N->>U: Toast "REGISTRO GUARDADO"
    N->>N: limpiar()
```

## 🔄 Diagrama de Secuencia - Ver y Editar Contacto

```mermaid
sequenceDiagram
    participant U as Usuario
    participant M as MainActivity
    participant V as VerActivity
    participant E as EditarActivity
    participant DB as DbContactos

    U->>M: Click en contacto de lista
    M->>V: Intent con ID
    V->>DB: verContacto(id)
    DB-->>V: Retorna Contactos
    V->>U: Muestra datos (solo lectura)
    
    U->>V: Click FAB Editar
    V->>E: Intent con ID
    E->>DB: verContacto(id)
    DB-->>E: Retorna Contactos
    E->>U: Muestra formulario editable
    
    U->>E: Modifica y presiona Guardar
    E->>DB: editarContacto(id, nombre, tel, email)
    DB-->>E: retorna boolean
    E->>V: Intent con ID
```

## 🔄 Diagrama de Secuencia - Filtrar Contactos

```mermaid
sequenceDiagram
    participant U as Usuario
    participant M as MainActivity
    participant A as ListaContactosAdapter
    participant DB as DbContactos

    M->>DB: mostrarContactos()
    DB-->>M: ArrayList<Contactos>
    M->>A: new ListaContactosAdapter(lista)
    A->>A: guarda listaOriginal
    M->>U: Muestra lista en RecyclerView
    
    U->>M: Escribe en SearchView
    M->>A: filtrado(texto)
    A->>A: filtra listaContactos
    A->>A: notifyDataSetChanged()
    A->>U: Actualiza RecyclerView
```

## 📦 Estructura de Paquetes

```
com.cdp.agenda
│
├── 📱 Activities
│   ├── MainActivity.java          (Lista de contactos + búsqueda)
│   ├── NuevoActivity.java         (Crear nuevo contacto)
│   ├── EditarActivity.java        (Editar contacto existente)
│   └── VerActivity.java           (Ver detalles de contacto)
│
├── 🎨 adaptadores
│   └── ListaContactosAdapter.java (Adaptador RecyclerView)
│
├── 🗄️ db
│   ├── DbHelper.java              (Helper SQLite - creación BD)
│   └── DbContactos.java           (CRUD de contactos)
│
└── 📋 entidades
    └── Contactos.java             (Modelo de datos)
```

## 🎯 Patrones de Diseño Identificados

### 1. **DAO (Data Access Object)**
- `DbContactos` actúa como DAO para la entidad `Contactos`
- Encapsula todas las operaciones CRUD

### 2. **ViewHolder Pattern**
- `ContactoViewHolder` dentro de `ListaContactosAdapter`
- Optimiza el rendimiento del RecyclerView

### 3. **Singleton Database Helper**
- `DbHelper` extiende SQLiteOpenHelper
- Gestión centralizada de la base de datos

### 4. **MVC (Model-View-Controller)**
- **Model**: `Contactos`, `DbContactos`, `DbHelper`
- **View**: Layouts XML (activity_main.xml, etc.)
- **Controller**: Activities (MainActivity, etc.)

## 🔑 Funcionalidades Principales

| Funcionalidad | Implementación |
|---------------|----------------|
| ✅ Crear contacto | `NuevoActivity` → `DbContactos.insertarContacto()` |
| 📋 Listar contactos | `MainActivity` → `DbContactos.mostrarContactos()` |
| 👁️ Ver contacto | `VerActivity` → `DbContactos.verContacto()` |
| ✏️ Editar contacto | `EditarActivity` → `DbContactos.editarContacto()` |
| 🗑️ Eliminar contacto | `VerActivity` → `DbContactos.eliminarContacto()` |
| 🔍 Buscar contacto | `MainActivity` → `ListaContactosAdapter.filtrado()` |

## 📊 Diagrama de Componentes Android

```mermaid
graph TB
    subgraph "Capa de Presentación"
        A1[MainActivity]
        A2[NuevoActivity]
        A3[EditarActivity]
        A4[VerActivity]
    end
    
    subgraph "Capa de Adaptadores"
        B1[ListaContactosAdapter]
        B2[ContactoViewHolder]
    end
    
    subgraph "Capa de Datos"
        C1[DbContactos]
        C2[DbHelper]
    end
    
    subgraph "Capa de Modelo"
        D1[Contactos]
    end
    
    subgraph "Android Framework"
        E1[SQLiteDatabase]
        E2[RecyclerView]
        E3[Intent]
    end
    
    A1 --> B1
    A1 --> C1
    A1 --> E2
    A1 --> E3
    
    A2 --> C1
    A3 --> C1
    A4 --> C1
    
    B1 --> B2
    B1 --> D1
    
    C1 --> C2
    C1 --> D1
    C2 --> E1
    
    style A1 fill:#4CAF50
    style A2 fill:#4CAF50
    style A3 fill:#4CAF50
    style A4 fill:#4CAF50
    style C1 fill:#2196F3
    style C2 fill:#2196F3
    style D1 fill:#FF9800
```

## 🔐 Base de Datos SQLite

### Tabla: t_contactos

| Campo | Tipo | Restricciones |
|-------|------|---------------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT |
| nombre | TEXT | NOT NULL |
| telefono | TEXT | NOT NULL |
| correo_electronico | TEXT | NULL |

### Operaciones CRUD

```sql
-- CREATE
INSERT INTO t_contactos (nombre, telefono, correo_electronico) VALUES (?, ?, ?);

-- READ ALL
SELECT * FROM t_contactos ORDER BY nombre ASC;

-- READ ONE
SELECT * FROM t_contactos WHERE id = ? LIMIT 1;

-- UPDATE
UPDATE t_contactos SET nombre = ?, telefono = ?, correo_electronico = ? WHERE id = ?;

-- DELETE
DELETE FROM t_contactos WHERE id = ?;
```

---

## 📝 Resumen de Arquitectura

La aplicación sigue una arquitectura en capas:

1. **Capa de Presentación**: 4 Activities que manejan la interfaz de usuario
2. **Capa de Adaptadores**: RecyclerView adapter para la lista de contactos
3. **Capa de Acceso a Datos**: DbContactos (DAO) y DbHelper (SQLite)
4. **Capa de Modelo**: Entidad Contactos (POJO)

La navegación entre Activities se realiza mediante Intents, y los datos se persisten en SQLite usando el patrón DAO.
