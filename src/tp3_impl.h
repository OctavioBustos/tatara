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

// ESTO NO VA. LOS BUCKES QUEDAN ORDENADOS DEPENDIENDO DEL CONTENEDOR QUE SE USE.
/*
	for(int i = 0; i < buckets.size(); i++){

		for(auto it = buckets[i].begin(); it != buckets[i].end(); it++){
			for(auto it2 = it; it2 != buckets[i].end(); it2++){
				if(*it > *it2){
					std::swap(it, it2);
				}
			}
		}
	}
*/
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

	auto falsos = generar_buckets<fajo::const_iterator, std::set<billete> >(falsos_conocidos.begin(), falsos_conocidos.end());

	int primerAnioEnVector = int(*falsos[0].begin());
	int ultimoAnioEnVector = primerAnioEnVector + falsos.size() - 1;

	std::set<billete> fajoConProbActualizadas;

    for (auto & b : a_ordenar){
    	int probabilidadBillete = 0; 

    	if( (int(b) >= primerAnioEnVector) and (int(b) <= ultimoAnioEnVector) ){
    		int indiceEnVectorFalsos = int(b) - primerAnioEnVector;
    		auto it = falsos[indiceEnVectorFalsos].find(b);

    		if(it == falsos[indiceEnVectorFalsos].end()){
    			probabilidadBillete = falsos[indiceEnVectorFalsos].size(); 
    		}
    		else{
    			probabilidadBillete = b.probabilidad_falso;
    		}
    	}

		fajoConProbActualizadas.insert(billete(b.numero_de_serie, probabilidadBillete));
    }

    fajo resultado;
    resultado.reserve(a_ordenar.size());

	for (auto it=fajoConProbActualizadas.rbegin(); it!=fajoConProbActualizadas.rend(); ++it){
		resultado.push_back(*it);
	}

    return resultado;
}
/*
///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 3
////

inline Matriz multiplicar_strassen(const Matriz& A, const Matriz& B, int K) {
    return multiplicar(A,B);
}
*/
#endif // TP3_IMPL_H
