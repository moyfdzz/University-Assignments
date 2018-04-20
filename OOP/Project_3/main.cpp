#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "Reserva.h"

ifstream materiales("Material.txt");
ifstream reservaciones("Reserva.txt");
Material *arrGlobalMaterial[20];
Reserva arrGlobalReserva[50];
int cantMateriales = 0;
int cantReservaciones = 0;

void cargarDatosMateriales()
{
    int idMaterial, numPag, duracion;
    char clave;
    string titulo, autor, sistemaOper;

    while(materiales >> idMaterial >> titulo >> clave)
        switch(clave)
        {
            case 'L':
                materiales >> numPag >> autor;
                arrGlobalMaterial[cantMateriales++] = new Libro(idMaterial, titulo, numPag, autor);

                break;
            case 'D':
                materiales >> duracion;
                arrGlobalMaterial[cantMateriales++] = new Disco(idMaterial, titulo, duracion);

                break;
            case 'S':
                materiales >> sistemaOper;
                arrGlobalMaterial[cantMateriales++] = new Software(idMaterial, titulo, sistemaOper);

                break;
        }
}

void cargarDatosReservaciones()
{
    int dd, mm, aa, idMaterial, idCliente;

    while(reservaciones >> dd >> mm >> aa >> idMaterial >> idCliente)
    {
        Fecha fechaReservacion(dd, mm, aa);
        arrGlobalReserva[cantReservaciones].setFechaReservacion(fechaReservacion);
        arrGlobalReserva[cantReservaciones].setIdMaterial(idMaterial);
        arrGlobalReserva[cantReservaciones].setIdCliente(idCliente);
        cantReservaciones++;
    }
}

void cargarDatosArchivos()
{
    cargarDatosMateriales();
    cargarDatosReservaciones();
}

void consultarMateriales()
{
    cout << '\n' << "La lista de materiales es la siguiente:" << '\n' << endl;

    for(int counter = 0; counter < cantMateriales; counter++)
    {
        arrGlobalMaterial[counter]->muestra();
        cout << endl;
    }
}

int infoDiasPrestamo(int idMaterialReservaciones, string &nombreMaterial)
{
    /* Recibe el ID del material de la reservación y lo compara con los IDs de los materiales. Si encuentra uno igual,
     * manda el nombre del material por parámetros por referencia y la función regresa la cantidad de días de
     * préstamo, dependiendo del material. */
    for(int counter = 0; counter < cantMateriales; counter++)
    {
        int idMaterialMateriales = arrGlobalMaterial[counter]->getIdMaterial();
        if(idMaterialReservaciones == idMaterialMateriales)
        {
            nombreMaterial = arrGlobalMaterial[counter]->getTitulo();
            return arrGlobalMaterial[counter]->cantidadDeDiasDePrestamos();
        }
    }
}

void consultarReservaciones()
{
    cout << '\n' << "La lista de reservaciones es la siguiente:" << '\n' << endl;

    for(int counter = 0; counter < cantReservaciones; counter++)
    {
        Fecha fechaReservacionInicial = arrGlobalReserva[counter].getFechaReservacion();

        cout << "Fecha de reservación inicial: " << fechaReservacionInicial << endl;

        int idMaterialReservaciones = arrGlobalReserva[counter].getIdMaterial();
        string nombreMaterial;
        int cantDiasPrestamo = infoDiasPrestamo(idMaterialReservaciones, nombreMaterial);

        Fecha fechaReservacionFinal = fechaReservacionInicial + cantDiasPrestamo;

        cout << "Fecha de reservación final: " << fechaReservacionFinal << endl;
        cout << "Nombre de Material: " << nombreMaterial << endl;
        cout << "ID de Cliente: " << arrGlobalReserva[counter].getIdCliente() << '\n' << endl;
    }
}

int infoDiasPrestamoPorM(int idMaterial, string &nombreMaterial)
{
    int cantDiasPrestamo = 0;

    /* Recibe el ID del material y lo compara con los IDs de los materiales del arreglo de las reservaciones. Si
     * encuentra uno igual, asigna la fecha de reservación del arreglo de reservaciones y la cantidad de días de préstamo.*/
    for(int counter = 0; counter < cantReservaciones; counter++)
    {
        int idMaterialReservaciones = arrGlobalReserva[counter].getIdMaterial();
        if(idMaterial == idMaterialReservaciones)
        {
            cantDiasPrestamo = arrGlobalMaterial[counter]->cantidadDeDiasDePrestamos();
            break;
        }
    }

    /* Recibe el ID del material y lo compara con los IDs de los materiales del arreglo de los materiales. Si encuentra
     * uno igual, asigna la fecha de reservación del arreglo de reservaciones y la cantidad de días de préstamo. */
    for(int counter = 0; counter < cantMateriales; counter++)
    {
        int idMaterialMateriales = arrGlobalMaterial[counter]->getIdMaterial();
        if(idMaterial == idMaterialMateriales)
        {
            nombreMaterial = arrGlobalMaterial[counter]->getTitulo();
            break;
        }
    }

    return cantDiasPrestamo;
}

bool encontrarMaterial(int idMaterial)
{
    bool encontrar = false;

    for(int counter = 0; counter < cantMateriales; counter++)
    {
        if(idMaterial == arrGlobalMaterial[counter]->getIdMaterial())
        {
            encontrar = true;
        }
    }

    return encontrar;
}

