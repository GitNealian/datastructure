#include "Expr.h"
//状态机的7种状态
enum STATE {EXPR_INITIAL,EXPR_NUMBER,EXPR_PARENTHESESL,EXPR_PARENTHESESR,
                    EXPR_POINT,EXPR_OPERATOR,EXPR_FAILURE};
//表达式括号匹配，成功返回1，否则返回0
int ParenthesesMatchP(char* expr)
{
    if(expr==NULL)
       return 0;
    //初始化堆栈
    Stack* stack;
    StackInit(&stack,TYPE_CHAR,strlen(expr));
    char c = '(';//用作拷贝对象
    while(*expr!='#' && *expr!='\0')//表达式结束之前
    {
        if(*expr == '(')//括号入栈
        {
            push(stack,&c);
        }else if(*expr == ')')
        {
            if((char*)pop(stack)==NULL)//遇到右括号而栈中没有相应的左括号说明匹配失败。
            {
                DestroyStack(stack);//清理堆栈
                return 0;
            }
        }
        expr++;//遍历
    }
    if(pop(stack)!=NULL)//匹配结束之后栈中仍然有左括号说明匹配失败
    {
        DestroyStack(stack);//清理堆栈
        return 0;
    }
    //否则匹配成功
    DestroyStack(stack);//清理堆栈
    return 1;
}

int numberP(char c)
{
    return c >= '0' && c <= '9';
}
int parenthesesP(char c)//左括号返回1，右括号返回2
{
    return c == '('?1:(c == ')'?2:0);
}
int pointP(char c)
{
    return c == '.';
}
int operatorP(char c)//'+' '-'同优先级 '*' '/' 同优先级
{
    switch(c)
    {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
    return 0;
}
//表达式合法性检查,合法返回，否则返回0
int ExprLegalP(char* expr)
{

    enum STATE state = EXPR_INITIAL;
    if(!ParenthesesMatchP(expr))
        return 0;
    while(*expr!='\0')
    {
        if(*expr == ' ')//忽略空格
        {
            expr++;
            continue;
        }
        switch(state)
        {
            case EXPR_INITIAL://initial
                //表达式首字符只能是数字和左括号
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(parenthesesP(*expr) == 1)
                    state = EXPR_PARENTHESESL;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_NUMBER://number
                //数字后面只能跟数字、小数点、操作符、右括号和结束符
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(pointP(*expr))
                    state = EXPR_POINT;
                else if(parenthesesP(*expr) == 2)
                    state = EXPR_PARENTHESESR;
                else if(operatorP(*expr))
                    state = EXPR_OPERATOR;
                else if(*expr == '#')//结束且合法
                    return 1;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_PARENTHESESL://parenthesesL
                //左括号后面只能跟数字
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_PARENTHESESR:
                //右括号后面只能跟操作符和结束符
                if(operatorP(*expr))
                    state = EXPR_OPERATOR;
                else if(*expr == '#')//结束且合法
                    return 1;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_POINT://point
                //小数点后面只能跟数字
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_OPERATOR://operator
                //操作符后面只能跟数字和左括号
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(parenthesesP(*expr) == 1)
                    state = EXPR_PARENTHESESL;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_FAILURE://failure
                //表达式不合法返回0
                return 0;
        }
        expr++;
    }
    return 0;
}
char* ExprMidToPost(char* expr)
{
    Stack *s;
    StackInit(&s,TYPE_CHAR,strlen(expr));
    //p用于保存转换的结果
    char* p = (char*)malloc(sizeof(char)*strlen(expr)*3);//考虑到空格的插入所以长度加2倍
    memset(p,0,strlen(expr)*3);
    int i = 0;
    char c = '(';
    while(*expr != '\0')
    {
        if(numberP(*expr))
        {
            //在连续的数字前和后插入空格以
            *(p+i) = ' ';
            i++;
            while(*expr!='#'&&*expr!='\0'&&(numberP(*expr)||pointP(*expr)))
            {
                *(p+i) = *expr;//数字直接输出到结果
                i++;
                expr++;
            }
            *(p+i) = ' ';
            i++;
        }
        if(operatorP(*expr))
        {
            while((char*)touch(s)!=NULL&&*(char*)touch(s)!='(')//在左括号之前的高优先级操作符出栈
            {
                if(operatorP(*(char*)touch(s))<operatorP(*expr))
                {
                    break;
                }else
                {
                    *(p+i) = *(char*)pop(s);
                    i++;
                }
            }
            push(s,expr);//当前操作符入栈
        }else if(parenthesesP(*expr)==1)//左括号入栈
            push(s,&c);
        else if(parenthesesP(*expr) == 2)
        {
            while(*(char*)touch(s)!='(')//遇到右括号将左括号之前的操作符出栈
            {
                *(p+i) = *(char*)pop(s);
                i++;
            }
            pop(s);//左括号出栈
        }
        else if(*expr == '#')//遇到结束符跳转换
            break;
        expr++;
    }
    while((char*)touch(s)!=NULL)//将栈中剩余操作符出栈
    {
        *(p+i) = *(char*)pop(s);
        i++;
    }
    DestroyStack(s);
    return p;
}
float ExprExecute(char* expr)
{
    float rst;
    Stack* stack;
    StackInit(&stack,TYPE_FLOAT,strlen(expr));
    float labmda = 0;
    while(*expr != '\0')
    {
        if(numberP(*expr))
        {
            sscanf(expr,"%f",&labmda);//获取操作数
            push(stack,&labmda);//操作数入栈
            while(*expr!='\0'&&*expr!=' ')//指针移动到操作数结束
                expr++;
        }
        else if(operatorP(*expr))
        {
            switch(*expr)//计算
            {
            case '+': rst = *(float*)pop(stack)+(*(float*)pop(stack));break;
            case '-': rst = *(float*)pop(stack);rst = (*(float*)pop(stack))-rst;break;//注意顺序
            case '*': rst = *(float*)pop(stack)*(*(float*)pop(stack));break;
            case '/': rst = *(float*)pop(stack);rst = (*(float*)pop(stack))/rst;break;//注意顺序
            default: rst = 0;
            }
            push(stack,&rst);//结果入栈
        }
        expr++;
    }
    rst = *(float*)pop(stack);//最终结果出栈
    DestroyStack(stack);
    return rst;
}
float ExprCalc(FILE* file,int max_len)
{
    char* expr = (char*)malloc(sizeof(char)*max_len);
    memset(expr,0,max_len);
    fgets(expr,max_len,file);

    if(ExprLegalP(expr))
    {
        return ExprExecute(ExprMidToPost(expr));
    }
    else
    {
        printf("表达式不合法。\n");
    }
    return -1;
}








