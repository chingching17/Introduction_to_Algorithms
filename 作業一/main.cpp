#include<iostream>
#include<vector>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<cmath>
using namespace std;
ofstream out;

void printTableau(vector<vector<int> > final,int h,int w)
{
	for(int i = 0 ; i < h ; i++){
		for(int j = 0 ; j < w ; j++){
			if(final[i][j] == INT_MAX){
				cout << 'x' << ' ';
				out << 'x' << ' ';
			}
			else
			{
				cout << final[i][j] << ' ';
				out << final[i][j] << ' ';
			}
		}
		cout << endl;
		out << endl;
	}
	cout << endl;
	out << endl;
}
void swap(int &num1,int &num2)
{
	int tmp;
	tmp = num1;
	num1 = num2;
	num2 = tmp;
}
void Arrange(vector<vector<int> > &vec, int hei, int wid)
{
	if(hei == 0 && wid == 0)
	{
		return;
	}	
	
	//arrange row0
	if(hei == 0)
	{
		if(vec[hei][wid-1] > vec[hei][wid]){
			swap(vec[hei][wid-1],vec[hei][wid]);
			Arrange(vec,hei, wid - 1);
		}
		return;
	}
	//arrange column0
	if(wid == 0)
	{
		if(vec[hei-1][wid] > vec[hei][wid] ){
			swap(vec[hei-1][wid],vec[hei][wid]);
			Arrange(vec,hei - 1 , wid );
		}
		return;
	}
	
	//go up
	if(vec[hei][wid] < vec[hei-1][wid]){
		swap(vec[hei][wid],vec[hei-1][wid]);
		Arrange(vec,hei-1,wid);
	}
	
	//go left
	if(vec[hei][wid] < vec[hei][wid-1]){
		swap(vec[hei][wid],vec[hei][wid-1]);
		Arrange(vec,hei,wid-1);
	}
}

void ExtractThenFix(vector<vector<int> > &vec,int nowhei,int nowwid)
{
	
	if(nowhei == vec.size() || nowwid == vec[0].size())
	{
		return;
	}
	
	//if at the last row, go right
	if(nowhei == vec.size()-1)
	{
		if(vec[nowhei][nowwid] > vec[nowhei][nowwid+1]){
			swap(vec[nowhei][nowwid],vec[nowhei][nowwid+1]);
			ExtractThenFix(vec,nowhei,nowwid+1);
		}
	}
	
	//go down
	else if(vec[nowhei][nowwid] > vec[nowhei+1][nowwid]){
		swap(vec[nowhei][nowwid],vec[nowhei+1][nowwid]);
		ExtractThenFix(vec,nowhei+1,nowwid);
	}
	
	//go right
	if(vec[nowhei][nowwid] > vec[nowhei][nowwid+1]){
		swap(vec[nowhei][nowwid],vec[nowhei][nowwid+1]);
		ExtractThenFix(vec,nowhei,nowwid+1);
	}

}

void StringToIntTab(vector<vector<string> > &inputs,vector<vector<int> > &outputs , int hei ,int wid)
{
	outputs.resize(hei);
	for(int i = 0  ; i < hei ; i++){
		for(int j = 0 ; j < wid ; j++){
			
			if(inputs[i][j] != "x"){
				int intnum = atoi(inputs[i][j].c_str());
				outputs[i].push_back(intnum);
				
			}			
			else
			{
				outputs[i].push_back(INT_MAX);
			}
		}
	}
}

int main(){
	ifstream file;
	
	file.open("input.txt");
	out.open("output.txt");
	
	if(!file){
		cout << "Read input.txt error" << endl;
		exit(1);
	}
	
	int testcase;
	file >> testcase;
	
	while(testcase--){
		int func;
		file >> func;
		if(func == 1){
			
			string str;
			getline(file,str);
			getline(file,str);
			vector<int> insertnums;
			istringstream stream(str);
	
			while(stream >> str){
				int num = atoi(str.c_str());
				insertnums.push_back(num);
			}
			
			vector<string> rows;
			vector<vector<string> > inputs; 
			vector<vector<int> > outputs;
			inputs.clear();
			outputs.clear();
			while(getline(file,str)){
				
				if(str.size() == 0){
					break;
				}
				istringstream stream(str);
				while(stream >> str){	
					rows.push_back(str);	
				}
				
				inputs.push_back(rows);
				rows.clear();			
			}

			StringToIntTab(inputs,outputs,inputs.size(),inputs[0].size());
			
			cout << "Insert " ;		
			out << "Insert " ;	
			for(int i = 0 ; i < insertnums.size() ; i++){
				cout << insertnums[i] << ' ';
				out << insertnums[i] << ' ';
			}	
			
			for(int i = 0 ; i < insertnums.size() ; i++){
				outputs[outputs.size()-1][outputs[0].size()-1] = insertnums[i];
				Arrange(outputs,outputs.size()-1,outputs[0].size()-1);
			}
			cout << endl;
			out << endl;
			printTableau(outputs,outputs.size(),outputs[0].size());
			
			
		}
		else{
			string str;
			getline(file,str);

			vector<string> rows;
			vector<vector<string> > inputs; 
			vector<vector<int> > outputs;
			inputs.clear();
			outputs.clear();
			while(getline(file,str)){
				
				if(str.size() == 0){
					break;
				}
				istringstream stream(str);
				while(stream >> str){	
					rows.push_back(str);	
				}
				
				inputs.push_back(rows);
				rows.clear();
		
			
			}

			StringToIntTab(inputs,outputs,inputs.size(),inputs[0].size());
			Arrange(outputs,outputs.size()-1,outputs[0].size()-1);
			
			cout << "Extract-min " << outputs[0][0] << endl;
			out << "Extract-min " << outputs[0][0] << endl;
			
			outputs[0][0] = INT_MAX;
			ExtractThenFix(outputs,0,0);
			printTableau(outputs,outputs.size(),outputs[0].size());
			
		}
	}	
	file.close();
	out.close();
}
