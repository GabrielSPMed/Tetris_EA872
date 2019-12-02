//
//  oo_model.cpp
//  Trabalho 1
//
//  Created by Gabriel Medeiros on 9/18/19.
//  Copyright © 2019 Gabriel Medeiros. All rights reserved.
//

#include "oo_model.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#define MAXI 4
#include "json.hpp"

using namespace std;

uint64_t get_now_us() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()*1017;
}
int done = 0;


Bloco::Bloco(int type){
    vector<vector<int>> matrix;
    if (type == 0){
        matrix =    {   { 0, 2, 0, 0 },
                        { 0, 2, 2, 2 } };
    }
    else if (type == 1){
        matrix =    {   { 0, 0, 2, 0 },
                        { 2, 2, 2, 0 } };
    }
    else if (type == 2){
        matrix =    {   { 0, 2, 2, 0 },
                        { 0, 2, 2, 0 } };
    }
    else if (type == 3){
        matrix =    {   { 0, 2, 2, 0 },
                        { 2, 2, 0, 0 } };
    }
    else if (type == 4){
        matrix =    {   { 0, 2, 2, 0 },
                        { 0, 0, 2, 2 } };
    }
    else if (type == 5){
        matrix =    {   { 0, 0, 0, 0 },
                        { 2, 2, 2, 2 } };
    }
    else if (type == 6){
        matrix =    {   { 0, 2, 0, 0 },
                        { 2, 2, 2, 0 } };
    }
    this->formato = matrix;
}
std::vector<std::vector<int>> Bloco::get_formato(){
    return this->formato;
}

ListaDeBlocos::ListaDeBlocos(){
    int size;
    if (done == 0) {
        srand(time(0)*100000000);
        done = 1;
    }
    std::vector<Bloco*> vector;
    
    size = rand() % 7;
    
    Bloco *b1 = new Bloco(size);
    
    size = rand() % 7;
    
    Bloco *b2 = new Bloco(size);
    
    vector.push_back(b1);
    vector.push_back(b2);
    
    this->blocos = vector;
}

Bloco * ListaDeBlocos::proximoBloco(){
    Bloco *b = this->blocos[0];
    this->blocos.erase(this->blocos.begin());

    
    int size = rand() % 7;
    
    Bloco *b1 = new Bloco(size);
    
    this->blocos.push_back(b1);
    
    return b;
}


std::vector<Bloco*> ListaDeBlocos::get_blocos(){
    return this->blocos;
}



Fisica::Fisica(){
    this->interactions = 0;
    
    this->period = 0.1;
    
    this->lines_cleared = 0;
    std::vector<std::vector<int>> well (24, std::vector<int>(10));
    int i, j;
    for(i=0;i<24;i++){
        for(j=0;j<10;j++){
            well[i][j]=0;
        }
    }
    this->well = well;
    this->lista= new ListaDeBlocos();
    this->lixo = 0;
}

std::vector<std::vector<int>> Fisica::getWell(){
    return well;
}
void Fisica::setPosicao(int posicao){
    this->posicao=posicao;
}
int Fisica::getInteractions(){
    return this->interactions;
}
void Fisica::setInteractions(int interaction){
    this->interactions = interaction;
}
int Fisica::getLinesCleared(){
    return this->lines_cleared;
}
int Fisica::containsBloco(){
    int i, j;
    for (i = 0; i < 24; i ++){
        for (j = 0; j < 10; j++){
            if (this->well[i][j] == 2){
                return 1;
            }
        }
    }
    return 0;
}
void Fisica::setAdversario(int adversario, int MAX){
    if (adversario != posicao && adversario<MAX){
        this->adversario=adversario;
    }
}
int Fisica::getAdversario(){
    return this->adversario;
}

void Fisica::setAdversario(int MAX){
    //srand(time(0)*100000000);
    int ad;
    ad = rand() % MAX;
    while(ad == posicao){
        ad = rand() % MAX;
    }
    this->adversario = ad;
}
void Fisica::setNome(std::string nome){
    //std::cout<<"entrei no set nome: "<< nome<<std::endl;
    this->nome = nome;
    //std::cout<<"sai do set nome\n";
}
std::string Fisica::getNome(){
    return this->nome;
}
void Fisica::setLixo(int lixo){
    this->lixo=lixo;
}
int Fisica::getLixo(){
    return this->lixo;
}

