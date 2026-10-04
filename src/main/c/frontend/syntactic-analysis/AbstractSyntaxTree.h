#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdbool.h>
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * Design notes:
 *
 * - The AST stores what the program says, not what it means. Anything that
 *   was not written is stored as "unspecified" (e.g., REPLACE_UNSPECIFIED,
 *   BLOCK_UNSPECIFIED, hasDamage = false). Defaults are applied later, in the
 *   semantic-analysis phase.
 * - Sets written with "|" are stored unexpanded: orientations, bases and
 *   turn actions as bitmasks, distances as a list.
 * - Repeatable elements (game fields, piece clauses, player fields, etc.) are
 *   stored as singly-linked lists, in source order, through a "next" pointer.
 *   Every destructor of a list element destroys the whole list from that node.
 * - Enumeration constants are prefixed (e.g., POV_NORTH, WIN_CAPTURE) so they
 *   don't collide with the token names that Bison generates (e.g., NORTH,
 *   CAPTURE), because both end up in the same scope.
 */

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum ActionFlag ActionFlag;
typedef enum BaseFlag BaseFlag;
typedef enum BlockMode BlockMode;
typedef enum DeclarationType DeclarationType;
typedef enum DistanceType DistanceType;
typedef enum EdgeMode EdgeMode;
typedef enum GameFieldType GameFieldType;
typedef enum OrientationFlag OrientationFlag;
typedef enum PieceClauseType PieceClauseType;
typedef enum PlayerFieldType PlayerFieldType;
typedef enum PointOfView PointOfView;
typedef enum RegionType RegionType;
typedef enum ReplaceMode ReplaceMode;
typedef enum WinConditionType WinConditionType;

typedef struct Declaration Declaration;
typedef struct Distance Distance;
typedef struct Game Game;
typedef struct GameField GameField;
typedef struct Identifier Identifier;
typedef struct MoveStep MoveStep;
typedef struct MoveTerm MoveTerm;
typedef struct Piece Piece;
typedef struct PieceClause PieceClause;
typedef struct Player Player;
typedef struct PlayerField PlayerField;
typedef struct Position Position;
typedef struct Program Program;
typedef struct Region Region;
typedef struct Turn Turn;
typedef struct TurnStep TurnStep;
typedef struct WinCondition WinCondition;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

/** Bitmask: "move", "attack" or "(move|attack)" in a turn step. */
enum ActionFlag {
	ACTION_MOVE = 1,
	ACTION_ATTACK = 2
};

/** Bitmask: "straight", "diagonal" or a set of both. */
enum BaseFlag {
	BASE_STRAIGHT = 1,
	BASE_DIAGONAL = 2
};

enum BlockMode {
	BLOCK_UNSPECIFIED,
	BLOCK_BLOCKED,
	BLOCK_UNBLOCKED
};

enum DeclarationType {
	PIECE_DECLARATION,
	PLAYER_DECLARATION
};

enum DistanceType {
	FINITE_DISTANCE,
	INFINITE_DISTANCE
};

enum EdgeMode {
	EDGES_BOUNDED,
	EDGES_WRAP
};

enum GameFieldType {
	BOARD_FIELD,
	BLOCKED_FIELD,
	CYCLE_FIELD,
	EDGES_FIELD,
	TURN_FIELD,
	WIN_FIELD
};

/** Bitmask: no bit set means that no orientation was written. */
enum OrientationFlag {
	ORIENTATION_FRONT = 1,
	ORIENTATION_BACK = 2,
	ORIENTATION_LEFT = 4,
	ORIENTATION_RIGHT = 8
};

enum PieceClauseType {
	ATTACKS_CLAUSE,
	HEALTH_CLAUSE,
	MOVES_CLAUSE,
	PROMOTE_CLAUSE,
	ROYAL_CLAUSE,
	VALUE_CLAUSE
};

enum PlayerFieldType {
	GOAL_FIELD,
	PLACEMENT_FIELD,
	POV_FIELD
};

enum PointOfView {
	POV_NORTH,
	POV_SOUTH,
	POV_EAST,
	POV_WEST
};

enum RegionType {
	REGION_FIRST_ROW,
	REGION_LAST_ROW,
	REGION_POSITION
};

enum ReplaceMode {
	REPLACE_UNSPECIFIED,
	REPLACE_CANT,
	REPLACE_CAN,
	REPLACE_MUST
};

