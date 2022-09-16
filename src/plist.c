#include "plist.h"

void PieceList__add(Square sq, Piece *list, size_t *len) {
	list[*len] = sq;
	(*len)++;
}
