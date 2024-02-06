#include <iostream>
#include <fstream>
#include <string>
# include "method.h"

using namespace std;


list_table reduce(string list[], int products){

}

bool validity(string x, string list[], int products){
    for(int i= 0; i < products; i++){
        if(list[i] == x){
            return true;
        }
    }
    return false;
}

bool expand(int position, string x, string list[], int products){
    x[position] = '0';
    bool zero = validity(x, list, products);
    x[position] = '1'; 
    bool one =  validity(x, list, products);
    if(zero == true && one == true){
        return true;
    }
    return false;
}

list_table remove_implicants(int position, string x, string list[], int products){ // incorrect need to check if the position also has an - 
    string new_x = x; 
    if(x[position] == '1'){
        new_x[position] = '0';
    }
    else{
        new_x[position] = '1';
    }
    for(int i = 0; i < products; i++){
        if(list[i] == x){
            x[position] = '-';
            list[i] = x;
        }
        if(list[i] == new_x){
            list[i].clear();
        }
    }
    list_table  new_list; 
    for(int i = 0; i < products; i++){
        new_list.list[i] = list[i]; 
    }
    return new_list;
}

list_table minimize(string list[], int products, int input){ 
    list_table new_list;
    for(int i = 0; i < products; i++){
        new_list.list[i] = list[i]; 
    }
    for(int i = 0; i < products; i++){
        if(new_list.list[i] != ""){ // if list is not empty 
            for(int j = 0; j < input; j++){
                bool expand_position = expand(j, new_list.list[i], list, products);
                if(expand_position == true){
                    new_list = remove_implicants(i, list[i], list, products);
                }
            }
        }
    }
}