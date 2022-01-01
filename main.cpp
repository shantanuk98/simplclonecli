#include <sqlite3.h>
#include  <iostream>
#include "connect_db.h"
#include  <bits/stdc++.h>
#include "user.h"
#include "merchant.h"
#include "txn.h"



using namespace std;

#define MAX_INP_LEN 60

string conv_to_str(char* c,int l){
    string tmp="";
    for(int i=0;i<l;i++){
        tmp+=c[i];
    }
    return tmp;
}

int exist(string table,string col,string val){
    string sql="select count(*) from "+table+" where "+col+"='"+val+"';";
    list<list<string>> qres=execute(sql);
    if(stod(qres.front().front())<1){
        cout<<val+" does not exist in table : "<<table<<" , column : "<<col<<endl;
        return 1;
    }
    return 0;
}

int already_exist(string table,string col,string val){
    string sql="select count(*) from "+table+" where "+col+"='"+val+"';";
    list<list<string>> qres=execute(sql);
    if(stod(qres.front().front())>1){
        cout<<val+" already exist in table : "<<table<<" , column : "<<col<<endl;
        return 1;
    }
    return 0;
}
int show_table(string table){
    string sql="select * from "+table+";";
    list<list<string>> qres = execute(sql);
    int ctr=0;

    sql="PRAGMA table_info('"+table+"');";
    list<list<string>> colres = execute(sql);
    cout<<"    ";
    for(auto i=colres.begin();i!=colres.end();i++){
        cout << setw(20) << left << *next((*i).begin())<<"|  ";
    }
    cout<<endl;
    for(int i=0;i<colres.size()*23+4;i++){
        cout<<"-";
    }
    cout<<endl;
    
    for(auto i=qres.begin();i!=qres.end();i++){
        ctr+=1;
        cout<<ctr<<" ) ";
        for(auto j=(*i).begin();j!=(*i).end();j++){
            cout << setw(20) << left << *j<<"|  ";
            //cout<<*j<<"\t\t"<<"|";
        }
        cout<<endl;
    }
    return 0;
}



int main(){

    connect();
    create_tables();
    int lpnd=1;

    
    while(true){
        char inp1[MAX_INP_LEN];
        cin.getline (inp1,MAX_INP_LEN);
        string inp=inp1;
        transform(inp.begin(), inp.end(), inp.begin(), ::tolower);
        istringstream iss(inp);
        string fw;
        iss >> fw;
        
        if(fw=="new"){
            string sw;
            iss >> sw;
            if(sw.empty()){
                cout<<"incorrect input , please try again"<<endl;
                continue;
            }
            if(sw=="user"){
                string name,email,credit_lmit;
                iss>>name;
                iss>>email;
                iss>>credit_lmit;       
                if(email.empty() || name.empty() || credit_lmit.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(already_exist("user","name",name)==1 || already_exist("user","email",email)==1 ) {
                    continue;
                }
                new_user(name,email,credit_lmit);

            }
            else if(sw=="merchant"){
                string name,discount;
                iss>>name;
                iss>>discount;
                if(discount.empty() || name.empty()){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(already_exist("merchant","name",name)==1 ) {
                    continue;
                }
                new_merchant(name,discount);

            }
            else if(sw=="txn"){
                string user,merchant,amount;
                iss>>user;
                iss>>merchant;
                iss>>amount;
                if(user.empty() || merchant.empty() || amount.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                
                txn(user,merchant,amount);

            }
            else{
                cout<<"unknown command , please try again "<<endl;
            }
        }
        else if(fw=="update"){
            string sw;
            iss >> sw;
            if(sw.empty()){
                cout<<"incorrect input , please try again"<<endl;
                continue;
            }
            if(sw=="merchant"){
                string merchant,new_discount;
                iss>>merchant;
                iss>>new_discount;
                if(exist("merchant","name",merchant)==1 ) {
                    continue;
                }
                if(merchant.empty() || new_discount.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                update_discount(merchant,new_discount);

            }
            else{
                cout<<"unknown command , please try again "<<endl;
            }

        }
        else if(fw=="payback"){
            string user,amt;
            iss >> user;
            iss >> amt;
            if(user.empty() || amt.empty()){
                cout<<"incorrect input , please try again"<<endl;
                continue;
            }
            if(exist("user","name",user)==1 ) {
                continue;
            }
            payback(user,amt);
        }
        else if(fw=="report"){
            string sw;
            iss >> sw;
            if(sw.empty()){
                cout<<"incorrect input , please try again"<<endl;
                continue;
            }
            if(sw=="discount"){
                string merchant;
                iss >> merchant;
                if( merchant.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(exist("merchant","name",merchant)==1 ) {
                    continue;
                }
                list<list<string>> dtd = discount_till_date(merchant);
                cout<<"discoutn till date for merchant "+merchant+" is : "+(dtd).front().front()<<endl;

            }
            else if(sw=="dues"){
                string user;
                iss >> user;
                if(user.empty()){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(exist("user","name",user)==1 ) {
                    continue;
                }
                list<double> dues = report_dues(user);
                if(dues.size()==0){
                    cout<<"no user with named : "+user<<endl;
                }
                else{
                    cout<<"dues for the user : "+user+" is "+to_string(dues.front())<<endl;
                }
            }
            else if(sw=="users-at-credit-limit"){
                string user;
                iss >> user;
                if(user.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                list<string> users_list = credit_limit_users(user);
                cout<<"no of users at credit limit are : "<<users_list.size()<<endl;
                int ctr=1;
                for(auto i=users_list.begin();i!=users_list.end();i++){
                    cout<<ctr<<") "<<*i<<endl;
                }
            }
            else if(sw=="total-dues"){
                cout<<"total dues are : "<<total_dues()<<endl;
            }
            else{
                cout<<"unknown command , please try again "<<endl;
            }

        }
        else if(fw=="show"){
            string sw;
            iss >> sw;
            if(sw=="user"){
                show_table("user");
            }
            else if(sw=="merchant"){
                show_table("merchant");
            }
            else if(sw=="transaction"){
                show_table("txn");
            }
            else{
                cout<<"no such data"<<endl;
                continue;
            }
        }
        else if(fw=="quit" || fw=="exit" || fw=="end" ){
            break;
        }
        else{
            cout<<"unknown command , please try again "<<endl;
            continue;
        }      
    }
}