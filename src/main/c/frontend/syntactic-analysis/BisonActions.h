#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "../Frontend.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdbool.h>
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule();

/**
 * Bison semantic actions.
 */

/** Program and top-level declarations. */

Program * ProgramSemanticAction(Declaration * declarationsBeforeGame, Game * game, Declaration * declarationsAfterGame);
Declaration * PieceDeclarationSemanticAction(Piece * piece);
Declaration * PlayerDeclarationSemanticAction(Player * player);

/** Game. */

Game * GameSemanticAction(GameField * fields);
GameField * BoardGameFieldSemanticAction(const signed int columns, const signed int rows);
GameField * BlockedGameFieldSemanticAction(Position * positions);
GameField * CycleGameFieldSemanticAction(Identifier * players);
GameField * EdgesGameFieldSemanticAction(const EdgeMode edgeMode);
GameField * TurnGameFieldSemanticAction(Turn * turn);
GameField * WinGameFieldSemanticAction(WinCondition * winCondition);
Identifier * IdentifierSemanticAction(char * name);
Turn * TurnSemanticAction(TurnStep * steps, const bool samePiece);
TurnStep * TurnStepSemanticAction(const bool optional, const unsigned int actions);
WinCondition * CaptureAllWinConditionSemanticAction();
WinCondition * CaptureWinConditionSemanticAction(const signed int count, char * piece);
WinCondition * CustomWinConditionSemanticAction();
WinCondition * PointsWinConditionSemanticAction(const signed int count);
WinCondition * ReachGoalWinConditionSemanticAction(char * piece);
WinCondition * ReachPositionWinConditionSemanticAction(Position * position, char * piece);

/** Pieces and move geometry. */

Piece * PieceSemanticAction(char * name, PieceClause * clauses);
PieceClause * AmountPieceClauseSemanticAction(const PieceClauseType type, const signed int amount);
PieceClause * AttacksPieceClauseSemanticAction(MoveTerm * alternatives, const bool hasDamage, const signed int damage);
PieceClause * MovesPieceClauseSemanticAction(MoveTerm * alternatives, const ReplaceMode replaceMode, const bool firstMove);
PieceClause * PromotePieceClauseSemanticAction(char * promotedPiece, Region * region);
PieceClause * RoyalPieceClauseSemanticAction();
Region * RegionSemanticAction(const RegionType type, Position * position);
MoveTerm * MoveTermSemanticAction(MoveStep * steps);
MoveStep * DirectionSemanticAction(const unsigned int orientations, const unsigned int bases);
MoveStep * MoveStepSemanticAction(const BlockMode blockMode, Distance * distances, MoveStep * direction);
Distance * FiniteDistanceSemanticAction(const signed int value, const bool only);
Distance * InfiniteDistanceSemanticAction();

/** Players. */

Player * PlayerSemanticAction(char * name, PlayerField * fields);
PlayerField * GoalPlayerFieldSemanticAction(Position * goal);
PlayerField * PlacementPlayerFieldSemanticAction(char * placedPiece, Position * positions);
PlayerField * PointOfViewPlayerFieldSemanticAction(const PointOfView pointOfView);

/** Lists: each action links an element to the rest of its list (right recursion). */

Declaration * LinkDeclarationSemanticAction(Declaration * declaration, Declaration * next);
Distance * LinkDistanceSemanticAction(Distance * distance, Distance * next);
GameField * LinkGameFieldSemanticAction(GameField * gameField, GameField * next);
Identifier * LinkIdentifierSemanticAction(Identifier * identifier, Identifier * next);
MoveStep * LinkMoveStepSemanticAction(MoveStep * moveStep, MoveStep * next);
MoveTerm * LinkMoveTermSemanticAction(MoveTerm * moveTerm, MoveTerm * next);
PieceClause * LinkPieceClauseSemanticAction(PieceClause * pieceClause, PieceClause * next);
PlayerField * LinkPlayerFieldSemanticAction(PlayerField * playerField, PlayerField * next);
Position * LinkPositionSemanticAction(Position * position, Position * next);
TurnStep * LinkTurnStepSemanticAction(TurnStep * turnStep, TurnStep * next);

#endif
