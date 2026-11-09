#include "Lab7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QAction>


Lab7::Lab7(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    configurarTabla();

    connect(ui.btnGuardar, &QPushButton::clicked,
        this, &Lab7::guardarContacto);
    connect(ui.actionRegistrar, &QAction::triggered,
        this, &Lab7::irARegistrar);
    connect(ui.actionHistorial, &QAction::triggered,
        this, &Lab7::irAHistorial);
    connect(ui.actionEditar, &QAction::triggered,
        this, &Lab7::irAEditar);
    connect(ui.actionEliminar, &QAction::triggered,
        this, &Lab7::irAEliminar);
    connect(ui.tblContactos, &QTableWidget::itemSelectionChanged,
        this, &Lab7::alCambiarSeleccion);
    connect(ui.btnActualizar, &QPushButton::clicked,
        this, &Lab7::actualizarContacto);
    connect(ui.btnEliminar, &QPushButton::clicked,
        this, &Lab7::eliminarContacto);

    mostrarPagina(Modo::Registrar);
}

Lab7::~Lab7()
{}

void Lab7::configurarTabla()
{
    ui.tblContactos->setColumnCount(3);
    ui.tblContactos->setHorizontalHeaderLabels(
        QStringList{ "Nombre", "Telefono", "Correo" });
    ui.tblContactos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tblContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tblContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tblContactos->setSortingEnabled(false);
    ui.tblContactos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tblContactos->verticalHeader()->setVisible(false);
}

void Lab7::refrescarTabla()
{
    QList<Contacto> contactos = archivoContactos.leerTodos();

    ui.tblContactos->clearContents();
    ui.tblContactos->setRowCount(contactos.size());

    for (int i = 0; i < contactos.size(); ++i) {
        ui.tblContactos->setItem(i, 0, new QTableWidgetItem(contactos[i].nombre));
        ui.tblContactos->setItem(i, 1, new QTableWidgetItem(contactos[i].telefono));
        ui.tblContactos->setItem(i, 2, new QTableWidgetItem(contactos[i].correo));
    }

    ui.tblContactos->clearSelection();
}

void Lab7::mostrarPagina(Modo nuevoModo)
{
    modo = nuevoModo;

    if (modo == Modo::Registrar) {
        ui.stackedWidget->setCurrentWidget(ui.pageRegistrar);
        ui.txtNombre->setFocus();
        return;
    }

    ui.stackedWidget->setCurrentWidget(ui.pageHistorial);
    refrescarTabla();

    ui.txtEditNombre->setEnabled(true);
    ui.txtEditTelefono->setEnabled(true);
    ui.txtEditCorreo->setEnabled(true);
    ui.btnActualizar->setEnabled(true);
    ui.btnEliminar->setEnabled(true);

    ui.txtEditNombre->clear();
    ui.txtEditTelefono->clear();
    ui.txtEditCorreo->clear();

    if (modo == Modo::Editar) {
        ui.lblEdicion->setText("Elegi una fila, modifica sus datos y presiona Guardar cambios:");
        ui.btnActualizar->setFocus();
    } else if (modo == Modo::Eliminar) {
        ui.lblEdicion->setText("Elegi una fila y presiona Eliminar seleccionado:");
        ui.btnEliminar->setFocus();
    } else {
        ui.lblEdicion->setText("Elegi una fila para editarla o eliminarla:");
    }
}

void Lab7::irARegistrar()
{
    mostrarPagina(Modo::Registrar);
}

void Lab7::irAHistorial()
{
    mostrarPagina(Modo::Historial);
}

void Lab7::irAEditar()
{
    mostrarPagina(Modo::Editar);
}

void Lab7::irAEliminar()
{
    mostrarPagina(Modo::Eliminar);
}

void Lab7::alCambiarSeleccion()
{
    if (modo == Modo::Registrar)
        return;

    cargarFilaEnCampos(ui.tblContactos->currentRow());
}

void Lab7::cargarFilaEnCampos(int fila)
{
    if (fila < 0 || fila >= ui.tblContactos->rowCount())
        return;

    ui.txtEditNombre->setText(ui.tblContactos->item(fila, 0)->text());
    ui.txtEditTelefono->setText(ui.tblContactos->item(fila, 1)->text());
    ui.txtEditCorreo->setText(ui.tblContactos->item(fila, 2)->text());
}

bool Lab7::validarDatos(const QString& nombre,
                        const QString& telefono,
                        const QString& correo)
{
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
            "Completa los tres campos.");
        return false;
    }

    for (const QString& dato : QStringList{ nombre, telefono, correo }) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No uses punto y coma ni saltos de linea.");
            return false;
        }
    }

    return true;
}

void Lab7::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisa permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardo correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}

void Lab7::actualizarContacto()
{
    int fila = ui.tblContactos->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Sin seleccion",
            "Elegi un contacto de la tabla para editar.");
        return;
    }

    QString nombre = ui.txtEditNombre->text().trimmed();
    QString telefono = ui.txtEditTelefono->text().trimmed();
    QString correo = ui.txtEditCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.actualizar(fila, nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo actualizar",
            "Revisa permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    refrescarTabla();
    ui.txtEditNombre->clear();
    ui.txtEditTelefono->clear();
    ui.txtEditCorreo->clear();
    QMessageBox::information(this, "Contacto actualizado",
        "Los cambios se guardaron correctamente.");
}

void Lab7::eliminarContacto()
{
    int fila = ui.tblContactos->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Sin seleccion",
            "Elegi un contacto de la tabla para eliminar.");
        return;
    }

    QString nombre = ui.tblContactos->item(fila, 0)->text();
    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this, "Confirmar eliminacion",
        "Queres eliminar el contacto \"" + nombre + "\"?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (respuesta != QMessageBox::Yes)
        return;

    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "No se pudo eliminar",
            "Revisa permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    refrescarTabla();
    ui.txtEditNombre->clear();
    ui.txtEditTelefono->clear();
    ui.txtEditCorreo->clear();
    QMessageBox::information(this, "Contacto eliminado",
        "El contacto se elimino correctamente.");
}
