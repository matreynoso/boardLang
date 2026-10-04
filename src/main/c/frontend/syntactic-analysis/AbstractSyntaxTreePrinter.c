#include "AbstractSyntaxTreePrinter.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static bool _printAbstractSyntaxTree = false;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreePrinterModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTreePrinter...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreePrinterModule() {
	_logger = createLogger("AbstractSyntaxTreePrinter");
	_printAbstractSyntaxTree = getBooleanOrDefault("PRINT_AST", _printAbstractSyntaxTree);
	return _shutdownAbstractSyntaxTreePrinterModule;
}

/* PRIVATE FUNCTIONS */

static const char * _blockModeToString(const BlockMode blockMode);
static const char * _edgeModeToString(const EdgeMode edgeMode);
static const char * _pointOfViewToString(const PointOfView pointOfView);
static const char * _replaceModeToString(const ReplaceMode replaceMode);
static void _printActions(const unsigned int actions);
static void _printBases(const unsigned int bases);
static void _printDeclaration(const unsigned int level, Declaration * declaration);
static void _printDistances(Distance * distance);
static void _printGame(const unsigned int level, Game * game);
static void _printGameField(const unsigned int level, GameField * gameField);
static void _printIndentation(const unsigned int level);
static void _printLine(const unsigned int level, const char * const format, ...);
static void _printMoveTerms(const unsigned int level, const PieceClauseType clauseType, MoveTerm * moveTerm);
static void _printOrientations(const unsigned int orientations);
static void _printPiece(const unsigned int level, Piece * piece);
static void _printPieceClause(const unsigned int level, PieceClause * pieceClause);
static void _printPlayer(const unsigned int level, Player * player);
static void _printPlayerField(const unsigned int level, PlayerField * playerField);
static void _printPositions(Position * position);
static void _printTurn(Turn * turn);
static void _printWinCondition(WinCondition * winCondition);

static const char * _blockModeToString(const BlockMode blockMode) {
	switch (blockMode) {
		case BLOCK_BLOCKED: return "blocked";
		case BLOCK_UNBLOCKED: return "unblocked";
		case BLOCK_UNSPECIFIED: return "unspecified";
		default: return "<unknown>";
	}
}

static const char * _edgeModeToString(const EdgeMode edgeMode) {
	switch (edgeMode) {
		case EDGES_BOUNDED: return "bounded";
		case EDGES_WRAP: return "wrap";
		default: return "<unknown>";
	}
}

static const char * _pointOfViewToString(const PointOfView pointOfView) {
	switch (pointOfView) {
		case POV_EAST: return "E";
		case POV_NORTH: return "N";
		case POV_SOUTH: return "S";
		case POV_WEST: return "W";
		default: return "<unknown>";
	}
}

static const char * _replaceModeToString(const ReplaceMode replaceMode) {
	switch (replaceMode) {
		case REPLACE_CAN: return "can";
		case REPLACE_CANT: return "cant";
		case REPLACE_MUST: return "must";
		case REPLACE_UNSPECIFIED: return "unspecified";
		default: return "<unknown>";
	}
}

/**
 * Prints the actions of a turn step (e.g., "(move|attack)").
 */
static void _printActions(const unsigned int actions) {
	if (actions == (ACTION_MOVE | ACTION_ATTACK)) {
		printf("(move|attack)");
	}
	else if (actions == ACTION_MOVE) {
		printf("move");
	}
	else if (actions == ACTION_ATTACK) {
		printf("attack");
	}
	else {
		printf("<unknown>");
	}
}

/**
 * Prints a set of bases (e.g., "straight|diagonal").
 */
static void _printBases(const unsigned int bases) {
	const char * separator = "";
	if (bases & BASE_STRAIGHT) {
		printf("%sstraight", separator);
		separator = "|";
	}
	if (bases & BASE_DIAGONAL) {
		printf("%sdiagonal", separator);
	}
}

static void _printDeclaration(const unsigned int level, Declaration * declaration) {
	for (; declaration != NULL; declaration = declaration->next) {
		switch (declaration->type) {
			case PIECE_DECLARATION:
				_printPiece(level, declaration->piece);
				break;
			case PLAYER_DECLARATION:
				_printPlayer(level, declaration->player);
				break;
			default:
				_printLine(level, "<unknown declaration>");
				break;
		}
	}
}

