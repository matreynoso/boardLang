%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/Handler.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Misc-Macros.html
 */
#define YY_USER_ACTION (currentLexicalAnalysisHandler())();

extern Handler currentLexicalAnalysisHandler();

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus LeaveMultilineCommentLexemeAction();
extern CompilationStatus PositionLexemeAction();
extern CompilationStatus SymbolLexemeAction(TokenLabel label);
extern CompilationStatus UnknownLexemeAction();

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
identifier			[A-Za-z_][A-Za-z0-9_]*
integer				[[:digit:]]+
position			[a-z]{1,2}[[:digit:]]+
whitespace			[[:space:]]+

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"/*"								{ return EnterMultilineCommentLexemeAction(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"				{ return LeaveMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>[^*]+			{ return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>.				{ return IgnoredLexemeAction(); }

"//"[^\n]*							{ return IgnoredLexemeAction(); }

"game"								{ return KeywordLexemeAction(GAME); }
"piece"								{ return KeywordLexemeAction(PIECE); }
"player"							{ return KeywordLexemeAction(PLAYER); }
"board"								{ return KeywordLexemeAction(BOARD); }
"x"									{ return KeywordLexemeAction(TIMES); }
"blocked"							{ return KeywordLexemeAction(BLOCKED); }
"edges"								{ return KeywordLexemeAction(EDGES); }
"bounded"							{ return KeywordLexemeAction(BOUNDED); }
"wrap"								{ return KeywordLexemeAction(WRAP); }
"cycle"								{ return KeywordLexemeAction(CYCLE); }
"turn"								{ return KeywordLexemeAction(TURN); }
"then"								{ return KeywordLexemeAction(THEN); }
"optional"							{ return KeywordLexemeAction(OPTIONAL); }
"same"								{ return KeywordLexemeAction(SAME); }
"move"								{ return KeywordLexemeAction(MOVE); }
"attack"							{ return KeywordLexemeAction(ATTACK); }
"win"								{ return KeywordLexemeAction(WIN); }
"capture"							{ return KeywordLexemeAction(CAPTURE); }
"all"								{ return KeywordLexemeAction(ALL_PIECES); }
"points"							{ return KeywordLexemeAction(POINTS); }
"reach"								{ return KeywordLexemeAction(REACH); }
"goal"								{ return KeywordLexemeAction(GOAL); }
"with"								{ return KeywordLexemeAction(WITH); }
"custom"							{ return KeywordLexemeAction(CUSTOM); }
"moves"								{ return KeywordLexemeAction(MOVES); }
"attacks"							{ return KeywordLexemeAction(ATTACKS); }
"promote"							{ return KeywordLexemeAction(PROMOTE); }
"at"								{ return KeywordLexemeAction(AT); }
"first"								{ return KeywordLexemeAction(FIRST); }
"last"								{ return KeywordLexemeAction(LAST); }
"row"								{ return KeywordLexemeAction(ROW); }
"value"								{ return KeywordLexemeAction(VALUE); }
"health"							{ return KeywordLexemeAction(HEALTH); }
"royal"								{ return KeywordLexemeAction(ROYAL); }
"damage"							{ return KeywordLexemeAction(DAMAGE); }
"cant"								{ return KeywordLexemeAction(CANT); }
"can"								{ return KeywordLexemeAction(CAN); }
"must"								{ return KeywordLexemeAction(MUST); }
"replace"							{ return KeywordLexemeAction(REPLACE); }
"unblocked"							{ return KeywordLexemeAction(UNBLOCKED); }
"only"								{ return KeywordLexemeAction(ONLY); }
"inf"								{ return KeywordLexemeAction(INF); }
"front"								{ return KeywordLexemeAction(FRONT); }
"back"								{ return KeywordLexemeAction(BACK); }
"left"								{ return KeywordLexemeAction(LEFT); }
"right"								{ return KeywordLexemeAction(RIGHT); }
"straight"							{ return KeywordLexemeAction(STRAIGHT); }
"diagonal"							{ return KeywordLexemeAction(DIAGONAL); }
"pov"								{ return KeywordLexemeAction(POV); }
"N"									{ return KeywordLexemeAction(NORTH); }
"S"									{ return KeywordLexemeAction(SOUTH); }
"E"									{ return KeywordLexemeAction(EAST); }
"W"									{ return KeywordLexemeAction(WEST); }

{position}							{ return PositionLexemeAction(); }
{identifier}						{ return IdentifierLexemeAction(); }
{integer}							{ return IntegerLexemeAction(); }

"{"									{ return SymbolLexemeAction(OPEN_BRACE); }
"}"									{ return SymbolLexemeAction(CLOSE_BRACE); }
":"									{ return SymbolLexemeAction(COLON); }
","									{ return SymbolLexemeAction(COMMA); }
"&"									{ return SymbolLexemeAction(AMPERSAND); }
"|"									{ return SymbolLexemeAction(PIPE); }
"("									{ return SymbolLexemeAction(OPEN_PARENTHESIS); }
")"									{ return SymbolLexemeAction(CLOSE_PARENTHESIS); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
