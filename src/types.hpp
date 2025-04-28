#ifndef TYPECHECK_HPP
#define TYPECHECK_HPP

#include "data_structures.hpp"
#include <string>
#include <cctype> // for isxdigit
#include <iostream>

inline int checkInt(std::string type)
{
    return type == "int";
}

inline bool isFloat(std::string type)
{
    return type == "float";
}

inline bool isChar(std::string type)
{
    return type == "char";
}

inline bool checkChar(std::string temp)
{
    // Validate character constant
    bool valid = false;
    // Simple character: 'x'
    if (temp.size() == 3 && temp[0] == '\'' && temp[2] == '\'')
    {
        return true;
    }
    // Escape sequence: '\n', '\t', etc.
    else if (temp.size() == 4 && temp[0] == '\'' && temp[3] == '\'' && temp[1] == '\\')
    {
        char esc = temp[2];
        return (esc == 'n' || esc == 't' || esc == 'r' || esc == '0' ||
                esc == '\\' || esc == '\'' || esc == '\"' || esc == 'a' ||
                esc == 'b' || esc == 'f' || esc == 'v');
    }
    // Hex escape sequence: '\xhh'
    else if (temp.size() >= 5 && temp.size() <= 8 && temp[0] == '\'' && temp.back() == '\'' && temp[1] == '\\' && temp[2] == 'x')
    {
        valid = true;
        for (size_t i = 3; i < temp.size() - 1; i++)
        {
            if (!std::isxdigit(temp[i]))
            {
                valid = false;
                return false;
                break;
            }
        }
    }
    return valid;
}
inline char giveChar(std::string temp)
{
    // Validate and extract character constant
    if (temp.size() == 3 && temp[0] == '\'' && temp[2] == '\'')
    {
        // Simple character: 'x'
        return temp[1];
    }
    else if (temp.size() == 4 && temp[0] == '\'' && temp[3] == '\'' && temp[1] == '\\')
    {
        // Escape sequence: '\n', '\t', etc.
        char esc = temp[2];
        switch (esc)
        {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '0': return '\0';
        case '\\': return '\\';
        case '\'': return '\'';
        case '\"': return '\"';
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'v': return '\v';
        default: return '\0'; // Invalid escape sequence
        }
    }
    else if (temp.size() >= 5 && temp.size() <= 8 && temp[0] == '\'' && temp.back() == '\'' && temp[1] == '\\' && temp[2] == 'x')
    {
        // Hex escape sequence: '\xhh'
        std::string hexValue = temp.substr(3, temp.size() - 4);
        if (std::all_of(hexValue.begin(), hexValue.end(), ::isxdigit))
        {
            return static_cast<char>(std::stoi(hexValue, nullptr, 16));
        }
    }
    return '\0'; // Invalid input
}

inline std::string searchIdentifierType(std::string id)
{
    sym_entry *n = lookup(id);
    if (n)
    {
        std::string s = n->type;
        return s;
    }
    return "";
}

inline std::string postfixExpression(std::string type, int rule_num)
{
    if (type.empty())
    {
        return "";
    }

    switch (rule_num)
    {
    case 1:
        return (type.back() == '*') ? type.substr(0, type.length() - 1) : "";
    case 2:
        return (type.size() >= 5 && type.substr(0, 5) == "FUNC_") ? type.substr(5, type.length() - 5) : "";
    case 3:
        return (checkInt(type) || isFloat(type)) ? type : "";
    default:
        return "";
    }
}

inline bool isAInt = false;
inline bool isBInt = false;
inline bool isABool = false;
inline bool isBBool = false;
inline bool isAFloat = false;
inline bool isBFloat = false;
inline bool isAChar = false;
inline bool isBChar = false;
inline bool isAAnyPtr = false;
inline bool isBAnyPtr = false;

inline void setVar(std::string a, std::string b)
{
    isAInt = checkInt(a);
    isBInt = checkInt(b);
    isABool = (a == "bool");
    isBBool = (b == "bool");
    isAFloat = isFloat(a);
    isBFloat = isFloat(b);
    isAChar = (a == "char");
    isBChar = (b == "char");
    isAAnyPtr = (!a.empty() && a.back() == '*');
    isBAnyPtr = (!b.empty() && b.back() == '*');
}

inline std::string checkType(std::string a, std::string b)
{
    setVar(a, b);
    
    if (a.empty() || b.empty())
        return "";

    if (a == b)
        return "ok";

    if (b == "void")
        return "";

    bool isAVoidPtr = (a == "void*");
    bool isBVoidPtr = (b == "void*");

    if ((isAVoidPtr && isBAnyPtr) || (isAAnyPtr && isBVoidPtr))
        return "ok";

    if ((isAInt && isBBool) || (isABool && isBInt))
        return "ok";

    if (isAChar || isAInt)
        isAFloat = true;

    if (isBChar || isBInt)
        isBFloat = true;

    if (isAFloat && isBFloat)
        return "ok";

    return "";
}

