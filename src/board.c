#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"
#include "fen.h"
#include "plist.h"
#include "errors.h"

void Board__clear(Board *b) {
	// NOTE: this is only needed for testing.. no need to optimize.

	for(size_t i = 0; i < 2 * 64; i++) {
		b->squares[i] = EMPTY;
	}
	b->castle_short[BLACK] = false;
	b->castle_short[WHITE] = false;
	b->castle_long[BLACK]  = false;
	b->castle_long[WHITE]  = false;
	b->move_number		   = 1;
	b->draw_counter		   = 0;
	b->ep_square		   = OTB;
	b->player			   = WHITE;

	for(Color c = BLACK; c <= WHITE; c++) {
		b->kings[c]		   = OTB;
		b->sliders_size[c] = 0;
		for(size_t i = 0; i < 13; i++) {
			b->sliders[c][i] = OTB;
		}
		b->queens_size[c] = 0;
		for(size_t i = 0; i < 9; i++) {
			b->queens[c][i] = OTB;
		}
		b->rooks_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->rooks[c][i] = OTB;
		}
		b->bishops_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->bishops[c][i] = OTB;
		}
		b->knights_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->knights[c][i] = OTB;
		}
		b->pawns_size[c] = 0;
		for(size_t i = 0; i < 8; i++) {
			b->pawns[c][i] = OTB;
		}
	}
}

void Board__clear_meta(Board *b) {
	// This function is called often and thus the "loop" is manually unrolled.
	// It is a lot faster than any looping construct.
	b->check_info = OTB;

	b->squares[0x8]	 = INFO_NONE;
	b->squares[0x9]	 = INFO_NONE;
	b->squares[0xa]	 = INFO_NONE;
	b->squares[0xb]	 = INFO_NONE;
	b->squares[0xc]	 = INFO_NONE;
	b->squares[0xd]	 = INFO_NONE;
	b->squares[0xe]	 = INFO_NONE;
	b->squares[0xf]	 = INFO_NONE;
	b->squares[0x18] = INFO_NONE;
	b->squares[0x19] = INFO_NONE;
	b->squares[0x1a] = INFO_NONE;
	b->squares[0x1b] = INFO_NONE;
	b->squares[0x1c] = INFO_NONE;
	b->squares[0x1d] = INFO_NONE;
	b->squares[0x1e] = INFO_NONE;
	b->squares[0x1f] = INFO_NONE;
	b->squares[0x28] = INFO_NONE;
	b->squares[0x29] = INFO_NONE;
	b->squares[0x2a] = INFO_NONE;
	b->squares[0x2b] = INFO_NONE;
	b->squares[0x2c] = INFO_NONE;
	b->squares[0x2d] = INFO_NONE;
	b->squares[0x2e] = INFO_NONE;
	b->squares[0x2f] = INFO_NONE;
	b->squares[0x38] = INFO_NONE;
	b->squares[0x39] = INFO_NONE;
	b->squares[0x3a] = INFO_NONE;
	b->squares[0x3b] = INFO_NONE;
	b->squares[0x3c] = INFO_NONE;
	b->squares[0x3d] = INFO_NONE;
	b->squares[0x3e] = INFO_NONE;
	b->squares[0x3f] = INFO_NONE;
	b->squares[0x48] = INFO_NONE;
	b->squares[0x49] = INFO_NONE;
	b->squares[0x4a] = INFO_NONE;
	b->squares[0x4b] = INFO_NONE;
	b->squares[0x4c] = INFO_NONE;
	b->squares[0x4d] = INFO_NONE;
	b->squares[0x4e] = INFO_NONE;
	b->squares[0x4f] = INFO_NONE;
	b->squares[0x58] = INFO_NONE;
	b->squares[0x59] = INFO_NONE;
	b->squares[0x5a] = INFO_NONE;
	b->squares[0x5b] = INFO_NONE;
	b->squares[0x5c] = INFO_NONE;
	b->squares[0x5d] = INFO_NONE;
	b->squares[0x5e] = INFO_NONE;
	b->squares[0x5f] = INFO_NONE;
	b->squares[0x68] = INFO_NONE;
	b->squares[0x69] = INFO_NONE;
	b->squares[0x6a] = INFO_NONE;
	b->squares[0x6b] = INFO_NONE;
	b->squares[0x6c] = INFO_NONE;
	b->squares[0x6d] = INFO_NONE;
	b->squares[0x6e] = INFO_NONE;
	b->squares[0x6f] = INFO_NONE;
	b->squares[0x78] = INFO_NONE;
	b->squares[0x79] = INFO_NONE;
	b->squares[0x7a] = INFO_NONE;
	b->squares[0x7b] = INFO_NONE;
	b->squares[0x7c] = INFO_NONE;
	b->squares[0x7d] = INFO_NONE;
	b->squares[0x7e] = INFO_NONE;
	b->squares[0x7f] = INFO_NONE;
}

