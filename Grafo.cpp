/*
 * File:   Grafo.cpp
 * Author: Alan
 *
 * Created on 2 de abril de 2015, 02:25 PM
 */

#include "Parse.h"
#include "Grafo.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <random>
#include <iostream>
#include <ctime>
#include <limits> // std::numeric_limits

#ifndef NULL
#define NULL 0
#endif // NULL

using namespace std;
using namespace line_parse;

Grafo::Grafo(int cntVrt, int prmAdy) {
    if (cntVrt >= 10 && 1 <= prmAdy)
    {
        this->cntVrt = cntVrt;
        arrVrt = new NdoVrt[cntVrt];
        srand(time(NULL));
        default_random_engine generador(rand());
        normal_distribution<double> distribucion(prmAdy, 2.0);
        for (int i = 0; i < cntVrt; i++)
        {
            for (int i = 0; i < cntVrt; i++)
            {
                int rnum = distribucion(generador);
                if (!xstAdy(i, rnum) && xstVrt(rnum))
                {
                    arrVrt[i].lstAdy.agr(rnum);
                    arrVrt[rnum].lstAdy.agr(i);
                }
            }
        }
    }
}

Grafo::Grafo(const Grafo& orig) {
    if (orig.cntVrt > 0)
    {
        cntVrt = orig.cntVrt;
        arrVrt = new NdoVrt[cntVrt];
        for (int i = 0; i < cntVrt; i++)
        {
            arrVrt[i].e = orig.arrVrt[i].e;
            arrVrt[i].tmpChqVrs = orig.arrVrt[i].tmpChqVrs;
            int *ady = orig.arrVrt[i].lstAdy.obtAdy();
            for (int j = 0; j < orig.arrVrt[i].lstAdy.obtCntAdy(); j++)
            {
                arrVrt[i].lstAdy.agr(ady[j]);
            }
            delete[] ady;
        }
    }
}

Grafo::Grafo(string nArch) {
    ifstream file;
    file.open(nArch.c_str(), ios::in);
    if (file.is_open() && file.good())
    {
        char line[256];
        memset(&line, 0, 256);
        file.getline(line, 256); //primera linea
        cntVrt = atoi(line);
        if (cntVrt < 0) return; // error
        arrVrt = new NdoVrt[cntVrt];
        int count = 0;
        do
        {
            memset(&line, 0, 256);
            file.getline(line, 256);
            string linea = line;
            unsigned int cant = cantidad_elementos(linea);
            if (cant > 0)
            {
                arrVrt[count].e = S;
                if (cant > 0)
                {
                    //arrVrt[count].lstAdy = new LstAdy();
                    for (int i = 0; i < cant; i++)
                    {
                        arrVrt[count].lstAdy.agr(elemento(linea, i));
                    }
                }
            }
            count++;
        } while (!file.eof() && count < cntVrt);
        return;
    }
    throw 1;
}

Grafo::~Grafo() {
    if (cntVrt > 0)
        delete[] arrVrt;
}

bool Grafo::xstVrt(int vrt) const {
    return (vrt < cntVrt && vrt >= 0); // Devuelve true si vrt esta entre 0 y cntVrt
}

bool Grafo::xstAdy(int vrtO, int vrtD) const {
    if (xstVrt(vrtO) && xstVrt(vrtD)) // Comprueba que el indice del vertice existe
    {
        int cantidad_adyacencias = arrVrt[vrtO].lstAdy.obtCntAdy();
        if (cantidad_adyacencias == 0) return false; // No hay adyacencias, por lo tanto la adyacencia no existe
        return arrVrt[vrtO].lstAdy.bus(vrtD);
    }
    return false; // No se encontr� adyacencia o el v�rtice no existe
}

int Grafo::obtCntAdy(int vrt) const
{
    if (xstVrt(vrt))
    {
        return arrVrt[vrt].lstAdy.obtCntAdy();
    }
}

