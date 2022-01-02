
int new_merchant(string name_inp,string email_inp,string discount_inp){
    int l=discount_inp.size();
    string discount="";
    if(discount_inp[l-1]=='%'){
        discount=discount_inp.substr(0,l-1);
    }
    else{
        discount=discount_inp;
    }
    string sql="insert into MERCHANT(NAME,EMAIL,DISCOUNT) \
        values('"+name_inp+"','"+email_inp+"',"+discount+");";

    execute_q(sql);
    cout<<name_inp<<"("<<discount_inp<<")"<<endl;
    return 0;

}

int update_discount(string merchant_inp,string new_discount_inp){
    string sql="update merchant set discount="+new_discount_inp+" where name='"+merchant_inp+"';";
    execute_q(sql);
    return 0;
}

list<list<string>> discount_till_date(string merchant_inp){
    string sql="select discount_till_date from merchant where name='"+merchant_inp+"';";
    return execute_q(sql);;
}

