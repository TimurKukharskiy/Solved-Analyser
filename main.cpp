// Solved Analyser 20.11.2013

#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

class Process_Dir
{
public:
	int arg_c;
	char **arg_v;
	map <string, int> include_counter;
	Process_Dir(int arg_c, char *arg_v[]):arg_c(arg_c), arg_v(arg_v){};

	// Analyser

	void Process(string Source_Path, string Source_File, int level){

		if (Source_File == "*"){
			string working_file;
			DIR *dir;
			struct dirent *ent;
			if((dir = opendir(Source_Path.c_str()))!= NULL) {
				while ((ent = readdir(dir))!= NULL) {
					if ((ent->d_type == DT_DIR) && (ent->d_name[0] != '.')){
						Process(Source_Path + (string) "/" + ent->d_name, "*", 0);
						continue;
					}
					working_file = ent->d_name;
					if (working_file.find(".cpp",working_file.length()-4)!= std::string::npos){
						include_counter[working_file]++;
						Process(Source_Path + (string) "/", working_file, 0);
					}
				}
			closedir(dir);
			} else {
			perror("Enter correct path!");
			}
		}else
		{
			string Data;
			if (level == 100)cout<< "Cyclic Include"<<endl;
			if (level == 0){
				include_counter[Source_File]--;
				Output_Level(Source_File, level, true);
			}
			ifstream loaded_file((Source_Path + Source_File).c_str());
			loaded_file.seekg(0, ios::end);
			Data.reserve(loaded_file.tellg());
			loaded_file.seekg(0, ios::beg);
			Data.assign((istreambuf_iterator<char>(loaded_file)), istreambuf_iterator<char>());
			string next_file;
			unsigned int Position = 0,
				Position_Left_Angle = 0, Position_Left_Quote = 0,
				Position_Right_Angle = 0, Position_Right_Quote = 0,
				Position_Comments = 0, Position_Small_Comments = 0;
			bool NoComments;

			while(Position != string::npos){
				NoComments = false;
				while (!NoComments){
					Position_Comments = Data.find("/*", Position);
					Position_Small_Comments = Data.find("//", Position);
					Position = Data.find("#include", Position);
					if (Position_Comments < Position) {
						Position_Comments = Data.find("*/", Position_Comments);
						Position = Position_Comments + 2;
						NoComments = false;
					}else if (Position_Small_Comments < Position){
						Position_Small_Comments = Data.find("\n", Position_Small_Comments);
						Position = Position_Small_Comments + 1;
						NoComments = false;
					}else NoComments = true;
				}
				if(Position == string::npos) return;
				Position_Left_Angle = Data.find("<", Position + 8);
				Position_Left_Quote = Data.find("\"", Position + 8);
				if (Position_Left_Angle < Position_Left_Quote) {
					Position_Right_Angle = Data.find(">", Position_Left_Angle + 1);
					Position = Position_Right_Angle + 1;
					next_file = Data.substr(Position_Left_Angle + 1,Position_Right_Angle - Position_Left_Angle - 1);
					include_counter[next_file]++;
					int arguments = 2;
					bool file_is_found = false;
					while (arguments < arg_c){
						string argument = arg_v[arguments];
						if (argument == "-I"){
							arguments++;
							continue;
						}
						file_is_found = Check_File_Present(arg_v[arguments] + (string) "/" + next_file);
						if (file_is_found){
							Output_Level(next_file, level + 1, file_is_found);
							Process(arg_v[arguments] + (string) "/", next_file, level + 1);
							break;
						}
						arguments++;
					}
					if(!file_is_found)Output_Level(next_file, level + 1, file_is_found);
				} else
				{
					Position_Right_Quote = Data.find("\"", Position_Left_Quote + 1);
					Position = Position_Right_Quote + 1;
					next_file = Data.substr(Position_Left_Quote + 1,Position_Right_Quote - Position_Left_Quote - 1);
					include_counter[next_file]++;
					if (Check_File_Present(Source_Path + next_file)){
						Output_Level(next_file, level +1, true);
						Process(Source_Path, next_file, level + 1);
					}else Output_Level(next_file, level +1, false);
				}
			}
			loaded_file.close();
		}
	};

	// Result Output Counter

	void OutputIncludeCounter(){
		map<string,int> Sorted_Include_Counter;
		cout << endl;
		map<string,int>::iterator it_outer = include_counter.begin();
		while(it_outer != include_counter.end()){
			map <string , int>::iterator it_next = it_outer;
			for (map<string,int>::iterator it_inner = it_outer;
				it_inner!=include_counter.end();++it_inner){
				if (it_inner->second > it_next->second) it_next = it_inner;
			}
			cout << it_next->first << " " << it_next->second << endl;
			include_counter.erase(it_next);
			it_outer = include_counter.begin();
		}

	}

	// Result Output Tree

	void Output_Level(string NameOfFile, int level, bool file_is_found){
		level = level * 2;
		while (level>0){
			cout<<".";
			level--;
		}
		cout<<NameOfFile;
		if (!file_is_found){
			cout<< " (!)"<<endl;
			return;
		} else cout<<endl;
	}

	// Check if file present

	bool Check_File_Present(string File_Path){
		ifstream check_file((File_Path).c_str());
		if (check_file.is_open()){
			check_file.close();
			return true;
		}else return false;
	}

	~Process_Dir(){};
};

int main(int argc, char *argv []){
	Process_Dir test(argc, argv);
	test.Process(argv [1],"*", 0);
	test.OutputIncludeCounter();
	return 0;
}

