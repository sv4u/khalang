(* Khalang *)

type float = real

datatype token = function | openparen | closeparen | openbrace | closebrace
               | plus | minus | times | power | eqaul | variable of string
               | constant of string | arrow | set | longform | done | neg

datatype expression = variable of string | constant of string
                    | plus of expression * expression
                    | minus of expression * expression
                    | times of expression * expression
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
                             | SOME r => (Constant (Real.toString r)) :: tokenize L
