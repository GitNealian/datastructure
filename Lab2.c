#include "Stack.h"
#include "Expr.h"
int Lab2()
{
    char str[] = "45+33*(36+54)+3.52*(45+47)#";
    char* s = ExprMidToPost(str);
    printf("�����Ǵ������ַ�����ñ��ʽ�Ĵ�����\n");
    printf("ת����ı��ʽ��%s\n",s);
    printf("����Ľ��Ϊ��%f\n",ExprExecute(s));

    FILE* file = fopen("C:\\Users\\nealian\\Documents\\data_structure\\DataStructure\\expr.txt","r");
    if(file!=NULL)
    {
        int i = 0;
        printf("\n�����Ǵ��ļ��л�ñ��ʽ�Ĵ�����\n");
        while(!feof(file))
        {
            printf("�ļ��е�%d�����ʽ��ֵΪ��%f\n",i,ExprCalc(file,20));
            i++;
        }
    }

    printf("\n�����Ǵӱ�׼���������ñ��ʽ�Ĵ�����\n");
    printf("��������ʽ����#������\n");
    printf("����Ľ��Ϊ��%f\n",ExprCalc(stdin,20));
//    char str2[][20] = {"3489","3+(8-#","e+f#","3+3*(5-3))#"," #",""};
//    int i;
//    for(i = 0; i < 6; i++)
//    {
//        if(ExprLegalP(str2[i]))
//        {
//            printf("���ʽ%d�Ϸ�\n",i);
//        }else
//        {
//            printf("���ʽ%d���Ϸ�\n",i);
//        }
//    }
    getchar();
    return 0;
}
