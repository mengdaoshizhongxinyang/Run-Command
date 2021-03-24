/*
 * @Author: mengdaoshizhongxinyang
 * @Date: 2021-03-24 09:14:05
 * @Description: 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <direct.h>
using namespace std;
using hash_t = size_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
hash_t hash_run_time(const char* str) {
    hash_t ret = basis;

    while (*str) {
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}
constexpr hash_t hash_compile_time(const char* str, hash_t last_value = basis) {
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}

constexpr hash_t operator "" _hash(const char* p, size_t) {
    return hash_compile_time(p);
}
vector<pair<string,string>> readFile(string file){
  ifstream in(file);
	string filename;
	string line;
  vector<pair<string,string>> back;
	if(in) // 有该文件
	{
		while (getline (in, line)) // line中不包括每行的换行符
		{
      int d=line.find('=');
      if(d==-1){
        back.push_back({line,""});
      }else{
        // cout<<line.substr(0,d)<<" "<<line.substr(d+1,line.length()-d-1)<<endl;
        back.push_back({line.substr(0,d),line.substr(d+1,line.length()-d-1)});
      }
			
		}
	}
	else // 没有该文件
	{
		cout <<"no such file" << endl;
	}
  return back;
}
//write instructions's description
vector<pair<string,string>> instructions({
  {"apps","Start a apps"},
  {"help","Show instruction list"}
});
void help()
{
  printf("These are common run commands used in various situations:\n\n");
  for(auto [name,description] : instructions){
    printf("\t%s\t\t%s\n",name.data(),description.data());
  }
}

void apps(int argc,char ** argv)
{
  vector<pair<string,string>> list;
  string route=(string)argv[0];
  int len=route.length();
  route=route.substr(0,len-7)+"config.txt";
  list=readFile(route);
  if(argc==2){
    printf("You must enter app's name");
  }
  for(auto [name,apps] : list){
    if(name==argv[2]){
      HINSTANCE hNewExe = ShellExecuteA(NULL, "open", apps.data(), NULL, NULL, SW_SHOW);
      return;
    }
  }
  printf("%s is not registered\n",argv[2]);
}

void registerApp(char* name)
{

}

int main(int argc,char ** argv)
{
  if(argc==1){
    help();
  }else{
    char* command=argv[1];
    switch(hash_run_time(command)){
      case "help"_hash:
        help();
        break;
      case "apps"_hash:
        apps(argc,argv);
        break;
      default:
        help();
    }
  }
  
  // HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "d:\\tese.log", NULL, NULL, SW_SHOW);
  
  return 0;
}