void Board__set_starting_position(Board *b) {
	assert(b != NULL);

	Error err = Board__set_fen(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	assert(err == OK);
}

Error Board__set_fen(Board *b, const char *fen) {
	if(b == NULL || fen == NULL) {
		return ERR_NULL_PTR;
	}

	MinBoard mb;
	Error	 err = parse_fen(fen, &mb);
	if(err != OK) {
		return err;
	}

	for(size_t i = 0; i < 64 * 2; i++) {
		b->squares[i] = EMPTY;
	}

	// Clear piece lists by zeroing their sizes.
	for(Color c = BLACK; c <= WHITE; c++) {
		b->sliders_size[c] = 0;
		b->queens_size[c]  = 0;
		b->rooks_size[c]   = 0;
		b->bishops_size[c] = 0;
		b->knights_size[c] = 0;
		b->pawns_size[c]   = 0;
	}

	// Copy over data from minboard.
	b->move_number	= mb.move_num;
	b->draw_counter = mb.half_moves;

	if(mb.ep_square != OTB) {
		b->ep_square = LOOKUP_0x88[mb.ep_square];
	} else {
		b->ep_square = OTB;
	}

	b->castle_short[BLACK] = mb.castle_short[BLACK];
	b->castle_short[WHITE] = mb.castle_short[WHITE];
	b->castle_long[BLACK]  = mb.castle_long[BLACK];
	b->castle_long[WHITE]  = mb.castle_long[WHITE];

	b->player = mb.color;

	for(size_t i = 0; i < 64; i++) {
		const Square sq	   = LOOKUP_0x88[i];
		const Piece	 piece = mb.squares[i];
		const Piece	 ptype = piece & PIECE_MASK;
		const Piece	 pcol  = piece & COLOR_ONLY_MASK;

		if(piece == EMPTY) {
			b->squares[sq] = EMPTY;
			continue;
		}

		switch(ptype) {
		case PAWN: PieceList__add(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
		case KNIGHT: PieceList__add(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
		case BISHOP:
			PieceList__add(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case ROOK:
			PieceList__add(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case QUEEN:
			PieceList__add(b->queens[pcol], &(b->queens_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case KING:
			b->squares[sq] = piece;
			b->kings[pcol] = sq;
			break;
		default: printf("ptype: %d\n", ptype); return ERR_INVALID_INPUT; // Skips squares assignment.
		}
		b->squares[sq] = piece;
	}

	// TODO: Detect checks (and pins?) here for init.
	b->check_info = CHECK_NONE;

	return OK;
}

// TODO: NULL check?
// TODO: what if square has a piece already?
// TODO: check if plists are full? (should never happen [maybe in chess variants])
void Board__add_piece(Board *b, Square sq, Piece p) {
	if(!on_board(sq)) {
		return;
	}

	const Piece ptype = p & PIECE_MASK;
	const Piece pcol  = p & COLOR_ONLY_MASK;

	switch(ptype) {
	case PAWN: PieceList__add(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
	case KNIGHT: PieceList__add(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
	case BISHOP:
		PieceList__add(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case ROOK:
		PieceList__add(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case QUEEN:
		PieceList__add(b->queens[pcol], &(b->queens_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case KING: b->kings[pcol] = sq; break;
	default:
		// This should never happen. Ignore all other values.
		return; // Skips squares assignment.
	}
	b->squares[sq] = p;
}

// TODO: NULL check?
void Board__del_piece(Board *b, Square sq) {
	if(!on_board(sq)) {
		return;
	}
	const Piece p	  = b->squares[sq];
	const Piece ptype = p & PIECE_MASK;
	const Piece pcol  = p & COLOR_ONLY_MASK;

	b->squares[sq] = EMPTY;
	switch(ptype) {
	case PAWN: PieceList__del(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
	case KNIGHT: PieceList__del(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
	case BISHOP:
		PieceList__del(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case ROOK:
		PieceList__del(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case QUEEN:
		PieceList__del(b->queens[pcol], &(b->queens_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case KING: b->kings[pcol] = sq; break;
	default:
		// This should never happen. Ignore all other values.
		break;
	}
}

bool Board__is_sq_attacked(Board *b, const Square sq, const Square ignore_sq, Color color) {
	const Color opp_color = flip_color(color);

	// TODO: benchmark if order of piece types has a significant impact here.

	// Detect attacks by knights.
	for(size_t i = 0; i < b->knights_size[opp_color]; i++) {
		Square opp_knight_sq = b->knights[opp_color][i];
		if(opp_knight_sq == ignore_sq) {
			continue;
		}
		Square diff = square_diff(opp_knight_sq, sq);
		if(contains_piece_type(DIFF_ATTACK_MAP[diff], KNIGHT)) {
			return true;
		}
	}

	// Detect attacks by sliders.
	if(Board__is_sq_attacked_by_slider(b, sq, ignore_sq, color)) {
		return true;
	}
	// TODO: should each slider have a call? (probably not)

	// Detect attacks by pawns.
	const Piece opp_pawn = opp_color | PAWN;

	Square potential_pawn_atk_sq = (Square)((Direction)sq + PAWN_CAPTURE_DIRS[color][0]);
	if(on_board(potential_pawn_atk_sq) && b->squares[potential_pawn_atk_sq] == opp_pawn) {
		// Found attacking pawn by lookup in reverse direction.
		return true;
	}
	potential_pawn_atk_sq = (Square)((Direction)sq + PAWN_CAPTURE_DIRS[color][1]);
	if(on_board(potential_pawn_atk_sq) && b->squares[potential_pawn_atk_sq] == opp_pawn) {
		// Found attacking pawn by lookup in reverse direction.
		return true;
	}

	// Detect attacks by kings.
	const Square opp_king_sq = b->kings[opp_color];
	Square		 diff		 = square_diff(opp_king_sq, sq);
	if(contains_piece_type(DIFF_ATTACK_MAP[diff], KING)) {
		return true;
	}

	return false;
}

bool Board__is_sq_attacked_by_slider(Board *b, const Square sq, const Square ignore_sq, Color color) {
	const Color opp_color = flip_color(color);

	for(size_t i = 0; i < b->sliders_size[opp_color]; i++) {
		const Square slider_sq = b->sliders[opp_color][i];
		if(slider_sq == ignore_sq) {
			// This can only eval true in pawn move legality tests.
			// If a pawn captured a piece, it still is in the piece list and must be ignored here.
			continue;
		}
		const Piece	 ptype = b->squares[slider_sq] & PIECE_MASK;
		const Square diff  = square_diff(sq, slider_sq);

		if(contains_piece_type(DIFF_ATTACK_MAP[diff], ptype)) {
			// The slider possibly attacks Square sq.
			const Direction dir = DIFF_DIR_MAP[diff];
			// Starting from sq we step through the path in question towards the enemy slider.
			Square step_sq = (Direction)sq + dir;
			while(true) {
				Piece cur_piece = b->squares[step_sq];
				if(cur_piece == EMPTY) {
					step_sq = (Direction)step_sq + dir;
					continue;
				} else if(has_color(cur_piece, color)) {
					// A friendly piece was encountered -> blocks any attack.
					break;
				} else if(contains_piece_type(cur_piece, ptype)) {
					// An attacking enemy slider was encountered.
					return true;
				} else {
					// Blocking enemy piece.
					break;
				}
			}
		}
	}

	return false;
}

void Board__detect_checks_and_pins(Board *b, Color color) {
	Board__clear_meta(b);
	const Color opp_color = flip_color(color);

	const Square king_sq	   = b->kings[color];
	int			 check_counter = 0;
	Info		 pin_marker	   = INFO_PIN_COUNTER_START;

	// Detect checks by knigts.
	for(size_t i = 0; i < b->knights_size[opp_color]; i++) {
		const Square knight_sq = b->knights[opp_color][i];
		const Square diff	   = square_diff(knight_sq, king_sq);
		if(contains_piece_type(DIFF_ATTACK_MAP[diff], KNIGHT)) {
			// A knight checks the king. Safe the info and break the loop.
			// There can never be a double check of two knights.
			b->check_info = knight_sq;
			check_counter++;
			b->squares[to_info_index(knight_sq)] = INFO_MASK_CHECK;
			break; // TODO: we could skip the pawn section here ?!
		}
	}

	// Detect checks by pawns.
	for(size_t i = 0; i < b->pawns_size[opp_color]; i++) {
		const Square pawn_sq = b->pawns[opp_color][i];
		for(int d = 0; d < PAWN_PUSH_CAPTURE_LEN; d++) {
			Direction dir = PAWN_CAPTURE_DIRS[opp_color][d];
			Square	  to  = (Square)((Direction)pawn_sq + dir);
			if(king_sq == to) {
				// A pawn checks the king. Safe the info and break the loop.
				// There can never be a double check of two pawns.
				b->check_info = pawn_sq;
				check_counter++;
				b->squares[to_info_index(pawn_sq)] = INFO_MASK_CHECK;
				goto EXIT_PAWN_CHECK; // double break -> exit pawn loops
			}
		}
	}

EXIT_PAWN_CHECK:

	// Detect checks and pins by sliders.
	// Queens
	check_counter += Board__detect_slider_checks_and_pins(b, color, &pin_marker, check_counter, b->queens_size[opp_color], b->queens[opp_color], QUEEN);
	if(check_counter > 1) {
		// Double check -> exit early.
		return;
	}
	// Rooks
	check_counter += Board__detect_slider_checks_and_pins(b, color, &pin_marker, check_counter, b->rooks_size[opp_color], b->rooks[opp_color], ROOK);
	if(check_counter > 1) {
		// Double check -> exit early.
		return;
	}
	// Bishops
	check_counter += Board__detect_slider_checks_and_pins(b, color, &pin_marker, check_counter, b->bishops_size[opp_color], b->bishops[opp_color], BISHOP);
	if(check_counter > 1) {
		// Double check -> exit early.
		return;
	}
}

int Board__detect_slider_checks_and_pins(Board *b, Color color, Info *pmarker, const int ccount, size_t plist_len, Square const *const plist, Piece ptype) {
	const Square king_sq = b->kings[color];
	int check_counter = 0;

	for(size_t i = 0; i < plist_len; i++) {
		const Square slider_sq = plist[i];
		const Square diff = square_diff(king_sq, slider_sq);
		if(contains_piece_type(DIFF_ATTACK_MAP[diff], ptype)) {
			// The slider possible checks the king or pins a piece in front of the king.
			const Direction diffdir = DIFF_DIR_MAP[diff];
 			// Starting from the king we step through the path in question towards the enemy slider.
			Info info = INFO_NONE;
			Square step_sq = (Square)((Direction)king_sq + diffdir);
			while(true) {
				Piece cur_piece = b->squares[step_sq];
				if(cur_piece == EMPTY) {
					step_sq = (Direction)step_sq + diffdir;
					continue;
				} else if(has_color(cur_piece, color)) {
					// A friendly piece was encountered on the path.
					if(info == INFO_NONE) {
						// First friendly piece on the path -> mark as possible pin.
						info = INFO_MASK_MAYBE_PINNED;	
					} else {
						// Another piece was previously marked -> two pieces on path means no pin.
 						// (There is an exception for EP capture but those are handled elsewhere.
						info = INFO_NONE;
						break; // This path contains no pin.
					}
				} else {
					// An enemy piece was encountered.
					if(step_sq == slider_sq) {
						// We reached the checker or pinner -> decide.
						if(info == INFO_NONE) {
							// No pinner was marked. Must be a check.
							check_counter++;
							b->check_info = slider_sq;
							info = INFO_MASK_CHECK;
							break;
						} else {
							// It's a pinner
							info = *pmarker;
							(*pmarker)++;
							break;
						}
					} else {
						// An enemy piece is blocking the potential checker/pinner.
						info = INFO_NONE;
						break;
					}
				}
			}

			if(info & INFO_MASK_CHECK || info & INFO_MASK_PINNED) {
				// A path that pins or checks was detected. It is now marked 
				// in the info board to enrich move generation.
				step_sq = slider_sq;
				while(step_sq != king_sq) {
					// TODO: should we mark the path only or include the slider square?
					b->squares[to_info_index(step_sq)] = info;
					step_sq = (Square)((Direction)(step_sq - diffdir));
				}
				// If it is a check we also need to mark the square behind the king.
				if(info & INFO_MASK_CHECK) {
					step_sq = (Square)((Direction)(king_sq - diffdir));
					if(on_board(step_sq)) {
						b->squares[to_info_index(step_sq)] = INFO_MASK_FORBIDDEN_ESCAPE;
					}
				}
			} 

			if(check_counter+ccount > 1) {
				// Double check detected.. leave early.
				return check_counter;
			}
		}
	}

	return check_counter;
}

Error Board__to_string(Board *b, char *str) {
	if(b == NULL || str == NULL) {
		return ERR_NULL_PTR;
	}

	// str := "  +-----------------+\n"
	// for r := 7; r >= 0; r-- {
	// 	str += strconv.Itoa(r+1) + " | "
	// 	for f := 0; f < 8; f++ {
	// 		idx := 16*r + f
	// 		if Piece(idx) == b.EpSquare {
	// 			str += ", "
	// 		} else {
	// 			str += PrintMap[b.Squares[idx]] + " "
	// 		}
	//
	// 		if f == 7 {
	// 			str += "|\n"
	// 		}
	// 	}
	// }
	// str += "  +-----------------+\n"
	// str += "    a b c d e f g h\n"

	return OK;
}
