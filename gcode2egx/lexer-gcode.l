%{
enum G_TOKENS {
  AXIS_A = 1,
  AXIS_B,
  AXIS_C,
  RADIAL_OFFSET,
  FEED_RATE,
  CMD_PREPARE,
  TOOL_LENGTH,
  ARC_CENTER_X,
  ARC_CENTER_Y,
  ARC_CENTER_Z,
  LOOP_COUNT,
  CMD_MACHINE,
  GOTO_LINE,
  PROGRAM_NAME,
  PARAMETER,
  PECK_INCREMENT,
  ARC_REDEFINE,
  SPEED,
  TOOL_SELECT,
  TOOL_NAME,
  AXIS_U,
  AXIS_V,
  AXIS_W,
  AXIS_X,
  AXIS_Y,
  AXIS_Z,

  PERCENT,
  PERIOD,

  NUM_INTEGER,
  NUM_FLOAT,
  CMD_UNK,
  UNK
};
%}


%option outfile="lexer-gcode.c" header-file="lexer-gcode.h"
%option noyywrap

digit       [0-9]
letter      [a-zA-Z]
decimal     \-?([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*)
integer     \-?[0-9]+

%%

A           { return AXIS_A; } /*Rotational axis arround X*/
B           { return AXIS_B; } /*Rotational axis arround Y*/
C           { return AXIS_C; } /*Rotational axis arround Z*/
D           { return RADIAL_OFFSET; }
E           { return CMD_UNK; } /*'Precision feedrate for threading on lathes'*/
F           { return FEED_RATE; }
G{digit}+   { return CMD_PREPARE; }
H           { return TOOL_LENGTH; } /*Tool length offset (incremental offset over C axis)*/
I           { return ARC_CENTER_X; } /*For G02 & G03*/
J           { return ARC_CENTER_Y; } /*  "  */
K           { return ARC_CENTER_Z; } /*  "  */
L           { return LOOP_COUNT; }
M{digit}+   { return CMD_MACHINE; }
N           { return GOTO_LINE; }
O{digit}+   { return PROGRAM_NAME; }
P           { return PARAMETER; }
Q           { return PECK_INCREMENT; } /*'Peck increment in canned cycles'*/
R           { return ARC_REDEFINE; }
S           { return SPEED; } /*G97 -> RPM; G96 -> m/min|sfm; */
T           { return TOOL_SELECT; } /*See M06*/
U           { return AXIS_U; }
V           { return AXIS_V; }
W           { return AXIS_W; }
X           { return AXIS_X; }
Y           { return AXIS_Y; }
Z           { return AXIS_Z; }
\/.*$       { return TOOL_NAME; }
%           { return PERCENT; }

{decimal}   { return NUM_FLOAT; }
{integer}   { return NUM_INTEGER; }

[ \t\n\r]   /*Skip whitespace*/
.           { printf("Ignored character (%c)\n", yytext[0]); return UNK; }
