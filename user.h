#include <sqlite3.h>
#include  <iostream>
#include  <bits/stdc++.h>


using namespace std;

int new_user(string name_inp,string email_inp,string credit_limit_inp){
    string name=name_inp;
    string email=email_inp;
    string credit_limit=credit_limit_inp;

    string sql="insert into USER(NAME,EMAIL,CREDIT_LIMIT) \
        values('"+name+"','"+email+"',"+credit_limit+");";

    execute(sql);
    cout<<"user "<<name<<" created"<<endl;
    return 0;

}

list<double> report_dues(string user_inp){
    string sql = "select dues from user where name='"+user_inp+"';";
    list<list<string>> qres = execute(sql);
    list<double> ans;
    for(auto i=qres.begin();i!=qres.end();i++){
        for(auto j=(*i).begin();j!=(*i).end();j++){
            string tmp="";
            tmp+=((*j).front());
            ans.push_back(stod(tmp));
        }
    }
    return ans;
}

list<string> credit_limit_users(string user_inp){
    string sql="select name from user where credit_limit=dues";
    list<list<string>> qres = execute(sql);
    list<string> ans;
    for(auto i=qres.begin();i!=qres.end();i++){
        ans.push_back((*i).front());
    }
    return ans;
}

double total_dues(){
    string sql="select sum(dues) from user;";
    list<list<string>> qres=execute(sql);
    return stod(qres.front().front());
}

int payback(string user_inp,string amt_inp){
    string sql="update user set dues=((select dues from user where name='"+user_inp+"')-"+amt_inp+") where name='"+user_inp+"';";
    execute(sql);
    return 0;
}