int Fisica::insertBloco(){
    Bloco *b = this->lista->proximoBloco();
    int i, j;
    for (i = 0; i < 2; i ++){
        for (j = 0; j < 4; j ++){
            if((this->well[i][j + 3] == 0)&&((b->get_formato()[i][j]==2) ||(b->get_formato()[i][j]==0) )){
                if (b->get_formato()[i][j]==2)
                this->well[i][j + 3] = b->get_formato()[i][j];
            } else {
                return 0;
            }
        }
    }
    return 1;
}
int Fisica::shouldFall(float dT){
    if (dT >= this->interactions*this->period){
        this->interactions = this->interactions + 1;
        return 1;
    }
    return 0;
}

int Fisica::findPieceRow(){
    int i, j;
    for (i = 23; i >= 0; i--){
        for (j=0;j<10;j++){
            if (this->well[i][j]==2) return i;
        }
    }
    return 0;
}

int Fisica::canMoveDown(){
    int i, j, count = 0;
    for(i=0;i<10;i++) if (this->well[23][i]==2) return 0;
    
    for (i=22; i>-1; i--){
        if(count<4){
            for (j=0;j<10;j++){
                if(this->well[i][j]==2){
                    count++;
                    if(this->well[1+i][j]==1) return 0;
                }
            }
        } else break;
    }
    return 1;
}

void Fisica::drop(int highest_column){
    int i, j, min;
    if (highest_column - 4 < 0) min =-1;
    else min = highest_column-4;
    for (i = highest_column; i > min; i--){
        for (j=0; j < 10; j++){
            if (this->well[i][j]==2){
                this->well[i+1][j]=2;
                this->well[i][j]=0;
            }
        }
    }
}

void Fisica::makeStatic(int highest_column){
    int i, j, min, aux = 0;
    if (highest_column - 4 < 0) min =-1;
    else min = highest_column-4;
    for (i = highest_column; i > min; i--){
        for (j=0; j < 10; j++){
            if (this->well[i][j]==2){
                this->well[i][j]=1;
                aux = 1;
            }
        }
    }
}
int Fisica::fullLine(int linha){
    int i, j;
    for (i=linha;i<24;i++){
        for (j=0;j<10;j++){
            if(this->well[i][j]!=1) break;
            if (j==9) return i;
        }
    }
    return 24;
}

void Fisica::shiftUpperLines(int linha){
    int i, j, substitute;
    for (i=linha;i>0;i--){
        for (j=0;j<10;j++){
            if(this->well[i][j]!=2){
                if(this->well[i-1][j]==1) substitute = 1;
                else substitute = 0;
                this->well[i][j]=substitute;
            }
        }
    }
    for (j=0;j<10;j++) this->well[0][j]=0;
}

void Fisica::cleanLines(Fisica **f){
    int linha = fullLine(0), lixo = -1;
    while(linha!=24){
        lixo++;
        this->lines_cleared++;
        shiftUpperLines(linha);
        linha = fullLine(linha);
    }
    if(lixo>=1){
        f[this->adversario]->setLixo(lixo + f[this->adversario]->getLixo());
//        this->lixo = lixo;
    }
}

int Fisica::findHighestBlock(){
    int i, j;
    for(i=0;i<24;i++){
        for(j=0;j<10;j++){
            if(this->well[i][j]==1){
                return i;
            }
        }
    }
    return 23;
}

void Fisica::shiftLinesUp(){
    int i, j;
    for (i=0;i<=23-lixo;i++){
        for(j=0;j<10;j++){
            if(this->well[i][j]!=2&&this->well[i+lixo][j]!=2)this->well[i][j] = this->well[i+lixo][j];
        }
    }
}

void Fisica::fillLixo(){
    int posicao = rand() % 10, i, j;
    for(i=23 - lixo + 1;i<24;i++){
        for(j=0;j<10;j++){
            if(j==posicao) well[i][j]=0;
            else well[i][j]=1;
        }
    }
}

int Fisica::applyLixo(){
    int highest = findHighestBlock();
    if(23 - highest + lixo >= 24) return 1;
    shiftLinesUp();
    fillLixo();
    lixo=0;
    return 0;
}


int Fisica::update(float dT, Fisica **f){
    if (!containsBloco()){
        if(!insertBloco()){
            return 0;
        }
        if(lixo!=0){
            if(applyLixo()){
                return 0;
            }
        }
    } else {
        if(shouldFall(dT)) {
            int deepest_row = findPieceRow();
            if (deepest_row != 23){
                if (canMoveDown()){
                    drop(deepest_row);
                    deepest_row--;
                    return 1;
                }
            }
            makeStatic(deepest_row);
            cleanLines(f);
        }
    }
    return 1;
}

