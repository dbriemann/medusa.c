#ifndef MLIST_H
#define MLIST_H

#include <stdio.h>

#include "base.h"
#include "bitmove.h"

#define MLIST_SIZE 512

typedef struct MoveList MoveList;
struct MoveList {
	size_t size;
	BitMove moves[MLIST_SIZE];
	// TODO: if a game has more than 512 half-moves this will blow up.
	// Option a) Ignore because it will never happen in millions of games.
	// Option b) Dynamically allocate 256 or so and resize if actually needed.
	// Option c) Delete early game history if maximum is reached.
	// NOTE: other game modes from classical might have different "maxima" here.
};

void    MoveList__clear(MoveList* list);
void    MoveList__put(MoveList* list, BitMove move);
BitMove MoveList__get(MoveList* list, size_t n);

#endif
