#ifndef WRAPPER_H
#define WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

extern int yylex();
extern int yyparse();
extern void yyerror(const char* s);

extern int line_num;
extern int col_num;
extern int error_count;
extern void update_line();
extern void update_col(int length);

#ifdef __cplusplus
}
#endif

#endif