/**
 * Prints a set of distances (e.g., "1 | 3 only | inf").
 */
static void _printDistances(Distance * distance) {
	for (; distance != NULL; distance = distance->next) {
		if (distance->type == INFINITE_DISTANCE) {
			printf("inf");
		}
		else {
			printf("%d%s", distance->value, distance->only ? " only" : "");
		}
		if (distance->next != NULL) {
			printf(" | ");
		}
	}
}

static void _printGame(const unsigned int level, Game * game) {
	_printLine(level, "Game");
	if (game != NULL) {
		_printGameField(1 + level, game->fields);
	}
}

static void _printGameField(const unsigned int level, GameField * gameField) {
	for (; gameField != NULL; gameField = gameField->next) {
		_printIndentation(level);
		switch (gameField->type) {
			case BLOCKED_FIELD:
				printf("Blocked: ");
				_printPositions(gameField->blocked);
				break;
			case BOARD_FIELD:
				printf("Board: %d x %d", gameField->columns, gameField->rows);
				break;
			case CYCLE_FIELD:
				printf("Cycle:");
				for (Identifier * identifier = gameField->cycle; identifier != NULL; identifier = identifier->next) {
					printf(" %s", identifier->name);
				}
				break;
			case EDGES_FIELD:
				printf("Edges: %s", _edgeModeToString(gameField->edgeMode));
				break;
			case TURN_FIELD:
				printf("Turn: ");
				_printTurn(gameField->turn);
				break;
			case WIN_FIELD:
				printf("Win: ");
				_printWinCondition(gameField->winCondition);
				break;
			default:
				printf("<unknown game field>");
				break;
		}
		printf("\n");
	}
}

static void _printIndentation(const unsigned int level) {
	for (unsigned int k = 0; k < level; ++k) {
		printf("  ");
	}
}

/**
 * Prints a complete line, indented according to its level in the tree.
 */
static void _printLine(const unsigned int level, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	_printIndentation(level);
	vprintf(format, arguments);
	printf("\n");
	va_end(arguments);
}

/**
 * Prints the alternatives of a move expression, with their own modifiers, and
 * the steps of each one.
 */
static void _printMoveTerms(const unsigned int level, const PieceClauseType clauseType, MoveTerm * moveTerm) {
	for (; moveTerm != NULL; moveTerm = moveTerm->next) {
		if (clauseType == MOVES_CLAUSE) {
			_printLine(level, "Alternative (replace: %s, first move: %s)",
				_replaceModeToString(moveTerm->replaceMode),
				moveTerm->firstMove ? "yes" : "no");
		}
		else if (moveTerm->hasDamage) {
			_printLine(level, "Alternative (damage: %d)", moveTerm->damage);
		}
		else {
			_printLine(level, "Alternative (damage: unspecified)");
		}
		for (MoveStep * moveStep = moveTerm->steps; moveStep != NULL; moveStep = moveStep->next) {
			_printIndentation(1 + level);
			printf("Step: block=%s, distances=", _blockModeToString(moveStep->blockMode));
			_printDistances(moveStep->distances);
			printf(", orientations=");
			_printOrientations(moveStep->orientations);
			printf(", bases=");
			_printBases(moveStep->bases);
			printf("\n");
		}
	}
}

/**
 * Prints a set of orientations (e.g., "front|left"), or "unspecified".
 */
static void _printOrientations(const unsigned int orientations) {
	if (orientations == 0) {
		printf("unspecified");
		return;
	}
	const char * separator = "";
	if (orientations & ORIENTATION_FRONT) {
		printf("%sfront", separator);
		separator = "|";
	}
	if (orientations & ORIENTATION_BACK) {
		printf("%sback", separator);
		separator = "|";
	}
	if (orientations & ORIENTATION_LEFT) {
		printf("%sleft", separator);
		separator = "|";
	}
	if (orientations & ORIENTATION_RIGHT) {
		printf("%sright", separator);
	}
}

static void _printPiece(const unsigned int level, Piece * piece) {
	_printLine(level, "Piece %s", piece->name);
	_printPieceClause(1 + level, piece->clauses);
}

