#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QSaveFile>
#include <QTextStream>
#include <QStringList>

QString ArchivoContactos::ruta() const {
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre, const QString& telefono, const QString& correo) const {
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta)) return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append)) return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = (archivo.write(datos) == datos.size());
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

QList<Contacto> ArchivoContactos::leerTodos() const {
    QList<Contacto> lista;
    QFile archivo(ruta());
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return lista;
    }

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty()) continue;

        QStringList partes = linea.split(';');
        if (partes.size() == 3) {
            lista.append({ partes[0], partes[1], partes[2] });
        }
    }
    archivo.close();
    return lista;
}

bool ArchivoContactos::reescribirArchivo(const QList<Contacto>& contactos) const {
    QSaveFile saveFile(ruta());
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&saveFile);
    for (const auto& c : contactos) {
        out << c.nombre << ";" << c.telefono << ";" << c.correo << "\n";
    }

    return saveFile.commit(); // Guarda atómicamente de forma segura
}

bool ArchivoContactos::actualizar(int indice, const QString& nombre, const QString& telefono, const QString& correo) const {
    QList<Contacto> contactos = leerTodos();
    if (indice < 0 || indice >= contactos.size()) return false;

    contactos[indice] = { nombre, telefono, correo };
    return reescribirArchivo(contactos);
}

bool ArchivoContactos::eliminar(int indice) const {
    QList<Contacto> contactos = leerTodos();
    if (indice < 0 || indice >= contactos.size()) return false;

    contactos.removeAt(indice);
    return reescribirArchivo(contactos);
}