# Evaluation

from parse import *
import math
import operator as op

# User defined Lisp procedure
class Procedure(object):
	def __init__(self, parms, body, env):
		self.parms, self.body, self.env = parms, body, env

	def __call__(self, *args):
		return eval(self.body, Env(self.parms, args, self.env))

# Environment
class Env(dict):
	def __init__(self, parms=(), args=(), outer=None):
		self.update(zip(parms, args))
		self.outer = outer

	def find(self, var): return self if (var in self) else self.outer.find(var)

# Manual definition of foldr and foldl
def foldr(combine, base, lst):
	if len(lst) == 0: return base
	else: return combine(lst[0], foldr(combine, base, lst[1:]))

def foldl(combine, acc, lst):
	if len(lst) == 0: return acc
	else: return foldl(combine, combine(acc, lst[0]), lst[1:])

# Create standard environment
def standard_env():
	env = Env()
	env.update(vars(math))
	env.update({
		'+': op.add,
		'-': op.sub,
		'*': op.mul,
		'/': op.div,
		'>': op.gt,
		'<': op.lt,
		'>=': op.ge,
		'<=': op.le,
		'=': op.eq,
		'abs': abs,
		'append': op.add,
		'apply': apply,
		'begin': lambda *x: x[-1],
		'car': lambda x: x[0],
		'cdr': lambda x: x[1:],
		'cons': lambda x,y: [x] + y,
		'eq?': op.is_,
		'equal?': op.eq,
		'length': len,
		'list': lambda *x: list(x),
		'list?': lambda x: isinstance(x, list),
		'map': map,
		'max': max,
		'filter': filter,
		'foldr': foldr,
		'foldl': foldl,
		'min': min,
		'not': op.not_,
		'null?': lambda x: x == [],
		'number?': lambda x: isinstance(x, Number),
		'procedure?': callable,
		'round': round,
		'symbol?': lambda x: isinstance(x, Symbol),
		})
	return env

#Initialize environment
global_env = standard_env()

_quote = Sym('quote')
_if = Sym('if')
_set = Sym('switchup')
_define = Sym('bless')
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

# Make predicates and functions of a user defined struct
def make_functions(name, param, env=global_env):
	create = 'bless-' + name
	check = name + '?'
	index_array = []
	key_array = []
	i = 0
	for par in param:
		index_array.append(i)
		i += 1

	for par in param:
		key_array.append(name + '-' + par + '-pos')

	env.update(zip(key_array, index_array))

	env[name + '-pos'] = lambda arr, index: arr[index]
	env[check] = lambda arr: len(arr) == eval(create)
	env[create] = len(param)

# Evaluate an expression
def eval(x, env=global_env):
	if isinstance(x, Symbol): # variable reference
		return env.find(x)[x]
	elif not isinstance(x, list): # constant literal
		return x                
	elif x[0] == _quote: # quotation
		(_, exp) = x
		return exp
	elif x[0] == _if: # conditional
		(_, test, conseq, alt) = x
		exp = (conseq if eval(test, env) else alt)
		return eval(exp, env)
	elif x[0] == _define: # definition
		(_, var, exp) = x
		env[var] = eval(exp, env)
	elif x[0] == _set: # assignment
		(_, var, exp) = x
		env.find(var)[var] = eval(exp, env)
	elif x[0] == _lambda: # procedure
		(_, parms, body) = x
		return Procedure(parms, body, env)
	elif x[0]== _checkexpect: # test exact
		(_, var, exp) = x
		return (eval(var, env) == eval(exp, env))
	elif x[0] == _checkwithin: # test range
		(_, var, lower_bound, upper_bound) = x
		return ((eval(var, env) <= eval(upper_bound, env) and (eval(var, env) >= eval(lower_bound, env))))
	elif x[0] == _member: # member?
		(_, var, lst) = x
		return (eval(var, env) in eval(lst, env))
	elif x[0] == _struct: # struct definition
		(_, name, params) = x
		make_functions(name, params, env)
	else: # procedure call
		proc = eval(x[0], env)
		if ( isinstance(x[0], str) and x[0].startswith('bless-')):
			args = [eval(arg, env) for arg in x[2:]]
			if len(args) != proc: 
				print('TypeError: ' + x[0] + ' requires %d values, given %d' % (proc,  len(args)))
			else:
				env[x[1]] = args
			return
		else: 
			args = [eval(arg, env) for arg in x[1:]]
		return proc(*args)

