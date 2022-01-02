
int txn(string user,string merchant, string amount){
    string sql="select credit_limit-dues from user where name='"+user+"';";
    list<list<string>> qres=execute_q(sql);
    double amt_left = stod((qres).front().front());
    if(stod(amount)>amt_left) {
        cout<<"rejected! (reason: credit limit)"<<endl;
    }
    else{
        string sql="";
        double discount=0;
        list<list<string>> qres;
        sql="update user set dues=("+amount+"+(select dues from user where name='"+user+"')) where name='"+user+"';";
        execute_q(sql);

        sql="select discount from merchant where name='"+merchant+"';";
        qres=execute_q(sql);
        discount=(stod(qres.front().front())*stod(amount))/100;
        cout<<"success!"<<endl;

        sql="update merchant set discount_till_date=("+to_string(discount)+"+(select discount_till_date from merchant where name='"+merchant+"')) where name='"+merchant+"';";
        execute_q(sql);

        sql="insert into txn(user,merchant,amount) values('"+user+"','"+merchant+"',"+amount+");";
        execute_q(sql);
    }
    return 0;
}

