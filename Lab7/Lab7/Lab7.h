#pragma once
#include "ArchivoContactos.h"
#include <QtWidgets/QMainWindow>
#include "ui_Lab7.h"

class Lab7 : public QMainWindow
{
    Q_OBJECT

public:
    Lab7(QWidget *parent = nullptr);
    ~Lab7();

private slots:
    void guardarContacto();
    void irARegistrar();
    void irAHistorial();
    void irAEditar();
    void irAEliminar();
    void alCambiarSeleccion();
    void actualizarContacto();
    void eliminarContacto();

private:
    enum class Modo { Registrar, Historial, Editar, Eliminar };

    Ui::Lab7Class ui;
    ArchivoContactos archivoContactos;
    Modo modo = Modo::Registrar;

    void configurarTabla();
    void refrescarTabla();
    void mostrarPagina(Modo nuevoModo);
    void cargarFilaEnCampos(int fila);
    bool validarDatos(const QString& nombre,
                      const QString& telefono,
                      const QString& correo);
};
