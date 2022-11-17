#include "board.hpp"

#include "board.hpp"

#include <fmt/format.h>

using namespace chess;

Board Board::setupStandardBoard() {
	Board tempBoard;

	tempBoard.board[0] = PieceBits::Black | PieceBits::Rook | PieceBits::CanCastle;
	tempBoard.board[1] = PieceBits::Black | PieceBits::Knight;
	tempBoard.board[2] = PieceBits::Black | PieceBits::Bishop;
	tempBoard.board[3] = PieceBits::Black | PieceBits::Queen;
	tempBoard.board[4] = PieceBits::Black | PieceBits::King | PieceBits::CanCastle;
	tempBoard.board[5] = PieceBits::Black | PieceBits::Bishop;
	tempBoard.board[6] = PieceBits::Black | PieceBits::Knight;
	tempBoard.board[7] = PieceBits::Black | PieceBits::Rook | PieceBits::CanCastle;

	tempBoard.board[56] = PieceBits::White | PieceBits::Rook | PieceBits::CanCastle;
	tempBoard.board[57] = PieceBits::White | PieceBits::Knight;
	tempBoard.board[58] = PieceBits::White | PieceBits::Bishop;
	tempBoard.board[59] = PieceBits::White | PieceBits::Queen;
	tempBoard.board[60] = PieceBits::White | PieceBits::King | PieceBits::CanCastle;
	tempBoard.board[61] = PieceBits::White | PieceBits::Bishop;
	tempBoard.board[62] = PieceBits::White | PieceBits::Knight;
	tempBoard.board[63] = PieceBits::White | PieceBits::Rook | PieceBits::CanCastle;

	for (int i = 0; i < 8; i++) {
		tempBoard.board[8 + i] = PieceBits::Black | PieceBits::Pawn;
		tempBoard.board[48 + i] = PieceBits::White | PieceBits::Pawn;
	}

	tempBoard.toMove = Side::White;
	tempBoard.halfmoveSinceStateAdvance = 0;
	tempBoard.turn = 1;

	return tempBoard;
}

std::string Board::toFenString() const {
	std::string returnString;

	for (int invRank = 0; invRank < 8; invRank++) {
		int numBlank = 0;

		for (int file = 0; file < 8; file++) {
			if (board[invRank * 8 + file] == PieceBits::None) {
				numBlank++;
				continue;
			}

			if (numBlank != 0) {
				returnString += std::to_string(numBlank);
				numBlank = 0;
			}

			switch (board[invRank * 8 + file]) {
			case PieceBits::Black | PieceBits::Rook:
			case PieceBits::Black | PieceBits::Rook | PieceBits::CanCastle:
				returnString += 'r'; break;
			case PieceBits::White | PieceBits::Rook:
			case PieceBits::White | PieceBits::Rook | PieceBits::CanCastle:
				returnString += 'R'; break;
			case PieceBits::Black | PieceBits::Knight:
				returnString += 'n'; break;
			case PieceBits::White | PieceBits::Knight:
				returnString += 'N'; break;
			case PieceBits::Black | PieceBits::Bishop:
				returnString += 'b'; break;
			case PieceBits::White | PieceBits::Bishop:
				returnString += 'B'; break;
			case PieceBits::Black | PieceBits::Queen:
				returnString += 'q'; break;
			case PieceBits::White | PieceBits::Queen:
				returnString += 'Q'; break;
			case PieceBits::Black | PieceBits::King:
			case PieceBits::Black | PieceBits::King | PieceBits::CanCastle:
				returnString += 'k'; break;
			case PieceBits::White | PieceBits::King:
			case PieceBits::White | PieceBits::King | PieceBits::CanCastle:
				returnString += 'K'; break;
			case PieceBits::Black | PieceBits::Pawn:
				returnString += 'p'; break;
			case PieceBits::White | PieceBits::Pawn:
				returnString += 'P'; break;
			default: break;
			}
		}

		if (numBlank != 0) {
			returnString += std::to_string(numBlank);
		}

		if (invRank != 7) {
			returnString += '/';
		}
	}

	if (toMove == Side::White) {
		returnString += " w ";
	} else {
		returnString += " b ";
	}

	{
		bool castleBitsPrinted = false;
		if ((board[63] & PieceBits::CanCastle) == PieceBits::CanCastle && (board[60] & PieceBits::CanCastle) == PieceBits::CanCastle) {
			returnString += 'K'; castleBitsPrinted = true;
		}

		if ((board[56] & PieceBits::CanCastle) == PieceBits::CanCastle && (board[60] & PieceBits::CanCastle) == PieceBits::CanCastle) {
			returnString += 'Q'; castleBitsPrinted = true;
		}

		if ((board[7] & PieceBits::CanCastle) == PieceBits::CanCastle && (board[4] & PieceBits::CanCastle) == PieceBits::CanCastle) {
			returnString += 'k'; castleBitsPrinted = true;
		}

		if ((board[0] & PieceBits::CanCastle) == PieceBits::CanCastle && (board[4] & PieceBits::CanCastle) == PieceBits::CanCastle) {
			returnString += 'q'; castleBitsPrinted = true;
		}

		if (!castleBitsPrinted) {
			returnString += '-';
		}
		returnString += ' ';
	}

	//TODO: print enpassant state once we track it.
	returnString += fmt::format("{} {} {}", en_passant_square.transform(indexToSquare).value_or("-"), halfmoveSinceStateAdvance, turn);

	return returnString;
}

