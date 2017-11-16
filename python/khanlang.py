# Khanlang

from evaluate import *

_quote = Sym('quote')
_if = Sym('if')
_set = Sym('set!')
_define = Sym('define')
_lambda = Sym('lambda')
_begin = Sym('begin')
_definemacro = Sym('define-macro')
_quasiquote = Sym('quasiquote')
_unquoto = Sym('unquote')
_unquotesplicing = Sym('unquote-splicing')
_checkexpect = Sym('check-expect')
_checkwithin = Sym('check-within')
_member = Sym('member?')
_struct = Sym('struct')

# REPL
def repl(prompt='Lion: '):
	print("Khanlang\nBless up\n")
	while True:
		inpt = raw_input(prompt)
		try:
			if inpt == "quit": break
			val = eval(parse(inpt))
			if val is not None: 
				print(khanify(val))
		except Exception as e:
			print '%s: %s' % (type(e).__name__, e)

# Convert Python object back into a Khanlang string
def khanify(exp):
	if isinstance(exp, list):
		return '(' + ' '.join(map(khanify, exp)) + ')'
	else:
		return str(exp)

repl()
