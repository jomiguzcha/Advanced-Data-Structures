#include <fstream>
#include <iostream>
#include "lsdh_tree.h"
#define SHELL_TOK_DELIM "," 

vector<char*> shell_split_line(string command) {
	char* token;
	char* next_token = NULL;
	vector<char*> palabras;

	char* c_command = new char[command.length() + 1];//copia el comando en char*
	strcpy_s(c_command, command.length() + 1, command.c_str());

	token = strtok_s(c_command, SHELL_TOK_DELIM, &next_token);
	while (token != NULL) {
		palabras.push_back(token);

		token = strtok_s(NULL, SHELL_TOK_DELIM, &next_token);
	}

	return palabras;
}

void read_record(lsdh_tree<float>& tree){ 
	ifstream dataset; 
	dataset.open("dataset.csv", ios::in); 

	vector<char*> row;//almacena la fila a leer spliteada
	string line, temp;
	float aux[num_dimension];
	
	while (!dataset.eof()){ 
		row.clear(); 
		line.clear();

		getline(dataset, line); 
		//cout << line << endl;

		row = shell_split_line(line);
		
		for (int i = 0; i < row.size(); ++i) {
			aux[i] = atof(row[i]);
		}

		tree.insert(aux);
	} 

	dataset.close();
} 
