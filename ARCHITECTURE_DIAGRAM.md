# Esquema de Arquitectura - Aplicación Agenda de Contactos

## Diagrama de Arquitectura General

```mermaid
graph TB
    subgraph "Capa de Presentación"
        MA[MainActivity<br/>Lista de Contactos]
        NA[NuevoActivity<br/>Crear Contacto]
        VA[VerActivity<br/>Ver Contacto]
        EA[EditarActivity<br/>Editar Contacto]
    end
    
    subgraph "Capa de Adaptadores"
        LCA[ListaContactosAdapter<br/>RecyclerView Adapter]
    end
    
    subgraph "Capa de Datos"
        DBC[DbContactos<br/>CRUD Operaciones]
        DBH[DbHelper<br/>SQLite Helper]
    end
    
    subgraph "Capa de Modelos"
        CONT[Contactos<br/>Entidad]
    end
    
    subgraph "Base de Datos"
        DB[(SQLite DB<br/>agenda.db<br/>t_contactos)]
    end
    
    MA -->|Muestra lista| LCA
    MA -->|Navega a| NA
    LCA -->|Obtiene datos| DBC
    LCA -->|Al hacer clic| VA
    
    NA -->|Guarda| DBC
    VA -->|Navega a| EA
    VA -->|Elimina| DBC
    EA -->|Actualiza| DBC
    EA -->|Vuelve a| VA
    
    DBC -->|Extiende| DBH
    DBC -->|Retorna| CONT
    DBC -->|Lee/Escribe| DB
    DBH -->|Crea/Gestiona| DB
    
    style MA fill:#4CAF50
    style NA fill:#2196F3
    style VA fill:#FF9800
    style EA fill:#F44336
    style LCA fill:#9C27B0
    style DBC fill:#00BCD4
    style DBH fill:#009688
    style CONT fill:#FFC107
    style DB fill:#795548
```

## Flujo de Navegación entre Activities

```mermaid
stateDiagram-v2
    [*] --> MainActivity
    
    MainActivity --> NuevoActivity: FAB/Menú Nuevo
    NuevoActivity --> MainActivity: Después de guardar
    
    MainActivity --> VerActivity: Click en contacto
    VerActivity --> EditarActivity: FAB Editar
    VerActivity --> MainActivity: FAB Eliminar
    
    EditarActivity --> VerActivity: Después de guardar
    
    MainActivity --> [*]
```

## Diagrama de Clases

```mermaid
classDiagram
    class MainActivity {
        -SearchView txtBuscar
        -RecyclerView listaContactos
        -FloatingActionButton fabNuevo
        -ListaContactosAdapter adapter
        +onCreate()
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
    
    class VerActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreo
        -FloatingActionButton fabEditar
        -FloatingActionButton fabEliminar
        -Contactos contacto
        -int id
        +onCreate()
        -lista()
    }
    
    class EditarActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreo
        -Button btnGuarda
        -Contactos contacto
        -int id
        +onCreate()
        -verRegistro()
    }
    
    class ListaContactosAdapter {
        -ArrayList~Contactos~ listaContactos
        -ArrayList~Contactos~ listaOriginal
        +onCreateViewHolder()
        +onBindViewHolder()
        +filtrado(String)
        +getItemCount()
    }
    
    class DbHelper {
        -DATABASE_VERSION: int
        -DATABASE_NOMBRE: String
        +TABLE_CONTACTOS: String
        +onCreate(SQLiteDatabase)
        +onUpgrade(SQLiteDatabase, int, int)
    }
    
    class DbContactos {
        -Context context
        +insertarContacto(String, String, String): long
        +mostrarContactos(): ArrayList~Contactos~
        +verContacto(int): Contactos
        +editarContacto(int, String, String, String): boolean
        +eliminarContacto(int): boolean
    }
    
    class Contactos {
        -int id
        -String nombre
        -String telefono
        -String correoElectornico
        +getId(): int
        +setId(int)
        +getNombre(): String
        +setNombre(String)
        +getTelefono(): String
        +setTelefono(String)
        +getCorreoElectornico(): String
        +setCorreoElectornico(String)
    }
    
    MainActivity --> ListaContactosAdapter
    MainActivity --> DbContactos
    MainActivity --> NuevoActivity
    
    ListaContactosAdapter --> Contactos
    ListaContactosAdapter --> VerActivity
    
    NuevoActivity --> DbContactos
    VerActivity --> DbContactos
    VerActivity --> EditarActivity
    EditarActivity --> DbContactos
    EditarActivity --> Contactos
    
    DbContactos --|> DbHelper
    DbContactos --> Contactos
```

