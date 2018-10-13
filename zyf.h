struct Token{
	unsigned char * tokenaddress;
	int tokenlen;
};
// 2������(ִ��ʱ��̬�����ڴ�ռ䣬ʶ������ȶ�̬����)
typedef enum { 
	BOOLCONSTANT,
	INTCONSTANT,
	REALCONSTANT,
	STRINGCONSTANT
}ConstantType;
struct Constant{
	ConstantType type;//�������ͣ���������
	int size;//���Ŀռ�
	struct Constant * nextConstant;//��һ�������ĵ�ַ
	int count;//���ü���������Ϊ��Ĵӱ����޳�
	char data[];
};
// 3������
struct Variable
{
	struct Token * name; // ָ�����Ƶĵ��ʵ�ַ
	struct Constant * value;//����ֵ����ִ�й����б仯�� ��ִ��ǰΪNULL
};
// 4�����ʽ
// һԪ���ʽ��
// �������ʽ��һ������ָ��
struct ConstantExp
{
	struct Constant* constant;
};
// �������ʽ��һ������ָ��
struct VariableExp
{
	struct Variable* variable;
};
// �������ñ�ʾʽ��һ����������Token��ָ��Ͳ����б�ָ�루���ʽ�б�
struct FunCallExp
{
	struct Token * functionname;
	struct explist * paralist;
};
// ���ϱ��ʽ: һ�����ʽָ��
struct ComplexExp
{
	struct Exp* expression;
};
// ȡ�����ʽ��һ��һԪ���ʽָ��
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
// ��

// �˳������ʽ��һ���˳������ʽָ���һ��ȡ�����ʽָ��
struct DivOrMulExp
{
	struct DivOrMulExp * divormulexp;
	struct UnaryExp * unaryexp;
};
// �Ӽ������ʽ: һ���ӷ����ʽָ���һ���˳������ʽָ��
struct AddOrSubExp
{
	struct AddOrSubExp * addorsubexp;
	struct DivOrMulExp * divormulexp;
};
// �Ƚϱ��ʽ�������Ӽ������ʽָ��
struct CompareExp
{
	struct AddOrSubExp * addorsubexp1;
	struct AddOrSubExp * addorsubexp2;
};
// �߼�����������ʽ��һ���߼�����������ʽָ���һ���Ƚϱ��ʽָ��
struct LogicOprationExp
{
	struct LogicOprationExp* logicoprationexp;
	struct CompareExp * compareexp;
};
// ��ֵ���ʽָ�룺һ���������ʽָ���һ���߼�����������ʽָ�롣
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
// 5�����
// ���ʽ��䣺��ֵ���ʽָ��������ñ��ʽָ��
struct ExpStmt
{
	struct Exp* exp;
};
// ������䣺һ���߼�����������ʽָ�������б�ָ��
struct IfStmt
{
	struct Exp* exp;
	struct StmtList * stmtlist;
};
// ѭ����䣺һ���߼�����������ʽָ�������б�ָ��
struct WhileStmt
{
	struct Exp* exp;
	struct StmtList * stmtlist;
};
// ������䣺һ���߼�����������ʽָ��
struct ReturnStmt
{
	struct Exp* exp;
};
// ȫ��������䣺һ��Token��ַ
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

// 6�������б�����ȫ�ֱ�����¼��
	// һ��������ַ��һ����һ���б���ĵ�ַ
struct VariableList
{
	struct Variable* variable;
	struct VariableList *next;
} ;
// 8������б�
	// һ������ַ��һ����һ��������ַ 
struct StmtList
{
	struct Stmt* stmt;
	struct StmtList *next;
};
// 9���Խ�����	һ��Token��ַ���������ƣ���һ���β��б��ַ�������б���һ������б�ָ��
struct SelfFunction
{
	struct Token * funcationname;
	struct VariableList * variablelist;
	struct StmtList stmtlist;
};

// ���ú���һ��Token��ַ���������ƣ���һ���β��б��ַ�������б�
struct BuildInFunction
{
	struct Token * funcationname;
	struct VariableList * list;
};
typedef enum {
	BUILDINFUNCTION,
	SELFFUNCTION
}FunctionType;

// �������������͡���������
struct Function
{
	FunctionType type;
	union{
		struct BuildInFunction buildinfunction;
		struct SelfFunction selffunction;
	} function;
};
// 10�������б�// һ��������ַ��һ����һ���������ַ
struct FunctionList
{
	struct Function fucntion;
	struct FunctionList * next;
};

// 11��������
// �����б�
// ����б�
// �����б�

struct ZYF
{
	struct FunctionList * functionlist;
	struct StmtList* stmtlist;
	struct VariableList* variablelist;
};
