#include <iostream>
#include "lex.h"

BM::Lexer::Token BM::Lexer::get() {
    auto preIndex = i;
    sIndex = i;
    if (updateLine) {
        l++;
        updateLine = false;
    }
    if (haveVirtualMul) {
        t.s = "*";
        t.t = MUL_TOKEN;
        haveVirtualMul = false;
        return t;
    }
    if (i >= script.length()) {
        t.s = "";
        t.t = PROGRAM_END;
        return t;
    }
    t.s = "";
    t.t = NO_STATUS;
    bool numberDot = false;
    for (; i < script.length(); i++) {
        if (IS_SPACE(script[i])) {
            if (t.t) {// 已经有分割出来的token, 遇到空符就意味着语句的结束
                i++;
                if (script[i - 1] == '\n'/* && lastI < (i - 1)*/)
                    updateLine = true;
                break;
            }
            if (script[i] == '\n'/* && lastI < (i - 1)*/)
                l++;
            // 尚未有分割出来的token, 所以不做处理
            continue;
        } else if (IS_OP(script[i])) {
            if (!t.s.empty() && (script[i] == '(' || script[i] == '[' || script[i] == '{')) {
                break;
            }
            if (t.t >= UNKNOWN_TOKEN && t.t <
                                        NOTE_TOKEN) {// 如果是是word token的话, 遇到符号就说明word token的结束, 但是数字如果遇到.除外, 因为这种情况是小数, 但是数字只有一个小数点, 所有当numberDot为true以后又有.就是分割界线了
                if (script[i] == '.' && t.t == NUMBER_TOKEN && !numberDot) {
                    numberDot = true;
                    t.s += script[i];
                    continue;
                } else break;
            }
            if (script[i] == '"' || script[i] == '\'') {
                if (t.t != NO_STATUS) break;
                auto s = script[i];
                bool trans = false;// 转义'\'
                t.s += script[i];
                for (i++; i < script.length(); i++) {
                    t.s += script[i];
                    if (script[i] == s) {
                        if (!trans) {
                            i++;
                            break;
                        } else {
                            trans = !trans;
                        }
                    }
                    if (script[i] == '\\') trans = !trans;
                    else if (trans) trans = false;
                }
                t.t = STRING_TOKEN;
                break;
            }
            if (script[i] == ';') {
                if (t.t == NO_STATUS) t.s = ";";
                break;
            }
            if (
                    script[i] == ')' || script[i] == ']' || script[i] == '}' ||
                    script[i] == '(' || script[i] == '[' || script[i] == '{') {
                if (t.t == NO_STATUS) {
                    t.s = script[i];
                    t.t = UNKNOWN_OP_TOKEN;
                    i++;
                }
                break;
            }
            t.t = UNKNOWN_OP_TOKEN;
//            if (t.t == NO_STATUS || (t.t > NOTE_TOKEN && t.t < END_TOKEN))
        } else {
            if (
                    t.t > NOTE_TOKEN && t.t < END_TOKEN
                    )
                break;// 如果是符号token的话, 遇到非符号就说明该token结束了
            if (t.t != UNKNOWN_TOKEN && (IS_NUM(script[i]) || (!t.s.empty() && t.s[0] == '0' && IS_LETTER(script[i])))) t.t = NUMBER_TOKEN;
            else if (t.t == NUMBER_TOKEN) {
                haveVirtualMul = true;
                break;
            }
            else t.t = UNKNOWN_TOKEN;
        }
        t.s += script[i];
    }

    // 判断类型
    // 运算符判断
    if (t.s == ";") {
        t.t = END_TOKEN;
        i++;
    } else if (t.s == "=") {
        t.t = SET_TOKEN;
    } else if (t.s == ":") {
        t.t = COLON_TOKEN;
    } else if (t.s == "==") {
        t.t = EQUAL_TOKEN;
    } else if (t.s == "!=") {
        t.t = NOT_EQUAL_TOKEN;
    } else if (t.s == "<=") {
        t.t = LEQUAL_TOKEN;
    } else if (t.s == ">=") {
        t.t = MEQUAL_TOKEN;
    } else if (t.s == "<") {
        t.t = LESS_TOKEN;
    } else if (t.s == ">") {
        t.t = MORE_TOKEN;
    } else if (t.s == "(") {
        t.t = BRACKETS_LEFT_TOKEN;
    } else if (t.s == ")") {
        t.t = BRACKETS_RIGHT_TOKEN;
    } else if (t.s == "[") {
        t.t = MIDDLE_BRACKETS_LEFT_TOKEN;
    } else if (t.s == "]") {
        t.t = MIDDLE_BRACKETS_RIGHT_TOKEN;
    } else if (t.s == "{") {
        t.t = BIG_BRACKETS_LEFT_TOKEN;
    } else if (t.s == "}") {
        t.t = BIG_BRACKETS_RIGHT_TOKEN;
    } else if (t.s == ".") {
        t.t = DOT_TOKEN;
    } else if (t.s == ",") {
        t.t = COMMA_TOKEN;
    } else if (t.s == "&") {
        t.t = MAND_TOKEN;
    } else if (t.s == "|") {
        t.t = MOR_TOKEN;
    } else if (t.s == "^") {
        t.t = MXOR_TOKEN;
    } else if (t.s == "~") {
        t.t = MNOT_TOKEN;
    } else if (t.s == "&&") {
        t.t = LAND_TOKEN;
    } else if (t.s == "||") {
        t.t = LOR_TOKEN;
    } else if (t.s == "!") {
        t.t = LNOT_TOKEN;
    } else if (t.s == "+") {
        t.t = ADD_TOKEN;
    } else if (t.s == "-") {
        t.t = SUB_TOKEN;
    } else if (t.s == "*") {
        t.t = MUL_TOKEN;
    } else if (t.s == "/") {
        t.t = DIV_TOKEN;
    } else if (t.s == "%") {
        t.t = MOD_TOKEN;
    } else if (t.s == "**") {
        t.t = POWER_TOKEN;
    } else if (t.s == "<<") {
        t.t = SLEFT_TOKEN;
    } else if (t.s == ">>") {
        t.t = SRIGHT_TOKEN;
    } else if (t.s == "+=") {
        t.t = ADD_TO_TOKEN;
    } else if (t.s == "-=") {
        t.t = SUB_TO_TOKEN;
    } else if (t.s == "*=") {
        t.t = MUL_TO_TOKEN;
    } else if (t.s == "/=") {
        t.t = DIV_TO_TOKEN;
    } else if (t.s == "%=") {
        t.t = MOD_TO_TOKEN;
    } else if (t.s == "**=") {
        t.t = POWER_TO_TOKEN;
    } else if (t.s == "<<=") {
        t.t = SLEFT_TO_TOKEN;
    } else if (t.s == ">>=") {
        t.t = SRIGHT_TO_TOKEN;
    } else if (t.s == "&=") {
        t.t = MAND_TO_TOKEN;
    } else if (t.s == "|=") {
        t.t = MOR_TO_TOKEN;
    } else if (t.s == "^=") {
        t.t = MXOR_TO_TOKEN;
    } else if (t.s == "++") {
        t.t = DADD_TOKEN;
    } else if (t.s == "--") {
        t.t = DSUB_TOKEN;
    } else if (t.s == "~~") {
        t.t = RANGE_TOKEN;
    } else if (t.s == "->") {
        t.t = REFER_TO_TOKEN;
    }
    // 特判
    else if (t.s[0] == '0' && t.s.length() > 2) {
        t.t = NUMBER_TOKEN;
        string s(t.s);
        char* endp;
        s.erase(0, 2);
        switch (t.s[1]) {
            case 'x':
                t.s = std::to_string(strtol(s.c_str(), &endp, 16));
                break;
            case 'b':
                t.s = std::to_string(strtol(s.c_str(), &endp, 2));
                break;
            case 'o':
                t.s = std::to_string(strtol(s.c_str(), &endp, 8));
                break;
        }
    }
    // key word token判断
    else if (t.s == "let") {
        t.t = LET_TOKEN;
    } else if (t.s == "def") {
        t.t = DEF_TOKEN;
    } else if (t.s == "if") {
        t.t = IF_TOKEN;
    } else if (t.s == "elif") {
        t.t = ELIF_TOKEN;
    } else if (t.s == "else") {
        t.t = ELSE_TOKEN;
    } else if (t.s == "switch") {
        t.t = SWITCH_TOKEN;
    } else if (t.s == "case") {
        t.t = CASE_TOKEN;
    } else if (t.s == "for") {
        t.t = FOR_TOKEN;
    } else if (t.s == "while") {
        t.t = WHILE_TOKEN;
    } else if (t.s == "do") {
        t.t = DO_TOKEN;
    } else if (t.s == "continue") {
        t.t = CONTINUE_TOKEN;
    } else if (t.s == "break") {
        t.t = BREAK_TOKEN;
    } else if (t.s == "refer") {
        t.t = REFER_TOKEN;
    } else if (t.s == "return") {
        t.t = RETURN_TOKEN;
    } else if (t.s == "in") {
        t.t = IN_TOKEN;
    } else if (t.s == "of") {
        t.t = OF_TOKEN;
    } else if (t.s == "null") {
        t.t = NULL_TOKEN;
    } else if (t.s == "undefined") {
        t.t = UNDEFINED_TOKEN;
    } else if (t.s == "enum") {
        t.t = ENUM_TOKEN;
    } else if (t.s == "using") {
        t.t = USING_TOKEN;
    } else if (t.s == "default") {
        t.t = DEFAULT_TOKEN;
    } else if (t.s == "class") {
        t.t = CLASS_TOKEN;
    } else if (t.s == "public") {
        t.t = PUBLIC_TOKEN;
    } else if (t.s == "private") {
        t.t = PRIVATE_TOKEN;
    } else if (t.s == "operator") {
        t.t = OPERATOR_TOKEN;
    } else if (t.s == "extends") {
        t.t = EXTENDS_TOKEN;
    } else if (t.s == "new") {
        t.t = NEW_TOKEN;
    } else if (t.s == "static") {
        t.t = STATIC_TOKEN;
    } else if (t.s == "import") {
        t.t = IMPORT_TOKEN;
    } else if (t.s == "export") {
        t.t = EXPORT_TOKEN;
    } else if (t.s == "as") {
        t.t = AS_TOKEN;
    } else if (t.s == "debugger") {
        t.t = DEBUGGER_TOKEN;
    } else if (t.s == "delete") {
        t.t = DELETE_TOKEN;
    } else if (t.s == "//") {
        t.t = NOTE_TOKEN;
        while (true) {
            if (script[++i] == '\n') break;
        }
    } else if (t.s == "/*") {
        t.t = NOTE_TOKEN;
        t.s = "/*";
        while (true) {
            if (script[i] == '*' && script[i + 1] == '/') break;
            t.s += script[i];
            i++;
        }
        i += 2;
    } else if (t.s == "pass") {
        t.t = PASS_TOKEN;
    } else {
        if (t.t == UNKNOWN_OP_TOKEN) {
            auto tmp = i;
            auto nowP = t.s.length();
            for (; tmp >= preIndex && script[tmp] != '='; tmp--, nowP--) ;
            if (script[tmp] == '=') {
                i = tmp + 1;
                t.s.erase(nowP);
            }
        }
    }
    if (t.t == NOTE_TOKEN) return get();
    if (i > lastI) lastI = i;
    if (noSplit && t.t != BRACKETS_LEFT_TOKEN) {
        noSplit = false;
        script.insert(i, ")");
    }
    if (preUnknown && i == lastI) {
        if (t.t == UNKNOWN_TOKEN || t.t == NUMBER_TOKEN || t.t == STRING_TOKEN || t.t == NULL_TOKEN || t.t == UNDEFINED_TOKEN) {
            noSplit = true;
            preUnknown = false;
            script.insert(preUnknownIndex, "(");
            i = preUnknownIndex + 1;
            l = preUnknownLine;
            return Token(BRACKETS_LEFT_TOKEN, "(");
        }
    }
    if (t.t == UNKNOWN_TOKEN) { preUnknownIndex = i; preUnknownLine = l; preUnknown = true; }
    else preUnknown = false;
    return t;
}