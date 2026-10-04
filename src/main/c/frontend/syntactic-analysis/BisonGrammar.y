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
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { free($$); } <string>
%destructor { destroyDeclaration($$); } <declaration>
%destructor { destroyDistance($$); } <distance>
%destructor { destroyGame($$); } <game>
%destructor { destroyGameField($$); } <gameField>
%destructor { destroyIdentifier($$); } <identifier>
%destructor { destroyMoveStep($$); } <moveStep>
%destructor { destroyMoveTerm($$); } <moveTerm>
%destructor { destroyPiece($$); } <piece>
%destructor { destroyPieceClause($$); } <pieceClause>
%destructor { destroyPlayer($$); } <player>
%destructor { destroyPlayerField($$); } <playerField>
%destructor { destroyPosition($$); } <position>
%destructor { destroyRegion($$); } <region>
%destructor { destroyTurn($$); } <turn>
%destructor { destroyTurnStep($$); } <turnStep>
%destructor { destroyWinCondition($$); } <winCondition>

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
%type <declaration> declarations declaration
%type <game> game_declaration
%type <gameField> game_fields game_field
%type <edgeMode> edge_mode
%type <identifier> identifier_list
%type <turn> turn
%type <turnStep> turn_steps turn_step
%type <boolean> optional_option same_piece_option first_move_option
%type <flags> action_set action orientation_set orientation_list orientation base_set base_list base
%type <winCondition> win_condition
%type <string> with_option
%type <piece> piece_declaration
%type <pieceClause> piece_clauses piece_clause
%type <replaceMode> replace_mode
%type <region> region
%type <moveTerm> move_alternatives move_term
%type <moveStep> move_steps move_step direction
%type <blockMode> block_mode
%type <distance> distance_set distance_list distance
%type <player> player_declaration
%type <playerField> player_fields player_field
%type <pointOfView> point_of_view
%type <position> position_list

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

/* ---------- Program ---------- */

program: declarations game_declaration declarations							{ $$ = ProgramSemanticAction($1, $2, $3); }
	;

declarations: %empty															{ $$ = NULL; }
	| declaration declarations													{ $$ = LinkDeclarationSemanticAction($1, $2); }
	;

declaration: piece_declaration													{ $$ = PieceDeclarationSemanticAction($1); }
	| player_declaration														{ $$ = PlayerDeclarationSemanticAction($1); }
	;

/* ---------- Game ---------- */

game_declaration: GAME OPEN_BRACE game_fields CLOSE_BRACE						{ $$ = GameSemanticAction($3); }
	;

game_fields: %empty																{ $$ = NULL; }
	| game_field game_fields													{ $$ = LinkGameFieldSemanticAction($1, $2); }
	;

game_field: BOARD COLON INTEGER[columns] TIMES INTEGER[rows]					{ $$ = BoardGameFieldSemanticAction($columns, $rows); }
	| BLOCKED COLON position_list												{ $$ = BlockedGameFieldSemanticAction($3); }
	| CYCLE COLON identifier_list												{ $$ = CycleGameFieldSemanticAction($3); }
	| EDGES COLON edge_mode														{ $$ = EdgesGameFieldSemanticAction($3); }
	| TURN COLON turn															{ $$ = TurnGameFieldSemanticAction($3); }
	| WIN COLON win_condition													{ $$ = WinGameFieldSemanticAction($3); }
	;

edge_mode: BOUNDED																{ $$ = EDGES_BOUNDED; }
	| WRAP																		{ $$ = EDGES_WRAP; }
	;

identifier_list: IDENTIFIER														{ $$ = IdentifierSemanticAction($1); }
	| IDENTIFIER identifier_list												{ $$ = LinkIdentifierSemanticAction(IdentifierSemanticAction($1), $2); }
	;

turn: turn_steps same_piece_option												{ $$ = TurnSemanticAction($1, $2); }
	;

