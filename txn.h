#include <sqlite3.h>
#include  <iostream>
#include  <bits/stdc++.h>



int txn(string user,string merchant, string amount){
    string sql="select credit_limit-dues from user where name='"+user+"';";
    list<list<string>> qres=execute(sql);
    double amt_left = stod((qres).front().front());
    if(stod(amount)>amt_left) {
        cout<<"transaction cancelled , amout exceeds credit limit"<<endl;
    }
    else{
        string sql="";
        double discount=0;
        list<list<string>> qres;
        sql="update user set dues=("+amount+"+(select dues from user where name='"+user+"')) where name='"+user+"';";
        execute(sql);

        sql="select discount from merchant where name='"+merchant+"';";
        qres=execute(sql);
        discount=(stod(qres.front().front())*stod(amount))/100;
        cout<<"discount : "<<discount<<endl;

        sql="update merchant set discount_till_date=("+to_string(discount)+"+(select discount_till_date from merchant where name='"+merchant+"')) where name='"+merchant+"';";
        execute(sql);

        sql="insert into txn(user,merchant,amount) values('"+user+"','"+merchant+"',"+amount+");";
        execute(sql);
    }
    return 0;
}

