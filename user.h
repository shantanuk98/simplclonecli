
using namespace std;

int new_user(string name_inp,string email_inp,string credit_limit_inp){
    string name=name_inp;
    string email=email_inp;
    string credit_limit=credit_limit_inp;

    string sql="insert into USER(NAME,EMAIL,CREDIT_LIMIT) \
        values('"+name+"','"+email+"',"+credit_limit+");";

    execute_q(sql);
    cout<<name<<"("<<credit_limit_inp<<")"<<endl;
    return 0;

}

list<double> report_dues(string user_inp){
    string sql = "select dues from user where name='"+user_inp+"';";
    list<list<string>> qres = execute_q(sql);
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

list<string> credit_limit_users(){
    string sql="select name from user where credit_limit=dues";
    list<list<string>> qres = execute_q(sql);
    list<string> ans;
    for(auto i=qres.begin();i!=qres.end();i++){
        ans.push_back((*i).front());
    }
    return ans;
}

list<list<string>> total_dues(){
    string sql="select name,dues from user where dues>0;";
    list<list<string>> qres=execute_q(sql);
    sql="select 'total',sum(dues) from user;";
    list<list<string>> qres1=execute_q(sql);
    qres.push_back(qres1.front());
    return qres;
}

double payback(string user_inp,string amt_inp){
    string sql="update user set dues=((select dues from user where name='"+user_inp+"')-"+amt_inp+") where name='"+user_inp+"';";
    execute_q(sql);
    sql="select dues from user where name='"+user_inp+"';";
    list<list<string>> qres=execute_q(sql);
    return stod(qres.front().front());
}

int update_credit_limit(string user_inp,string new_credit_limit){
    string sql="update user set credit_limit="+new_credit_limit+" where name='"+user_inp+"';";
    execute_q(sql);
    cout<<"credit limit for user: "+user_inp+" is now "+new_credit_limit<<endl;
    return 0;

}
