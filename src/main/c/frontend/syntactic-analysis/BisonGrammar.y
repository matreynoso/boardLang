%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

/**
 * The error reporting function for Bison parser.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

/**
 * @see https://www.gnu.org/software/bison/manual/html_node/Location-Default-Action.html
 */
# define YYLLOC_DEFAULT(location, rhs, k) \
	do { \
		if ((k)) { \
			(location).first_column = YYRHSLOC((rhs), 1).first_column; \
			(location).first_line = YYRHSLOC((rhs), 1).first_line; \
			(location).last_column = YYRHSLOC((rhs), (k)).last_column; \
			(location).last_line = YYRHSLOC((rhs), (k)).last_line; \
		} else { \
			(location).first_column = (location).last_column = YYRHSLOC((rhs), 0).last_column; \
			(location).first_line = (location).last_line = YYRHSLOC((rhs), 0).last_line; \
		} \
		(currentSyntacticAnalysisHandler())(); \
	} while (0)

%}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

%union {
	/** Terminals. */

	signed int integer;
	char * string;
	Position * position;
	TokenLabel token;

	/** Non-terminals. */

	Declaration * declaration;
	Distance * distance;
	Game * game;
	GameField * gameField;
	Identifier * identifier;
	MoveStep * moveStep;
	MoveTerm * moveTerm;
	Piece * piece;
	PieceClause * pieceClause;
	Player * player;
	PlayerField * playerField;
	Program * program;
	Region * region;
	Turn * turn;
	TurnStep * turnStep;
	WinCondition * winCondition;

	/** Non-terminals without their own node (flags, modes and options). */

	BlockMode blockMode;
	bool boolean;
	EdgeMode edgeMode;
	PointOfView pointOfView;
	ReplaceMode replaceMode;
	unsigned int flags;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @todo Phase 4: declare one destructor per pointer type of the union
 *	(including "free($$)" for <string>), except for <program>.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

/** Terminals with a semantic value. */
%token <integer> INTEGER "integer"
%token <string> IDENTIFIER "identifier"
%token <position> POSITION "position"

/** Keywords: top-level blocks. */
%token <token> GAME "game"
%token <token> PIECE "piece"
%token <token> PLAYER "player"

/** Keywords: game fields. */
%token <token> BOARD "board"
%token <token> TIMES "x"
%token <token> BLOCKED "blocked"
%token <token> EDGES "edges"
%token <token> BOUNDED "bounded"
%token <token> WRAP "wrap"
%token <token> CYCLE "cycle"
%token <token> TURN "turn"
%token <token> THEN "then"
%token <token> OPTIONAL "optional"
%token <token> SAME "same"
%token <token> MOVE "move"
%token <token> ATTACK "attack"
%token <token> WIN "win"
%token <token> CAPTURE "capture"
%token <token> ALL_PIECES "all"
%token <token> POINTS "points"
%token <token> REACH "reach"
%token <token> GOAL "goal"
%token <token> WITH "with"
%token <token> CUSTOM "custom"

/** Keywords: piece clauses. */
%token <token> MOVES "moves"
%token <token> ATTACKS "attacks"
%token <token> PROMOTE "promote"
%token <token> AT "at"
%token <token> FIRST "first"
%token <token> LAST "last"
%token <token> ROW "row"
%token <token> VALUE "value"
%token <token> HEALTH "health"
%token <token> ROYAL "royal"
%token <token> DAMAGE "damage"
%token <token> CANT "cant"
%token <token> CAN "can"
%token <token> MUST "must"
%token <token> REPLACE "replace"

/** Keywords: move geometry. */
%token <token> UNBLOCKED "unblocked"
%token <token> ONLY "only"
%token <token> INF "inf"
%token <token> FRONT "front"
%token <token> BACK "back"
%token <token> LEFT "left"
%token <token> RIGHT "right"
%token <token> STRAIGHT "straight"
%token <token> DIAGONAL "diagonal"

/** Keywords: player fields. */
%token <token> POV "pov"
%token <token> NORTH "N"
%token <token> SOUTH "S"
%token <token> EAST "E"
%token <token> WEST "W"

/** Symbols. */
%token <token> OPEN_BRACE "{"
%token <token> CLOSE_BRACE "}"
%token <token> COLON ":"
%token <token> COMMA ","
%token <token> AMPERSAND "&"
%token <token> PIPE "|"
%token <token> OPEN_PARENTHESIS "("
%token <token> CLOSE_PARENTHESIS ")"

/** Internal tokens (never used by the grammar rules). */
%token <token> CLOSE_COMMENT "*/"
%token <token> OPEN_COMMENT "/*"
%token <token> EXCEPTION
%token <token> IGNORED
%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

/** @todo Phase 4: replace this placeholder with the boardLang grammar. */
program: %empty													{ $$ = NULL; }
	;

%%
