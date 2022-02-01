#include <iostream>
#include <vector>
#include <random>
#include <thread>

std::random_device rd;


bool inputValidation(int fishes, int sizeX, int sizeY){
    if (sizeX<2 || sizeY < 2 ){
        std::cout<< "aquarium is too small";
        return false;
    }
    else if (!fishes){
        std::cout<< "no fishes, no fun";
        return false;
    }
    else if (fishes/sizeY*sizeX ){
        std::cout<< "too many fishes, crime against fishanity";
        return false;
    }
    return true;
}
void makeAquarium(int sizeX, int sizeY,std::vector<std::vector<char>>& aquarium ){
    aquarium.resize(sizeX+2);
    for (auto & i : aquarium) {
        i.resize(sizeY+2);
    }
    for (int i = 0; i < aquarium.size(); ++i) {
        for (int j = 0; j < aquarium[i].size(); ++j) {
            if (i == 0 || i == aquarium.size() - 1 || j == 0 || j == aquarium[i].size() - 1) {
                aquarium[i][j]='H';
            }
            else if (i == 1 && j > 0 && j < aquarium[i].size() - 1) {
                aquarium[i][j]='-';
            } else {
                aquarium[i][j]=' ';
            }
        }
    }
}
void fillAq(int fishes,std::vector<std::vector<char>>& aquarium){
    for (int i = 0; i < fishes; ++i) {
        int x, y;
        x=rd()%aquarium.size();
        y=rd()%aquarium[0].size();
        if (aquarium[x][y-1]==' '&& aquarium[x][y]==' ' && aquarium[x][y+1]==' '){
            if (rd()%2){                //TODO nahradit vlakny
                aquarium[x][y-1]='<';
                aquarium[x][y]='O';
                aquarium[x][y+1]='X';
            }
            else{       //ryba kouka na opacnou stranu
                aquarium[x][y-1]='X';
                aquarium[x][y]='O';
                aquarium[x][y+1]='>';
            }
        }
        else        {
            --i;
        }

    }
}
class Fish{
    int x, y;
    bool direction;
//    int color;  //priprava na barvu
public:
    Fish(int x, int y, bool direction) : x(x), y(y), direction(direction) {}

    void rotate(std::vector<std::vector<char>>& aquarium){
        if (this->direction){
            aquarium[this->x][this->y-1]='X';
            aquarium[this->x][this->y+1]='>';
        }
        else {
            aquarium[this->x][this->y-1]='<';
            aquarium[this->x][this->y+1]='X';
        }
        this->direction=! this->direction;
    }
    void move(std::vector<std::vector<char>>& aquarium){    //neotestovano
        if (this->direction){
            if (this->y-2=='H'){
                rotate(aquarium);
            }
            else {
                this->y-=1;
                aquarium[this->x][this->y-1]='<';
                aquarium[this->x][this->y]='O';
                aquarium[this->x][this->y+1]='X';
                aquarium[this->x][this->y+2]=' ';
            }
        }
        else{
            if (this->y+2=='H'){
                rotate(aquarium);
            }
            else {
                this->y+=1;
                aquarium[this->x][this->y-2]=' ';
                aquarium[this->x][this->y-1]='X';
                aquarium[this->x][this->y]='O';
                aquarium[this->x][this->y+1]='>';
            }
        }
    }
};
void printAq(std::vector<std::vector<char>>& aquarium){
    for (auto & i : aquarium) {
        for (char j : i) {
            std::cout<< j;
        }
        std::cout<< std::endl;
    }
}
void endOfStep(std::vector<std::vector<char>>& aquarium){
    #if defined(_WIN_32)
        system("cls")
    #endif
    #if defined(linux)
        std::cout << "\033[2J\033[1;1H";
    #endif
    printAq(aquarium);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

void fishLogic(int x, int y, bool direction, std::vector<std::vector<char>>& aquarium){
    Fish fish(x, y, direction);
    if (direction){
        aquarium[x][y-1]='<';
        aquarium[x][y]='O';
        aquarium[x][y+1]='X';
    } else{
        aquarium[x][y-1]='X';
        aquarium[x][y]='O';
        aquarium[x][y+1]='>';
    }
//    while (true) {
//        fish.move(aquarium);
//    }
}

void pFillAq(int fishes,std::vector<std::vector<char>>& aquarium, std::vector<std::thread> &pFishes){
    for (int i = 0; i < fishes; ++i) {
        int x, y;
        x=rd()%aquarium.size();
        y=rd()%aquarium[0].size();
        if (aquarium[x][y-1]==' '&& aquarium[x][y]==' ' && aquarium[x][y+1]==' '){
            if (rd()%2){
                pFishes.emplace_back(std::thread(fishLogic, x, y, true, std::ref(aquarium)));
            }
            else{       //ryba kouka na opacnou stranu
                pFishes.emplace_back(std::thread(fishLogic, x, y, false, std::ref(aquarium)));
            }
        }
        else        {
            --i;
        }
        for (std::thread & fish : pFishes){
            if (fish.joinable())
                fish.join();
        }
    }
}
int main() {
    int fishes, sizeX, sizeY;
    std::cout   << "Aquarium heigh :";
    std::cin    >> sizeX;
    std::cout   << "Aquarium width :";
    std::cin    >> sizeY;
    std::cout   << "Number of fishes:";
    std::cin    >> fishes;
    std::cout   << std::endl;
    if (!inputValidation(fishes, sizeX, sizeY)){
        return 1;
    }
    std::vector<std::vector<char>> aquarium;
    std::vector<std::thread> pFishes;
    makeAquarium(sizeX, sizeY, aquarium);
    pFillAq(fishes, aquarium, pFishes);
    printAq(aquarium);
    //TODO obarvit rybky https://gist.github.com/vratiu/9780109
    return 0;
}
