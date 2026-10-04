#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

/**
 * Every destructor of a list element (i.e., a node with a "next" pointer)
 * destroys the whole list, starting from that node.
 */

void destroyDeclaration(Declaration * declaration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (declaration != NULL) {
		switch (declaration->type) {
			case PIECE_DECLARATION:
				destroyPiece(declaration->piece);
				break;
			case PLAYER_DECLARATION:
				destroyPlayer(declaration->player);
				break;
			default:
				logError(_logger, "The specified declaration type is unknown: %d", declaration->type);
				break;
		}
		destroyDeclaration(declaration->next);
		free(declaration);
	}
}

void destroyDistance(Distance * distance) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (distance != NULL) {
		destroyDistance(distance->next);
		free(distance);
	}
}

void destroyGame(Game * game) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (game != NULL) {
		destroyGameField(game->fields);
		free(game);
	}
}

void destroyGameField(GameField * gameField) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (gameField != NULL) {
		switch (gameField->type) {
			case BOARD_FIELD:
			case EDGES_FIELD:
				break;
			case BLOCKED_FIELD:
				destroyPosition(gameField->blocked);
				break;
			case CYCLE_FIELD:
				destroyIdentifier(gameField->cycle);
				break;
			case TURN_FIELD:
				destroyTurn(gameField->turn);
				break;
			case WIN_FIELD:
				destroyWinCondition(gameField->winCondition);
				break;
			default:
				logError(_logger, "The specified game-field type is unknown: %d", gameField->type);
				break;
		}
		destroyGameField(gameField->next);
		free(gameField);
	}
}

void destroyIdentifier(Identifier * identifier) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (identifier != NULL) {
		free(identifier->name);
		destroyIdentifier(identifier->next);
		free(identifier);
	}
}

void destroyMoveStep(MoveStep * moveStep) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (moveStep != NULL) {
		destroyDistance(moveStep->distances);
		destroyMoveStep(moveStep->next);
		free(moveStep);
	}
}

void destroyMoveTerm(MoveTerm * moveTerm) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (moveTerm != NULL) {
		destroyMoveStep(moveTerm->steps);
		destroyMoveTerm(moveTerm->next);
		free(moveTerm);
	}
}

void destroyPiece(Piece * piece) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (piece != NULL) {
		free(piece->name);
		destroyPieceClause(piece->clauses);
		free(piece);
	}
}

void destroyPieceClause(PieceClause * pieceClause) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (pieceClause != NULL) {
		switch (pieceClause->type) {
			case ATTACKS_CLAUSE:
				destroyMoveTerm(pieceClause->attackAlternatives);
				break;
			case HEALTH_CLAUSE:
			case ROYAL_CLAUSE:
			case VALUE_CLAUSE:
				break;
			case MOVES_CLAUSE:
				destroyMoveTerm(pieceClause->moveAlternatives);
				break;
			case PROMOTE_CLAUSE:
				free(pieceClause->promotedPiece);
				destroyRegion(pieceClause->region);
				break;
			default:
				logError(_logger, "The specified piece-clause type is unknown: %d", pieceClause->type);
				break;
		}
		destroyPieceClause(pieceClause->next);
		free(pieceClause);
	}
}

void destroyPlayer(Player * player) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (player != NULL) {
		free(player->name);
		destroyPlayerField(player->fields);
		free(player);
	}
}

void destroyPlayerField(PlayerField * playerField) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (playerField != NULL) {
		switch (playerField->type) {
			case GOAL_FIELD:
				destroyPosition(playerField->goal);
				break;
			case PLACEMENT_FIELD:
				free(playerField->placedPiece);
				destroyPosition(playerField->positions);
				break;
			case POV_FIELD:
				break;
			default:
				logError(_logger, "The specified player-field type is unknown: %d", playerField->type);
				break;
		}
		destroyPlayerField(playerField->next);
		free(playerField);
	}
}

void destroyPosition(Position * position) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (position != NULL) {
		free(position->lexeme);
		destroyPosition(position->next);
		free(position);
	}
}

void destroyProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		destroyGame(program->game);
		destroyDeclaration(program->declarations);
		free(program);
	}
}

void destroyRegion(Region * region) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (region != NULL) {
		destroyPosition(region->position);
		free(region);
	}
}

void destroyTurn(Turn * turn) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (turn != NULL) {
		destroyTurnStep(turn->steps);
		free(turn);
	}
}

void destroyTurnStep(TurnStep * turnStep) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (turnStep != NULL) {
		destroyTurnStep(turnStep->next);
		free(turnStep);
	}
}

void destroyWinCondition(WinCondition * winCondition) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (winCondition != NULL) {
		free(winCondition->piece);
		destroyPosition(winCondition->position);
		free(winCondition);
	}
}