int* Grafo::obtAdy(int vrt) const {
    if (xstVrt(vrt))
    {
        return arrVrt[vrt].lstAdy.obtAdy();
    }
    return NULL;
}

int Grafo::obtTotAdy() const {
    int cant = 0;
    for (int i = 0; i < cntVrt; i++)
    {
        cant += arrVrt[i].lstAdy.obtCntAdy();
    }
    return cant;
}

int Grafo::obtTotVrt() const {
    return cntVrt;
}

double Grafo::obtPrmAdy() const {
    double prom = 0;
    for (int i = 0; i < cntVrt; i++)
    {
        prom += ((double)arrVrt[i].lstAdy.obtCntAdy());
    }
    return prom / (double)cntVrt;
}

Grafo::E Grafo::obtEst(int vrt) const {
    if (xstVrt( vrt) == true){
        return arrVrt[vrt].e;
    }
}

bool Grafo::operator==(const Grafo& grf) const {
    if (this->cntVrt != grf.cntVrt) return false;
    for (int i = 0; i < this->cntVrt; i++)
    {
        if (arrVrt[i].lstAdy.obtCntAdy() != grf.arrVrt[i].lstAdy.obtCntAdy()) return false;
        int *ady = obtAdy(i), *adygrf = grf.obtAdy(i);
        for (int j = 0; j < arrVrt[i].lstAdy.obtCntAdy(); j++)
        {
            if (ady[j] != adygrf[j])
            {
                delete[] ady; delete[] adygrf;
                return false;
            }
        }
        delete[] ady; delete[] adygrf;
    }
    return true;
}

double Grafo::promLongCmnsCrts() const {
    int sum = 0, total = cntVrt*(cntVrt-1)/2;
    int **matriz = Floyd_Warshall();
    for (int i = 0; i < cntVrt; i++)
    {
        for (int j = i; j < cntVrt; j++)
        {
            if (xstAdy(i, j))
                sum += matriz[i][j];
        }
    }
    for(int i = 0; i < cntVrt; i++)
    {
        delete[] matriz[i];
    }
    delete[] matriz;
    return ((double)sum/(double)total);
}

double Grafo::centralidadIntermedial(int vrt) const { // no se va a implementar

}

double Grafo::coeficienteAgrupamiento(int vrt) const {
    if (xstVrt(vrt))
    {
        double cant = (double)obtCntAdy(vrt), triangles = 0.0;
        if (cant == 0 || cant == 1) return 0;
        double t_g = (cant*(cant-1))/2;
        int *ady = obtAdy(vrt);
        for (int i = 0; i < cant; i++)
        {
            for (int j = i+1; j < cant; j++)
            {
                if (xstAdy(ady[i], ady[j]))
                {
                    triangles++;
                }
            }
        }
        delete[] ady;
        return (triangles / t_g);
    }
    return 0;
}

void Grafo::modEst(int vrt, E ne) {
    if (xstVrt(vrt))
    {
        arrVrt[vrt].e = ne;
    }
}

int **Grafo::Floyd_Warshall() const {
    int** path;
    path = new int*[cntVrt];
    for(int i = 0; i < cntVrt; i++)
    {
        path[i] = new int[cntVrt];
        path[i][i] = 0;
    }
    for (int i = 0; i < cntVrt; i++)
    {
        for (int j = 0; j < cntVrt; j++)
        {
            if (xstAdy(i, j))
            {
                path[i][j] = 1;
            }
            else
            {
                path[i][j] = std::numeric_limits<int>::max();
            }
        }
    }
    for(int k = 0; k < cntVrt; k++)
        for(int i = 0; i < cntVrt; i++)
            for(int j = 0; j < cntVrt; j++){
                if (path[i][k] == std::numeric_limits<int>::max() || path[k][j] == std::numeric_limits<int>::max()) continue;
                int dt = path[i][k] + path[k][j];
                if(path[i][j] > dt)
                    path[i][j] = dt;
            }

    return path;
}
