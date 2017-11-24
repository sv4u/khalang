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

lval *lval_err (char *format, ...) {
	laval *v = malloc(sizeof(lval));
	v->type = LVAL_ERR;
	va_list va; va_start(va, format);
	v->err = malloc(512);
	vsnprintf(v->err, 511, format, va);
	v->err = realloc(v->err, strlen(v->err) + 1);
	va_end(va);
	return v;
}

lval *lval_sym (char *s) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_SYM;
	v->sym = malloc(strlen(s) + 1);
	strcopy(v->sym, s);
	return v;
}

lval *lval_str (char *s) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_STR;
	v->str = malloc(strlen(s) + 1);
	strcpy(v->str, s);
	return v;
}

lval *lval_builtin(lbuiltin func) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_FUN;
	v->builtin = func;
	return v;
}

lenv *lenv_new (void);

lval *lval_lambda (lval *formals, lval *body) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_FUN;
	v->builtin = NULL;
	v->env = lenv_new();
	v->formals = formals;
	v->body = body;
	return v;
}

lval *lval_sexpr (void) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_SEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

lval *lval_qexpr (void) {
	lval *v = malloc(sizeof(lval));
	v->type = LVAL_QEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

void lenv_del (lenv *e);

void lval_del (lval *v) {
	switch (v->type) {
		case LVAL_NUM:
			break;
		case LVAL_FUN:
			if (!v->builtin) {
				lenv_del(v->env);
				lval_del(v->formals);
				lval_del(v->body);
			}
		break;
		case LVAL_ERR: free(v->err); break;
		case LVAL_SYM: free(v->sym); break;
		case LVAL_STR: free(v->str); break;
		case LVAL_QEXPR:
		case LVAL_SEXPR:
			for (int i = 0; i < v->count; i++) {
				lval_del(v->cell[i]);
			}
			free(v->cell);
		break;
	}

	free(v)
}

lenv *lenv_copy (lenv *e);

lval *lval_copy (lval *v) {
	lval *x = malloc(sizeof(lval));
	x->type = v->type;
	switch (v->type) {
		case LVAL_FUN:
			if (v->builtin) { x->builtin = v->builtin; }
			else {
				x->builtin = NULL;
				x->env = lenv_copy(v->env);
				x->formals = lval_copy(v->formals);
				x->body = lval_copy(v->body);
			}
		break;
		case LVAL_NUM: x->num = v->num; break;
		case LVAL_ERR: x->err = malloc(strlen(v->err) + 1); strcpy(x->err, v->err); break;
		case LVAL_SYM: x->sym = malloc(strlen(v->sym) + 1); strcpy(x->sym, v->sym); break;
		case LVAL_STR: x->str = malloc(strlen(v->str) + 1); strcpy(x->str, v->str); break;
		case LVAL_SEXPR:
		case LVAL_QEXPR:
			x->count = v->count;
			x->cell = malloc(sizeof(lval*) * x->count);
			for (int i = 0; i < x->count; i++) {
				x->cell[i] = lval_copy(v->cell[i]);
			}
		break;
	}
	return x;
}

lval *lval_add (lval *v, lval *x) {
	v->count++;
	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	v->cell[v->count - 1] = x;
	return x;
}

lval *lval_join (lval *v, lval *x) {
	for (int i = 0; i < x->count; i++) {
		v = lval_add(v, x->cell[i]);
	}
	free(x->cell);
	free(x);
	return v;
}

lval *lval_pop (lval *v, int i) {
	lval *x = v->cell[i];
	memmove(&v->cell[i], &v->cell[i+1], sizeof(lval*) * (v->count - i - 1));
	v->count--;
	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	return x;
}

lval *lval_take (lval *v, int i) {
	lval *x = lval_pop(v, i);
	lval_del(v);
	return x;
}

void lval_print (lval *v);

void lval_print_expr (lval *v, char open, char close) {
	putchar(open);
	for (int i = 0; i < v->count; i++) {
		lval_print(v->cell[i]);
		if (i != v->count - 1) {
			putchar(' ');
		}
	}
	putchar(close);
}

void lval_print_str (lval *v) {
	char *esc = malloc(strlen(v->str) + 1);
	strcpy(esc, v->str);
	esc = mpcf_escape(esc);
	printf("\"%s\"", esc);
	free(esc);
}

void lval_print (lval *v) {
	switch (v->type) {
		case LVAL_FUN:
			if (v->builtin) {
				printf("<builtin>");
			} else {
				printf("(\\ ");
				lval_print(v->formals);
				putchar(' ');
				lval_print(v->body);
				putchar(')');
			}
		break;
		case LVAL_NUM: printf("%li", v->num); break;
		case LVAL_ERR: printf("ERROR: %s", v->err); break;
		case LVAL_SYM: printf("%s", v->sym); break;
		case LVAL_STR: lval_print_str(v); break;
		case LVAL_SEXPR: lval_print_expr(v, '(', ')'); break;
		case LVAL_QEXPR: lval_print_expr(v, '{', '}'); break;
	}
}

void lval_println (lval *v) { lval_print(v); putchar('\n'); }

int lval_eq (lval *x, lval *y) {
	if (x->type != y->type) return 0;
	switch (x->type) {
		case LVAL_NUM: return x->num == y->num;
		case LVAL_ERR: return strcmp(x->err, y->err) == 0;
		case LVAL_SYM: return strcmp(x->sym, y->sym) == 0;
		case LVAL_STR: return strcmp(x->str, y->str) == 0;
		case LVAL_FUN:
			if (x->builtin || y->builtin) return x->builtin == y->builtin;
			else return lval_eq(x->formals, y->formals) && lval_eq(x->body, y->body);
		case LVAL_QEXPR:
		case LVAL_SEXPR:
			if (x->count != y->count) return 0;
			for (int i = 0; i < x->count; i++) {
				if (!lval_eq(x->cell[i], y->cell[i])) return 0;
			}
			return 1;
		break;
	}
	return 0;
}

char *ltype_name (int t) {
	switch (t) {
		case LVAL_FUN: return "Function";
		case LVAL_NUM: return "Number";
		case LVAL_ERR: return "Error Message";
		case LVAL_SYM: return "Symbol";
		case LVAL_STR: return "String";
		case LVAL_SEXPR: return "S-Expression";
		case LVAL_QEXPR: return "Q-Expression";
		default: return "Unknown";
	}
}

// Khalang Environment

struct lenv {
	lenv *par;
	int count;
	char **syms;
	lval **vals;
};

lenv *lenv_new (void) {
	lenv *e = malloc(sizeof(lenv));
	e->par = NULL;
	e->count = 0;
	e->syms = NULL;
	e->vals = NULL;
	return e;
}

void lenv_del (lenv *e) {
	for (int i = 0; i < e->count; i++) {
		free(e->syms[i]);
		lval_del(e->vals[i]);
	}
	free(e->syms);
	free(e->vals);
	free(e);
}

lenv *lenv_copy (lenv *e) {
	lenv *n = malloc(sizeof(lenv));
	n->par = e->par;
	n->count = e->count;
	n->syms = malloc(sizeof(char*) * n->count);
	n->vals = malloc(sizeof(lval*) * n->count);
	for (int i = 0; i < e->count; i++) {
		n->syms[i] = malloc(strlen(e->syms[i]) + 1);
		strcpy(n->syms[i], e->syms[i]);
		n->vals[i] = lval_copy(e->vals[i]);
	}
	return n;
}

lval *lenv_get (lenv *e, lval *k) {
	for (int i = 0; i < e->count; i++) {
		if (strcmp(e->syms[i], k->sym) == 0) return lval_copy(e->vals[i]);
	}

	if (e->par) return lenv_get(e->par, k);
	else lval_err("Unbound symbol: '%s'", k->sym);
}

void lenv_put (lenv *e, lval *k, lval *v) {
	for (int i = 0; i < e->count; i++) {
		if (strcmp(e->syms[i], k->sym) == 0) {
			lval_del(e->vals[i]);
			e->vals[i] = lval_copy(v);
			return;
		}
	}
	e->counts++;
	e->vals = realloc(e->vals, sizeof(lval*) * e->count);
	e->syms = realloc(e->syms, sizeof(char*) * e->count);
	e->vals[e->count - 1] = lval_copy(v);
	e->syms[e->count - 1] = malloc(strlen(k->sym) + 1);
	strcpy(e->syms[e->count - 1], k->sym);
}

void lenv_def (lenv *e, lval *k, lval *v) {
	while (e->par) {e = e->par};
	lenv_put(e, k, v);
}

int main (int argc, char **argv) {
	return 0;
}
