//
//  oo_model.cpp
//  Trabalho 1
//
//  Created by Gabriel Medeiros on 9/18/19.
//  Copyright © 2019 Gabriel Medeiros. All rights reserved.
//

#include "oo_model_client.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "json.hpp"
#define MAX 4
using namespace std;

uint64_t get_now_us() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()*1017;
}
int done = 0;



Tela::Tela() {
    this->maxI = 24;
    this->maxJ = 10;
}
int Tela::getDead(){
    return this->dead;
}
void Tela::playboard(){
    int i;
    for (i=0;i<24;i++){
        move(i, 10);
        echochar('|');
        
    }
//    for (i=0;i<10;i++){
//        move(25, i);
//        echochar('_');
//    }
}

void Tela::printScr(int i, int j, std::string str){
    int count;
    for (count = 0; count<str.length();count++){
        move(i, j+count);
        echochar(str[count]);
    }
}

void Tela::parseNome(std::string nomes){
    int i, j=0, k=0, x;
    char aux[15];
    for (i=0;i<MAX;i++){
        while(nomes[j]!='|'){
            aux[k] = nomes[j];
            j++;
            k++;
        }
        j++;
        k=0;
        printScr(8 + i, 20, aux);
        for(x=0;x<15;x++) aux[x]=0;
    }
}

void Tela::init(std::string nomes, int posicao) {
    initscr();                   /* Start curses mode         */
    raw();                         /* Line buffering disabled    */
    curs_set(0);           /* Do not display cursor */
    playboard();
    std::string lixo("Lixo: ");
    printScr(20, 20, lixo);
    parseNome(nomes);
    this->dead = 0;
    this->posicao=posicao;
}

void Tela::stop() {
    endwin();
}

Tela::~Tela() {
    this->stop();;
}

void Tela::update(std::vector<std::vector<int>> well, int lixo, std::vector<int> adversarios){
    int i, j;
    std::string space;
    for (i =0; i<this->maxI;i++){
        for (j=0;j<this->maxJ;j++){
            move(i,j);
            if(well[i][j]==1) echochar('#');
            else if(well[i][j]==2) echochar('@');
            else echochar(' ');
            
        }
    }
    std::string str = std::to_string(lixo);
    for(i=0;i<MAX-1;i++) space.append("  ");
    printScr(20, 27, str);
    for(i=0;i<MAX;i++){
        str.clear();
        if(adversarios[i]==9){
            if(posicao == i){
                this->dead = 1;
                return;
            }
            printScr(8+i, 40, space);
            printScr(8 + i, 40, "MORREU COITADO");
        } else {
            for(j=0;j<MAX;j++){
                if(adversarios[j]==i){
                    str.append(std::to_string(j));
                    str.append(" ");
                }
            }
            printScr(8+i, 40, space);
            printScr(8 + i, 40, str);
        }
    }
    refresh();
}

void Tela::clear(){
    int i, j;
    for (i=0;i<24;i++){
        for(j=0;j<10;j++){
            move(i,j);
            echochar(' ');
        }
    }
    for(i=25;i<60;i++){
        move(10, i);
        echochar(' ');
    }
}


void threadfun (char *keybuffer, int *control)
{
    char c;
    while ((*control) == 1) {
        c = getch();
        if (c!=ERR) (*keybuffer) = c;
        else (*keybuffer) = 0;
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
    return;
}



Teclado::Teclado() {
}

Teclado::~Teclado() {
}



void Teclado::init() {
    // Inicializa ncurses
    raw();                         /* Line buffering disabled    */
    keypad(stdscr, TRUE);     /* We get F1, F2 etc..        */
    noecho();                     /* Don't echo() while we do getch */
    curs_set(0);           /* Do not display cursor */
    
    this->rodando = 1;
    std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
    
    (this->kb_thread).swap(newthread);
    
}

void Teclado::stop() {
    this->rodando = 0;
    (this->kb_thread).join();
}

char Teclado::getchar() {
    char c = this->ultima_captura;
    this->ultima_captura = 0;
    return c;
}


RelevantData::RelevantData() {
}

RelevantData::RelevantData(std::vector<std::vector<int>> a, int b, std::vector<int> adversarios) {
  this->data.a = wellToString(a);
  this->data.b = b;
    this->data.c=adversariosToString(adversarios);
}

void RelevantData::init(std::vector<std::vector<int>> a, int b) {
  this->data.a = wellToString(a);
  this->data.b = b;
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
    for(int i =0;i<MAX;i++){
        str.append(std::to_string(adversarios[i]));
    }
    return str;
}

std::vector<int> RelevantData::stringToAdversarios(std::string str){
    std::vector<int> vector(MAX);
    for(int i=0;i<MAX;i++){
        vector[i] = str[i] - '0';
    }
    return vector;
}

void RelevantData::dump() {
  std::cout << this->data.a << '\n' << this->data.b << std::endl;
}