int Fisica::canMoveRight(){
    int i, j, count = 0;
    for (i =0; i<24; i++){
        if (this->well[i][9]==2) return 0;
        if(count<4){
            for (j=8;j>-1;j--){
                if(this->well[i][j]==2){
                    count++;
                    if(this->well[i][j+1]==1) return 0;
                    break;
                }
            }
        }
    }
    return 1;
}

void Fisica::moveRight(){
    if (canMoveRight()){
        int i, j, count = 0;
        for (i = 0; i < 24;i++){
            for(j=9; j >=0;j--){
                if (this->well[i][j]==2){
                    this->well[i][j]=0;
                    this->well[i][j+1]=2;
                    count++;
                }
                if (count==4) return;
            }
        }
    }
}

int Fisica::canMoveLeft(){
    int i, j, count=0;
    for (i =0; i<24; i++){
        if (this->well[i][0]==2) return 0;
        if(count<4){
            for(j=1;j<10;j++){
                if(this->well[i][j]==2) {
                    count++;
                    if(this->well[i][j-1]==1) return 0;
                    break;
                }
            }
        }
    }
    return 1;
}

void Fisica::moveLeft(){
    if (canMoveLeft()){
        int i, j, count = 0;
        for (i = 0; i < 24;i++){
            for(j=0; j < 10;j++){
                if (this->well[i][j]==2){
                    this->well[i][j]=0;
                    this->well[i][j-1]=2;
                    count++;
                }
                if (count==4) return;
            }
        }
    }
}

void Fisica::spin(){
    if(containsBloco()){
        int i, j, aux = 0, ie, je, count=0;
        std::vector<int> m (3);
        std::vector<int> k (3);
        
//        if (highest_row - 4 < 0) i =0;
//        else i = highest_row-3;
        
        for (i=0;i<=23;i++){
            for(j=0;j<10;j++){
                if (count==3) break;
                if(this->well[i][j]==2){
                    if(!aux){
                        aux=1;
                        ie=i;
                        je=j;
                    }else{
                        k[count]=i-ie;
                        m[count]=j-je;
                        count++;
                    }
                }
            }
            if (count==3) break;
        }
        
        for (i=0;i<3;i++){
            if(ie-m[i]<0||ie-m[i]>23||je+k[i]<0||je+k[i]>9) return;
            if(this->well[ie-m[i]][je+k[i]]==1) return;
        }
        for(i=0;i<3;i++){
            this->well[ie+k[i]][je+m[i]] = 0;
        }
        for(i=0;i<3;i++){
            this->well[ie-m[i]][je+k[i]] = 2;
        }
    }
}




RelevantData::RelevantData() {
}

RelevantData::RelevantData(std::vector<std::vector<int>> a, int b, std::vector<int> adversarios) {
  this->data.a = wellToString(a);
  this->data.b = b;
    this->data.c=adversariosToString(adversarios);
}

void RelevantData::init(std::vector<std::vector<int>> a, int b, std::vector<int> adversarios) {
  this->data.a = wellToString(a);
  this->data.b = b;
    this->data.c=adversariosToString(adversarios);
}

RelevantData::RelevantData(std::string buffer_in) {
  this->unserialize(buffer_in);
}

std::string RelevantData::serialize() {
  nlohmann::json j;
  j["a"] = this->data.a;
  j["b"] = this->data.b;
    j["c"] = this->data.c;
  return j.dump();
}

void RelevantData::unserialize(std::string buffer_in) {
  nlohmann::json j;
  j = nlohmann::json::parse(buffer_in);
  this->data.a = j.at("a");
  this->data.b = j.at("b");
    this->data.c = j.at("c");
}
std::string RelevantData::wellToString(std::vector<std::vector<int>> well){
    std::string str;
    int i, j;
    for(i=0;i<=23;i++){
        for(j=0;j<=9;j++){
            str+=std::to_string(well[i][j]);
        }
    }
    return str;
}

std::vector<std::vector<int>> RelevantData::stringToWell(std::string str){
    std::vector<std::vector<int>> well (24, std::vector<int>(10));
    int i, j, k=0;
    for(i=0;i<=23;i++){
        for(j=0;j<=9;j++){
            well[i][j]=str[k] - '0';
            k++;
        }
    }
    return well;
}

std::string RelevantData::adversariosToString(std::vector<int> adversarios){
    std::string str;
    for(int i =0;i<MAXI;i++){
        str.append(std::to_string(adversarios[i]));
    }
    return str;
}

std::vector<int> RelevantData::stringToAdversarios(std::string str){
    std::vector<int> vector(MAXI);
    for(int i=0;i<MAXI;i++){
        vector[i] = str[i] - '0';
    }
    return vector;
}

void RelevantData::dump() {
  std::cout << this->data.a << '\n' << this->data.b << std::endl;
}
