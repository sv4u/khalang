// Khalang

#include "mpc.h"

static char buffer[2048];

char *readline (char *prompt) {
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char *copy = malloc(strlen(buffer) + 1);
	strcpy(copy, buffer);
	copy[strlen(copy) - 1] = '\0';
	return copy;
}

void add_history(char *empty) {}

// Parser Definitions

mpc_parser_t *Number;
mpc_parser_t *Symbol;
mpc_parser_t *String;
mpc_parser_t *Comment;
mpc_parser_t *Sexpr;
mpc_parser_t *Qexpr;
mpc_parser_t *Expr;
mpc_parser_t *Khaled;

// Forward Definitions

struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

// Khaled Values

enum {LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_STR, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR};

typedef lval*(*lbuiltin) (lenv*, lval*);

struct lval {
	int type;

	long num;
	char *err;
	char *sym;
	char *str;

	lbuiltin builtin;
	lenv* env;
	lval* formals;
	lval* body;

	int count;
	lval **cells;
};

lval *lval_num (long x) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_NUM;
	v->num = x;
	return v;
}

int main(int argc, char **argv) {
	return 0;
}
