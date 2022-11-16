
#include <board.hpp>

#include "catch2/catch_test_macros.hpp"

TEST_CASE("Fen string, initial board state", "[board]") {
	auto board = chess::Board::setupStandardBoard();

	REQUIRE(board.toFenString() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("Fen string, early game", "[board]") {
	auto board = chess::Board::setupStandardBoard();

	// move pawn from e2 to e4
	// this is currently manual as I haven;t made move functions.

	board.play_lan("e2e4");

	CHECK(board.toFenString() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

	board.play_lan("c7c5");

	CHECK(board.toFenString() == "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

	board.play_lan("g1f3");

	CHECK(board.toFenString() == "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");

}

TEST_CASE("Square to index", "[board][index]") {
	CHECK(chess::Board::squareToIndex("a8") == 0);
	CHECK(chess::Board::squareToIndex("h8") == 7);
	CHECK(chess::Board::squareToIndex("a1") == 56);
	CHECK(chess::Board::squareToIndex("h1") == 63);
}

TEST_CASE("Index to square and back", "[board][index]") {
	for (int index = 0; index < 64; index++) {
		auto square = chess::Board::indexToSquare(index);
		CAPTURE(square);
		auto twiceSquare = chess::Board::indexToSquare(chess::Board::squareToIndex(square));
		CAPTURE(twiceSquare);
		CHECK(index == chess::Board::squareToIndex(square));
	}
}
