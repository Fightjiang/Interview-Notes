#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 
#include <memory>
#include <map>
#include <set>
using namespace std ; 

using line_no = vector<string>::size_type ; 

class QueryResult {
    
    friend ostream& print(ostream& , const QueryResult &); // 友元函数 , 能访问类内部非公开的变量

    public : 
        QueryResult (string s , 
                     shared_ptr<set<line_no> > p , 
                     shared_ptr<vector<string> > f) 
                     : sought(s) , lines(p) , file(f) { }

    private : 
        string sought ; // 查询的单词
        shared_ptr<set<line_no> > lines ; // 出现的行号
        shared_ptr<vector<string> > file ; // 输入的文件
};

class TextQuery {

    public : 
        
        TextQuery(ifstream&) ; 
        QueryResult query(const string & ) const ; 

    private : 
        shared_ptr<vector<string> > file ; // 输入文件
        map<string , shared_ptr<set<line_no> > > wm ; 

};


string make_plural(size_t ctr , const string &word , const string &ending){
    return ctr > 1 ? word + ending : word ; 
}


ostream& print(ostream & os , const QueryResult & qr) 
{
    os <<'\n' <<qr.sought << " occurs " <<qr.lines->size() << " " 
       << make_plural(qr.lines->size() , "time" , "s") << endl ;

    /*
    (line 36) A set element contains only a key  ;
    (line 128) operator creates a new element ;    
    */
    for(auto num : *qr.lines) {
        os << "\t(line " << num << ") " << *(qr.file->begin() + num - 1) << endl ; 
    } 

    return os ; 
}

TextQuery::TextQuery(ifstream &is) : file(new vector<string> )
{
    string text ; 
    while(getline(is , text)) { // 读入文件每一行到 text 
        file->push_back(text) ; // 保存此行文本
        int n = file->size() ; // 当前行号
        istringstream line(text) ; // 将行文本分解成 单词
        string word ; 
        while(line >> word) {
            auto &lines = wm[word] ; // lines 是一个 shared_ptr 而且是一个 引用 , 因此改变 lines 也会改变 wm 中的元素
            if(!lines) {
                lines.reset(new set<line_no>) ; // 分配一个新的 set 
            }
            lines->insert(n) ; // 用 -> 是因为 lines 是一个智能指针  ； 将此行号插入 set 中 ; 
        }
    }
}

QueryResult TextQuery::query(const string & sought) const
{
    // 如果未找到 sought , 我们将返回一个指向此 set 的指针 ， 它是一个指向空的行号 set 的 shared_ptr ; 
    static shared_ptr<set<line_no> > nodata(new set<line_no> ) ; 

    // 使用 find 而不是下标运算符来查找单词 ，避免将单词添加到 wm 中 !
    auto loc = wm.find(sought) ; // 返回的是一个 pair 类型的地址
    if(loc == wm.end() ) {
        return QueryResult(sought , nodata , file) ; 
    }else {
        return QueryResult(sought , loc->second , file) ;
    }
}


void runQueries(ifstream &infile)
{
    // infile 是一个 ifstream , 指向我们要处理的文件
    TextQuery tq(infile) ; // 保存文件并建立查询 map 
    
    // 与用户交互 ：提示用户输入需要查询的单词，完成查询并打印结果

    while(true){
        cout << "enter word to look for , or q to quit :" ; 
        string s ; 
        if(!(cin >> s) || s == "q" ) break ; 
        // 指向查询并打印结果 
        print(cout , tq.query(s)) << endl ; 
    }

}
int main()
{
    ifstream input("in.txt") ; 
    runQueries(input) ; 
    return 0 ; 
}