#ifndef PLIST_H
#define PLIST_H

#include <stdio.h>

#include "base.h"

void PieceList__add(Piece *list, size_t *len, Square sq);
void PieceList__del(Piece *list, size_t *len, Square sq);
void PieceList__del_index(Piece *list, size_t *len, size_t i);
void PieceList__move(Piece *list, size_t len, Square from, Square to);

#endif
