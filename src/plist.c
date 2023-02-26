#include "plist.h"

void PieceList__add(Piece* list, size_t* len, Square sq) {
	list[*len] = sq;
	(*len)++;
}

void PieceList__del(Piece* list, size_t* len, Square sq) {
	for(size_t i = 0; i < *len; i++) {
		if(list[i] == sq) {
			PieceList__del_index(list, len, i);
			return;
		}
	}
}

void PieceList__del_index(Piece* list, size_t* len, size_t i) {
	(*len)--;
	if(i < *len) {
		// If i ist not pointing to last element, we replace it with the last.
		list[i] = list[*len];
	}
}
