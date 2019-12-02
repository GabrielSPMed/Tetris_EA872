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


class Bloco {
private:
    std::vector<std::vector<int>> formato; //Formato do bloco em matriz de vectors
public:
    Bloco(int type); //Construtor
    std::vector<std::vector<int>> get_formato(); //getter
};

class ListaDeBlocos {
private:
    std::vector<Bloco*> blocos; //vetor de blocos da lista
    
public:
    ListaDeBlocos(); //Construtor
    Bloco *proximoBloco(); //Pega o proximo bloco da fila e randomiza outro para colocar na fila
    std::vector<Bloco*> get_blocos(); //getter
};

class Fisica {
private:
    std::vector<std::vector<int>> well; //tabuleiro contendo 0:vazio 1:estatico 2:movimento
    float period; //Periodo no qual deve-se cair o bloco em movimento
    int interactions; //Numero de vezes que se caiu os blocos em queda. Usado para calcular quando se deve cair.
    int lines_cleared; //Numero de linhas limpas
    ListaDeBlocos *lista; //Fila com os proximos dois blocos
    int adversario; //Adversario para mandar lixo
    int lixo; //Numero de linhas de lixo a serem acrescentadas
    std::string nome; //Nome do usuario
    int posicao;
public:
    void setPosicao (int posicao);
    int getAdversario();
    void setAdversario (int adversario, int MAX); //muda seu adversario
    void setAdversario (int MAX); //Seleciona um adversario aleatorio para o jogador
    void setNome (std::string nome); //seta o nome
    void setLixo(int lixo); //seta seu lixo
    int getLixo();//getta o lixo
    int applyLixo(); //aplica o lixo ao well
    void shiftLinesUp(); //desloca tudo para cima
    void fillLixo();
    std::string getNome(); //Pega o nome
    int findHighestBlock(); //encontra o 1 mais acima
    Fisica(); //Construtor
    std::vector<std::vector<int>> getWell();//getter
    float getPeriod(); //getter
    int getInteractions(); //getter
    void setInteractions(int interactions); //setter
    int containsBloco(); //Verifica se o tabuleiro contem um bloco em movimento
    int insertBloco(); //Tenta inserir um bloco em movimento no tabuleiro, avisa se nao der
    int shouldFall(float deltaT); //Verifica se ja deu o tempo de uma queda
    int findPieceRow(); //Acha a fileira mais alta contendo um 2
    int canMoveDown();//Verifica se ha o bloco pode cair
    void drop(int highest_column); //Movimenta todos os 2 da matriz um para baixo
    void makeStatic(int highest_column); //Transforma todos os 2 da matriz em 1
    int update(float deltaT, Fisica **f); //Atualiza a well de acordo com o tempo passado
    void moveRight(); //mexe pra direita
    int canMoveRight(); //Verifica se o bloco pode ser movido para a direita
    void moveLeft(); //mexe pra esquerda
    int canMoveLeft(); //Verifica se o bloco pode ser movido para a esquerda
    void cleanLines(Fisica **f); //limpa linhas cheias
    int fullLine(int linha); //descobre que linha esta cheia a partir da linha dada
    void shiftUpperLines(int linha);//apaga a linha acima e desloca as demais
    void spin(); //Gira o bloco
    int getLinesCleared(); //getter
    
};


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
    void init(std::vector<std::vector<int>> a, int b, std::vector<int> adversarios);
    std::string serialize();
    void unserialize(std::string buffer_in);
    std::string wellToString(std::vector<std::vector<int>> well);
    std::vector<std::vector<int>> stringToWell(std::string str);
    std::vector<int> stringToAdversarios(std::string str);
    std::string adversariosToString(std::vector<int> adversarios);
    void dump();
};
#endif /* oo_model_hpp */