void Board::play_lan(std::string_view sv) {
	// TODO: I currently don't handle piece names

	auto startSquare = sv.substr(0, 2);

	// TODO: I also don't handle capture letter

	auto endSquare = sv.substr(2, 2);

	auto startIndex = squareToIndex(startSquare);
	auto endIndex = squareToIndex(endSquare);

	en_passant_square.reset();

	if ((board[startIndex] & PieceBits::PieceMask) == PieceBits::Pawn) {
		halfmoveSinceStateAdvance = 0;

		int rankDiff = std::abs(startSquare[1] - endSquare[1]);
		if (rankDiff == 2) {
			en_passant_square = startIndex;
			if (toMove == Side::White) {
				*en_passant_square -= 8;
			} else {
				*en_passant_square += 8;
			}
		}

	} else {
		halfmoveSinceStateAdvance++;
	}

	board[endIndex] = board[startIndex];
	board[startIndex] = PieceBits::None;

	if (toMove == Side::White) {
		toMove = Side::Black;
	} else {
		toMove = Side::White;
		turn++;
	}
}

void Board::make_move(const Move& move) {
	if (move.isCastleMove) {
		return;
	}
	if (move.isEnpassantMove) {
		return;
	}

	if (move.promotion_piece || (board[move.from_square] & PieceBits::Pawn) || board[move.to_square]) {
		halfmoveSinceStateAdvance = 0;
	} else {
		halfmoveSinceStateAdvance++;
	}

	if (toMove == Side::White) {
		toMove = Side::Black;
	} else {
		toMove = Side::White;
		turn++;
	}

	if (move.promotion_piece) {
		board[move.to_square] = (board[move.from_square] & ~PieceBits::PieceMask) | move.promotion_piece;
	} else {
		board[move.to_square] = board[move.from_square];
	}

	board[move.from_square] = 0;

	en_passant_square = move.en_passant_square;
}

void Board::unmake_move(const Move& move) {
	if (move.isCastleMove) {
		return;
	}
	if (move.isEnpassantMove) {
		return;
	}

	halfmoveSinceStateAdvance = move.prev_half_move_clock;

	if (toMove == Side::White) {
		turn--;
		toMove = Side::Black;
	} else {
		toMove = Side::White;
	}

	if (move.promotion_piece) {
		board[move.from_square] = (board[move.to_square] & ~PieceBits::PieceMask) | PieceBits::Pawn;
	} else {
		board[move.from_square] = board[move.to_square];
	}
	if (move.captured_piece) {
		board[move.to_square] = move.captured_piece;
	} else {
		board[move.to_square] = 0;
	}

	en_passant_square = move.prev_en_passant_square;
}

