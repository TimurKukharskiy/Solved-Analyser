#include "test.h"
#include <test.cpp>


#include <dirent.h>
#include <string.h>
#include <stdio.h>
using namespace std;
class Process_Dir
{
public:
	int arg_c;
	char **arg_v;
	Process_Dir(int arg_c, char *arg_v[]):arg_c(arg_c), arg_v(arg_v){};

	void Process(string Source_File){



		if (Source_File == "*"){
			string working_string;
			DIR *dir;
			struct dirent *ent;
			if((dir = opendir(Source_Path.c_str()))!= NULL) {
				while ((ent = readdir(dir))!= NULL) {
					if ((ent->d_type == DT_DIR) && (ent->d_name[0] != '.'))
						Process(Source_Path + (string) "/" + ent->d_name);
						test_string = ent->d_name;
				if (test_string.find(".cpp",test_string.length()-4)!= std::string::npos){
					cout<<test_string<<endl;

				}
			//	if (test_string.find(".h",test_string.length()-2)!= std::string::npos)cout<<Source_Path + (string) "/" + test_string<<endl;
		//		cout<<test_string.find(".cpp")<<endl;
	//			cout<<test_string<<endl;
//				if (Search(".cpp", ent->d_name, 0, 4) != -1)
			//	printf("%s \n", ent->d_name);
			}
			closedir(dir);
		} else {
			perror("Enter correct path!");
		}

	};
/*	int Search(string Search_str, char *Search_Buffer,
			int Start_Of_Search, int Finish_Of_Search){
		string Compare_String;
		while (Start_Of_Search + Search_str.length() < Finish_Of_Search ){
				cout<<Search_Buffer[Start_Of_Search]<<endl;
		//		memcpy(&Compare_String, &Search_Buffer[Start_Of_Search], 1);//Search_str.length()+1);
		//		Compare_String[1] = 0;
		//		cout<<Compare_String<<endl;

		//	if (Compare_String == Search_str) return Start_Of_Search;
			Start_Of_Search++;
		}
		cout<< "-1" <<endl;
		return -1;
	}*/
	~Process_Dir(){};
};
int main(int argc, char *argv []){
	Process_Dir test(argc, argv);
	test.Process("*");

}

