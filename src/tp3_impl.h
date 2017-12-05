#ifndef TP3_IMPL_H
#define TP3_IMPL_H

#include "tp3.h"

#include <limits>
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 1
////

template <typename iterator, typename bucket>
vector<bucket> generar_buckets(iterator input_begin, iterator input_end) {

	if(input_begin == input_end){
		return vector<bucket>();
	}

	int max = int(*input_begin); 
	int min = int(*input_begin); 	
	int lenght = 0; 
	for(auto it = input_begin; it != input_end; it++){
		if(max <= int(*it)){ max = int(*it);}
		if(min >= int(*it)){ min = int(*it);}
		lenght++;
	}
	vector<bucket> buckets((max-min)+1);

	for(auto it = input_begin; it != input_end; it++){
		int bucketIndex = int(*it)-min;
		buckets[bucketIndex].insert(buckets[bucketIndex].end(), (*it));
	}

	return buckets;
}


template <typename bucket>
vector<typename bucket::value_type> aplanar_buckets(const std::vector<bucket> & B) {

	int firstBucketSize = 0; 

	for(int i = 0; i < B.size(); i++){
		if(B[i].size() > 0){
			int firstBucketSize = B[1].size();
			break;
		}
	}

	vector<typename bucket::value_type> res(firstBucketSize);

	for(int i = 0; i < B.size(); i++){
		if(B[i].size() > 0){
			res.insert(res.end(), B[i].begin(), B[i].end());
		}
	}
    return res;
}


///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 2
////

fajo ordenar_por_probabilidad(const fajo& falsos_conocidos, const fajo & a_ordenar) {

    // ORDENO FALSOS_CONOCIDOS EN BUCKETS PARA PODER BUSCAR BILLETES POR AÑO. O(N*log(m)) 
	auto falsos = generar_buckets<fajo::const_iterator, std::set<billete> >(falsos_conocidos.begin(), falsos_conocidos.end());

    // CONSULTO EL PRIMER AÑO (PRIMER BUCKET) PARA PODER INDEXAR BIEN LA BÚSQUEDA EN falsos.
	int primerAnioEnVector = int(*falsos[0].begin());

    // fajoConProbActualizadas VA A GUARDAR (EN FORMA ORDENADA POR SER UN SET) UNA COPIA DE  a_ordenar 
    // CON LAS PROBABILIDADES DE TODOS LOS BILLETES ACTUALIZADA.
	//std::set<billete> fajoConProbActualizadas;

	std::vector<billete> fajoConProbActualizadas;

    for (auto & b : a_ordenar){

        // LA PROBABILIDAD POR DEFECTO DE b ES probabilidad_max. 
    	int probabilidadBillete = b.probabilidad_falso;

            // SI NO ENCUENTRO EL BILLETE EN falsos, ENTONCES LE ACTUALIZO LA PROBABILIDAD 
            // EN FUNCIÓN DEL AÑO EN EL QUE FUE IMPRESO. 
            // INVARIANTE: EN falsos EXISTE AL MENOS UN BILLETE FALSO POR AÑO: ESTO ME PERMITE
            // BUSCAR EN EL VECTOR CON LA SEGURIDAD DE QUE NO VA A TIRAR SEG FAULT. 

    		int indiceEnVectorFalsos = int(b) - primerAnioEnVector;

    		if(falsos[indiceEnVectorFalsos].find(b) == falsos[indiceEnVectorFalsos].end()){
    			probabilidadBillete = falsos[indiceEnVectorFalsos].size(); 
    		}

		//fajoConProbActualizadas.insert(billete(b.numero_de_serie, probabilidadBillete));
    	fajoConProbActualizadas.push_back(billete(b.numero_de_serie, probabilidadBillete));
    }

	std::sort(fajoConProbActualizadas.begin(), fajoConProbActualizadas.end(), [](billete a, billete b){
	return a.probabilidad_falso > b.probabilidad_falso || (a.probabilidad_falso == b.probabilidad_falso
	&& b.numero_de_serie < a.numero_de_serie); });

	return fajoConProbActualizadas;
}

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 3
////

inline void sumarMatrices(Matriz& A, Matriz& B, Matriz& C, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

inline void restarMatrices(Matriz& A, Matriz& B, Matriz& C, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }   
}

inline void strassen(Matriz& A, Matriz& B, Matriz& C, int K) {
    if(A.size() <= K){
		C =  multiplicar(A,B);
		return;
    }else{
        int midSize = A.size()/2;
        vector<double> aux (midSize);
		Matriz	a11(midSize,aux),
				a12(midSize,aux),
				a21(midSize,aux),
				a22(midSize,aux),
				b11(midSize,aux),
				b12(midSize,aux),
				b21(midSize,aux),
				b22(midSize,aux),
				c11(midSize,aux),
				c12(midSize,aux),
				c21(midSize,aux),
				c22(midSize,aux),
				m1(midSize,aux),
				m2(midSize,aux),
				m3(midSize,aux),
				m4(midSize,aux),
				m5(midSize,aux),
				m6(midSize,aux),
				m7(midSize,aux),
				aRes(midSize,aux),
				bRes(midSize,aux);
 
        for (int i = 0; i < midSize; i++) {
            for (int j = 0; j < midSize; j++) {
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + midSize];
                a21[i][j] = A[i + midSize][j];
                a22[i][j] = A[i + midSize][j + midSize];
 
                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + midSize];
                b21[i][j] = B[i + midSize][j];
                b22[i][j] = B[i + midSize][j + midSize];
            }
        }
 
        sumarMatrices(a11, a22, aRes, midSize);
        sumarMatrices(b11, b22, bRes, midSize);
        strassen(aRes, bRes, m1, midSize);
        sumarMatrices(a21, a22, aRes, midSize);
        strassen(aRes, b11, m2, midSize);
        restarMatrices(b12, b22, bRes, midSize);
        strassen(a11, bRes, m3, midSize);
        restarMatrices(b21, b11, bRes, midSize);
        strassen(a22, bRes, m4, midSize);
        sumarMatrices(a11, a12, aRes, midSize);
        strassen(aRes, b22, m5, midSize);
        restarMatrices(a21, a11, aRes, midSize);
        sumarMatrices(b11, b12, bRes, midSize);
        strassen(aRes, bRes, m6, midSize);
        restarMatrices(a12, a22, aRes, midSize);
        sumarMatrices(b21, b22, bRes, midSize);
        strassen(aRes, bRes, m7, midSize);
        sumarMatrices(m3, m5, c12, midSize);
        sumarMatrices(m2, m4, c21, midSize);
        sumarMatrices(m1, m4, aRes, midSize);
        sumarMatrices(aRes, m7, bRes, midSize);
        restarMatrices(bRes, m5, c11, midSize);
        sumarMatrices(m1, m3, aRes, midSize);
        sumarMatrices(aRes, m6, bRes, midSize);
        restarMatrices(bRes, m2, c22, midSize);


        for (int i = 0; i < midSize ; i++) {

            for (int j = 0 ; j < midSize ; j++) {
                C[i][j] = c11[i][j];
                C[i][j + midSize] = c12[i][j];
                C[i + midSize][j] = c21[i][j];
                C[i + midSize][j + midSize] = c22[i][j];
            }

        }


    }
}

inline Matriz multiplicar_strassen(Matriz& A, Matriz& B, int K) {
    unsigned int n = A.size();

    Matriz C(n, vector<double>(n)); 
    strassen(A,B,C,K);

    return C;
}



#endif // TP3_IMPL_H