## Estructura de Base de Datos

```mermaid
erDiagram
    t_contactos {
        INTEGER id PK "AUTO_INCREMENT"
        TEXT nombre "NOT NULL"
        TEXT telefono "NOT NULL"
        TEXT correo_electronico "NULLABLE"
    }
```

## Operaciones CRUD

```mermaid
sequenceDiagram
    participant User
    participant Activity
    participant DbContactos
    participant SQLiteDatabase
    
    Note over User,SQLiteDatabase: CREATE (Crear Contacto)
    User->>Activity: Ingresa datos
    Activity->>DbContactos: insertarContacto(nombre, telefono, correo)
    DbContactos->>SQLiteDatabase: INSERT INTO t_contactos
    SQLiteDatabase-->>DbContactos: id
    DbContactos-->>Activity: long id
    Activity-->>User: Toast confirmación
    
    Note over User,SQLiteDatabase: READ (Leer Contactos)
    User->>Activity: Abre MainActivity
    Activity->>DbContactos: mostrarContactos()
    DbContactos->>SQLiteDatabase: SELECT * FROM t_contactos
    SQLiteDatabase-->>DbContactos: Cursor
    DbContactos-->>Activity: ArrayList<Contactos>
    Activity-->>User: Muestra lista
    
    Note over User,SQLiteDatabase: UPDATE (Actualizar Contacto)
    User->>Activity: Edita contacto
    Activity->>DbContactos: editarContacto(id, nombre, telefono, correo)
    DbContactos->>SQLiteDatabase: UPDATE t_contactos WHERE id
    SQLiteDatabase-->>DbContactos: éxito
    DbContactos-->>Activity: boolean
    Activity-->>User: Toast confirmación
    
    Note over User,SQLiteDatabase: DELETE (Eliminar Contacto)
    User->>Activity: Confirma eliminación
    Activity->>DbContactos: eliminarContacto(id)
    DbContactos->>SQLiteDatabase: DELETE FROM t_contactos WHERE id
    SQLiteDatabase-->>DbContactos: éxito
    DbContactos-->>Activity: boolean
    Activity-->>User: Regresa a lista
```

## Funcionalidad de Búsqueda

```mermaid
sequenceDiagram
    participant User
    participant MainActivity
    participant Adapter
    participant Lista
    
    User->>MainActivity: Escribe en SearchView
    MainActivity->>Adapter: filtrado(texto)
    Adapter->>Lista: stream().filter()
    Lista-->>Adapter: Lista filtrada
    Adapter->>Adapter: notifyDataSetChanged()
    Adapter-->>User: Muestra resultados
    
    User->>MainActivity: Borra búsqueda
    MainActivity->>Adapter: filtrado("")
    Adapter->>Adapter: Restaura listaOriginal
    Adapter->>Adapter: notifyDataSetChanged()
    Adapter-->>User: Muestra todos los contactos
```

## Resumen de Componentes

### Activities
- **MainActivity**: Pantalla principal con RecyclerView, SearchView y FAB para crear contactos
- **NuevoActivity**: Formulario para agregar un nuevo contacto
- **VerActivity**: Muestra los detalles del contacto (solo lectura) con opciones para editar/eliminar
- **EditarActivity**: Formulario para modificar un contacto existente

### Adaptadores
- **ListaContactosAdapter**: Gestiona la visualización de contactos en RecyclerView con funcionalidad de filtrado

### Capa de Datos
- **DbHelper**: Extiende SQLiteOpenHelper, crea y gestiona la base de datos
- **DbContactos**: Extiende DbHelper, implementa operaciones CRUD

### Modelo
- **Contactos**: Entidad con campos id, nombre, telefono, correoElectornico

### Base de Datos
- **agenda.db**: Base de datos SQLite versión 2
- **t_contactos**: Tabla con campos id, nombre, telefono, correo_electronico
