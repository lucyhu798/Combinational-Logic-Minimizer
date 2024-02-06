#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>

//#include "method.h"
using namespace std;

// need to create a structure to put an array to make this all work hopefully. 

int input = 24;
int outputs = 28;
int products = 1886;
string type = "f";
string input_name[24]; // the variable name for input 
string output_name[28]; // the variable name for output 
int literals = 0; 
int final_products = 0;
int final_literals = 0; 

struct list_table {
    string list[28][1000]; 
};

list_table o_values;
list_table list; 

string write(int output){
    string result = output_name[output];
    result.append(" = "); 
    for(int i = 0; i < products; i++){
        if(o_values.list[output][i] == "1" || o_values.list[output][i] == "2"){
            if(list.list[output][i] != ""){
                //cout << "word : " << list.list[output][i] << '\n';
                final_products += 1;  
                for(int j = 0; j < input; j++){
                    if(list.list[output][i][j] == '1' && o_values.list[output][i] == "1" ){
                        final_literals = final_literals +  1; 
                        result.append(input_name[j]);
                    }
                    else if(list.list[output][i][j] == '0' && o_values.list[output][i] == "1" ){
                        final_literals = final_literals + 1; 
                        result.append("!");
                        result.append(input_name[j]);
                    }
                }
                if(o_values.list[output][i] == "1"){
                    result.append(" + ");
                }
            }
        }
    }
    result.erase(result.size()-3);
    return result;
}

bool validity(int position, string x, int output, int row){
    for(int i = row; i < products; i++){
        if(list.list[output][i] == x){
            return true;
        }
    }
    for(int j = 0; j < input; j++){
        if(x[j] == '-'){
            x[j] = '0';
            bool zero = validity(position, x, output, row);
            x[j] = '1';
            bool one = validity(position, x, output, row); 
            if(zero == true && one == true){
                return true; 
            }
            x[j] = '-';
        }
    }
    return false; 
}

bool valid(int position, string x, int output, int row){
    for(int i = row; i < products; i++){
        if(list.list[output][i] == x){
            if(o_values.list[output][i] == "2"){
                list.list[output][i].clear();
                return false;
            }
            return true;
        }
    }
    return false;
}

bool expand(int position, string x, int output, int row){
    bool zero; 
    bool one; 
    x[position] = '0';
    if(input < 10){
        zero = validity(position, x, output, row);
    }
    else{
        zero = valid(position, x, output, row);
    } 
    x[position] = '1'; 
    if(input < 10){
        one =  validity(position, x, output, row);
    }
    else{
        one =  valid(position, x, output, row);
    }
    if(zero == true && one == true){
        return true;
    }
    return false;
}

list_table remove_implicants(int row, int position, string x, int output){
    bool valid;  
    for(int i = row + 1; i < products; i++){
        for(int j = 0; j < input; j++){
            if(x[j] == '-'){
                valid = true;
            }
            else if(x[j] == '1'){
                if(list.list[output][i][j] != '1'){
                    valid = false; 
                    break;
                }
                else{
                    valid = true; 
                }
            }
            else{
                if(list.list[output][i][j] != '0'){
                    valid = false; 
                    break;
                }
                else{
                    valid = true;
                }
            }
        }
        if(valid == true){
            list.list[output][i].clear();
        }
    }
    return list;
}

list_table minimize(int output){ 
    for(int i = 0; i < products; i++){
        if((list.list[output][i] != "") && (o_values.list[output][i] == "1")){ 
             for(int j = 0; j < input; j++){
                if(list.list[output][i][j] == '-'){
                    continue; 
                }
                bool expand_position = expand(j, list.list[output][i], output, i);
                if(expand_position == true){
                    list.list[output][i][j] = '-';
                    list =  remove_implicants(i, j, list.list[output][i], output);
                }
            }
        }
    }
    return list;
}
 

int main (int argc, char **argv) {
    time_t start, end;
    time(&start);
    ifstream myfile (argv[1]);
    ofstream tofile; 
    string word, variable;
    if (myfile.is_open()){
        while(myfile >> word){
            if(word == ".type"){
                myfile >> type;
            }
            else if(word == ".i"){
                myfile >> input; 
            }
            else if(word == ".o"){
                myfile >> outputs;
            }
            else if(word == ".ilb"){
                for(int i = 0; i < input; i++){
                    myfile >> variable;
                    input_name[i] = variable; 
                }
            }
            else if(word == ".ob"){ 
                for(int i = 0; i < outputs; i++){
                    myfile >> variable;
                    output_name[i] = variable; 
                }
            }
            else if(word == ".p"){
                myfile >> products; 
            }
            else if(word == ".e"){
                myfile.close();
            }
            else{ // this is where all the numbers and tables will be at.
                for(int i = 0; i < products; i++){ 
                    myfile >> variable;
                    bool not_one = true;  
                    for(int j = 0; j < outputs; j++){ 
                        if(variable[j] == '1' || variable[j] == '2'){  
                            if(variable[j] == '1' ){
                                not_one = false; 
                            }
                            list.list[j][i] = word;
                            o_values.list[j][i] = variable[j]; 
                            break;
                        }
                    }
                    if(not_one == false){
                        for(int k = 0; k < input; k++){
                            if(word[k] != '-'){
                                literals = literals + 1; 
                            }
                        }
                    }
                    myfile >> word;
                }
            }
        }
    }
    string result; 
    string outfile = argv[1];
    outfile.append("_output");
    tofile.open(outfile);
    tofile << "initial products: ";
    tofile << products ; 
    tofile << "\n";
    tofile << "initial literals: ";
    tofile << literals;
    tofile << "\n";
    for(int i = 0; i < outputs; i++){
        list = minimize(i);
        result = write(i);
        tofile << result; 
        tofile << "\n";
    }
    time(&end);
    double time_taken = double(end - start);
    tofile << "optimized products: "; 
    tofile << final_products;
    tofile << "\n";
    tofile << "optimized literals: ";
    tofile << final_literals; 
    tofile << "\n";
    tofile << "Time taken is: "; 
    tofile << time_taken;
    tofile << setprecision(5);
    tofile << " sec";
    tofile.close();
    return 0;
}