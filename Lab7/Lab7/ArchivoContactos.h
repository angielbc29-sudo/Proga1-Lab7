#pragma once
#include <QString>
#include <QList>

struct Contacto {
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos {
public:
    QString ruta() const;
    bool guardar(const QString& nombre, const QString& telefono, const QString& correo) const;
    QList<Contacto> leerTodos() const;
    bool actualizar(int indice, const QString& nombre, const QString& telefono, const QString& correo) const;
    bool eliminar(int indice) const;

private:
    bool reescribirArchivo(const QList<Contacto>& contactos) const;
};