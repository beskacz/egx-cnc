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
  LPAREN,
  RPAREN,

  NUM_INTEGER,
  NUM_FLOAT,
  CMD_UNK,
  UNK
};
%}

%option noyywrap

digit       [0-9]
letter      [a-zA-Z]
decimal     \-?([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*)
integer     \-?[0-9]+

%%

A           { return AXIS_A; } /*Rotational axis arround X*/
B           { printf ("%s\n", yytext); return AXIS_B; } /*Rotational axis arround Y*/
C           { printf ("%s\n", yytext); return AXIS_C; } /*Rotational axis arround Z*/
D           { printf ("%s\n", yytext); return RADIAL_OFFSET; }
E           { printf ("%s\n", yytext); return CMD_UNK; } /*'Precision feedrate for threading on lathes'*/
F           { printf ("%s\n", yytext); return FEED_RATE; }
G{digit}+   { printf ("%s\n", yytext); return CMD_PREPARE; }
H           { printf ("%s\n", yytext); return TOOL_LENGTH; } /*Tool length offset (incremental offset over C axis)*/
I           { printf ("%s\n", yytext); return ARC_CENTER_X; } /*For G02 & G03*/
J           { printf ("%s\n", yytext); return ARC_CENTER_Y; } /*  "  */
K           { printf ("%s\n", yytext); return ARC_CENTER_Z; } /*  "  */
L           { printf ("%s\n", yytext); return LOOP_COUNT; }
M{digit}+   { printf ("%s\n", yytext); return CMD_MACHINE; }
N           { printf ("%s\n", yytext); return GOTO_LINE; }
O{digit}+   { printf ("%s\n", yytext); return PROGRAM_NAME; }
P           { printf ("%s\n", yytext); return PARAMETER; }
Q           { printf ("%s\n", yytext); return PECK_INCREMENT; } /*'Peck increment in canned cycles'*/
R           { printf ("%s\n", yytext); return ARC_REDEFINE; }
S           { printf ("%s\n", yytext); return SPEED; } /*G97 -> RPM; G96 -> m/min|sfm; */
T           { printf ("%s\n", yytext); return TOOL_SELECT; } /*See M06*/
U           { printf ("%s\n", yytext); return AXIS_U; }
V           { printf ("%s\n", yytext); return AXIS_V; }
W           { printf ("%s\n", yytext); return AXIS_W; }
X           { printf ("%s\n", yytext); return AXIS_X; }
Y           { printf ("%s\n", yytext); return AXIS_Y; }
Z           { printf ("%s\n", yytext); return AXIS_Z; }
\/.*$       { printf ("%s\n", yytext); return TOOL_NAME; }
%           { printf ("%s\n", yytext); return PERCENT; }
\(          { printf ("%s\n", yytext); return LPAREN; }
\)          { printf ("%s\n", yytext); return RPAREN; }

{decimal}   { printf ("%s\n", yytext); return NUM_FLOAT; }
{integer}   { printf ("%s\n", yytext); return NUM_INTEGER; }

[ \t\n\r]   /*Skip whitespace*/
.           { fprintf(stderr, "Ignored character (%c)\n", yytext[0]); return UNK; }
