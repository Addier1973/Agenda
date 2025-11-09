# Esquema de la Aplicación Agenda de Contactos

## Diagrama de Arquitectura

```mermaid
graph TB
    subgraph "Capa de Presentación"
        MA[MainActivity<br/>Lista de Contactos]
        NA[NuevoActivity<br/>Crear Contacto]
        VA[VerActivity<br/>Ver Detalles]
        EA[EditarActivity<br/>Editar Contacto]
        LCA[ListaContactosAdapter<br/>RecyclerView Adapter]
    end

    subgraph "Capa de Lógica de Negocio"
        DBC[DbContactos<br/>Operaciones CRUD]
    end

    subgraph "Capa de Datos"
        DBH[DbHelper<br/>SQLiteOpenHelper]
        SQL[(SQLite Database<br/>agenda.db)]
    end

    subgraph "Entidades"
        CONT[Contactos<br/>id, nombre, telefono, correoElectornico]
    end

    MA -->|"muestra lista"| LCA
    MA -->|"nuevo contacto"| NA
    MA -->|"buscar"| LCA
    LCA -->|"click item"| VA
    NA -->|"insertar"| DBC
    VA -->|"editar"| EA
    VA -->|"eliminar"| DBC
    EA -->|"actualizar"| DBC
    DBC -->|"hereda"| DBH
    DBH -->|"gestiona"| SQL
    DBC -->|"retorna"| CONT
    LCA -->|"usa"| CONT
    VA -->|"muestra"| CONT
    EA -->|"edita"| CONT
```

## Flujo de Navegación entre Actividades

```mermaid
stateDiagram-v2
    [*] --> MainActivity: Inicio App
    
    MainActivity --> NuevoActivity: Click FAB / Menú Nuevo
    MainActivity --> VerActivity: Click en Contacto
    
    NuevoActivity --> MainActivity: Guardar Contacto
    
    VerActivity --> EditarActivity: Click FAB Editar
    VerActivity --> MainActivity: Eliminar Contacto
    
    EditarActivity --> VerActivity: Guardar Cambios
    
    MainActivity --> MainActivity: Buscar Contactos
```

## Estructura de Base de Datos

```mermaid
erDiagram
    T_CONTACTOS {
        int id PK "AUTOINCREMENT"
        string nombre "NOT NULL"
        string telefono "NOT NULL"
        string correo_electronico "NULL"
    }
```

## Diagrama de Clases

```mermaid
classDiagram
    class MainActivity {
        -SearchView txtBuscar
        -RecyclerView listaContactos
        -ArrayList~Contactos~ listaArrayContactos
        -FloatingActionButton fabNuevo
        -ListaContactosAdapter adapter
        +onCreate(Bundle)
        +onCreateOptionsMenu(Menu)
        +onQueryTextChange(String)
        -nuevoRegistro()
    }

    class NuevoActivity {
        -EditText txtNombre
        -EditText txtTelefono
        -EditText txtCorreoElectronico
        -Button btnGuarda
        +onCreate(Bundle)
        -limpiar()
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
        +onCreate(Bundle)
        -lista()
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
        +onCreate(Bundle)
        -verRegistro()
    }

    class ListaContactosAdapter {
        -ArrayList~Contactos~ listaContactos
        -ArrayList~Contactos~ listaOriginal
        +onCreateViewHolder(ViewGroup, int)
        +onBindViewHolder(ContactoViewHolder, int)
        +filtrado(String)
        +getItemCount()
        +ContactoViewHolder
    }

    class DbContactos {
        -Context context
        +insertarContacto(String, String, String) long
        +mostrarContactos() ArrayList~Contactos~
        +verContacto(int) Contactos
        +editarContacto(int, String, String, String) boolean
        +eliminarContacto(int) boolean
    }

    class DbHelper {
        -int DATABASE_VERSION
        -String DATABASE_NOMBRE
        -String TABLE_CONTACTOS
        +onCreate(SQLiteDatabase)
        +onUpgrade(SQLiteDatabase, int, int)
    }

    class Contactos {
        -int id
        -String nombre
        -String telefono
        -String correoElectornico
        +getId() int
        +setId(int)
        +getNombre() String
        +setNombre(String)
        +getTelefono() String
        +setTelefono(String)
        +getCorreoElectornico() String
        +setCorreoElectornico(String)
    }

    MainActivity --> ListaContactosAdapter
    MainActivity --> DbContactos
    MainActivity --> NuevoActivity
    ListaContactosAdapter --> Contactos
    ListaContactosAdapter --> VerActivity
    NuevoActivity --> DbContactos
    VerActivity --> Contactos
    VerActivity --> DbContactos
    VerActivity --> EditarActivity
    EditarActivity --> Contactos
    EditarActivity --> DbContactos
    DbContactos --|> DbHelper
    DbContactos --> Contactos
```

## Operaciones CRUD

```mermaid
sequenceDiagram
    participant U as Usuario
    participant MA as MainActivity
    participant DBC as DbContactos
    participant DBH as DbHelper
    participant SQL as SQLite DB

    Note over U,SQL: Crear Contacto
    U->>MA: Click Nuevo
    MA->>U: Abre NuevoActivity
    U->>DBC: Insertar Contacto
    DBC->>DBH: getWritableDatabase()
    DBH->>SQL: INSERT INTO t_contactos
    SQL-->>DBC: id generado
    DBC-->>U: Contacto creado

    Note over U,SQL: Leer Contactos
    U->>MA: Abre App
    MA->>DBC: mostrarContactos()
    DBC->>DBH: getReadableDatabase()
    DBH->>SQL: SELECT * FROM t_contactos
    SQL-->>DBC: Cursor con datos
    DBC-->>MA: ArrayList<Contactos>
    MA->>U: Muestra lista

    Note over U,SQL: Actualizar Contacto
    U->>MA: Click en Contacto
    MA->>DBC: verContacto(id)
    DBC->>SQL: SELECT WHERE id
    SQL-->>DBC: Datos contacto
    DBC-->>U: Contacto
    U->>DBC: editarContacto(id, datos)
    DBC->>SQL: UPDATE t_contactos
    SQL-->>DBC: OK
    DBC-->>U: Actualizado

    Note over U,SQL: Eliminar Contacto
    U->>DBC: eliminarContacto(id)
    DBC->>SQL: DELETE FROM t_contactos
    SQL-->>DBC: OK
    DBC-->>U: Eliminado
```

## Descripción de Componentes

### Actividades
- **MainActivity**: Pantalla principal que muestra la lista de contactos en un RecyclerView con funcionalidad de búsqueda
- **NuevoActivity**: Formulario para crear nuevos contactos
- **VerActivity**: Muestra los detalles de un contacto en modo solo lectura con opciones para editar o eliminar
- **EditarActivity**: Permite modificar los datos de un contacto existente

### Base de Datos
- **DbHelper**: Extiende SQLiteOpenHelper, gestiona la creación y actualización de la base de datos
- **DbContactos**: Extiende DbHelper, contiene todas las operaciones CRUD (Create, Read, Update, Delete) para los contactos

### Entidades
- **Contactos**: Modelo de datos con los campos: id, nombre, telefono, correoElectornico

### Adaptadores
- **ListaContactosAdapter**: Adaptador para el RecyclerView que muestra la lista de contactos e implementa filtrado por nombre