static void _printPieceClause(const unsigned int level, PieceClause * pieceClause) {
	for (; pieceClause != NULL; pieceClause = pieceClause->next) {
		switch (pieceClause->type) {
			case ATTACKS_CLAUSE:
				_printLine(level, "Attacks");
				_printMoveTerms(1 + level, ATTACKS_CLAUSE, pieceClause->alternatives);
				break;
			case HEALTH_CLAUSE:
				_printLine(level, "Health: %d", pieceClause->amount);
				break;
			case MOVES_CLAUSE:
				_printLine(level, "Moves");
				_printMoveTerms(1 + level, MOVES_CLAUSE, pieceClause->alternatives);
				break;
			case PROMOTE_CLAUSE:
				_printIndentation(level);
				printf("Promote: %s at ", pieceClause->promotedPiece);
				switch (pieceClause->region->type) {
					case REGION_FIRST_ROW: printf("first row"); break;
					case REGION_LAST_ROW: printf("last row"); break;
					case REGION_POSITION: _printPositions(pieceClause->region->position); break;
					default: printf("<unknown region>"); break;
				}
				printf("\n");
				break;
			case ROYAL_CLAUSE:
				_printLine(level, "Royal");
				break;
			case VALUE_CLAUSE:
				_printLine(level, "Value: %d", pieceClause->amount);
				break;
			default:
				_printLine(level, "<unknown piece clause>");
				break;
		}
	}
}

static void _printPlayer(const unsigned int level, Player * player) {
	_printLine(level, "Player %s", player->name);
	_printPlayerField(1 + level, player->fields);
}

static void _printPlayerField(const unsigned int level, PlayerField * playerField) {
	for (; playerField != NULL; playerField = playerField->next) {
		_printIndentation(level);
		switch (playerField->type) {
			case GOAL_FIELD:
				printf("Goal: ");
				_printPositions(playerField->goal);
				break;
			case PLACEMENT_FIELD:
				printf("Placement: %s at ", playerField->placedPiece);
				_printPositions(playerField->positions);
				break;
			case POV_FIELD:
				printf("Pov: %s", _pointOfViewToString(playerField->pointOfView));
				break;
			default:
				printf("<unknown player field>");
				break;
		}
		printf("\n");
	}
}

/**
 * Prints a list of positions with their decoded coordinates, as
 * "lexeme (column:row)".
 */
static void _printPositions(Position * position) {
	for (; position != NULL; position = position->next) {
		printf("%s (%d:%d)%s", position->lexeme, position->column, position->row, position->next != NULL ? ", " : "");
	}
}

static void _printTurn(Turn * turn) {
	for (TurnStep * turnStep = turn->steps; turnStep != NULL; turnStep = turnStep->next) {
		printf("%s", turnStep->optional ? "optional " : "");
		_printActions(turnStep->actions);
		printf("%s", turnStep->next != NULL ? " then " : "");
	}
	if (turn->samePiece) {
		printf(", same piece");
	}
}

static void _printWinCondition(WinCondition * winCondition) {
	switch (winCondition->type) {
		case WIN_CAPTURE:
			printf("capture %d %s", winCondition->count, winCondition->piece);
			break;
		case WIN_CAPTURE_ALL:
			printf("capture all");
			break;
		case WIN_CUSTOM:
			printf("custom");
			break;
		case WIN_POINTS:
			printf("points %d", winCondition->count);
			break;
		case WIN_REACH_GOAL:
			printf("reach goal");
			break;
		case WIN_REACH_POSITION:
			printf("reach ");
			_printPositions(winCondition->position);
			break;
		default:
			printf("<unknown win condition>");
			return;
	}
	if (winCondition->piece != NULL && winCondition->type != WIN_CAPTURE) {
		printf(" with %s", winCondition->piece);
	}
}

/* PUBLIC FUNCTIONS */

void printAbstractSyntaxTree(Program * program) {
	if (!_printAbstractSyntaxTree || program == NULL) {
		return;
	}
	logDebugging(_logger, "Printing the AST...");
	_printLine(0, "Program");
	_printGame(1, program->game);
	_printDeclaration(1, program->declarations);
	fflush(stdout);
}
