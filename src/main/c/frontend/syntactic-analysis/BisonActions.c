#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** @todo: Override this with your own implementation. */
static void _defaultHandler() {
	// ...
}

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	onSyntacticAnalysisAction(_defaultHandler);
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static GameField * _createGameField(const GameFieldType type);
static PieceClause * _createPieceClause(const PieceClauseType type);
static PlayerField * _createPlayerField(const PlayerFieldType type);
static WinCondition * _createWinCondition(const WinConditionType type);
static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Creates an empty game field of the specified type.
 */
static GameField * _createGameField(const GameFieldType type) {
	GameField * gameField = calloc(1, sizeof(GameField));
	gameField->type = type;
	return gameField;
}

/**
 * Creates an empty piece clause of the specified type.
 */
static PieceClause * _createPieceClause(const PieceClauseType type) {
	PieceClause * pieceClause = calloc(1, sizeof(PieceClause));
	pieceClause->type = type;
	return pieceClause;
}

/**
 * Creates an empty player field of the specified type.
 */
static PlayerField * _createPlayerField(const PlayerFieldType type) {
	PlayerField * playerField = calloc(1, sizeof(PlayerField));
	playerField->type = type;
	return playerField;
}

/**
 * Creates an empty win condition of the specified type.
 */
static WinCondition * _createWinCondition(const WinConditionType type) {
	WinCondition * winCondition = calloc(1, sizeof(WinCondition));
	winCondition->type = type;
	return winCondition;
}

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

/** Program and top-level declarations. */

Program * ProgramSemanticAction(Declaration * declarationsBeforeGame, Game * game, Declaration * declarationsAfterGame) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->game = game;
	if (declarationsBeforeGame == NULL) {
		program->declarations = declarationsAfterGame;
	}
	else {
		Declaration * last = declarationsBeforeGame;
		while (last->next != NULL) {
			last = last->next;
		}
		last->next = declarationsAfterGame;
		program->declarations = declarationsBeforeGame;
	}
	_compilerState->abstractSyntaxtTree = program;
	return program;
}

Declaration * PieceDeclarationSemanticAction(Piece * piece) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration * declaration = calloc(1, sizeof(Declaration));
	declaration->type = PIECE_DECLARATION;
	declaration->piece = piece;
	return declaration;
}

Declaration * PlayerDeclarationSemanticAction(Player * player) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration * declaration = calloc(1, sizeof(Declaration));
	declaration->type = PLAYER_DECLARATION;
	declaration->player = player;
	return declaration;
}

/** Game. */

Game * GameSemanticAction(GameField * fields) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Game * game = calloc(1, sizeof(Game));
	game->fields = fields;
	return game;
}

GameField * BoardGameFieldSemanticAction(const signed int columns, const signed int rows) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(BOARD_FIELD);
	gameField->columns = columns;
	gameField->rows = rows;
	return gameField;
}

GameField * BlockedGameFieldSemanticAction(Position * positions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(BLOCKED_FIELD);
	gameField->blocked = positions;
	return gameField;
}

GameField * CycleGameFieldSemanticAction(Identifier * players) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(CYCLE_FIELD);
	gameField->cycle = players;
	return gameField;
}

GameField * EdgesGameFieldSemanticAction(const EdgeMode edgeMode) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(EDGES_FIELD);
	gameField->edgeMode = edgeMode;
	return gameField;
}

GameField * TurnGameFieldSemanticAction(Turn * turn) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(TURN_FIELD);
	gameField->turn = turn;
	return gameField;
}

GameField * WinGameFieldSemanticAction(WinCondition * winCondition) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	GameField * gameField = _createGameField(WIN_FIELD);
	gameField->winCondition = winCondition;
	return gameField;
}

Identifier * IdentifierSemanticAction(char * name) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Identifier * identifier = calloc(1, sizeof(Identifier));
	identifier->name = name;
	return identifier;
}

Turn * TurnSemanticAction(TurnStep * steps, const bool samePiece) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Turn * turn = calloc(1, sizeof(Turn));
	turn->steps = steps;
	turn->samePiece = samePiece;
	return turn;
}

TurnStep * TurnStepSemanticAction(const bool optional, const unsigned int actions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TurnStep * turnStep = calloc(1, sizeof(TurnStep));
	turnStep->optional = optional;
	turnStep->actions = actions;
	return turnStep;
}

WinCondition * CaptureAllWinConditionSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return _createWinCondition(WIN_CAPTURE_ALL);
}

WinCondition * CaptureWinConditionSemanticAction(const signed int count, char * piece) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WinCondition * winCondition = _createWinCondition(WIN_CAPTURE);
	winCondition->count = count;
	winCondition->piece = piece;
	return winCondition;
}

WinCondition * CustomWinConditionSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return _createWinCondition(WIN_CUSTOM);
}

WinCondition * PointsWinConditionSemanticAction(const signed int count) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WinCondition * winCondition = _createWinCondition(WIN_POINTS);
	winCondition->count = count;
	return winCondition;
}

WinCondition * ReachGoalWinConditionSemanticAction(char * piece) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WinCondition * winCondition = _createWinCondition(WIN_REACH_GOAL);
	winCondition->piece = piece;
	return winCondition;
}

WinCondition * ReachPositionWinConditionSemanticAction(Position * position, char * piece) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WinCondition * winCondition = _createWinCondition(WIN_REACH_POSITION);
	winCondition->position = position;
	winCondition->piece = piece;
	return winCondition;
}

