#include "mlist.h"

void MoveList__clear(MoveList* list) {
	list->size = 0;
}

void MoveList__put(MoveList* list, BitMove move) {
	list->moves[list->size] = move;
	list->size++;
}

// TODO: wrong behavior if list is empty..
BitMove MoveList__get(MoveList* list, size_t n) {
	return list->moves[n];
}
