%{
enum G_TOKENS {
  RAISE = 1,
  PLUNGE,
  MILL,

  AXIS_X,
  AXIS_Y,
  AXIS_Z,

  NUM_INTEGER,
  NUM_FLOAT,

  UNK
};
%}

%option noyywrap

digit       [0-9]
letter      [a-zA-Z]
decimal     \-?([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*)
integer     \-?[0-9]+

%%

G00         { printf ("%s\n", yytext); return RAISE; }
G01         { printf ("%s\n", yytext); return PLUNGE; }
G04         { printf ("%s\n", yytext); return MILL; }
P0          { }
\(.*        { }
X           { printf ("%s\n", yytext); return AXIS_X; }
Y           { printf ("%s\n", yytext); return AXIS_Y; }
Z           { printf ("%s\n", yytext); return AXIS_Z; }

{decimal}   { printf ("%s\n", yytext); return NUM_FLOAT; }
{integer}   { printf ("%s\n", yytext); return NUM_INTEGER; }

[ \t\n\r]   /*Skip whitespace*/
.           { fprintf(stderr, "!%s\n", yytext); return UNK; }
