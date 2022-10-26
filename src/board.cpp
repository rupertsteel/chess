
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
	returnString += fmt::format("{} {} {}", enPassantSquare, halfmoveSinceStateAdvance, turn);

	return returnString;
}
