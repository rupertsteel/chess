
#include <board.hpp>

#include "catch2/catch_test_macros.hpp"

TEST_CASE("Fen string, initial board state", "[board]") {
	auto board = chess::Board::setupStandardBoard();

	REQUIRE(board.toFenString() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
