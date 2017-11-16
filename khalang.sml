(* Khalang *)

type float = real

datatype token = function | openparen | closeparen | openbrace | closebrace
               | plus | minus | times | equal | variable of string
               | constant of string | arrow | set | longform | done | neg

datatype expression = var of string | const of string
                    | add of expression * expression
                    | sub of expression * expression
                    | mult of expression * expression
                    | negative of expression

exception Error

fun tokenize [] = []
  | tokenize (x::L) = case x of
                        "bless" => function :: tokenize L
                      | "(" => openparen :: tokenize L
                      | ")" => closeparen :: tokenize L
                      | "{" => openbrace :: tokenize L
                      | "}" => closebrace :: tokenize L
                      | "king" => set :: tokenize L
                      | "major" => longform :: tokenize L
                      | "key" => done :: tokenize L
                      | "=" => equal :: tokenize L
                      | "~" => neg :: tokenize L
                      | "+" => plus :: tokenize L
                      | "-" => minus :: tokenize L
                      | "*" => times :: tokenize L
                      | "=>" => arrow :: tokenize L
                      | x => case Real.fromString x of
                               NONE => (variable x) :: tokenize L
                             | SOME r => (constant (Real.toString r)) :: tokenize L
                            
fun tokentostring x = case x of
                        function => "bless"
                      | openparen => "(" | closeparen => ")"
                      | openbrace => "{" | closebrace => "}" 
                      | set => "king" | longform => "major" | done => "key"
                      | equal => "=" | neg => "~" | plus => "+"
                      | minus => "-" | times => "*" | arrow => "=>"
                      | constant s => "constant " ^ s
                      | variable s => "variable " ^ s

fun readtoken s [] = (print "you played yourself. neva play yourself.\n"; raise Error)
  | readtoken s (t::T) = if s = t then T
                         else
                           let
                             val m = "i caught a "  ^ (tokentostring s) ^
                             " but i appreciate a " ^ (tokentostring t)
                           in
                             (print m; raise Error)
                           end

fun parsevariable T = let
  val tokens = readtoken openparen T
  fun loop [] = (print "you played yourself. never play yourself.\n"; raise Error)
    | loop ((variable x) :: T) = let val (v, t) = loop T in (x :: v, t) end
    | loop (closeparen :: T) = ([], T)
    | loop _ = (print "i expected a variable or a bracket but you fooled me.\n"; raise Error)
in
  loop T
end
