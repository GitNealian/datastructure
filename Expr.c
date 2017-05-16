#include "Expr.h"
//״̬����7��״̬
enum STATE {EXPR_INITIAL,EXPR_NUMBER,EXPR_PARENTHESESL,EXPR_PARENTHESESR,
                    EXPR_POINT,EXPR_OPERATOR,EXPR_FAILURE};
//���ʽ����ƥ�䣬�ɹ�����1�����򷵻�0
int ParenthesesMatchP(char* expr)
{
    if(expr==NULL)
       return 0;
    //��ʼ����ջ
    Stack* stack;
    StackInit(&stack,TYPE_CHAR,strlen(expr));
    char c = '(';//������������
    while(*expr!='#' && *expr!='\0')//���ʽ����֮ǰ
    {
        if(*expr == '(')//������ջ
        {
            push(stack,&c);
        }else if(*expr == ')')
        {
            if((char*)pop(stack)==NULL)//���������Ŷ�ջ��û����Ӧ��������˵��ƥ��ʧ�ܡ�
            {
                DestroyStack(stack);//�����ջ
                return 0;
            }
        }
        expr++;//����
    }
    if(pop(stack)!=NULL)//ƥ�����֮��ջ����Ȼ��������˵��ƥ��ʧ��
    {
        DestroyStack(stack);//�����ջ
        return 0;
    }
    //����ƥ��ɹ�
    DestroyStack(stack);//�����ջ
    return 1;
}

int numberP(char c)
{
    return c >= '0' && c <= '9';
}
int parenthesesP(char c)//�����ŷ���1�������ŷ���2
{
    return c == '('?1:(c == ')'?2:0);
}
int pointP(char c)
{
    return c == '.';
}
int operatorP(char c)//'+' '-'ͬ���ȼ� '*' '/' ͬ���ȼ�
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
//���ʽ�Ϸ��Լ��,�Ϸ����أ����򷵻�0
int ExprLegalP(char* expr)
{

    enum STATE state = EXPR_INITIAL;
    if(!ParenthesesMatchP(expr))
        return 0;
    while(*expr!='\0')
    {
        if(*expr == ' ')//���Կո�
        {
            expr++;
            continue;
        }
        switch(state)
        {
            case EXPR_INITIAL://initial
                //���ʽ���ַ�ֻ�������ֺ�������
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(parenthesesP(*expr) == 1)
                    state = EXPR_PARENTHESESL;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_NUMBER://number
                //���ֺ���ֻ�ܸ����֡�С���㡢�������������źͽ�����
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(pointP(*expr))
                    state = EXPR_POINT;
                else if(parenthesesP(*expr) == 2)
                    state = EXPR_PARENTHESESR;
                else if(operatorP(*expr))
                    state = EXPR_OPERATOR;
                else if(*expr == '#')//�����ҺϷ�
                    return 1;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_PARENTHESESL://parenthesesL
                //�����ź���ֻ�ܸ�����
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_PARENTHESESR:
                //�����ź���ֻ�ܸ��������ͽ�����
                if(operatorP(*expr))
                    state = EXPR_OPERATOR;
                else if(*expr == '#')//�����ҺϷ�
                    return 1;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_POINT://point
                //С�������ֻ�ܸ�����
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_OPERATOR://operator
                //����������ֻ�ܸ����ֺ�������
                if(numberP(*expr))
                    state = EXPR_NUMBER;
                else if(parenthesesP(*expr) == 1)
                    state = EXPR_PARENTHESESL;
                else
                    state = EXPR_FAILURE;
                break;
            case EXPR_FAILURE://failure
                //���ʽ���Ϸ�����0
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
    //p���ڱ���ת���Ľ��
    char* p = (char*)malloc(sizeof(char)*strlen(expr)*3);//���ǵ��ո�Ĳ������Գ��ȼ�2��
    memset(p,0,strlen(expr)*3);
    int i = 0;
    char c = '(';
    while(*expr != '\0')
    {
        if(numberP(*expr))
        {
            //������������ǰ�ͺ����ո���
            *(p+i) = ' ';
            i++;
            while(*expr!='#'&&*expr!='\0'&&(numberP(*expr)||pointP(*expr)))
            {
                *(p+i) = *expr;//����ֱ����������
                i++;
                expr++;
            }
            *(p+i) = ' ';
            i++;
        }
        if(operatorP(*expr))
        {
            while((char*)touch(s)!=NULL&&*(char*)touch(s)!='(')//��������֮ǰ�ĸ����ȼ���������ջ
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
            push(s,expr);//��ǰ��������ջ
        }else if(parenthesesP(*expr)==1)//��������ջ
            push(s,&c);
        else if(parenthesesP(*expr) == 2)
        {
            while(*(char*)touch(s)!='(')//���������Ž�������֮ǰ�Ĳ�������ջ
            {
                *(p+i) = *(char*)pop(s);
                i++;
            }
            pop(s);//�����ų�ջ
        }
        else if(*expr == '#')//������������ת��
            break;
        expr++;
    }
    while((char*)touch(s)!=NULL)//��ջ��ʣ���������ջ
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
            sscanf(expr,"%f",&labmda);//��ȡ������
            push(stack,&labmda);//��������ջ
            while(*expr!='\0'&&*expr!=' ')//ָ���ƶ�������������
                expr++;
        }
        else if(operatorP(*expr))
        {
            switch(*expr)//����
            {
            case '+': rst = *(float*)pop(stack)+(*(float*)pop(stack));break;
            case '-': rst = *(float*)pop(stack);rst = (*(float*)pop(stack))-rst;break;//ע��˳��
            case '*': rst = *(float*)pop(stack)*(*(float*)pop(stack));break;
            case '/': rst = *(float*)pop(stack);rst = (*(float*)pop(stack))/rst;break;//ע��˳��
            default: rst = 0;
            }
            push(stack,&rst);//�����ջ
        }
        expr++;
    }
    rst = *(float*)pop(stack);//���ս����ջ
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
        printf("���ʽ���Ϸ���\n");
    }
    return -1;
}








