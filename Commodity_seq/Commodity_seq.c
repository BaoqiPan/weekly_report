#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME_LEN 50     //商品名称
#define MAX_ID_LEN 20       //商品编号
#define INIT_CAPACITY 100   //商品最大数量
#define WARNING_STOCK 10    //预警库存数量

#define MAX_USERS 2         //最大用户数量
#define MAX_USERNAME_LEN 20 //用户名长度
#define MAX_PASSWORD_LEN 20 //密码长度

typedef enum 
{
    ADMIN,
    USER,
    INVALID_USER
} UserRole;

typedef enum 
{
    ADMIN_USER,
    NORMAL_USER,
    INVALID_USERNAME
} Username;

typedef struct 
{
    UserRole role;
    char password[MAX_PASSWORD_LEN + 1]; // 密码长度为6，加1是为了存储字符串结束符
} User;

User users[MAX_USERS] = 
{
    {ADMIN, "000000"},
    {USER, "000000"}
};

/* 商品信息结构体 */
typedef struct
{
    char id[MAX_ID_LEN];      // 商品编号
    char name[MAX_NAME_LEN];  // 商品名称
    double price;            // 商品单价
    int stock;               // 当前库存
    time_t last_update;       // 最后更新时间
} Goods;


/* 顺序表结构体 */
typedef struct 
{
    Goods *items;    // 存储元素的数组
    int count;       // 当前实际存储数量
    int capacity;    // 顺序表可达最大存储容量
} SeqList;

// 函数声明
void InitList(SeqList *list);       // 初始化顺序表

void AddGoods(SeqList *list);       // 添加商品

void DeleteGoods(SeqList *list);    // 删除商品

void ModifyGoods(SeqList *list);    // 修改商品信息

void SearchGoods(SeqList *list);    // 查找商品

void ShowAll(SeqList *list);        // 显示所有商品

void LoadFromFile(SeqList *list);   // 文件加载

void SortMenu(SeqList *list);       // 排序菜单

void Statistics(SeqList *list);     // 统计功能

void ExpandCapacity(SeqList *list); // 扩容顺序表

int FindById(SeqList *list, const char *id);    // 根据编号查找商品

void PrintGoods(Goods *goods);      // 打印单个商品信息

void ClearInputBuffer();            // 清空输入缓冲区

void DisplayMenu();                 // 显示菜单

void StockWarning(SeqList *list);   // 库存预警

void GenerateReport(SeqList *list); // 生成报表

void BackupData(SeqList *list);     // 数据备份

void RestoreData(SeqList *list);    // 数据恢复

void LogUpdate(const char *action, Goods *goods); // 日志记录

void ViewLog();                     // 查看日志 

UserRole Login(User *logged_in_user); // 用户登录

Username GetUsername(const char *input_username); // 获取用户名

void ChangePassword(User *logged_in_user); // 修改密码

void SaveUserData();                // 保存用户数据（密码）

void LoadUserData();                // 加载用户数据（密码）

void showmain();                    // 显示使用指南

// 全局变量
const char *filename = "goods.dat";
const char *backupfile = "goods_backup.dat";
const char *user_data_file = "user_data.dat";

