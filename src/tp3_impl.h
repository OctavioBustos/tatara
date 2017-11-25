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

	// Busco el máximo en la colección y la longitud de la colección.
	// En base a estas dos cosas calculo la cantidad de buckets. 
	int max = int(*input_begin); 
	int lenght = 0; 
	auto it = input_begin;
	for(it; it != input_end; it++){
		if(max <= int(*it)){ max = int(*it);}
		lenght++;
	}

	int cantBuckets = ( lenght != 0) ? max / lenght : 0;

	vector<bucket> buckets(cantBuckets+1);

	it = input_begin;
	for(it; it != input_end; it++){
		int bucketIndex = ( cantBuckets != 0) ? abs(int(*it)) % cantBuckets : 0; 
		buckets[bucketIndex].insert(buckets[bucketIndex].end(), (*it));
	}	

	for(int i = 0; i < cantBuckets; i++){
		buckets[i].sort();
	}

	return buckets;
}

/*
template <typename bucket>
vector<typename bucket::value_type> aplanar_buckets(const std::vector<bucket> & B) {
    return vector<typename bucket::value_type>();
}

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 2
////

fajo ordenar_por_probabilidad(const fajo& falsos_conocidos, const fajo & a_ordenar) {
    return a_ordenar;
}

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 3
////

inline Matriz multiplicar_strassen(const Matriz& A, const Matriz& B, int K) {
    return multiplicar(A,B);
}
*/
#endif // TP3_IMPL_H
