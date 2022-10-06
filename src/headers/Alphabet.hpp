#pragma once

class Alphabet
{
public:
    // Token                 Lexeme
    static const int CONST = 0;         // (C | c)(O | o)(N | n)(S | s)(T | t)
    static const int INT = 1;           // (I | i)(N | n)(T | t)
    static const int CHAR = 2;          // (C | c)(H | h)(A | a)(R | r)
    static const int WHILE = 3;         // (W | w)(H | h)(I | i)(L | l)(E | e)
    static const int IF = 4;            // (I | i)(F | f)
    static const int FLOAT = 5;         // (F | f)(L | l)(O | o)(A | a)(T | t)
    static const int ELSE = 6;          // (E | e)(L | l)(S | s)(E | e)
    static const int AND = 7;           // &&
    static const int OR = 8;            // ||
    static const int NOT = 9;           // !
    static const int ATRIB = 10;        // :=
    static const int EQUAL = 11;        // =
    static const int OPENPAR = 12;      // (
    static const int CLOSEPAR = 13;     // )
    static const int LESSTHAN = 14;     // <
    static const int GREATERTHAN = 15;  // >
    static const int NOTEQUAL = 16;     // !=
    static const int GREATEREQUAL = 17; // >=
    static const int LESSEQUAL = 18;    // <=
    static const int COMMA = 19;        // ,
    static const int PLUS = 20;         // +
    static const int MINNUS = 21;       // -
    static const int TIMES = 22;        // *
    static const int DIVIDE = 23;       // /
    static const int SEMICOLON = 24;    // ;
    static const int OPENBRACE = 25;    // {
    static const int CLOSEBRACE = 26;   // }
    static const int READLN = 27;       // (R | r)(E | e)(A | a)(D | d)(L | l)(N |n)
    static const int DIV = 28;          // (D | d)(I | i)(V | v)
    static const int STRING = 29;       // (S | s)(T | t)(R | r)(I | i)(N | n)(G | g)
    static const int WRITE = 30;        // (W | w)(R | r)(I | i)(T | t)(E | e)
    static const int WRITELN = 31;      // (W | w)(R | r)(I | i)(T | t)(E | e)(L | l)(N | n)
    static const int MOD = 32;          // (M | m)(O | o)(D | d)
    static const int OPENBRACKET = 33;  // [
    static const int CLOSEBRACKET = 34; // ]
    static const int TRUE = 35;         // (T | t)(R | r)(U | u)(E | e)
    static const int FALSE = 36;        // (F | f)(A | a)(L | l)(S |s)(E | e)
    static const int BOOLEAN = 37;      // (B | b)(O | o)(O | o)(L | l)(E | e)(A | a)(N| n)
    static const int ID = 40;           // (l | _) (l | d | _) -> l = letra; d = dígito
    static const int CONSTANT = 41;     // (('c') | ("(n)*") | (d+) | (d*.d+))
                                        // c -> valid and printable char
                                        // n -> valid char != /n
};