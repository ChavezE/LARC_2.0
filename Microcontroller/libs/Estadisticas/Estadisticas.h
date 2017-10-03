#ifndef ESTADISTICAS_H
    #define ESTADISTICAS_H

#include <QuickSort.h>
#include <math.h>

/**
 * Instanciar por medio de los createNew___
 *
 * Nota: Al obtener valores como mediana y moda, cambia de orden el arreglo datos.
 *
 * Pendiente:
 * - Tener en los createNew un argumento default que si estan ordenados los datos 
 *  para no hacer el sort
 */
template <class T>
class Estadisticas{
public:
    static Estadisticas<T> createNewAllEstadisticas(T datos[], int cantDatos);
    static Estadisticas<T> createNewPromedio(T datos[], int cantDatos);
    static Estadisticas<T> createNewMediana(T datos[], int cantDatos);
    static Estadisticas<T> createNewPromedioYDesviacionEst(T datos[], int cantDatos);

    double getPromedio();
    T getMediana();
    double getDesviacionEst();
    T getModa();

private:
    Estadisticas(double promedio, T mediana, double desviacionEst, T moda);

    static double getPromedio(T datos[], int cantDatos);
    static T getMediana(T datos[], int cantDatos);
    static double getDesviacionEst(T datos[], int cantDatos, double prom);
    static T getModa(T datos[], int cantDatos);

    static void ordenar(T datos[], int cantDatos);

    const double promedio;
    const T mediana;
    const double desviacionEst;
    const T moda;

};


template <class T>
Estadisticas<T>::Estadisticas(double promedio, T mediana, double desviacionEst, T moda):
    promedio(promedio), mediana(mediana), desviacionEst(desviacionEst), moda(moda) {

}


template <class T>
Estadisticas<T> Estadisticas<T>::createNewAllEstadisticas(T datos[], int cantDatos) {
    Estadisticas<T>::ordenar(datos, cantDatos);
    const double prom = getPromedio(datos, cantDatos);
    Estadisticas estadisticas(prom,
                            getMediana(datos, cantDatos),
                            getDesviacionEst(datos, cantDatos, prom),
                            getModa(datos, cantDatos));
    return estadisticas;
}

template <class T>
Estadisticas<T> Estadisticas<T>::createNewPromedio(T datos[], int cantDatos) {
    Estadisticas estadisticas(getPromedio(datos, cantDatos), 0, 0, 0);
    return estadisticas;
}

template <class T>
Estadisticas<T> Estadisticas<T>::createNewMediana(T datos[], int cantDatos){
    Estadisticas estadisticas(0, getMediana(datos, cantDatos), 0, 0);
    return estadisticas;
}

template <class T>
Estadisticas<T> Estadisticas<T>::createNewPromedioYDesviacionEst(T datos[], int cantDatos) {
    const double prom = getPromedio(datos, cantDatos);
    Estadisticas estadisticas(prom, 0, 
                            getDesviacionEst(datos, cantDatos, prom), 0);
    return estadisticas;
}


template <class T>
double Estadisticas<T>::getPromedio() {
    return this->promedio;
}

template <class T>
T Estadisticas<T>::getMediana() {
    return this->mediana;
}

template <class T>
double Estadisticas<T>::getDesviacionEst() {
    return this->desviacionEst;
}

template <class T>
T Estadisticas<T>::getModa() {
    return this->moda;
}


template <class T>
void Estadisticas<T>::ordenar(T datos[], int cantDatos) {
    quickSort(datos, 0, cantDatos-1);
}


template <class T>
double Estadisticas<T>::getPromedio(T datos[], int cantDatos) {
    double prom = datos[0];

    for(int x = 1; x < cantDatos; x++) {
        prom += datos[x];
    }

    return prom/cantDatos;
}

// Los datos ya llegan ordenados
template <class T>
T Estadisticas<T>::getMediana(T datos[], int cantDatos) {
    return datos[cantDatos/2]; // Por acuerdo se agarrara el de enmedio si impar cantDatos, y el de arriba si es par
}

template <class T>
double Estadisticas<T>::getDesviacionEst(T datos[], int cantDatos, double prom) {
    double result = 0;

    for(int x = 0; x < cantDatos; x++) {
        double mientr = double(datos[x]) - prom; // Para asegurar no desbordamiento si T es unsigned
        result += mientr * mientr;
    }

    return sqrt(result/cantDatos);

}

// Los datos ya llegan ordenados
// Por acuerdo se agarrara el primero si hay varios con misma cantidad.
template <class T>
T Estadisticas<T>::getModa(T datos[], int cantDatos) {
    int cantidadMayor = 1;
    T moda = datos[0];

    int count = 1;
    T datoActual = datos[0];
    for (int x = 1; x < cantDatos; x++) {
        if (datos[x] == datoActual) {
            count++;
        } else {
            if (count > cantidadMayor) {
                cantidadMayor = count;
                moda = datoActual;
            }

            datoActual = datos[x];
            count = 1;
        }
    }

    if (count > cantidadMayor) {
        cantidadMayor = count;
        moda = datoActual;
    }

    return moda;
}

#endif