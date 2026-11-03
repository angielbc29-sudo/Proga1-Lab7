# Lab 7 - Agenda de contactos (Qt)

Aplicacion de escritorio hecha con Qt Widgets y C++ para registrar, visualizar, editar y eliminar contactos. Los datos se guardan en un archivo de texto, sin usar base de datos.

## Funcionalidades

- **Registrar:** ingresa nombre, telefono y correo. Valida que los tres campos esten completos y que no contengan punto y coma ni saltos de linea.
- **Historial:** muestra todos los contactos guardados en una tabla con las columnas Nombre, Telefono y Correo.
- **Editar:** permite seleccionar una fila, modificar sus tres datos y guardar los cambios en el archivo.
- **Eliminar:** permite seleccionar una fila, pedir confirmacion y borrar solo ese contacto.
- **Navegacion:** menu superior con las cuatro opciones (Registrar, Historial, Editar, Eliminar).
- **Confirmaciones:** muestra un mensaje solo cuando la operacion se realizo correctamente.
- **Validaciones:** si faltan datos o hay caracteres no permitidos, no se escribe nada en el archivo.

## Requisitos

- Visual Studio con la carga de trabajo **Desarrollo para el escritorio con C++**.
- **Qt 6** (probado con Qt 6.12.0, `msvc2022_64`).
- Extension **Qt Visual Studio Tools**.
- La definicion `QT_NO_DEPRECATED_WARNINGS` ya esta configurada en el proyecto.

## Como compilar y ejecutar

1. Abrir la solucion `Lab7/Lab7.slnx` en Visual Studio.
2. Elegir la configuracion `Debug` y la plataforma `x64`.
3. Ir a **Compilar > Recompilar solucion**.
4. Ejecutar con `Ctrl + F5`.

## Donde se guardan los datos

El archivo se guarda en la carpeta del usuario:

```
C:\Users\<usuario>\Lab7Datos\contactos.txt
```

En este equipo la ruta es:

```
C:\Users\angie\Lab7Datos\contactos.txt
```

Cada linea representa un contacto con el formato:

```
nombre;telefono;correo
```

Ejemplo:

```
Ana Mora;8888-1111;ana@example.com
Carlos Rojas;7777-2222;carlos@example.com
Maria Lopez;6666-3333;maria@example.com
```

El archivo `contactos-ejemplo.txt` incluido en el repositorio contiene esos datos de prueba. Para usarlo, copiarlo a `C:\Users\<usuario>\Lab7Datos\contactos.txt` (o registrar los contactos desde la aplicacion).

## Como comprobe editar y eliminar

1. Registre contactos de prueba (Ana Mora, luis rojas y Jesus Mora).
2. Abri **Navegacion > Historial** y verifique que aparecieran en la tabla.
3. Abri **Navegacion > Editar**, seleccione la fila de luis rojas, cambie el telefono y presione **Guardar cambios**. Aparecio el mensaje **Contacto actualizado** y la tabla se refresco sin duplicar filas ni modificar los otros contactos.
4. Abri **Navegacion > Eliminar**, seleccione un contacto y confirme la eliminacion. Aparecio el mensaje **Contacto eliminado** y la fila desaparecio de la tabla. Tambien probe cancelar la confirmacion: los datos quedaron intactos.
5. Cierre y volvi a abrir la aplicacion: los contactos y el telefono modificado se conservaron en el archivo.
6. Tambien probe intentar editar sin seleccionar una fila (aparece un aviso) y abrir el historial con el archivo vacio (la tabla queda vacia sin errores).

## Estructura del proyecto

```
Lab7/
  Lab7.slnx
  Lab7/
    Lab7.ui                  Formulario, tabla y menu (Qt Designer)
    Lab7.h / Lab7.cpp        Logica de la ventana y navegacion
    ArchivoContactos.h/.cpp  Lectura y escritura del archivo .txt
    main.cpp                 Punto de entrada de la aplicacion
contactos-ejemplo.txt        Datos de prueba
capturas/                    Capturas para la entrega
README.md
```

## Capturas para la entrega

En la carpeta `capturas/` estan incluidas:

- `01-guardado.png`: formulario con el mensaje **Guardado completo** y la ruta del archivo.
- `02-historial.png`: tabla del historial con los contactos guardados.

Tambien se agregaron capturas adicionales del formulario completo, la edicion, la confirmacion de eliminacion y la eliminacion concretada.

## Notas

- La tabla no tiene ordenamiento para que el numero de fila coincida siempre con el registro del archivo.
- No se permite editar directamente sobre la tabla; los cambios se hacen en los campos de edicion.
- La aplicacion esta pensada para una sola instancia a la vez.
- Para editar o eliminar se lee el archivo completo, se modifica el registro elegido y se reescribe todo con `QSaveFile`, que reemplaza el archivo solo si la escritura termina bien.
