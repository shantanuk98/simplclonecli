#include <sqlite3.h>
#include  <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <list>
#include "connect_db.h"
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
    list<list<string>> qres=execute_q(sql);
    if(stod(qres.front().front())<1){
        cout<<val+" does not exist in table : "<<table<<" , column : "<<col<<endl;
        return 1;
    }
    return 0;
}

int already_exist(string table,string col,string val){
    string sql="select count(*) from "+table+" where "+col+"='"+val+"';";
    list<list<string>> qres=execute_q(sql);
    if(stod(qres.front().front())>1){
        cout<<val+" already exist in table : "<<table<<" , column : "<<col<<endl;
        return 1;
    }
    return 0;
}
int show_table(string table){
    string sql="select * from "+table+";";
    list<list<string>> qres = execute_q(sql);
    int ctr=0;

    sql="PRAGMA table_info('"+table+"');";
    list<list<string>> colres = execute_q(sql);
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
        }
        cout<<endl;
    }
    return 0;
}



int main(){

    connect();
    create_tables();
    for(int i=0;i<34;i++) cout<<"-";
    cout<<endl;
    cout<<"---- simple clone app started ---- "<<endl;
    for(int i=0;i<34;i++) cout<<"-";
    cout<<endl;

    
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
                string name,email,discount;
                iss>>name;
                iss>>email;
                iss>>discount;
                if(email.empty() || discount.empty() || name.empty()){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(already_exist("merchant","name",name)==1 || already_exist("merchant","email",email)==1  ) {
                    continue;
                }
                new_merchant(name,email,discount);

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
                if(exist("merchant","name",merchant)==1 || exist("user","name",user)==1  ) {
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
                if(merchant.empty() || new_discount.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(exist("merchant","name",merchant)==1 ) {
                    continue;
                }
                update_discount(merchant,new_discount);

            }
            if(sw=="user"){
                string user,credit_limit;
                iss>>user;
                iss>>credit_limit;
                
                if(user.empty() || credit_limit.empty() ){
                    cout<<"incorrect input , please try again"<<endl;
                    continue;
                }
                if(exist("user","name",user)==1 ) {
                    continue;
                }
                update_credit_limit(user,credit_limit);

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
            double amt_left=payback(user,amt);
            cout<<user<<"("<<amt_left<<")"<<endl;
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
                    cout<<"no user with name : "+user<<endl;
                }
                else{
                    cout<<"dues for the user : "+user+" is "+to_string(dues.front())<<endl;
                }
            }
            else if(sw=="users-at-credit-limit"){
                
                list<string> users_list = credit_limit_users();
                cout<<"no of users at credit limit are : "<<users_list.size()<<endl;
                int ctr=0;
                for(auto i=users_list.begin();i!=users_list.end();i++){
                    ctr+=1;
                    cout<<ctr<<") "<<*i<<endl;
                }
            }
            else if(sw=="total-dues"){
                list<list<string>> qres = total_dues();
                for(auto i=qres.begin();i!=qres.end();i++){
                    cout<<*(*i).begin()<<": "<<*next((*i).begin())<<endl;
                }
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