void consultarReservacionesDeMaterial()
{
    int idMaterial;
    cout << "Ingrese el ID del material: ";
    cin >> idMaterial;

    bool encontro = encontrarMaterial(idMaterial);
    bool materialReservado = false;

    while(!encontro)
    {
        cout << "El material no fue encontrado. Por favor intente con otro ID. " << endl;
        cin >> idMaterial;
        encontro = encontrarMaterial(idMaterial);
    }

    cout << '\n' << "Las reservaciones vigentes son las siguientes: " << '\n' << endl;

    for(int counter = 0; counter < cantReservaciones; counter++)
    {
        if(idMaterial == arrGlobalReserva[counter].getIdMaterial())
        {
            materialReservado = true;
            string nombreMaterial;
            Fecha fechaReservacionInicial = arrGlobalReserva[counter].getFechaReservacion();
            int cantDiasPrestamo = infoDiasPrestamoPorM(idMaterial, nombreMaterial);
            Fecha fechaReservacionFinal = fechaReservacionInicial + cantDiasPrestamo;

            cout << "Nombre del material: " << nombreMaterial << endl;
            cout << "Fecha de reservación inicial: " << fechaReservacionInicial << endl;
            cout << "Fecha de reservación final: " << fechaReservacionFinal << '\n' << endl;
        }
    }

    if(!materialReservado)
        cout << "El material solicitado no ha sido reservado" << '\n' << endl;
}

void consultarReservacionesDeFecha()
{
    int dd, mm, aa, contador = 0;
    cout << "Ingrese la fecha (dd/mm/aa): ";
    cin >> dd >> mm >> aa;

    cout << endl;

    Fecha fechaUsuario(dd, mm, aa);

    cout << "La lista de materiales reservados en la fecha " << fechaUsuario << " es la siguiente:" << '\n' << endl;

    for(int counter = 0; counter < cantReservaciones; counter++)
    {
        string nombreMaterial;
        Fecha fechaReservacionInicial = arrGlobalReserva[counter].getFechaReservacion();
        int idMaterial = arrGlobalReserva[counter].getIdMaterial();
        int cantDiasPrestamo = infoDiasPrestamoPorM(idMaterial, nombreMaterial);
        Fecha fechaReservacionFinal = fechaReservacionInicial + cantDiasPrestamo;
        if(fechaUsuario >= fechaReservacionInicial && fechaUsuario <= fechaReservacionFinal)
        {
            contador++;
            cout << "Nombre del material: ";
            for(int counter2 = 0; counter2 < cantMateriales; counter2++)
            {
                if(idMaterial == arrGlobalMaterial[counter2]->getIdMaterial())
                {
                    cout << arrGlobalMaterial[counter2]->getTitulo();
                    break;
                }
            }
            cout << '\n' << "ID del cliente: " << arrGlobalReserva[counter].getIdCliente() << '\n' << endl;
        }
    }

    if(contador == 0)
    {
        cout << "Ningún material está reservado en esa fecha." << endl;
    }
}

void hacerReservacion()
{
    int idCliente, idMaterial, dd, mm, aa;
    cout << "Ingrese su ID de cliente: ";
    cin >> idCliente;

    cout << "Ingrese el ID del material: ";
    cin >> idMaterial;

    bool encontro = encontrarMaterial(idMaterial);

    while(!encontro)
    {
        cout << "El material no fue encontrado. Por favor intente con otro ID. " << endl;
        cin >> idMaterial;
        encontro = encontrarMaterial(idMaterial);
    }

    cout << "Ingrese la fecha en la que quiere reservar el material (dd/mm/aa): ";
    cin >> dd >> mm >> aa;

    Fecha fechaUsuario(dd, mm, aa);

    bool encuentraReservacion = false;

    for(int counter = 0; counter < cantReservaciones; counter++)
    {
        string nombreMaterial;
        Fecha fechaReservacionInicial = arrGlobalReserva[counter].getFechaReservacion();
        int cantDiasPrestamo = infoDiasPrestamoPorM(idMaterial, nombreMaterial);
        Fecha fechaReservacionFinal = fechaReservacionInicial + cantDiasPrestamo;
        if(fechaUsuario >= fechaReservacionInicial && fechaUsuario <= fechaReservacionFinal)
        {
            encuentraReservacion = true;
            cout << "El material ya está reservado en la fecha " << fechaUsuario << endl;
            break;
        }
    }

    if(!encuentraReservacion)
    {
        arrGlobalReserva[cantReservaciones].setFechaReservacion(fechaUsuario);
        arrGlobalReserva[cantReservaciones].setIdMaterial(idMaterial);
        arrGlobalReserva[cantReservaciones].setIdCliente(idCliente);
        cantReservaciones++;
        cout << "Su reservación fue realizada con éxito. " << '\n' << endl;
    }
}

void mostrarMenu()
{
    char opcion;

    do
    {
        cout << "M E N U " << endl;
        cout << "a. Consultar la lista de materiales" << endl;
        cout << "b. Consultar la lista de reservaciones" << endl;
        cout << "c. Consultar las reservaciones de un material dado" << endl;
        cout << "d. Consultar las reservaciones de una fecha dada" << endl;
        cout << "e. Hacer una reservación" << endl;
        cout << "f. Terminar" << endl;
        cout << "Opcion -> ";
        cin >> opcion;

        switch (opcion)
        {
            case 'a':
                consultarMateriales();

                break;

            case 'b':
                consultarReservaciones();

                break;
            case 'c':
                consultarReservacionesDeMaterial();

                break;
            case 'd':
                consultarReservacionesDeFecha();

                break;
            case 'e':
                hacerReservacion();

                break;
        }
    } while (opcion != 'f');
}

int main()
{
    cargarDatosArchivos();
    mostrarMenu();
    materiales.close();
    reservaciones.close();

    return 0;
}