/** Pieces and move geometry. */

Piece * PieceSemanticAction(char * name, PieceClause * clauses) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Piece * piece = calloc(1, sizeof(Piece));
	piece->name = name;
	piece->clauses = clauses;
	return piece;
}

PieceClause * AmountPieceClauseSemanticAction(const PieceClauseType type, const signed int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PieceClause * pieceClause = _createPieceClause(type);
	pieceClause->amount = amount;
	return pieceClause;
}

PieceClause * AttacksPieceClauseSemanticAction(MoveTerm * alternatives, const bool hasDamage, const signed int damage) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PieceClause * pieceClause = _createPieceClause(ATTACKS_CLAUSE);
	pieceClause->attackAlternatives = alternatives;
	pieceClause->hasDamage = hasDamage;
	pieceClause->damage = damage;
	return pieceClause;
}

PieceClause * MovesPieceClauseSemanticAction(MoveTerm * alternatives, const ReplaceMode replaceMode, const bool firstMove) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PieceClause * pieceClause = _createPieceClause(MOVES_CLAUSE);
	pieceClause->moveAlternatives = alternatives;
	pieceClause->replaceMode = replaceMode;
	pieceClause->firstMove = firstMove;
	return pieceClause;
}

PieceClause * PromotePieceClauseSemanticAction(char * promotedPiece, Region * region) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PieceClause * pieceClause = _createPieceClause(PROMOTE_CLAUSE);
	pieceClause->promotedPiece = promotedPiece;
	pieceClause->region = region;
	return pieceClause;
}

PieceClause * RoyalPieceClauseSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	return _createPieceClause(ROYAL_CLAUSE);
}

Region * RegionSemanticAction(const RegionType type, Position * position) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Region * region = calloc(1, sizeof(Region));
	region->type = type;
	region->position = position;
	return region;
}

MoveTerm * MoveTermSemanticAction(MoveStep * steps) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MoveTerm * moveTerm = calloc(1, sizeof(MoveTerm));
	moveTerm->steps = steps;
	return moveTerm;
}

MoveStep * DirectionSemanticAction(const unsigned int orientations, const unsigned int bases) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	MoveStep * moveStep = calloc(1, sizeof(MoveStep));
	moveStep->orientations = orientations;
	moveStep->bases = bases;
	return moveStep;
}

MoveStep * MoveStepSemanticAction(const BlockMode blockMode, Distance * distances, MoveStep * direction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	direction->blockMode = blockMode;
	direction->distances = distances;
	return direction;
}

Distance * FiniteDistanceSemanticAction(const signed int value, const bool only) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Distance * distance = calloc(1, sizeof(Distance));
	distance->type = FINITE_DISTANCE;
	distance->value = value;
	distance->only = only;
	return distance;
}

Distance * InfiniteDistanceSemanticAction() {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Distance * distance = calloc(1, sizeof(Distance));
	distance->type = INFINITE_DISTANCE;
	return distance;
}

/** Players. */

Player * PlayerSemanticAction(char * name, PlayerField * fields) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Player * player = calloc(1, sizeof(Player));
	player->name = name;
	player->fields = fields;
	return player;
}

PlayerField * GoalPlayerFieldSemanticAction(Position * goal) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PlayerField * playerField = _createPlayerField(GOAL_FIELD);
	playerField->goal = goal;
	return playerField;
}

PlayerField * PlacementPlayerFieldSemanticAction(char * placedPiece, Position * positions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PlayerField * playerField = _createPlayerField(PLACEMENT_FIELD);
	playerField->placedPiece = placedPiece;
	playerField->positions = positions;
	return playerField;
}

PlayerField * PointOfViewPlayerFieldSemanticAction(const PointOfView pointOfView) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PlayerField * playerField = _createPlayerField(POV_FIELD);
	playerField->pointOfView = pointOfView;
	return playerField;
}

/** Lists. */

Declaration * LinkDeclarationSemanticAction(Declaration * declaration, Declaration * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	declaration->next = next;
	return declaration;
}

Distance * LinkDistanceSemanticAction(Distance * distance, Distance * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	distance->next = next;
	return distance;
}

GameField * LinkGameFieldSemanticAction(GameField * gameField, GameField * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	gameField->next = next;
	return gameField;
}

Identifier * LinkIdentifierSemanticAction(Identifier * identifier, Identifier * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	identifier->next = next;
	return identifier;
}

MoveStep * LinkMoveStepSemanticAction(MoveStep * moveStep, MoveStep * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	moveStep->next = next;
	return moveStep;
}

MoveTerm * LinkMoveTermSemanticAction(MoveTerm * moveTerm, MoveTerm * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	moveTerm->next = next;
	return moveTerm;
}

PieceClause * LinkPieceClauseSemanticAction(PieceClause * pieceClause, PieceClause * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	pieceClause->next = next;
	return pieceClause;
}

PlayerField * LinkPlayerFieldSemanticAction(PlayerField * playerField, PlayerField * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	playerField->next = next;
	return playerField;
}

Position * LinkPositionSemanticAction(Position * position, Position * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	position->next = next;
	return position;
}

TurnStep * LinkTurnStepSemanticAction(TurnStep * turnStep, TurnStep * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	turnStep->next = next;
	return turnStep;
}
