#ifndef BERRYMATH_INTERPRETER_H
#define BERRYMATH_INTERPRETER_H

#include <fstream>
#include <string>
#include "value.h"
#include "ast.h"
#include "types.h"
using std::string;

namespace BM {
#define PASS_ERROR "__ERROR__"
#define PASS_RETURN "__RETURN__"
#define PASS_ENDFUN "__RETURNFUN__"
#define PASS_THIS "__THIS__"
#define PASS_UPVALUE "__UPVALUE__"
#define PASS_LASTKEY "__LASTKEY__"
#define PASS_BREAK "__BREAK__"
#define PASS_CONTINUE "__CONTINUE__"
#define PASS_NEXTOP "__NEXTOPERATOR__"
#define PROTO_PRIVATE_TAG "__private__"
    inline std::ofstream tmpOut;
    class Interpreter {
    public:
        Interpreter(const string& s = "", const string& fn = "", Interpreter *p = nullptr) : script(s), filename(fn), ast(new AST(s)),
                                                                               parent(p),
                                                                               scope(new Scope(p ? p->scope : nullptr)),
                                                                               child(false), upscope("\033[32mglobal\033[0m") {
            script += ";pass";
            if (parent) upscope = parent->upscope;
        }
        void open(const string& s, const string& fn = "") {
            script = s;
            if (ast) ast->open(script);
            else ast = new AST(s);
            filename = fn;
            script += ";pass";
            if (parent) upscope = parent->upscope;
            else upscope = "\033[32mglobal\033[0m";
        }

        string compile();
        Object *run();
        Object *runCC();
        string fn() { return filename; }
        ~Interpreter() {
            if (!child && ast) delete ast;
            delete scope;
        }
        Variable &operator[](const string &s) { return *scope->get(s); }
        void set(const string &name, Object *v) { scope->set(name, v); }
        Variable *get(const string &name, Scope::Flag flag = Scope::ALL_MIGHT) { return scope->get(name, flag); }
        void del(const string &name) { scope->del(name); }
        string upscope;
        void import(Object*, const string&, const string&);
        void Using(Object*, AST::node*);
        void stack(unsigned = 0);
        void debug();
        void clear() { delete scope;scope = new Scope(parent ? parent->scope : nullptr); }
        static Object* Array() {
            return Interpreter("[]").run()->getThenDelete(PASS_RETURN);
        }
    private:
        bool child = false;
        bool loaded = false;
        AST *ast;
        Scope *scope;
        Interpreter *parent;
        string script;
        string filename;

#define THROW { exports->set(PASS_ERROR, new Number(1)); return exports; }
#define CHECKITER(e, ast) \
    if (!e || e->get(PASS_ERROR) || e->empty()) { \
        std::cerr << "\tat <" << filename << ":" << upscope << ">:" << ast->line() << std::endl; \
        THROW; \
    }
#define RIGHTEXPRTYPE(left, right) if (left->type() == NUMBER && right->type() == NUMBER)
#define WRONGEXPRTYPE(op) else { std::cerr << "TypeError: Cannot perform " << op << " operations\n\tat <" << filename <<  ":" << upscope << ">:" \
            << ast->line() << std::endl; \
            THROW; \
}
#define WRONGSCRIPT(token) else { std::cerr << "TypeError: Cannot " << token << "\n\tat <" << filename <<  ":" << upscope << ">:" \
            << ast->line() << std::endl; \
            THROW; \
}
#define WRONG(name, s)  else { std::cerr << name << ": " << s << "\n\tat <" << filename <<  ":" << upscope << ">:" \
            << ast->line() << std::endl; \
            THROW; \
}
#define NOTDEFINED(v, name) if (!v) { \
        std::cerr << "ReferenceError: " << name << " is not defined\n\tat <" << filename << ":" << upscope << ">:" \
        << ast->line()  << std::endl; \
        THROW; \
    }
#define CHECK_AND_THROW_IF_CANNOT_GET(_ast) { \
    if (!_ast) { std::cerr << "InterpreterError: Unable to analyze your '" << rootValue << "' statement\n\tat <" << filename << ":" << upscope << ">:" \
        << ast->line()  << std::endl; \
        THROW; \
        } \
}
#define CHECKPASSNEXTOP(e) { auto v = e->get(PASS_NEXTOP);if (v) { auto value = (Number*)e->get(PASS_RETURN);if (value->type() == NUMBER) { value->value() += ((Number*)v)->value(); } WRONGEXPRTYPE("self increment or decrement"); } }
#define OPERPASSNEXTOP(conE, con) { auto ret = conE->get(PASS_RETURN); \
        if (ret->type() == OBJECT) { \
            con = ret; \
        } else { \
            con = ret->copy(); \
            CHECKPASSNEXTOP(conE); \
        } }
#define FREE(v) { if (v) v->unbind(); }
#ifdef I_OS_WIN32
        // 拓展库path
#define BMMPATH "C:\\BM\\libraries\\lib"
        // 模块库path
#define BMLMPATH "C:\\BM\\modules\\"
#else
        // 拓展库path
#define BMMPATH "/usr/local/BM/libraries/lib"
        // 模块库path
#define BMLMPATH "/usr/local/BM/modules/"
#endif
#define DEFAULT_IMPORT_NAME "bmlang"
#define PASS_MODULE_NAME "__THIS_MODULE_NAME__"
#define LEFT_NODE get(0)
#define RIGHT_NODE get(1)

        friend class NativeFunction;

    public:
        using initModuleFun = BM::Object *(*)();
    };
}


#endif //BERRYMATH_INTERPRETER_H
