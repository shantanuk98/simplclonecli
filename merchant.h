using namespace std;

int new_merchant(string name_inp,string discount_inp){
    string name=name_inp;
    string discount=discount_inp;

    string sql="insert into MERCHANT(NAME,DISCOUNT) \
        values('"+name+"',"+discount+");";

    execute(sql);
    cout<<"merchant "<<name_inp<<" created"<<endl;
    return 0;

}

int update_discount(string merchant_inp,string new_discount_inp){
    string sql="update merchant set discount="+new_discount_inp+" where name='"+merchant_inp+"';";
    execute(sql);
    return 0;
}

list<list<string>> discount_till_date(string merchant_inp){
    string sql="select discount_till_date from merchant where name='"+merchant_inp+"';";
    return execute(sql);;
}

