#include<iostream> 
#include<fstream> 
#include<stdlib.h> 
using namespace std ; 

int main()
{
    ifstream in("test.txt") ;

    ofstream out("test1.txt" , ofstream::app) ; // 追加
    ofstream out("test1.txt") ; // 文件之前的内容丢弃
    if(in.is_open()){
        while(!in.eof()){
            string data ; 
            in >> data ; 
            out << data  ; 
            /*
            getline(in , data) ; 
            */
        }
    }else {
        out<<"凉凉"<<endl ; 
    }
    in.close() ; 
    out.close() ; 
    
   
    ifstream infile  ; 
    infile.open("test.txt") ; 

    ofstream outfile ; 
    outfile.open("test1.txt" , ofstream::app) ; 
 
    if(infile.is_open()){
    
        string data  ;
        infile >> data  ; 
        cout<<data<<endl ; 
    
    }else {
        cout<<"凉凉"<<endl ;
    }
    infile.close() ; 
    outfile.close() ; 

    return 0 ; 
}