# Parser

from __future__ import division
import re, sys, StringIO

# Lisp list = Python list
# Lisp number = Python int OR float
# Symbol = str
# List = list
# Number = (int, float)

class Symbol(str): pass
Symbol = str
List   = list
Number = (int, float)

class _Symbol(str): pass

symbol_table = {}

def Sym(s):
	if s not in symbol_table:
		symbol_table[s] = Symbol(s)
	return symbol_table[s]

_quote = Sym('quote')
_if = Sym('if')
_set = Sym('set!')
_define = Sym('define')
_lambda = Sym('lambda')
_begin = Sym('begin')
_definemacro = Sym('define-macro')
_quasiquote = Sym('quasiquote')
_unquote = Sym('unquote')
_unquotesplicing = Sym('unquote-splicing')
_checkexpect = Sym('check-expect')
_checkwithin = Sym('check-within')
_member = Sym('member?')
_struct = Sym('struct')

# Tokenizer
def tokenize(C):
	C = C.replace('(', ' ( ').replace(')', ' ) ')
	C = C.replace('\"', ' \" ').replace(';', ' ;').split()
	return C

# Read expression from a list of tokens
def readfromtokens(toks):
	if len(toks) == 0:
		raise SyntaxError('unexpected End of File while reading')
	tok = toks.pop(0)
	if '(' == tok:
		L = []
		while toks[0] != ')':
			L.append(readfromtokens(toks))
		toks.pop(0)
		return L
	
	elif '"' == tok:
		L = []
		while toks[0] != '"':
			L.append(readfromtokens(toks))
		end_quote = toks.pop(0)
		string = tok
		string += " ".join(L)
		string += end_quote
		return ['quote', string]

	elif ';' == tok:
		L = []
		L.append(tok)
		while toks[0] != '\n':
			L.append(readfromtokens(toks))
		new_line = toks.pop(0)
		L.append(new_line)
		string = " ".join(L)
		return ['quote', string]

	elif ')' == tok:
		raise SyntaxError('unexpected )')

	else:
		return atom(tok)

# Numbers -> numbers; everything else -> Symbol
def atom(tok):
	try: return int(tok)
	except ValueError:
		try: return float(tok)
		except ValueError:
			return Symbol(tok)

def parse(C):
	return readfromtokens(tokenize(C))

# Testing
# print(parse("(define factorial (lambda (x) (if (<= x 1) 1 (* x (factorial (- x 1)))))); work please"))
