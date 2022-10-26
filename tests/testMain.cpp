
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

	board.board[36] = board.board[52];
	board.board[52] = chess::PieceBits::None;
	board.toMove = chess::Side::Black;
	board.enPassantSquare = "e3";

	CHECK(board.toFenString() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

	board.board[26] = board.board[10];
	board.board[10] = chess::PieceBits::None;
	board.toMove = chess::Side::White;
	board.turn++;
	board.enPassantSquare = "c6";

	CHECK(board.toFenString() == "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

	board.board[45] = board.board[62];
	board.board[62] = chess::PieceBits::None;
	board.toMove = chess::Side::Black;
	board.halfmoveSinceStateAdvance++;
	board.enPassantSquare = "-";

	CHECK(board.toFenString() == "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}
