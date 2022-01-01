#include<string>
#include <stdio.h>
#include <sqlite3.h>
#include <cstring>
#include  <bits/stdc++.h>

using namespace std;

sqlite3* db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int connect(){
    int rc;

    rc=sqlite3_open("simpl",&db);
    if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   return 0;

   
}

int create_tables(){

    int rc=0;
    char *zErrMsg = 0;
    string sql ;

   
    //creating user table if it dont exist already

    sql = "CREATE TABLE IF NOT EXISTS USER("  
      "ID INTEGER PRIMARY KEY     AUTOINCREMENT," 
      "NAME           TEXT    NOT NULL," 
      "EMAIL          TEXT    NOT NULL," 
      "CREDIT_LIMIT   REAL    NOT NULL," 
      "DUES           REAL    DEFAULT 0 );";

   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error in creating user table: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "user table created successfully\n");
   }

    //creating merchant table if it doesnt already exist

   sql = "CREATE TABLE IF NOT EXISTS MERCHANT(" 
      "ID INTEGER PRIMARY KEY    AUTOINCREMENT," 
      "NAME           TEXT    NOT NULL," 
      "DISCOUNT   REAL    DEFAULT 0," 
      "DISCOUNT_TILL_DATE           REAL    DEFAULT 0 );";

   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error in creating merchant table: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "merchant table created successfully\n");
   }

   //creating transaction table if it doesnt already exist

   sql = "CREATE TABLE IF NOT EXISTS TXN(" 
      "ID INTEGER PRIMARY KEY     AUTOINCREMENT," 
      "USER           TEXT    NOT NULL," 
      "MERCHANT       TEXT    NOT NULL," 
      "AMOUNT         REAL    DEFAULT 0 );";

   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error in creating transaction table: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "transaction table created successfully\n");
   }

    return 0;
}

list<list<string>>  execute(string sql_inp){

    int rc;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;

   const char* sql = sql_inp.c_str();
   rc = sqlite3_prepare(db,sql,strlen(sql),&stmt,NULL);

   if (rc != SQLITE_OK)
	{
	  sqlite3_finalize (stmt);
	}
   list<list<string>> qres;
   while (sqlite3_step (stmt) == SQLITE_ROW)
	{
     list<string> rres;
     int colcnt = sqlite3_column_count(stmt);
	  const unsigned char *text;
     for(int i=0;i<colcnt;i++){
        const unsigned char* tmp1=reinterpret_cast<const unsigned char*>("NULL");
        text = sqlite3_column_text(stmt, i)? sqlite3_column_text(stmt, i) : tmp1;
        string tmp=std::string(reinterpret_cast<const char*>(text));
        rres.push_back(tmp);
     }
	  text = sqlite3_column_text (stmt, 1);
	  qres.push_back(rres);
	}
   sqlite3_finalize (stmt);
      
   return qres;

}