enum WinConditionType {
	WIN_CAPTURE,
	WIN_CAPTURE_ALL,
	WIN_CUSTOM,
	WIN_POINTS,
	WIN_REACH_GOAL,
	WIN_REACH_POSITION
};

/** A board square, already decoded by the lexer (e.g., "aa2" = 27:2). */
struct Position {
	signed int column;
	signed int row;
	char * lexeme;
	Position * next;
};

/** A player name inside a "cycle" field. */
struct Identifier {
	char * name;
	Identifier * next;
};

struct Distance {
	DistanceType type;
	signed int value;
	bool only;
	Distance * next;
};

/** One step of a chain (the operands of "&"). */
struct MoveStep {
	BlockMode blockMode;
	Distance * distances;
	unsigned int orientations;
	unsigned int bases;
	MoveStep * next;
};

/**
 * One alternative of a move expression (the operands of ","). Each
 * alternative has its own modifiers.
 */
struct MoveTerm {
	MoveStep * steps;
	/** Only in "moves" clauses. */
	ReplaceMode replaceMode;
	bool firstMove;
	/** Only in "attacks" clauses. */
	bool hasDamage;
	signed int damage;
	MoveTerm * next;
};

struct Region {
	RegionType type;
	Position * position;
};

struct PieceClause {
	PieceClauseType type;
	union {
		/** MOVES_CLAUSE and ATTACKS_CLAUSE. */
		MoveTerm * alternatives;
		/** PROMOTE_CLAUSE. */
		struct {
			char * promotedPiece;
			Region * region;
		};
		/** HEALTH_CLAUSE and VALUE_CLAUSE. */
		signed int amount;
	};
	PieceClause * next;
};

struct Piece {
	char * name;
	PieceClause * clauses;
};

struct PlayerField {
	PlayerFieldType type;
	union {
		/** POV_FIELD. */
		PointOfView pointOfView;
		/** GOAL_FIELD. */
		Position * goal;
		/** PLACEMENT_FIELD. */
		struct {
			char * placedPiece;
			Position * positions;
		};
	};
	PlayerField * next;
};

struct Player {
	char * name;
	PlayerField * fields;
};

struct TurnStep {
	unsigned int actions;
	bool optional;
	TurnStep * next;
};

struct Turn {
	TurnStep * steps;
	bool samePiece;
};

struct WinCondition {
	WinConditionType type;
	/** WIN_CAPTURE and WIN_POINTS. */
	signed int count;
	/** WIN_CAPTURE, and WIN_REACH_* with "with" (NULL otherwise). */
	char * piece;
	/** WIN_REACH_POSITION. */
	Position * position;
};

struct GameField {
	GameFieldType type;
	union {
		/** BOARD_FIELD. */
		struct {
			signed int columns;
			signed int rows;
		};
		/** BLOCKED_FIELD. */
		Position * blocked;
		/** CYCLE_FIELD. */
		Identifier * cycle;
		/** EDGES_FIELD. */
		EdgeMode edgeMode;
		/** TURN_FIELD. */
		Turn * turn;
		/** WIN_FIELD. */
		WinCondition * winCondition;
	};
	GameField * next;
};

struct Game {
	GameField * fields;
};

/** A "piece" or "player" block, in source order. */
struct Declaration {
	DeclarationType type;
	union {
		Piece * piece;
		Player * player;
	};
	Declaration * next;
};

struct Program {
	Game * game;
	Declaration * declarations;
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyDeclaration(Declaration * declaration);
void destroyDistance(Distance * distance);
void destroyGame(Game * game);
void destroyGameField(GameField * gameField);
void destroyIdentifier(Identifier * identifier);
void destroyMoveStep(MoveStep * moveStep);
void destroyMoveTerm(MoveTerm * moveTerm);
void destroyPiece(Piece * piece);
void destroyPieceClause(PieceClause * pieceClause);
void destroyPlayer(Player * player);
void destroyPlayerField(PlayerField * playerField);
void destroyPosition(Position * position);
void destroyProgram(Program * program);
void destroyRegion(Region * region);
void destroyTurn(Turn * turn);
void destroyTurnStep(TurnStep * turnStep);
void destroyWinCondition(WinCondition * winCondition);

#endif
