
#pragma once

#include <array>
#include <string>
#include <optional>

namespace chess {

enum class PieceBits : uint8_t {
	None = 0,
	Pawn = 1,
	Rook = 2,
	Knight = 3,
	Bishop = 4,
	Queen = 5,
	King = 6,

	PieceMask = 7,

	White = 8,
	Black = 16,

	SideMask = 24,

	CanCastle = 32,
};

enum class Side {
	Black,
	White
};

constexpr PieceBits operator|(PieceBits lhs, PieceBits rhs) {
	using T = std::underlying_type_t<PieceBits>;
	return static_cast<PieceBits>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

constexpr PieceBits& operator|=(PieceBits& lhs, PieceBits rhs) {
	lhs = lhs | rhs;
	return lhs;
}

constexpr PieceBits operator&(PieceBits lhs, PieceBits rhs) {
	using T = std::underlying_type_t<PieceBits>;
	return static_cast<PieceBits>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

constexpr PieceBits& operator&=(PieceBits& lhs, PieceBits rhs) {
	lhs = lhs & rhs;
	return lhs;
}

class Board {
public:
	std::array<PieceBits, 64> board {};
	Side toMove = Side::White;
	int halfmoveSinceStateAdvance = 0;
	int turn = 1;

	std::string enPassantSquare = "-"; //A complete hack for now, do this properly later.

	static Board setupStandardBoard();

	std::string toFenString() const;

	void play_lan(std::string_view sv);

	static int squareToIndex(std::string_view sv);

};

}
