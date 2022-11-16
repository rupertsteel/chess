
#pragma once

#include <array>
#include <string>
#include <optional>

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

	static int squareToIndex(std::string_view sv);
	static std::string indexToSquare(uint8_t idx);

};

}
