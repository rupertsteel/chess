
#pragma once

#include <array>
#include <string>
#include <optional>
#include <vector>

namespace chess {

namespace PieceBits {
	constexpr uint8_t None = 0;
	constexpr uint8_t Pawn = 1;
	constexpr uint8_t Rook = 2;
	constexpr uint8_t Knight = 3;
	constexpr uint8_t Bishop = 4;
	constexpr uint8_t Queen = 5;
	constexpr uint8_t King = 6;

	constexpr uint8_t PieceMask = 7;

	constexpr uint8_t White = 8;
	constexpr uint8_t Black = 16;

	constexpr uint8_t SideMask = 24;

	constexpr uint8_t CanCastle = 32;


}

enum class Side {
	Black,
	White
};

// Handles a fully reversible move tracking, so we need to go forward and back
// From square (make and unmake)
// To square (make and unmake)
// Captured piece (unmake)
// To promote piece (make and unmake (if this is set, then the unit was a pawn))
//
// makeMove()
//   if isCastleMove do other stuff - do this later
//   if 
//   board[toSquare] = board[fromSquare]
//   board[fromSquare] = 0

struct Move {
	uint8_t from_square;
	uint8_t to_square;
	uint8_t captured_piece;
	uint8_t promotion_piece;
	std::optional<uint8_t> prev_en_passant_square;
	std::optional<uint8_t> en_passant_square;
	int prev_half_move_clock;
	bool isCastleMove;
	bool isEnpassantMove;

	constexpr bool isPromotion() const {
		return !!promotion_piece;
	}
};

class Board {
public:
	std::array<uint8_t, 64> board {};
	Side toMove = Side::White;
	int halfmoveSinceStateAdvance = 0;
	int turn = 1;

	std::optional<uint8_t> en_passant_square;

	static Board setupStandardBoard();

	std::string toFenString() const;

	void play_lan(std::string_view sv);

	// Makes a move
	// This assumes move is a valid move
	void make_move(const Move& move);
	void unmake_move(const Move& move);

	static uint8_t sideToPieceBit(Side side);
	std::vector<Move> getPseudoLegalMoves() const;

	void calcPawnMoves(std::vector<Move>& toAppend, uint8_t loc) const;
	void calcKnightMoves(std::vector<Move>& toAppend, uint8_t loc) const;

	uint64_t perftLevel(int depth);

	static uint64_t perft(int depth);

	static int squareToIndex(std::string_view sv);
	static std::string indexToSquare(uint8_t idx);

	static constexpr uint8_t getRank(uint8_t loc) {
		return loc >> 3;
	}

	static constexpr uint8_t getFile(uint8_t loc) {
		return loc & 0x7;
	}
};

}