inline std::string checkExplicitType(std::string a, std::string b)
{
    setVar(a, b);
    if (a.empty() || b.empty())
        return "";

    if (a == b)
        return "ok1";

    if ((isAInt && isBBool) || (isABool && isBInt))
        return "ok2";

    if (isBChar && isAInt)
        return "ok";

    if (isAChar && isBInt)
        return "ok";
    
    if (isAInt && isBFloat)
        return "ok";

    if (isBInt && isAFloat)
        return "ok";
    
    if (isAChar && isBFloat)
        return "ok";

    if (isAFloat && isBChar)
        return "ok";

    bool isAVoidPtr = (a == "void*");
    bool isBVoidPtr = (b == "void*");

    if ((isAVoidPtr && isBAnyPtr) || (isAAnyPtr && isBVoidPtr))
        return "ok9";

    if (b == "void")
        return "";

    return "";
}

inline std::string unaryExp(std::string op, std::string type)
{
    if (op == "*")
    {
        type = postfixExpression(type, 1);
    }
    else if (op == "&")
    {
        type += "*";
    }
    else if (op == "+" || op == "-")
    {
        if (!(type == "int" || type == "float"))
            return "";
    }
    else if (op == "~")
    {
        if (!(type == "int" || type != "bool"))
            return "";
    }
    else if (op == "!")
    {
        if (!(type == "int" || type != "bool"))
            return "";
    }
    return type;
}

inline std::string Exp(std::string a, std::string b, std::string op)
{
    setVar(a, b);
    if (op == "*" || op == "/")
    {
        if ((isAChar || isAInt || isAFloat) && (isBChar || isBInt || isBFloat))
        {
            if (isAFloat || isBFloat)
                return "float";
            else if (isAInt || isBInt)
                return "int";
            else
                return "char";
        }
        return "";
    }
    else if (op == "%")
    {
        if (isAInt && isBInt)
            return "int";
        return "";
    }
    else if (op == "+" || op == "-")
    {
        // if (isAInt && isBInt)
        //     return "int";
        // else if ((isAInt || isAFloat) && (isBInt || isBFloat))
        //     return "float";
        // else if ((isAInt && isBChar) || (isAChar && isBInt))
        //     return "int";
        if ((isAChar || isAInt || isAFloat) && (isBChar || isBInt || isBFloat))
        {
            if (isAFloat || isBFloat)
                return "float";
            else if (isAInt || isBInt)
                return "int";
            else
                return "char";
        }
        else if (isAInt && isBAnyPtr)
            return b;
        else if (isAAnyPtr && isBInt)
            return a;
        return "";
    }
    return "";
}

inline std::string shiftExp(std::string a, std::string b)
{
    setVar(a, b);
    if (isAInt && isBInt)
        return "ok";
    return "";
}

inline std::string relExp(std::string a, std::string b)
{
    setVar(a, b);
    if ((isAChar || isAInt || isAFloat) && (isBChar || isBInt || isBFloat))
    {
        if (isAFloat || isBFloat)
            return "float";
        else if (isAInt || isBInt)
            return "int";
        else
            return "char";
    }
    
    // if ((isAInt || isAFloat || isAChar) && (isBInt || isBFloat || isBChar))
    //     return "ok";
    return "";
}

inline std::string eqExp(std::string a, std::string b)
{
    setVar(a, b);
    if (a == b)
        return "ok";
    if ((isAInt || isAFloat || isAChar) && (isBInt || isBFloat || isBChar))
        return "ok";
    return "";
}

inline std::string bitExp(std::string a, std::string b)
{
    setVar(a, b);
    if (isABool && isBBool)
        return "ok";
    if ((isABool || isAInt) && (isBBool || isBInt))
        return "int";
    return "";
}

inline std::string assignExp(const std::string &a, const std::string &b, const std::string &op)
{
    if (op == "=")
    {
        return checkType(a, b);
    }

    const std::string arithOps = "*%/+ -";
    const std::string bitwiseOps = "&|^";

    // Arithmetic assignments: *=, /=, %=, +=, -=
    if (op.size() == 2 && op[1] == '=' && arithOps.find(op[0]) != std::string::npos)
    {
        return Exp(a, b, std::string(1, op[0])) != "" ? "ok" : "";
    }

    // Shift assignments: >>=, <<=
    if (op == ">>=" || op == "<<=")
    {
        return shiftExp(a, b) != "" ? "ok" : "";
    }

    // Bitwise assignments: &=, |=, ^=
    if (op.size() == 2 && op[1] == '=' && bitwiseOps.find(op[0]) != std::string::npos)
    {
        return bitExp(a, b) != "" ? "ok" : "";
    }

    return "";
}

inline std::string condExp(std::string a, std::string b)
{
    setVar(a, b);
    if (a == b)
        return a;
    if (isAChar || isAInt)
        a = "float";
    if (isBChar || isBInt)
        b = "float";
    if (isAFloat && isBFloat)
        return "float";
    if (isAAnyPtr && isBAnyPtr)
        return "void*";
    return "";
}

#endif // TYPECHECK_HPP