turn_steps: turn_step															{ $$ = $1; }
	| turn_step THEN turn_steps													{ $$ = LinkTurnStepSemanticAction($1, $3); }
	;

turn_step: optional_option action_set											{ $$ = TurnStepSemanticAction($1, $2); }
	;

optional_option: %empty															{ $$ = false; }
	| OPTIONAL																	{ $$ = true; }
	;

same_piece_option: %empty														{ $$ = false; }
	| SAME PIECE																{ $$ = true; }
	;

action_set: action																{ $$ = $1; }
	| OPEN_PARENTHESIS action[left] PIPE action[right] CLOSE_PARENTHESIS		{ $$ = $left | $right; }
	;

action: MOVE																	{ $$ = ACTION_MOVE; }
	| ATTACK																	{ $$ = ACTION_ATTACK; }
	;

win_condition: CAPTURE INTEGER IDENTIFIER										{ $$ = CaptureWinConditionSemanticAction($2, $3); }
	| CAPTURE ALL_PIECES														{ $$ = CaptureAllWinConditionSemanticAction(); }
	| POINTS INTEGER															{ $$ = PointsWinConditionSemanticAction($2); }
	| REACH POSITION with_option												{ $$ = ReachPositionWinConditionSemanticAction($2, $3); }
	| REACH GOAL with_option													{ $$ = ReachGoalWinConditionSemanticAction($3); }
	| CUSTOM																	{ $$ = CustomWinConditionSemanticAction(); }
	;

with_option: %empty																{ $$ = NULL; }
	| WITH IDENTIFIER															{ $$ = $2; }
	;

/* ---------- Pieces ---------- */

piece_declaration: PIECE IDENTIFIER OPEN_BRACE piece_clauses CLOSE_BRACE		{ $$ = PieceSemanticAction($2, $4); }
	;

piece_clauses: %empty															{ $$ = NULL; }
	| piece_clause piece_clauses												{ $$ = LinkPieceClauseSemanticAction($1, $2); }
	;

piece_clause: MOVES COLON move_alternatives replace_mode first_move_option		{ $$ = MovesPieceClauseSemanticAction($3, $4, $5); }
	| ATTACKS COLON move_alternatives											{ $$ = AttacksPieceClauseSemanticAction($3, false, 0); }
	| ATTACKS COLON move_alternatives DAMAGE INTEGER							{ $$ = AttacksPieceClauseSemanticAction($3, true, $5); }
	| PROMOTE COLON IDENTIFIER AT region										{ $$ = PromotePieceClauseSemanticAction($3, $5); }
	| VALUE COLON INTEGER														{ $$ = AmountPieceClauseSemanticAction(VALUE_CLAUSE, $3); }
	| HEALTH COLON INTEGER														{ $$ = AmountPieceClauseSemanticAction(HEALTH_CLAUSE, $3); }
	| ROYAL																		{ $$ = RoyalPieceClauseSemanticAction(); }
	;

replace_mode: %empty															{ $$ = REPLACE_UNSPECIFIED; }
	| CANT REPLACE																{ $$ = REPLACE_CANT; }
	| CAN REPLACE																{ $$ = REPLACE_CAN; }
	| MUST REPLACE																{ $$ = REPLACE_MUST; }
	;

first_move_option: %empty														{ $$ = false; }
	| FIRST MOVE																{ $$ = true; }
	;

region: LAST ROW																{ $$ = RegionSemanticAction(REGION_LAST_ROW, NULL); }
	| FIRST ROW																	{ $$ = RegionSemanticAction(REGION_FIRST_ROW, NULL); }
	| POSITION																	{ $$ = RegionSemanticAction(REGION_POSITION, $1); }
	;

/* ---------- Move geometry: "," (alternatives) > "&" (chain) > "|" (sets) ---------- */

move_alternatives: move_term													{ $$ = $1; }
	| move_term COMMA move_alternatives											{ $$ = LinkMoveTermSemanticAction($1, $3); }
	;

