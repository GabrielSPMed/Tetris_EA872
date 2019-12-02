//
//  client.cpp
//  Trabalho 1
//
//  Created by Gabriel Medeiros on 10/27/19.
//  Copyright © 2019 Gabriel Medeiros. All rights reserved.
//


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <ncurses.h>
#include <string.h>

#include "oo_model_client.hpp"

#define MAX 4

void screenUpdate(int *socket_fd, Tela *tela){
    char reply[500];
    RelevantData d1;
    while(1){
        recv(*socket_fd, reply, 500, 0);
        d1.unserialize(reply);
        tela->update(d1.stringToWell(d1.data.a), d1.data.b, d1.stringToAdversarios(d1.data.c));
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }
}

int main() {
  //Inicializacao
    char nome[100];
    std::string meuNome;
    std::cout << "\nbem vindx ao tetris do medeiros\n";
    std::cout << "Digite um nome curto de uns 10 caracteres sei la\n";
    std::cin.get(nome, 10);
    meuNome = nome;
    
  /* Agora, meu socket funciona como um descritor de arquivo usual */
    int socket_fd;
    struct sockaddr_in target;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    target.sin_family = AF_INET;
    target.sin_port = htons(3001);
    inet_aton("172.20.10.4", &(target.sin_addr));
    if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
        return 0;
    }
    send(socket_fd, nome, 10, 0);
    
    char *str = (char*) malloc(15);
    int posicao;
    
    std::cout << "Recebendo os nomes\n";
    
    std::string nomes;
    
    for (int i=0;i<MAX;i++){
        nomes.append(std::to_string(i));
        nomes.append(". ");
        for(int z=0;z<15;z++) str[z]=0;
        recv(socket_fd, str, 15, 0);
        std::cout<<"recebi o nome "<<str<<std::endl;
        nomes.append(str);
        nomes.append("|");
        if(str==meuNome){
            posicao = i;
            std::cout<< "minha posicao e "<<posicao<<std::endl;
        }
    }
   std::cout<<"String final: "<< nomes <<str<<std::endl;
    std::cout << "Agora espera iniciar o jogo\n";
    //while de ok
    while(1){
        recv(socket_fd, str, 2, 0);
        if(str[0]=='o') break;
    }
    
    Tela *tela = new Tela();
       tela->init(nomes, posicao);
       
    Teclado *teclado = new Teclado();
    teclado->init();
    
    std::thread newthread(screenUpdate, &(socket_fd), tela);
    while(tela->getDead()!=1){
        char c = teclado->getchar();
        if (c=='l'||c=='j'||c=='z'||(c-'0'>=0 && c-'0'<=8)){
            str[0]=c;
            send(socket_fd, str, 2, 0);
            
        }
//        if(c=='q'){
//            str[0]=c;
//            send(socket_fd, str, 2, 0);
//            newthread.join();
//            tela->stop();
//            teclado->stop();
//            close(socket_fd);
//            return 0;
//        }
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }
  newthread.join();
  tela->stop();
  teclado->stop();
  close(socket_fd);
    std::cout<<"Parabens voce morreu\n";
  return 0;
}


