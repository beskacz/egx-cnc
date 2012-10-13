#include "lex.yy.c"

int main(){
  int token_id;
  int retv = 0;
  while(1){
	  token_id = yylex();
	  if (token_id == UNK)
		  retv = -1;
          if (token_id == 0)
                  break;
  }
}