move_term: move_steps															{ $$ = MoveTermSemanticAction($1); }
	;

move_steps: move_step															{ $$ = $1; }
	| move_step AMPERSAND move_steps											{ $$ = LinkMoveStepSemanticAction($1, $3); }
	;

move_step: block_mode distance_set direction									{ $$ = MoveStepSemanticAction($1, $2, $3); }
	;

block_mode: %empty																{ $$ = BLOCK_UNSPECIFIED; }
	| BLOCKED																	{ $$ = BLOCK_BLOCKED; }
	| UNBLOCKED																	{ $$ = BLOCK_UNBLOCKED; }
	;

distance_set: distance															{ $$ = $1; }
	| OPEN_PARENTHESIS distance_list CLOSE_PARENTHESIS							{ $$ = $2; }
	;

distance_list: distance															{ $$ = $1; }
	| distance PIPE distance_list												{ $$ = LinkDistanceSemanticAction($1, $3); }
	;

distance: INTEGER																{ $$ = FiniteDistanceSemanticAction($1, false); }
	| INTEGER ONLY																{ $$ = FiniteDistanceSemanticAction($1, true); }
	| INF																		{ $$ = InfiniteDistanceSemanticAction(); }
	;

/*
 * The orientation set is not an optional non-terminal on purpose: both sets
 * may start with "(", so an empty alternative would force a shift/reduce
 * conflict right after the distance.
 */
direction: base_set																{ $$ = DirectionSemanticAction(0, $1); }
	| orientation_set base_set													{ $$ = DirectionSemanticAction($1, $2); }
	;

orientation_set: orientation													{ $$ = $1; }
	| OPEN_PARENTHESIS orientation_list CLOSE_PARENTHESIS						{ $$ = $2; }
	;

orientation_list: orientation													{ $$ = $1; }
	| orientation_list PIPE orientation											{ $$ = $1 | $3; }
	;

orientation: FRONT																{ $$ = ORIENTATION_FRONT; }
	| BACK																		{ $$ = ORIENTATION_BACK; }
	| LEFT																		{ $$ = ORIENTATION_LEFT; }
	| RIGHT																		{ $$ = ORIENTATION_RIGHT; }
	;

base_set: base																	{ $$ = $1; }
	| OPEN_PARENTHESIS base_list CLOSE_PARENTHESIS								{ $$ = $2; }
	;

base_list: base																	{ $$ = $1; }
	| base_list PIPE base														{ $$ = $1 | $3; }
	;

base: STRAIGHT																	{ $$ = BASE_STRAIGHT; }
	| DIAGONAL																	{ $$ = BASE_DIAGONAL; }
	;

/* ---------- Players ---------- */

player_declaration: PLAYER IDENTIFIER OPEN_BRACE player_fields CLOSE_BRACE		{ $$ = PlayerSemanticAction($2, $4); }
	;

player_fields: %empty															{ $$ = NULL; }
	| player_field player_fields												{ $$ = LinkPlayerFieldSemanticAction($1, $2); }
	;

player_field: POV COLON point_of_view											{ $$ = PointOfViewPlayerFieldSemanticAction($3); }
	| GOAL COLON POSITION														{ $$ = GoalPlayerFieldSemanticAction($3); }
	| IDENTIFIER AT position_list												{ $$ = PlacementPlayerFieldSemanticAction($1, $3); }
	;

point_of_view: NORTH															{ $$ = POV_NORTH; }
	| SOUTH																		{ $$ = POV_SOUTH; }
	| EAST																		{ $$ = POV_EAST; }
	| WEST																		{ $$ = POV_WEST; }
	;

/* ---------- Shared ---------- */

position_list: POSITION															{ $$ = $1; }
	| POSITION COMMA position_list												{ $$ = LinkPositionSemanticAction($1, $3); }
	;

%%