int Board::squareToIndex(std::string_view sv) {
	//TODO: validation

	return sv[0] - 'a' + ('8' - sv[1]) * 8;
}

std::string Board::indexToSquare(uint8_t idx) {
	std::string returnStr;
	returnStr.push_back('a' + (idx & 0x7));
	returnStr.push_back('8' - ((idx & 0x38) >> 3));

	return returnStr;
}

uint8_t chess::Board::sideToPieceBit(Side side) {
	if (side == Side::Black) {
		return PieceBits::Black;
	} else {
		return PieceBits::White;
	}
}

std::vector<Move> Board::getPseudoLegalMoves() const {
	std::vector<Move> returnMoves;

	auto sideBitand = sideToPieceBit(toMove);

	for (int i = 0; i < 64; i++) {
		if (board[i] & sideBitand) {
			switch (board[i] & PieceBits::PieceMask) {
			// case PieceBits::Pawn:
			// 	calcPawnMoves(returnMoves, i);
			// 	break;
			case PieceBits::Knight:
				calcKnightMoves(returnMoves, i);
				break;
			}
		}
	}

	return returnMoves;
}

void Board::calcPawnMoves(std::vector<Move>& toAppend, uint8_t loc) const {
	
}

void Board::calcKnightMoves(std::vector<Move>& toAppend, uint8_t loc) const {

	bool isLeft2Valid = getFile(loc) >= 2;
	bool isLeft1Valid = getFile(loc) >= 1;
	bool isRight1Valid = getFile(loc) <= 6;
	bool isRight2Valid = getFile(loc) <= 5;

	bool isTop2Valid = getRank(loc) >= 2;
	bool isTop1Valid = getRank(loc) >= 1;
	bool isBot1Valid = getRank(loc) <= 6;
	bool isBot2Valid = getRank(loc) <= 5;

	auto sideBitand = sideToPieceBit(toMove);

	auto calcMove = [&](uint8_t newLoc) {
		if (board[newLoc] != 0) {
			if ((board[newLoc] & PieceBits::SideMask) == sideBitand) {
				return;
			}
		}

		Move m{
			loc,
			newLoc,
			board[newLoc],
			0,
			en_passant_square,
			std::nullopt,
			halfmoveSinceStateAdvance,
			false,
			false
		};
		toAppend.push_back(m);
	};

	if (isTop2Valid && isRight1Valid) {
		calcMove(loc - 15);
	}
	if (isTop1Valid && isRight2Valid) {
		calcMove(loc - 6);
	}
	if (isBot1Valid && isRight2Valid) {
		calcMove(loc + 10);
	}
	if (isBot2Valid && isRight1Valid) {
		calcMove(loc + 17);
	}
	if (isBot2Valid && isLeft1Valid) {
		calcMove(loc + 15);
	}
	if (isBot1Valid && isLeft2Valid) {
		calcMove(loc + 6);
	}
	if (isTop1Valid && isLeft2Valid) {
		calcMove(loc - 10);
	}
	if (isTop2Valid && isLeft1Valid) {
		calcMove(loc - 17);
	}
}

uint64_t Board::perftLevel(int depth) {
	if (depth == 0) {
		return 1;
	}

	auto moves = getPseudoLegalMoves();

	uint64_t sumMoves = 0;

	for (auto& move : moves) {
		make_move(move);
		sumMoves += perftLevel(depth - 1);
		unmake_move(move);
	}

	return sumMoves;
}

uint64_t Board::perft(int depth) {
	Board board = setupStandardBoard();

	return board.perftLevel(depth);
}
