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

inline void multiplicarMatrices(Matriz& A, Matriz& B, Matriz& C) {
	for (size_t i=0; i < A.size(); ++i)
		for (size_t j=0; j < A.size(); ++j)
			for (size_t k=0; k < B.size(); ++k)
				C[i][j] += A[i][k] * B[k][j];
}


inline void strassen(Matriz& A, Matriz& B, Matriz& C, unsigned long K) {
    if(A.size() <= K){
		C = multiplicar(A,B);
		return;
    }else{
        unsigned long midSize = A.size()/2;
        vector<double> aux (midSize);
		Matriz	A11(midSize,aux),
				A22(midSize,aux),
				B11(midSize,aux),
				B22(midSize,aux),

				M1(midSize,aux),
				M2(midSize,aux),
				M3(midSize,aux),
				M4(midSize,aux),
				M5(midSize,aux),
				M6(midSize,aux),
				M7(midSize,aux),

                A1122(midSize,aux),
                B1122(midSize, aux),
                A2122(midSize, aux),
                B12_22(midSize, aux),
                B21_11(midSize, aux),
                A1112(midSize, aux),
                A21_11(midSize, aux),
                B1112(midSize, aux),
                A12_22(midSize, aux),
                B2122(midSize, aux)
        ;
 
        for (int i = 0; i < midSize; i++) {
            for (int j = 0; j < midSize; j++) {

                double a11 = A[i][j];
                double a12 = A[i][j + midSize];
                double a21 = A[i + midSize][j];
                double a22 = A[i + midSize][j + midSize];

                double b11 = B[i][j];
                double b12 = B[i][j + midSize];
                double b21 = B[i + midSize][j];
                double b22 = B[i + midSize][j + midSize];

                A11[i][j] = a11;
                A22[i][j] = a22;
                B11[i][j] = b11;
                B22[i][j] = b22;
                A1122[i][j] = a11 + a22;
                B1122[i][j] = b11 + b22;
                A2122[i][j] = a21 + a22;
                B12_22[i][j] = b12 - b22;
                B21_11[i][j] = b12 - b11;
                A1112[i][j] = a11 + a12;
                A21_11[i][j] = a21 - a11;
                B1112[i][j] = b11 + b12;
                A12_22[i][j] = a12 - a22;
                B2122[i][j] = b21 + b22;

            }
        }

        strassen(A1122, B1122, M1, K);
        strassen(A2122, B11, M2, K);
        strassen(A11, B12_22, M3, K);
        strassen(A22, B21_11, M4, K);
        strassen(A1112, B22, M5, K);
        strassen(A21_11, B1112, M6, K);
        strassen(A12_22, B2122, M7, K);

        for (int i = 0; i < midSize ; i++) {
            for (int j = 0 ; j < midSize ; j++) {
                C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
                C[i][j + midSize] = M3[i][j] + M5[i][j];
                C[i + midSize][j] = M2[i][j] + M4[i][j];
                C[i + midSize][j + midSize] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
            }
        }

    }
}

inline Matriz multiplicar_strassen(Matriz& A, Matriz& B, int K) {
    unsigned long n = A.size();

    Matriz C(n, vector<double>(n)); 
    strassen(A,B,C,K);

    return C;
}



#endif // TP3_IMPL_H
