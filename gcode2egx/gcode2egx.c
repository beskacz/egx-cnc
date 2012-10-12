#include "lexer-gcode.h"

int main(){
  int token_n;
  while(1){
    token_n = yylex();
    if (token_n != 0)
      printf("%d\n", token_n);
    else
      break;
  }
}
