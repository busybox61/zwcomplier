struct Token{
	unsigned char * tokenaddress;
	int tokenlen;
};
// 2、常量(执行时动态分配内存空间，识别出后先动态分配)
typedef enum { 
	BOOLCONSTANT,
	INTCONSTANT,
	REALCONSTANT,
	STRINGCONSTANT
}ConstantType;
struct Constant{
	ConstantType type;//常量类型，包括四种
	int size;//量的空间
	struct Constant * nextConstant;//下一个常量的地址
	int count;//引用计数，计数为零的从表中剔除
	char data[];
};
// 3、变量
struct Variable
{
	struct Token * name; // 指向名称的单词地址
	struct Constant * value;//变量值，在执行过程中变化， 在执行前为NULL
};
// 4、表达式
// 一元表达式｛
// 常量表达式：一个常量指针
struct ConstantExp
{
	struct Constant* constant;
};
// 变量表达式：一个变量指针
struct VariableExp
{
	struct Variable* variable;
};
// 函数调用表示式：一个函数名（Token）指针和参数列表指针（表达式列表）
struct FunCallExp
{
	struct Token * functionname;
	struct explist * paralist;
};
// 复合表达式: 一个表达式指针
struct ComplexExp
{
	struct Exp* expression;
};
// 取反表达式：一个一元表达式指针
struct NegativeExp
{
	struct Exp* expression;
};
typedef enum {
	CONSTANTEXP,
	VARIABLEEXP,
	FUNCALLEXP,
	COMPLEXEXP,
	NEGATIVEEXP
}UnaryExpType;
struct UnaryExp
{
	UnaryExpType type;
	union {
		struct ConstantExp constantexp;
		struct VariableExp variableexp;
		struct FunCallExp funcallexp;
		struct ComplexExp complexexp;
		struct NegativeExp negativeexp;
	} expression;
};
// ｝

// 乘除法表达式：一个乘除法表达式指针和一个取反表达式指针
struct DivOrMulExp
{
	struct DivOrMulExp * divormulexp;
	struct UnaryExp * unaryexp;
};
// 加减法表达式: 一个加法表达式指针和一个乘除法表达式指针
struct AddOrSubExp
{
	struct AddOrSubExp * addorsubexp;
	struct DivOrMulExp * divormulexp;
};
// 比较表达式：两个加减法表达式指针
struct CompareExp
{
	struct AddOrSubExp * addorsubexp1;
	struct AddOrSubExp * addorsubexp2;
};
// 逻辑或且运算表达式：一个逻辑或且运算表达式指针和一个比较表达式指针
struct LogicOprationExp
{
	struct LogicOprationExp* logicoprationexp;
	struct CompareExp * compareexp;
};
// 赋值表达式指针：一个变量表达式指针和一个逻辑或且运算表达式指针。
struct EvaluateExp
{
	struct VariableExp *variableexp;
	struct LogicOprationExp * logicoprationexp;
};
typedef enum {
	EVALUATEEXP,
	LOGICOPRATIONEXP,
	COMPAREEXP,
	ADDORSUBEXP,
	DIVORMULEXP,
	UNARYEXP
}ExpType;
struct Exp
{
	ExpType type;
	union {
		struct EvaluateExp evaluateexp;
		struct LogicOprationExp logicoprationexp;
		struct CompareExp compareexp;
		struct AddOrSubExp addorsubexp;
		struct DivOrMulExp divormulexp;
		struct UnaryExp unaryexp;
	} exp;
};
// 5、语句
// 表达式语句：赋值表达式指针或函数调用表达式指针
struct ExpStmt
{
	struct Exp* exp;
};
// 条件语句：一个逻辑或且运算表达式指针和语句列表指针
struct IfStmt
{
	struct Exp* exp;
	struct StmtList * stmtlist;
};
// 循环语句：一个逻辑或且运算表达式指针和语句列表指针
struct WhileStmt
{
	struct Exp* exp;
	struct StmtList * stmtlist;
};
// 返回语句：一个逻辑或且运算表达式指针
struct ReturnStmt
{
	struct Exp* exp;
};
// 全局申明语句：一个Token地址
struct VariableGolbalStmt
{
	struct Token* variablename;
};
typedef enum {
	EXPSTMT,
	IFSTMT,
	WHILESTMT,
	RETURNSTMT,
	VARIABLEGOLBALSTMT
}StmtType;
struct Stmt
{
	StmtType type;
	union{
		struct ExpStmt expstmt;
		struct IfStmt ifstmt;
		struct WhileStmt whilestmt;
		struct ReturnStmt returnstmt;
		struct VariableGolbalStmt variablegolbalstmt;
	}stmt;
};

// 6、变量列表（用于全局变量记录）
	// 一个变量地址，一个下一个列表项的地址
struct VariableList
{
	struct Variable* variable;
	struct VariableList *next;
} ;
// 8、语句列表
	// 一个语句地址，一个下一个语句项地址 
struct StmtList
{
	struct Stmt* stmt;
	struct StmtList *next;
};
// 9、自建函数	一个Token地址（函数名称）、一个形参列表地址（变量列表）、一个语句列表指针
struct SelfFunction
{
	struct Token * funcationname;
	struct VariableList * variablelist;
	struct StmtList stmtlist;
};

// 内置函数一个Token地址（函数名称）、一个形参列表地址（变量列表）
struct BuildInFunction
{
	struct Token * funcationname;
	struct VariableList * list;
};
typedef enum {
	BUILDINFUNCTION,
	SELFFUNCTION
}FunctionType;

// 函数（函数类型、函数对象）
struct Function
{
	FunctionType type;
	union{
		struct BuildInFunction buildinfunction;
		struct SelfFunction selffunction;
	} function;
};
// 10、函数列表// 一个函数地址，一个下一个函数项地址
struct FunctionList
{
	struct Function fucntion;
	struct FunctionList * next;
};

// 11、解释器
// 函数列表
// 语句列表
// 变量列表

struct ZYF
{
	struct FunctionList * functionlist;
	struct StmtList* stmtlist;
	struct VariableList* variablelist;
};
