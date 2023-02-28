#include "double_linklist.h"
account acc;
char name;
int dangnhap = 0;
int soLanNhapSai = 0;

void readfile()
{
    FILE *f1;
    f1 = fopen("account.txt", "r");
    if (f1 == NULL)
    {
        printf("Khong the mo file!!!");
        exit(0);
    }

    while (!feof(f1))
    {
        fscanf(f1, "%s %s %d\n", acc.username, acc.pass, &acc.status);
        append(acc);
    }
    fclose(f1);
}

void printData(Node *p)
{
    printf("username: %s\nstatus: %d\n", (p->ele).username, (p->ele).status);
}

void addAcc()
{
    printf("Nhap tai khoan: \n");
    printf("username: ");
    scanf("%s%*c", acc.username);
    Node *tmp;
    for (tmp = root; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->ele.username, acc.username) == 0)
        {
            printf("Tai khoan da ton tai!!\n");
            return;
        }
    }
    printf("password: ");
    scanf("%s%*c", acc.pass);
    acc.status = 1;
    append(acc);
    printf("Them tai khoan thanh cong\n");
    saveToFile();
}
void saveToFile()
{
    FILE *f = fopen("account.txt", "w");
    if (f == NULL)
    {
        printf("Loi cap nhat file.\n");
        return;
    }

    for (cur = root; cur != NULL; cur = cur->next)
    {
        fprintf(f, "%s %s %d\n", cur->ele.username, cur->ele.pass, cur->ele.status);
    }
    fclose(f);
}
void signIn()
{
    int a = 0;
    printf("\nNhap tai khoan: \n");
    printf("username: ");
    scanf("%s%*c", acc.username);

    Node *tmp;
    for (tmp = root; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->ele.username, acc.username) == 0)
        {
            a = -1;
            if (tmp->ele.status == 0)
            {
                printf("Account blocked\n");
                return;
            }

            printf("password: ");
            scanf("%s%*c", acc.pass);

            if (strcmp(tmp->ele.pass, acc.pass) == 0)
            {
                soLanNhapSai = 0;
                printf("Dang nhap thanh cong\n");
                printf("---------------------------------");
                name = tmp->ele.username;
                dangnhap = -1;
                return;
            }

            printf("Sai mat khau!\n");
            soLanNhapSai++;
            if (soLanNhapSai == 3)
            {
                printf("Account is blocked\n");
                tmp->ele.status = 0;
                saveToFile();
                return;
            }
        }
    }
    if (a == 0)
    {
        printf("Tai khoan khong ton tai!");
        return;
    }
}

void search()
{
    int a = 0;
    printf("Ten tai khoan: ");
    scanf("%s", acc.username);
    if (dangnhap == 0)
    {
        printf("Tai khoan chua dang nhap!\n");
        return;
    }
    else
    {
        Node *tmp;
        for (tmp = root; tmp != NULL; tmp = tmp->next)
        {
            if (strcmp(tmp->ele.username, acc.username) == 0)
            {
                a = -1;
                printData(tmp);
                return;
            }
        }
        if (a == 0)
        {
            printf("Tai khoan chua ton tai!\n");
            return;
        }
    }
}

void signOut()
{
    printf("Ten tai khoan: ");
    scanf("%s", acc.username);
    if (dangnhap == 0 || acc.username != name)
    {
        printf("Tai khoan chua dang nhap!");
        return;
    }
    else
    {
        Node *tmp;
        for (tmp = root; tmp != NULL; tmp = tmp->next)
        {
            if (strcmp(tmp->ele.username, acc.username) == 0)
            {
                printf("Goodbye <3");
                return;
            }
        }
    }
}

int main()
{
    int x;
    readfile();

    while (1)
    {
        printf("\nUSER MANAGEMENT PROGRAM\n");
        printf("---------------------------------\n");
        printf("1. Register\n"
               "2. Sign in\n"
               "3. Search\n"
               "4. Sign out\n"
               "Your choice (1-4, other to quit): ");
        scanf("%d%*c", &x);

        switch (x)
        {
        case 1:
            addAcc();
            break;
        case 2:
            signIn();
            break;
        case 3:
            search();
            break;
        case 4:
            signOut();
            break;

        default:
            Free(root);
            exit(0);
        }
    }
    return 0;
}