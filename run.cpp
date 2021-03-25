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
 #include <unistd.h>
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

string config_path="";

vector<string> read_file(string file){
  ifstream in(file);
	string filename;
	string line;
  vector<string> back;
	if(in) // 有该文件
	{
		while (getline (in, line)) // line中不包括每行的换行符
		{
      back.push_back(line);
		}
	}
	else // 没有该文件
	{
		cout <<"config.txt is not find" << endl;
	}
  return back;
}

vector<pair<string,string>> get_config_text(){
  if(config_path==""){
    char sz_module_file_path[MAX_PATH];
    int n = GetModuleFileNameA(0, sz_module_file_path, MAX_PATH);

    string route=sz_module_file_path;
    const size_t last_index=route.rfind('\\');
    string path;
    if (std::string::npos != last_index)
    {
        path = route.substr(0, last_index);
    }
    config_path=path+"\\config.txt";
  }
  vector<string> file_content=read_file(config_path);
  
  vector<pair<string,string>> config_list;
  for(auto line:file_content){
    int d=line.find('=');
    if(d!=-1){
      config_list.push_back({line.substr(0,d),line.substr(d+1,line.length()-d-1)});
    }else{
      config_list.push_back({line,""});
    }
  }
  return config_list;
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
  list=get_config_text();
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

void registerApp(int argc,char ** argv)
{
  string new_name,new_path;
  if(argc<4){
    printf("Enter app's name\n");
    cin>>new_name;
    getchar();
    printf("Enter app's path\n");
    getline(cin,new_path);
  }else{
    new_name=argv[2];
    new_path=argv[3];
    for(int i=4;i<argc;i++){
      new_path+=" "+(string)argv[i];
    }
  }
  ifstream fin(new_path);
  if(!fin){
    printf("File is not find\n");
    return;
  }
  
  vector<pair<string,string>> config_list=get_config_text();
  for(auto [name,path] : config_list){
    if(name==new_name){
      printf("The name %s is registed\n",new_name.data());
      return;
    }
  }
  ofstream new_file(config_path);
  for(auto [name,path] : config_list){
    new_file<<(name+"="+path)<<endl;
  }
  new_file<<(new_name+"="+new_path);
  printf("Register success\n");
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
      case "register"_hash:
        registerApp(argc,argv);
        break;
      default:
        help();
    }
  }
  
  // HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "d:\\tese.log", NULL, NULL, SW_SHOW);
  
  return 0;
}
