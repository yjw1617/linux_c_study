#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#define DATABASE "stu.db"
void do_insert(sqlite3* db){
    int id;
    char name[32] = {0};
    int score;

    printf("Input id:");
    scanf("%d", &id);
    getchar();//回收垃圾字符

    printf("Input name:");
    scanf("%s", name);
    getchar();//回收垃圾字符

    printf("Input score:");
    scanf("%d", &score);
    getchar();//回收垃圾字符

    char sql[128] = {0};
    char* errmsg = NULL;
    sprintf(sql, "insert into stu values(%d, '%s', %d);", id, name, score);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }else{
        printf("insert success!\n");
    }
}

void do_delete(sqlite3* db){
   
    char name[32] = {0};
   

    printf("Input delete name:");
    scanf("%s", name);
    getchar();//回收垃圾字符

    char sql[128] = {0};
    char* errmsg = NULL;
    sprintf(sql, "delete from stu where name = '%s';",name);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }else{
        printf("delete success!\n");
    }
}


void do_update(sqlite3* db){
    char name[32] = {0};
    char upname[32] = {0};
    printf("Input name:");
    scanf("%s", name);
    getchar();//回收垃圾字符

    printf("Input update name:");
    scanf("%s", upname);
    getchar();//回收垃圾字符

    char sql[128] = {0};
    char* errmsg = NULL;
    sprintf(sql, "update stu set name='%s' where name='%s';", upname, name);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }else{
        printf("update success!\n");
    }
}

//和下面一个函数组成查询的第一种方法
int callback(void* para,int f_num,char** f_value,char** f_name){
    int i = 0;
    for(i = 0; i < f_num; i++){
        printf("%s-------", f_value[i]);
    }
    putchar(10);
    return 0;
}

int do_query(sqlite3* db){
    char sql[128] = {0};
    char* errmsg = NULL;
    char name[20] = {0};
    printf("input query name:");
    scanf("%s", name);
    getchar();

    sprintf(sql, "select * from stu where name = '%s';", name);
    if(sqlite3_exec(db, sql, callback, NULL, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }
}
//查询的第二种方法
int do_query1(sqlite3* db){
    char sql[128] = {0};
    char* errmsg = NULL;
    char name[20] = {0};
    printf("input query name:");
    scanf("%s", name);
    getchar();

    char** resultp;
    int row;//行数
    int column;//列数
    sprintf(sql, "select * from stu where name = '%s';", name);
    if(sqlite3_get_table(db, sql, &resultp, &row, &column, &errmsg) != SQLITE_OK){
        printf("%s\n", errmsg);
    }
    int i, j;
    int index = column;
    printf("%d\n", column);
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("%-11s", resultp[index++]);
        }
        putchar(10);
    }
}
int main(){
    sqlite3* db;
    if(sqlite3_open(DATABASE, &db) != SQLITE_OK){
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }else{
        printf("sqlite3 open success\n");
    }
    
    //创建一张数据库的表格
    char* errmsg;
    if(sqlite3_exec(db,                                  /* An open database */
    "create table stu(id Integer, name char, score Integer);",                           /* SQL to be evaluated */
    NULL,  /* Callback function *///当为查询语句的时候才调用
    NULL,                                    /* 1st argument to callback */
    &errmsg                              /* Error msg written here */
    ) != SQLITE_OK){
        printf("%s\n", errmsg);
    }else{
        printf("create table success\n");
    }
    char cmd = 0;
    while(1){
        printf("********************************************");
        printf("1:insert 2:delete 3:query 4:update 5:quit.\n");
        printf("********************************************");

        cmd = fgetc(stdin);
        getchar();
        switch (cmd)
        {
        case /* constant-expression */'1':
            do_insert(db);
            /* code */
            break;
        case /* constant-expression */'2':
            do_delete(db);
            /* code */
            break;
        case /* constant-expression */'3':
            // do_query(db);
            do_query1(db);
            /* code */
            break;
        case /* constant-expression */'4':
            do_update(db);
            /* code */
            break;
        case /* constant-expression */'5':
            sqlite3_close(db);
            exit(0);
            /* code */
            break;            
        default:
            printf("input err\n");
            break;
        }
    }
    return 0;
}