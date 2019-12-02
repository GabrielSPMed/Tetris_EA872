//
//  server.cpp
//  Trabalho 1
//
//  Created by Gabriel Medeiros on 10/27/19.
//  Copyright © 2019 Gabriel Medeiros. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "oo_model.hpp"

#define MAX 4

using namespace std::chrono;
uint64_t get_now_ms() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void recieve(int *connection_fd, char *c){
    char input_buffer[5];
    while(1){
        recv(*connection_fd, input_buffer, 2, 0);
        *c = input_buffer[0];
    }
}

int lives(int death[]){
    int lives =0;
    for(int i=0;i<MAX;i++)lives+=death[i];
    return lives;
}

int main() {
    Fisica **f = (Fisica**) malloc(MAX);
    int i;
    std::vector<int> adversarios;
    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    int *connection_fd = (int*) malloc(MAX);
    
    for(i=0;i<MAX;i++){
        f[i] = new Fisica();
        f[i]->setPosicao(i);
        f[i]->setAdversario(MAX);
        adversarios.push_back(f[i]->getAdversario());
    }
    
    int socket_fd;
    struct sockaddr_in myself, client;
    socklen_t client_size = (socklen_t)sizeof(client);
    char input_buffer[15];
    std::string ok;
    

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    myself.sin_family = AF_INET;
    myself.sin_port = htons(3001);
    inet_aton("172.20.10.4", &(myself.sin_addr));

    if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
      std::cout << "problemas ao abrir porta\n";
    }
    RelevantData d1;
    
    listen(socket_fd, 2);
    i=0;
    while(i!=MAX){
       connection_fd[i] = accept(socket_fd, (struct sockaddr*)&client, &client_size);
        //std::cout << connection_fd[i]<< std::endl;
        for (int z=0; z<15;z++)input_buffer[z]='\0';
        recv(connection_fd[i], input_buffer, 15, 0);
        //std::cout<<input_buffer<<std::endl;
        //std::string s(input_buffer);
        f[i]->setNome(input_buffer);
        std::cout <<"Conectado usuario "<< f[i]->getNome()<< std::endl;
        i++;
    }
    int j;
    
    for (i=0;i<MAX;i++){
        for(j=0;j<MAX;j++){
            send(connection_fd[i], f[j]->getNome().c_str(), 15, 0);
            //std::cout<<"enviando nome: "<<f[j]->getNome()<<std::endl;
        }
    }
    ok = "o";
    for(i=0;i<MAX;i++){
        send(connection_fd[i], ok.c_str(), 2, 0);
        
    }
    
    std::thread newthread0(recieve, &(connection_fd[0]), &(input_buffer[0]));
    std::thread newthread1(recieve, &(connection_fd[1]), &(input_buffer[1]));
    std::thread newthread2(recieve, &(connection_fd[2]), &(input_buffer[2]));
    std::thread newthread3(recieve, &(connection_fd[3]), &(input_buffer[3]));
    
    t0 = get_now_ms();
    i = 0;
    j=0;
    int death[MAX];
    for(int k=0;k<MAX;k++) death[k]=1;
    while (lives(death)!=1){
        if(death[i]!=0){
            t1 = get_now_ms();
            deltaT = (t1-t0)/1000;
            death[i] = f[i]->update(deltaT, f);
            if(death[i]==0){
                adversarios[i]=9;
                for(int k=0;k<MAX;k++){
                    if (adversarios[k]==i){
                        while(death[adversarios[k]]!=0){
                            f[k]->setAdversario(MAX);
                            adversarios[k]=f[k]->getAdversario();
                        }
                    }
                }
            }
            while(j<MAX){
                if(input_buffer[j]=='l'){
                    f[j]->moveRight();
                }
                else if(input_buffer[j]=='j'){
                    f[j]->moveLeft();
                }
                else if(input_buffer[j]=='z'){
                    f[j]->spin();
                }
                else if(input_buffer[j]-'0'>=0 && input_buffer[j] - '0' <=8){
                    //std::cout<<"chegou "<<input_buffer[j]<<std::endl;
                    if(adversarios[j]!=9){
                        f[j]->setAdversario(input_buffer[j]-'0', MAX);
                        adversarios[j] = f[j]->getAdversario();
                    }
                }
                if(i==j||input_buffer[j]=='z'||input_buffer[j]=='j'||input_buffer[j]=='l'||(input_buffer[j]-'0'>=0 && input_buffer[j] - '0' <=9)){
                    input_buffer[j] = 0;
                    d1.init(f[j]->getWell(), f[j]->getLixo(), adversarios);
                    send(connection_fd[j], d1.serialize().c_str(), 500, 0);
                }
                j++;
            }
            j=0;
            std::this_thread::sleep_for (std::chrono::milliseconds(70));
        }
        i++;
        if(i==MAX) i=0;
    }
    for(int k=0;k<MAX;k++){
        if(death[k]==1){
            std::cout<<"O vencedor foi: "<<f[k]->getNome()<<std::endl;
        }
    }
        
    close(socket_fd);
    
    newthread0.join();
    newthread1.join();
    newthread2.join();
    newthread3.join();
    return 0;
}
