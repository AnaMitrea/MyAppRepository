#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define max 10000

static int callback(void* str, int argc, char** argv, char** azColName);
static int callback_insert (void *str, int argc, char **argv, char **azColName);

static int callback(void* str, int argc, char** argv, char** azColName)
{
    int i;
    char* data = (char*) str;
  
    for (i = 0; i < argc; i++) 
    {
        strcat(data, azColName[i]);
        strcat(data, " = ");

        if(argv[i])
        {
            //printf("%s = %s\n", azColName[i], argv[i]);
            strcat(data, argv[i]);
        }
        else
        {
            //printf("%s = %s\n", azColName[i], "-");
            strcat(data,"-");
        }
        strcat(data,"\n");
    }
  
    //printf("\n");
    strcat(data, "\n");
    return 0;
}

static int callback_insert (void *str, int argc, char **argv, char **azColName)
{
    int i;
    char* data = (char*) str;

    for (i = 0; i < argc; i++)
    {
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "-");
    }
    return 0;
}


int main(int argc, char* argv[])
{
    sqlite3* db;
    int rc = sqlite3_open("SoftwareApp.db", &db);

    if (rc)	//verificare conexiune
    {
        fprintf(stderr, "error: %s\n", sqlite3_errmsg (db));
        return 0;
    }
    else
        printf("Conexiune Ok \n");


    char* zErrMsg; // mesaju de eroare
    char str[max]; // primul argument din callback
    char sql[max]; // interogarea sql
    char result[max];

    /* resetare variabile*/
    memset (result, 0, sizeof (result));
    str[0]=0;
    sql[0]=0;
/*
    printf("------------PRIMUL SELECT--------- \n\n");

/* INTEROGARE CU SELECT*/
    sprintf(sql, "SELECT * FROM Application;");
    rc = sqlite3_exec(db,sql,callback,str, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        sprintf (result,"SQL error: %s\n", zErrMsg);
        sqlite3_free (zErrMsg);
    }
    else
    {
        str[strlen(str)-1]='\0';
        sprintf(result, "%s", str);
    }
    printf("%s", result);

    printf("\n--------------------------\n");

    /* resetare variabile*/
    memset (result, 0, sizeof (result));


/* INTEROGARE CU INSERT*/
    printf("------------LA INSERT-----------\n\n");
    char insertApp[6][max] = {"Facebook", "Fb Dev", "FB.deb", "Free", "Communication", "YES"};

    int k = 4; // de la id-ul 4 se vor insera

    sprintf(sql,"INSERT INTO Application VALUES(%d, '%s', '%s', '%s', '%s', '%s', '%s');", k, &insertApp[0][0], &insertApp[1][0], &insertApp[2][0], &insertApp[3][0], &insertApp[4][0], &insertApp[5][0]);

    rc = sqlite3_exec(db,sql,callback,str,&zErrMsg);

    if(rc != SQLITE_OK)
    {
        sprintf (result,"SQL error: %s\n", zErrMsg);
        sqlite3_free (zErrMsg);
    }
    else
    {
        printf("Done inserting\n");
        
        str[0]=0;

           // AFISARE ce am inserat
        printf("---------------AFISARE DUPA INSERARE------------ \n \n");
     
        sprintf(sql, "SELECT * FROM Application;");
        rc = sqlite3_exec(db,sql,callback,str, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            sprintf (result,"SQL error: %s\n", zErrMsg);
            sqlite3_free (zErrMsg);
        }
        else
        {
            str[strlen(str)-1] = '\0';
            sprintf(result, "%s", str);
        }
        printf("%s \n", result);
    }

    sqlite3_close(db);
    return 0;
}