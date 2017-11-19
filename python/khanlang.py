# Khanlang

from evaluate import *
import random

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

sayings = ["#blessup", "The key is to make it.", "Baby, you smart! You loyal! You a genius!", "Another one. No, another two.", "Be a star. Be a superstar.", "Bless up. Don't play yourself.", "Give thanks to the most high.", "The key is: never fold.", "They never said winning was easy."]

# REPL
def repl(prompt='lion:>>> '):
	print("Khanlang")
	print(random.choice(sayings))
	while True:
		inpt = raw_input(prompt)
		try:
			if inpt == "played-myself":
				print(random.choice(sayings) + "\n")
				break
			val = eval(parse(inpt))
			if val is not None: 
				print(khanify(val))
		except Exception as e:
			print '%s: %s' % (type(e).__name__, e)
		print(random.choice(sayings))

# Convert Python object back into a Khanlang string
def khanify(exp):
	if isinstance(exp, list):
		return '(' + ' '.join(map(khanify, exp)) + ')'
	else:
		return str(exp)

repl()
