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
	std::set<billete> fajoConProbActualizadas;


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

		fajoConProbActualizadas.insert(billete(b.numero_de_serie, probabilidadBillete));
    }

    fajo resultado;
    resultado.reserve(a_ordenar.size());

    // ITERO SOBRE fajoConProbActualizadas PARA CREAR EL VECTOR A DEVOLVER.
	for (auto it=fajoConProbActualizadas.rbegin(); it!=fajoConProbActualizadas.rend(); ++it){
		resultado.push_back(*it);
	}

    return resultado;
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
        int newTam = A.size()/2;
        vector<double> inner (newTam);
		Matriz	a11(newTam,inner), 
				a12(newTam,inner), 
				a21(newTam,inner), 
				a22(newTam,inner),
				b11(newTam,inner), 
				b12(newTam,inner), 
				b21(newTam,inner), 
				b22(newTam,inner),
				c11(newTam,inner), 
				c12(newTam,inner), 
				c21(newTam,inner), 
				c22(newTam,inner),
				p1(newTam,inner), 
				p2(newTam,inner), 
				p3(newTam,inner), 
				p4(newTam,inner), 
				p5(newTam,inner), 
				p6(newTam,inner), 
				p7(newTam,inner),
				aResult(newTam,inner), 
				bResult(newTam,inner);
 
        for (int i = 0; i < newTam; i++) {
            for (int j = 0; j < newTam; j++) {
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + newTam];
                a21[i][j] = A[i + newTam][j];
                a22[i][j] = A[i + newTam][j + newTam];
 
                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + newTam];
                b21[i][j] = B[i + newTam][j];
                b22[i][j] = B[i + newTam][j + newTam];
            }
        }
 
 
        sumarMatrices(a11, a22, aResult, newTam); 
        sumarMatrices(b11, b22, bResult, newTam); 
        strassen(aResult, bResult, p1, newTam); 
 
        sumarMatrices(a21, a22, aResult, newTam);
        strassen(aResult, b11, p2, newTam); 
 
        restarMatrices(b12, b22, bResult, newTam);
        strassen(a11, bResult, p3, newTam); 
 
        restarMatrices(b21, b11, bResult, newTam);
        strassen(a22, bResult, p4, newTam);
 
        sumarMatrices(a11, a12, aResult, newTam); 
        strassen(aResult, b22, p5, newTam); 
 
        restarMatrices(a21, a11, aResult, newTam); 
        sumarMatrices(b11, b12, bResult, newTam); 
        strassen(aResult, bResult, p6, newTam); 
 
        restarMatrices(a12, a22, aResult, newTam); 
        sumarMatrices(b21, b22, bResult, newTam); 
        strassen(aResult, bResult, p7, newTam); 
 
        sumarMatrices(p3, p5, c12, newTam);
        sumarMatrices(p2, p4, c21, newTam); 
 
        sumarMatrices(p1, p4, aResult, newTam);
        sumarMatrices(aResult, p7, bResult, newTam); 
        restarMatrices(bResult, p5, c11, newTam); 

        sumarMatrices(p1, p3, aResult, newTam);
        sumarMatrices(aResult, p6, bResult, newTam); 
        restarMatrices(bResult, p2, c22, newTam); 
 
        for (int i = 0; i < newTam ; i++) {
            for (int j = 0 ; j < newTam ; j++) {
                C[i][j] = c11[i][j];
                C[i][j + newTam] = c12[i][j];
                C[i + newTam][j] = c21[i][j];
                C[i + newTam][j + newTam] = c22[i][j];
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
