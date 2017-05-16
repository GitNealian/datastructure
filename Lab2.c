#include "Stack.h"
#include "Expr.h"
int Lab2()
{
    char str[] = "45+33*(36+54)+3.52*(45+47)#";
    char* s = ExprMidToPost(str);
    printf("下面是从已有字符串获得表达式的处理结果\n");
    printf("转换后的表达式：%s\n",s);
    printf("计算的结果为：%f\n",ExprExecute(s));

    FILE* file = fopen("C:\\Users\\nealian\\Documents\\data_structure\\DataStructure\\expr.txt","r");
    if(file!=NULL)
    {
        int i = 0;
        printf("\n下面是从文件中获得表达式的处理结果\n");
        while(!feof(file))
        {
            printf("文件中第%d个表达式的值为：%f\n",i,ExprCalc(file,20));
            i++;
        }
    }

    printf("\n下面是从标准输入输出获得表达式的处理结果\n");
    printf("请输入表达式（以#结束）\n");
    printf("计算的结果为：%f\n",ExprCalc(stdin,20));
//    char str2[][20] = {"3489","3+(8-#","e+f#","3+3*(5-3))#"," #",""};
//    int i;
//    for(i = 0; i < 6; i++)
//    {
//        if(ExprLegalP(str2[i]))
//        {
//            printf("表达式%d合法\n",i);
//        }else
//        {
//            printf("表达式%d不合法\n",i);
//        }
//    }
    getchar();
    return 0;
}
