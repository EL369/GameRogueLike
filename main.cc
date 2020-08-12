#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "floor.h"

#include "player.h"
#include "enemy.h"


std::string defaultFile ="cc3kfloor.txt";

std::vector<std::shared_ptr<Floor>> floors;

int main(int argc, char* argv[]) {
    std::ifstream file(defaultFile);
    if (argc > 1) {
        std::ifstream file(argv[1]);
        if(!file.good()){
            std::cout<<"File is invalid.";
            exit(1);
        }
    }
    std::vector<std::vector<char>> grid;
    std::string s;
    std::vector<char> line;
    int id = 0;
    int row = 0;
    int col = 79;
    while (std::getline(file, s)){
        for(int i=0; i<col; ++i){
            line.emplace_back(s[i]);            
            // std::cout<<"---char---"<<line[i]<<std::endl;
        }grid.emplace_back(line);
        ++row;
        s = "";
        line.clear();
        if(row==25){
            floors.emplace_back(std::make_shared<Floor>(grid, id));
            // std::cout<<"--id---"<<id<<std::endl;
            ++id;
            row = 0;
        }
    }
    bool restart= true;
    while (restart){
        restart = false;
        std::cout<<"Please choose your character: \ns for shade(defualt), d for drow, v for vampire, t for troll, g for goblin"<<std::endl;
        char playerChar;
        std::shared_ptr<Player> player;
        while (true){
            std::cin>>playerChar;
            if (playerChar == 'd'){
                player = std::make_shared<Drow>();
                break;
            }
            else if (playerChar == 'v'){
                player = std::make_shared<Vampire>();
                break;
            }
            else if (playerChar == 't'){
                player = std::make_shared<Troll>();
                break;
            }
            else if (playerChar == 'g'){
                player = std::make_shared<Goblin>();
                break;
            }
            else{
                player = std::make_shared<Shade>();
                break;
            }
        }
        bool exit = false;
        std::cout<<"Your character is: "<<playerChar<<std::endl;
        std::cout<<"Generating the floor..."<<std::endl;
        for(int i=0; i<5; ++i){
            std::cout << "--------Level "<< std::to_string(i+1)<<"--------" << std::endl;
            (floors[i])->generateAll(player);
            floors[i]->getPlayer()->removeTempPotion();
            std::string cmd, arg;
            bool enemyMoving = true;
            while (std::cin>>cmd){
                if(cmd == "no" || cmd== "so" || cmd == "ea" || cmd == "we" || cmd == "ne" || cmd == "nw" || cmd == "se" || cmd == "sw"){
                    (floors[i])->playerMove(cmd);
                }
                else if (cmd == "u"){
                    std::cin>>arg;
                    (floors[i])->playerUsePotion(arg);
                }
                else if (cmd == "a"){
                    std::cin>>arg;
                    (floors[i])->playerAttack(arg);
                }
                else if (cmd == "f"){
                    enemyMoving = !enemyMoving;
                }
                else if (cmd == "q"){
                    exit = true;
                    break;
                }
                else if (cmd == "r"){
                    restart = true;
                    exit = true;
                    floors[i]->getPlayer()->removeTempPotion();
                    std::cout << "Restarts the game" << std::endl;
                    floors[i]->clearFloor();
                    break;
                }
                else{
                    floors[i]->setAction("Invalid command");
                }
                floors[i]->enemyAttackPlayer();
                if (enemyMoving == true){
                    floors[i]->Enemymove();
                }
                floors[i]->print();
                if (player->getAlive() == false){
                    exit = true;
                    break;
                }
                if(floors[i]->getReachStair()){
                    std::cout<< "Player reached the stair, entering the next level..."<<std::endl;
                    floors[i]->getPlayer()->removeTempPotion();
                    break;
                }
            }
            if(exit) break;
        }
    }
}