int main() 
{
    SeqList list;
    User current_user;
    UserRole role;
    int choice;

    showmain();
    LoadUserData(); // 加载用户数据

    InitList(&list);

    role = Login(&current_user);
    if (role == INVALID_USER) 
    {
        return 1; // 登录失败，退出程序
    }

    do {
        DisplayMenu();
        printf("请输入选项：");
        scanf("%d", &choice);
        ClearInputBuffer();

        switch(choice) 
        {
            case 1: 
                if (role == ADMIN) 
                {
                    AddGoods(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 2: 
                if (role == ADMIN) 
                {
                    DeleteGoods(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 3: 
                if (role == ADMIN) 
                {
                    ModifyGoods(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 4: SearchGoods(&list); break;
            case 5: ShowAll(&list); break;
            case 6: 
                if (role == ADMIN) 
                {
                    SortMenu(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 7: Statistics(&list); break;
            case 8: StockWarning(&list); break;
            case 9: GenerateReport(&list); break;
            case 10: 
                if (role == ADMIN) 
                {
                    BackupData(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 11: 
                if (role == ADMIN) 
                {
                    RestoreData(&list);
                } 
                else 
                {
                    printf("权限不足，无法执行此操作！\n");
                }
                break;
            case 12: ViewLog(); break;
            case 13: 
                ChangePassword(&current_user);
                break;
            case 0: 
                if (role == ADMIN) 
                {
                    BackupData(&list);
                    printf("数据已保存，退出系统！\n");
                } 
                else 
                {
                    printf("退出系统！\n");
                }
                SaveUserData(); // 保存用户数据
                break;
            default:
                printf("无效选项，请重新输入！\n");
        }
    } while(choice != 0);

    free(list.items);
    return 0;
}

// 初始化顺序表
void InitList(SeqList *list) 
{
    list->items = (Goods*)malloc(INIT_CAPACITY * sizeof(Goods));//最大存储个数乘以单个商品内容大小
    if(!list->items) 
    {
        printf("内存分配失败！\n");
        exit(1);
    }
    list->count = 0;    //商品个数赋值为0
    list->capacity = INIT_CAPACITY;//最大存储个数
}

// 扩容顺序表
void ExpandCapacity(SeqList *list) 
{
    int new_capacity;
    Goods *new_items;

    new_capacity = list->capacity * 2;
    new_items = (Goods*)realloc(list->items, new_capacity * sizeof(Goods)); //ralloc函数会尝试扩展现有的内存块，
    if(!new_items)                                                                 //如果无法扩展，它会分配一个新的内存块，
    {                                                                  //并将旧内存块中的数据复制到新内存块中。然后释放旧的内存块。
        printf("内存扩容失败！\n");                                     //所以数据不会丢失
        return;
    }
    list->items = new_items;
    list->capacity = new_capacity;
    printf("顺序表已扩容至%d\n", new_capacity);
}

//添加商品
void AddGoods(SeqList *list) 
{
    Goods new_goods;

    if(list->count >= list->capacity) 
    {
        ExpandCapacity(list);
    }

    printf("请输入商品编号：");
    fgets(new_goods.id, MAX_ID_LEN, stdin);
    new_goods.id[strcspn(new_goods.id, "\n")] = '\0';

    if(FindById(list, new_goods.id) != -1) 
    {
        printf("该商品编号已存在！\n");
        return;
    }

    printf("请输入商品名称：");
    fgets(new_goods.name, MAX_NAME_LEN, stdin);
    new_goods.name[strcspn(new_goods.name, "\n")] = '\0';

    printf("请输入商品价格：");
    while(scanf("%lf", &new_goods.price) != 1 || new_goods.price <= 0) 
    {
        printf("价格无效，请重新输入：");
        ClearInputBuffer();
    }

    printf("请输入库存数量：");
    while(scanf("%d", &new_goods.stock) != 1 || new_goods.stock < 0) 
    {
        printf("库存无效，请重新输入：");
        ClearInputBuffer();
    }
    ClearInputBuffer();

    new_goods.last_update = time(NULL);

    list->items[list->count++] = new_goods;
    printf("商品添加成功！\n");

    // 记录日志
    LogUpdate("添加商品", &new_goods);
}

// 删除商品
void DeleteGoods(SeqList *list) 
{
    char id[MAX_ID_LEN];
    int index;
    Goods deleted_goods;
    int i;

    printf("请输入要删除的商品编号：");
    fgets(id, MAX_ID_LEN, stdin);
    id[strcspn(id, "\n")] = '\0';

    index = FindById(list, id);
    if(index == -1) 
    {
        printf("未找到该商品！\n");
        return;
    }

    deleted_goods = list->items[index];

    for(i = index; i < list->count-1; i++) 
    {
        list->items[i] = list->items[i+1];
    }
    list->count--;
    printf("商品删除成功！\n");

    // 记录日志
    LogUpdate("删除商品", &deleted_goods);
}

// 修改商品信息
void ModifyGoods(SeqList *list) 
{
    char id[MAX_ID_LEN];
    int index;
    Goods *g;
    Goods old_goods;
    char new_name[MAX_NAME_LEN];
    double new_price;
    int adjustment;

    printf("请输入要修改的商品编号：");
    fgets(id, MAX_ID_LEN, stdin);
    id[strcspn(id, "\n")] = '\0';

    index = FindById(list, id);
    if(index == -1) 
    {
        printf("未找到该商品！\n");
        return;
    }

    g = &list->items[index];
    printf("当前信息：\n");
    PrintGoods(g);

    old_goods = *g; // 记录修改前的商品信息

    printf("请输入新商品名称（直接回车保留原值）：");
    fgets(new_name, MAX_NAME_LEN, stdin);
    if(new_name[0] != '\n') 
    {
        new_name[strcspn(new_name, "\n")] = '\0';
        strcpy(g->name, new_name);
    }

    printf("请输入新价格（负数保留原值）：");
    if(scanf("%lf", &new_price) == 1 && new_price > 0) 
    {
        g->price = new_price;
    }
    ClearInputBuffer();

    printf("请输入库存调整量（+/-）：");
    if(scanf("%d", &adjustment) == 1) 
    {
        if(g->stock + adjustment >= 0) 
        {
            g->stock += adjustment;
        } 
        else 
        {
            printf("调整后库存不能为负！\n");
        }
    }
    ClearInputBuffer();

    g->last_update = time(NULL);

    printf("商品信息修改成功！\n");

    // 记录日志
    LogUpdate("修改商品", g);
}

// 查找商品
int FindById(SeqList *list, const char *id) 
{
    int i;
    for(i = 0; i < list->count; i++) 
    {
        if(strcmp(list->items[i].id, id) == 0) 
        {
            return i;
        }
    }
    return -1;
}

// 显示所有商品
void ShowAll(SeqList *list) 
{
    int i;
    if(list->count == 0) 
    {
        printf("库存为空！\n");
        return;
    }

    printf("\n%-19s%-24s%-12s%-10s%-25s\n", "商品编号", "商品名称", "价格", "库存", "最后更新时间");

    for(i = 0; i < list->count; i++) 
    {
        PrintGoods(&list->items[i]);
    }
    printf("共%d件商品\n", list->count);
}

// 打印单个商品信息
void PrintGoods(Goods *goods)
{
    struct tm *tm_info;
    char time_str[20];

    tm_info = localtime(&goods->last_update);
    strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    
    printf("%-15s%-20s%-10.2f%-8d%-20s\n", goods->id, goods->name, goods->price, goods->stock, time_str);
}

void DisplayMenu() {
    printf("\n======== 库存管理系统 ========\n");
    printf("1. 添加商品\n");
    printf("2. 删除商品\n");
    printf("3. 修改商品\n");
    printf("4. 查询商品\n");
    printf("5. 显示所有\n");
    printf("6. 排序功能\n");
    printf("7. 统计分析\n");
    printf("8. 库存预警\n");
    printf("9. 生成报表\n");
    printf("10. 数据备份\n");
    printf("11. 数据恢复\n");
    printf("12. 查看日志\n");
    printf("13. 修改密码\n");
    printf("0. 退出系统\n");
    printf("==============================\n");
}

void ClearInputBuffer() 
{
    int c;

    while((c = getchar()) != '\n' && c != EOF);
}


/* 新增比较函数类型 */
typedef int (*CompareFunc)(const void*, const void*);

/* 排序比较函数 */
int CompareIDAsc(const void *a, const void *b) 
{
    return strcmp(((Goods*)a)->id, ((Goods*)b)->id);
}

int CompareIDDesc(const void *a, const void *b) 
{
    return -CompareIDAsc(a, b);
}

int CompareNameAsc(const void *a, const void *b) 
{
    return strcmp(((Goods*)a)->name, ((Goods*)b)->name);
}

int CompareNameDesc(const void *a, const void *b) 
{
    return -CompareNameAsc(a, b);
}

int ComparePriceAsc(const void *a, const void *b) 
{
    return (int)(((Goods*)a)->price - ((Goods*)b)->price);
}

int ComparePriceDesc(const void *a, const void *b) 
{
    return -ComparePriceAsc(a, b);
}

int CompareStockAsc(const void *a, const void *b) 
{
    return ((Goods*)a)->stock - ((Goods*)b)->stock;
}

int CompareStockDesc(const void *a, const void *b) 
{
    return -CompareStockAsc(a, b);
}

/* 排序菜单 */
void SortMenu(SeqList *list) 
{
    int choice, order;
    CompareFunc compare = NULL;

    if(list->count == 0) 
    {
        printf("库存为空，无需排序！\n");
        return;
    }

    printf("\n==== 排序选项 ====\n");
    printf("1. 按商品编号排序\n");
    printf("2. 按商品名称排序\n");
    printf("3. 按商品价格排序\n");
    printf("4. 按库存数量排序\n");
    printf("请选择排序字段：");
    scanf("%d", &choice);
    ClearInputBuffer();

    printf("\n==== 排序顺序 ====\n");
    printf("1. 升序排序\n");
    printf("2. 降序排序\n");
    printf("请选择排序顺序：");
    scanf("%d", &order);
    ClearInputBuffer();

    switch(choice) 
    {
        case 1:
            compare = (order == 1) ? CompareIDAsc : CompareIDDesc;
            break;
        case 2:
            compare = (order == 1) ? CompareNameAsc : CompareNameDesc;
            break;
        case 3:
            compare = (order == 1) ? ComparePriceAsc : ComparePriceDesc;
            break;
        case 4:
            compare = (order == 1) ? CompareStockAsc : CompareStockDesc;
            break;
        default:
            printf("无效选项！\n");
            return;
    }

    qsort(list->items, list->count, sizeof(Goods), compare);

    printf("排序完成！\n");

    ShowAll(list);
}

/* 统计功能 */
void Statistics(SeqList *list) 
{
    double total_value;
    int total_stock;
    double max_price;
    double min_price;
    int warning_count;
    int i;

    if(list->count == 0) 
    {
        printf("库存为空！\n");
        return;
    }

    total_value = 0;
    total_stock = 0;
    max_price = list->items[0].price;
    min_price = list->items[0].price;
    warning_count = 0;

    for(i = 0; i < list->count; i++) 
    {
        Goods *g = &list->items[i];

        total_value += g->price * g->stock;
        total_stock += g->stock;
        
        if(g->price > max_price) max_price = g->price;
        if(g->price < min_price) min_price = g->price;
        if(g->stock < WARNING_STOCK) warning_count++;
    }

    printf("\n==== 统计信息 ====\n");
    printf("商品种类总数：%d\n", list->count);
    printf("库存总量：%d\n", total_stock);
    printf("库存总价值：%.2f\n", total_value);
    printf("最高价格：%.2f\n", max_price);
    printf("最低价格：%.2f\n", min_price);
    printf("平均价格：%.2f\n", total_value / total_stock);
    printf("需补货商品数：%d\n", warning_count);
}

/* 查询功能 */
void SearchGoods(SeqList *list) 
{
    int choice;

    printf("\n==== 查询方式 ====\n");
    printf("1. 按编号查询\n");
    printf("2. 按名称查询\n");
    printf("请选择查询方式：");
    scanf("%d", &choice);
    ClearInputBuffer();

    if(choice == 1) 
    {
        char id[MAX_ID_LEN];
        int index;

        printf("请输入商品编号：");

        fgets(id, MAX_ID_LEN, stdin);
        
        id[strcspn(id, "\n")] = '\0';

        index = FindById(list, id);
        if(index == -1) 
        {
            printf("未找到该商品！\n");
        } 
        else 
        {
            printf("\n查询结果：\n");
            printf("\n%-19s%-24s%-12s%-10s%-25s\n", "商品编号", "商品名称", "价格", "库存", "最后更新时间");
            PrintGoods(&list->items[index]);
        }
    } 
    else if(choice == 2) 
    {
        char keyword[MAX_NAME_LEN];
        int found;
        int i;

        printf("请输入商品名称关键字：");
        fgets(keyword, MAX_NAME_LEN, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        printf("\n查询结果：\n");
        printf("\n%-19s%-24s%-12s%-10s%-25s\n", "商品编号", "商品名称", "价格", "库存", "最后更新时间");
        found = 0;
        for(i = 0; i < list->count; i++) 
        {
            if(strstr(list->items[i].name, keyword) != NULL) //强大的搜索字串功能
            {
                PrintGoods(&list->items[i]);
                found++;
            }
        }
        printf("共找到%d条相关记录\n", found);
    }
    else 
    {
        printf("无效选择！\n");
    }
}

void StockWarning(SeqList *list) 
{
    int warning_count;
    int i;

    if (list->count == 0) 
    {
        printf("库存为空，无需预警！\n");
        return;
    }

    printf("\n==== 库存预警 ====\n");
    printf("%-19s%-24s%-12s%-10s%-25s\n", "商品编号", "商品名称", "价格", "库存", "最后更新时间");

    warning_count = 0;
    for (i = 0; i < list->count; i++) 
    {
        if (list->items[i].stock < WARNING_STOCK) 
        {
            PrintGoods(&list->items[i]);
            warning_count++;
        }
    }

    if (warning_count == 0) 
    {
        printf("所有商品库存充足，无需预警。\n");
    } 
    else 
    {
        printf("共%d件商品需要补货。\n", warning_count);
    }
}

void GenerateReport(SeqList *list) 
{
    time_t now;
    struct tm *tm_info;
    char report_file[50];
    FILE *fp;
    double total_value;
    int total_stock;
    int i;

    if (list->count == 0) 
    {
        printf("库存为空，无法生成报表！\n");
        return;
    }

    // 获取当前时间并格式化为文件名
    now = time(NULL);
    tm_info = localtime(&now);
    strftime(report_file, 50, "report_%Y%m%d%H%M%S.txt", tm_info);

    // 打开文件进行写入
    fp = fopen(report_file, "w");
    if (!fp) 
    {
        printf("报表生成失败！\n");
        return;
    }

    // 写入报表头部信息
    fprintf(fp, "库存统计报告\n");
    fprintf(fp, "生成时间：%s\n", ctime(&now));
    fprintf(fp, "==============================\n");

    // 写入统计信息
    total_value = 0;
    total_stock = 0;
    for (i = 0; i < list->count; i++) 
    {
        total_value += list->items[i].price * list->items[i].stock;
        total_stock += list->items[i].stock;
    }

    fprintf(fp, "商品种类总数：%d\n", list->count);
    fprintf(fp, "库存总量：%d\n", total_stock);
    fprintf(fp, "库存总价值：%.2f\n\n", total_value);

    // 写入商品明细
    fprintf(fp, "商品明细：\n");
    fprintf(fp, "%-15s%-20s%-10s%-8s%-20s\n", "商品编号", "商品名称", "价格", "库存", "最后更新时间");
    for (i = 0; i < list->count; i++) 
    {
        struct tm *tm_info = localtime(&list->items[i].last_update);
        char time_str[20];
        strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(fp, "%-15s%-20s%-10.2f%-8d%-20s\n", 
                list->items[i].id, 
                list->items[i].name,
                list->items[i].price,
                list->items[i].stock,
                time_str);
    }

    fclose(fp);
    printf("报表已生成：%s\n", report_file);
}

void BackupData(SeqList *list) 
{
    FILE *fp;

    fp = fopen(backupfile, "wb");
    if (!fp) 
    {
        printf("备份文件打开失败！\n");
        return;
    }

    if (fwrite(&list->count, sizeof(int), 1, fp) != 1) 
    {
        printf("写入商品数量失败！\n");
        fclose(fp);
        return;
    }

    if (fwrite(list->items, sizeof(Goods), list->count, fp) != list->count) 
    {
        printf("写入商品数据失败！\n");
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("数据已备份至%s\n", backupfile);
}

void RestoreData(SeqList *list) 
{
    FILE *fp;
    int count;

    fp = fopen(backupfile, "rb");
    if (!fp) 
    {
        printf("备份文件打开失败！\n");
        return;
    }

    if (fread(&count, sizeof(int), 1, fp) != 1) 
    {
        printf("读取商品数量失败！\n");
        fclose(fp);
        return;
    }

    while (list->capacity < count) 
    {
        ExpandCapacity(list);
    }

    if (fread(list->items, sizeof(Goods), count, fp) != count) 
    {
        printf("读取商品数据失败！\n");
        fclose(fp);
        return;
    }

    list->count = count;
    fclose(fp);
    printf("数据已从%s恢复\n", backupfile);
}

void LogUpdate(const char *action, Goods *goods) 
{
    FILE *fp;
    struct tm *tm_info;
    char time_str[20];
    const char *logfile = "update_log.txt";

    fp = fopen(logfile, "a");
    if (!fp) 
    {
        printf("日志文件打开失败！\n");
        return;
    }

    tm_info = localtime(&goods->last_update);
    strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(fp, "%s: %s, %s, %.2f, %d, %s\n", 
            action, 
            goods->id, 
            goods->name,
            goods->price,
            goods->stock,
            time_str);

    fclose(fp);
}

void ViewLog() 
{
    const char *logfile = "update_log.txt"; // 日志文件名
    FILE *fp;
    char line[256];

    fp = fopen(logfile, "r");

    if (!fp) 
    {
        printf("日志文件打开失败！\n");
        return;
    }

    printf("\n==== 操作日志 ====\n");

    while (fgets(line, sizeof(line), fp)) 
    {
        printf("%s", line);
    }

    fclose(fp);
}

Username GetUsername(const char *input_username) 
{
    if (strcmp(input_username, "admin") == 0) 
    {
        return ADMIN_USER;
    } 
    else if (strcmp(input_username, "user") == 0) 
    {
        return NORMAL_USER;
    } 
    else 
    {
        return INVALID_USERNAME;
    }
}


UserRole Login(User *logged_in_user) 
{
    char input_username[MAX_NAME_LEN];
    char input_password[MAX_PASSWORD_LEN + 1];
    Username username;

    printf("请输入用户名：");
    scanf("%s", input_username);

    printf("请输入密码：");
    scanf("%s", input_password);

    username = GetUsername(input_username);

    if (username != INVALID_USERNAME && strcmp(input_password, users[username].password) == 0) 
    {
        *logged_in_user = users[username];
        return users[username].role;
    }

    printf("用户名或密码错误！\n");

    return INVALID_USER;
}

void ChangePassword(User *logged_in_user) 
{
    char new_password[MAX_PASSWORD_LEN + 1];
    int i;

    printf("请输入新密码（6位）：");

    scanf("%6s", new_password);

    // 修改当前用户的密码
    strcpy(logged_in_user->password, new_password);

    // 同步到 users 数组中
    for (i = 0; i < MAX_USERS; i++) 
    {
        if (users[i].role == logged_in_user->role) 
        {
            strcpy(users[i].password, new_password);
            break;
        }
    }

    printf("密码修改成功！\n");
}

void SaveUserData() 
{
    FILE *fp;

    fp = fopen(user_data_file, "wb");
    if (!fp) 
    {
        printf("用户数据文件打开失败！\n");
        return;
    }

    if (fwrite(users, sizeof(User), MAX_USERS, fp) != MAX_USERS) 
    {
        printf("写入用户数据失败！\n");
    }

    fclose(fp);
    printf("用户数据已保存至%s\n", user_data_file);
}

void LoadUserData() 
{
    FILE *fp;

    fp = fopen(user_data_file, "rb");
    if (!fp) 
    {
        printf("用户数据文件打开失败，使用默认用户数据。\n");
        printf("用户名：admin/user，初始密码：000000\n");
        return;
    }

    if (fread(users, sizeof(User), MAX_USERS, fp) != MAX_USERS) 
    {
        printf("读取用户数据失败，使用默认用户数据。\n");
    }

    fclose(fp);
    printf("用户数据已从%s加载\n", user_data_file);
}

void showmain()
{
    printf("欢迎使用库存管理系统！\n");
    printf("使用指南：\n");
    printf("1. 管理员账号：admin，初始密码：000000\n");
    printf("2. 普通用户账号：user，初始密码：000000\n");
    printf("3. 管理员账号可进行所有操作，普通用户账号仅可查询商品信息\n");
    printf("4. 操作日志将记录在update_log.txt文件中\n");
    printf("5. 用户数据（包括密码）将保存在user_data.dat文件中\n");
    printf("6. 商品数据将保存在goods.dat文件中\n");
    printf("7. 数据备份文件为goods_backup.dat\n");
    printf("8. 报表文件将生成在当前目录下\n");
    printf("9. 欢迎使用！\n");
}