//
//  oo_model.hpp
//  Trabalho 1
//
//  Created by Gabriel Medeiros on 9/18/19.
//  Copyright © 2019 Gabriel Medeiros. All rights reserved.
//

#ifndef oo_model_hpp
#define oo_model_hpp

#include <thread>
#include <vector>
#include <chrono>
#include <string>



class Tela {
private:
    int maxI, maxJ;
    int dead;
    int posicao;

public:
    Tela();
    ~Tela();
    void stop();
    int getDead();
    void init(std::string nomes, int posicao);
    void playboard(); //Cria os caracteres estaticos no tabuleiro
    void update(std::vector<std::vector<int>> well, int lixo, std::vector<int> adversarios); //Atualiza a tela conforme a movimentacao do well na fisica
    void printScr(int i, int j, std::string str); //Imprime na tela a string dada na posicao (i,j)
    void clear(); //Limpa a tela
    void parseNome(std::string nomes); //parseia os nomes coletados para a inicializacao
};


void threadfun (char *keybuffer, int *control);



class Teclado {
private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;
    std::thread loop_thread;

public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();

};

//void capture(Teclado *teclado, int *control, Fisica *fisica, Tela *tela);

struct DataContainer {
    std::string a;
    int b;
    std::string c;
};

class RelevantData {

  public:
    DataContainer data;
    RelevantData();
    RelevantData(std::vector<std::vector<int>> a, int b, std::vector<int> adversarios);
    RelevantData(std::string buffer_in);
    void init(std::vector<std::vector<int>> a, int b);
    std::string serialize();
    void unserialize(std::string buffer_in);
    std::string wellToString(std::vector<std::vector<int>> well);
    std::vector<std::vector<int>> stringToWell(std::string str);
    std::vector<int> stringToAdversarios(std::string str);
    std::string adversariosToString(std::vector<int> adversarios);
    void dump();
};
#endif /* oo_model_hpp */
