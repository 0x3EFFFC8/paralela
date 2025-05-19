#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <omp.h>

using namespace std;

enum EntityType { EMPTY, ROCK, RABBIT, FOX };

struct Entity {
    EntityType type;
    int x, y, birthGen, lastEat;
};

struct pair_hash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<long long>()(((long long)p.first << 32) | (unsigned int)p.second);
    }
};

int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, R, C, N;
const vector<pair<int, int>> directions = {{-1,0},{0,1},{1,0},{0,-1}};
unordered_map<pair<int, int>, Entity*, pair_hash> grid;
vector<Entity*> rocks, rabbits, foxes;

void addEntity(EntityType type, int x, int y, int birthGen = 0, int lastEat = 0) {
    Entity* newEntity = new Entity{type, x, y, birthGen, lastEat};
    grid[{x, y}] = newEntity;
    if (type == ROCK) rocks.push_back(newEntity);
    else if (type == RABBIT) rabbits.push_back(newEntity);
    else if (type == FOX) foxes.push_back(newEntity);
}

Entity* getEntity(int x, int y) {
    auto it = grid.find({x, y});
    return it != grid.end() ? it->second : nullptr;
}

vector<pair<int, int>> getValidMoves(Entity* entity, bool foxLookingForFood) {
    vector<pair<int, int>> validMoves;
    for (const auto& dir : directions) {
        int nx = entity->x + dir.first;
        int ny = entity->y + dir.second;
        if (nx >= 0 && nx < R && ny >= 0 && ny < C) {
            Entity* target = getEntity(nx, ny);
            if (!foxLookingForFood) {
                if (target == nullptr) validMoves.emplace_back(nx, ny);
            } else { 
                if (target != nullptr && target->type == RABBIT) 
                    validMoves.emplace_back(nx, ny);
            }
        }  
    }
    return validMoves;
}

void processRabbits(int currentGen) {
    unordered_map<pair<int, int>, vector<Entity*>, pair_hash> destinationMap;
    vector<unordered_map<pair<int, int>, vector<Entity*>, pair_hash>> threadMaps(omp_get_max_threads());
    vector<Entity*> rabbitsToProcess = rabbits;
    int n = rabbitsToProcess.size();
    rabbits.clear();

    #pragma omp parallel
    {
        auto& localDestinationMap = threadMaps[omp_get_thread_num()];

        #pragma omp for schedule(dynamic, 100)
        for (int i = 0; i < n; i++) {
            Entity* rabbit = rabbitsToProcess[i];
            vector<pair<int, int>> validMoves = getValidMoves(rabbit, false);
            if (!validMoves.empty()) {
                int choice = (currentGen + rabbit->x + rabbit->y) % validMoves.size();
                pair<int, int> newPos = validMoves[choice];
                if ((currentGen - rabbit->birthGen) >= GEN_PROC_RABBITS) {
                    Entity* newRabbit = new Entity{RABBIT, rabbit->x, rabbit->y, currentGen + 1, currentGen + 1};
                    localDestinationMap[{rabbit->x, rabbit->y}].push_back(newRabbit);
                    rabbit->birthGen = currentGen + 1;
                }
                rabbit->x = newPos.first;
                rabbit->y = newPos.second;
            }
            localDestinationMap[{rabbit->x, rabbit->y}].push_back(rabbit);
        }
    }

    #pragma omp parallel sections
    {
        #pragma section
        {
            for (auto& threadMap : threadMaps) {
                for (auto& [pos, rabbits] : threadMap) {
                    destinationMap[pos].insert(destinationMap[pos].end(), rabbits.begin(), rabbits.end());
                }
            } 
        }
        #pragma section
        {
            grid.clear();
        }
    }

    for (auto& [pos, rabbitList] : destinationMap) {
        auto oldest = max_element(rabbitList.begin(), rabbitList.end(),
            [currentGen](Entity* a, Entity* b) {
                return (currentGen - a->birthGen) < (currentGen - b->birthGen);
        });

        grid[pos] = *oldest;
        rabbits.push_back(*oldest);

        for (Entity* rabbit : rabbitList) {
            if (rabbit != *oldest) delete rabbit;
        }
    }
    #pragma omp sections
    {
        #pragma section
        {
            for (Entity* rock : rocks) grid[{rock->x, rock->y}] = rock;
            for (Entity* fox : foxes) grid[{fox->x, fox->y}] = fox;
        }
        #pragma section
        {
            destinationMap.clear();
        }
    }
}

void processFoxes(int currentGen) {
    unordered_map<pair<int, int>, vector<Entity*>, pair_hash> destinationMap;
    vector<unordered_map<pair<int, int>, vector<Entity*>, pair_hash>> threadMaps(omp_get_max_threads());
    vector<unordered_set<Entity*>> threadRabbitsToRemove(omp_get_max_threads());
    vector<Entity*> foxesToProcess = foxes;
    int n = foxesToProcess.size();
    foxes.clear();

    #pragma omp parallel
    {
        auto& localDestinationMap = threadMaps[omp_get_thread_num()];
        auto& localRabbitsToRemove = threadRabbitsToRemove[omp_get_thread_num()];

        #pragma omp for schedule(dynamic, 100)
        for (int i = 0; i < n; i++) {
            Entity* fox = foxesToProcess[i];
            vector<pair<int, int>> rabbitMoves = getValidMoves(fox, true);
            bool ateRabbit = false;

            if (!rabbitMoves.empty()) {
                int choice = (currentGen + fox->x + fox->y) % rabbitMoves.size();
                pair<int, int> newPos = rabbitMoves[choice];
            
                Entity* rabbit = getEntity(newPos.first, newPos.second);
                if (rabbit) localRabbitsToRemove.insert(rabbit);

                if ((currentGen - fox->birthGen) >= GEN_PROC_FOXES) {
                    Entity* newFox = new Entity{FOX, fox->x, fox->y, currentGen + 1, currentGen + 1};
                    localDestinationMap[{fox->x, fox->y}].push_back(newFox);
                    fox->birthGen = currentGen + 1;
                }

                fox->x = newPos.first;
                fox->y = newPos.second;
                fox->lastEat = currentGen + 1;
                localDestinationMap[{fox->x, fox->y}].push_back(fox);
                ateRabbit = true;
            }

            if (!ateRabbit && (currentGen - fox->lastEat) + 1 < GEN_FOOD_FOXES) {
                vector<pair<int, int>> emptyMoves = getValidMoves(fox, false);
                if (!emptyMoves.empty()) {
                    int choice = (currentGen + fox->x + fox->y) % emptyMoves.size();
                    pair<int, int> newPos = emptyMoves[choice];
                    if ((currentGen - fox->birthGen) >= GEN_PROC_FOXES) {
                        Entity* newFox = new Entity{FOX, fox->x, fox->y, currentGen + 1, currentGen + 1};
                        localDestinationMap[{fox->x, fox->y}].push_back(newFox);
                        fox->birthGen = currentGen + 1;
                    }
                    fox->x = newPos.first;
                    fox->y = newPos.second;
                }
                localDestinationMap[{fox->x, fox->y}].push_back(fox);
            }
        }
    }

    #pragma omp parallel sections 
    {
        #pragma section
        {
            unordered_set<Entity*> uniqueRabbitsToRemove;
            for (const auto& localSet : threadRabbitsToRemove) {
                uniqueRabbitsToRemove.insert(localSet.begin(), localSet.end());
            }
        
            for (Entity* rabbit : uniqueRabbitsToRemove) {
                auto it = find(rabbits.begin(), rabbits.end(), rabbit);
                if (it != rabbits.end()) {
                    rabbits.erase(it);
                    delete rabbit;
                }
            }
        }
        #pragma section
        {
            for (auto& threadMap : threadMaps) {
                for (auto& [pos, foxes] : threadMap) {
                    destinationMap[pos].insert(destinationMap[pos].end(), foxes.begin(), foxes.end());
                }
            }  
        }
        #pragma section
        {
            grid.clear();
        }
    }

    for (auto& [pos, foxList] : destinationMap) {
        sort(foxList.begin(), foxList.end(), [](Entity* a, Entity* b) {
            if (a->birthGen != b->birthGen) 
                return a->birthGen < b->birthGen;
            return a->lastEat > b->lastEat;
        });
                    
        grid[pos] = foxList[0];
        foxes.push_back(foxList[0]);

        for (size_t i = 1; i < foxList.size(); i++) {
            delete foxList[i];
        }
    }
    
    #pragma omp parallel sections
    {
        #pragma section
        {
            for (Entity* rock : rocks) grid[{rock->x,rock->y}] = rock;
            for (Entity* rabbit : rabbits) grid[{rabbit->x,rabbit->y}] = rabbit;
        }
        #pragma section
        {
            destinationMap.clear();
        }
    }
}

void printWorld(int generation) {
    if (generation != 0) cout << endl << endl;
    cout << "Generation " << generation << endl;

    for (int i = 0; i < C + 2; i++) cout << "-";
    cout << "   ";
    for (int i = 0; i < C + 2; i++) cout << "-";
    cout << " ";
    for (int i = 0; i < C + 2; i++) cout << "-";
    cout << endl;

    vector<vector<char>> m1(R, vector<char>(C, ' ')), m2(R, vector<char>(C, ' ')), m3(R, vector<char>(C, ' '));

    for (Entity* rock : rocks) {
        if (rock->x >= 0 && rock->x < R && rock->y >= 0 && rock->y < C) {
            m1[rock->x][rock->y] = '*';
            m2[rock->x][rock->y] = '*';
            m3[rock->x][rock->y] = '*';
        }
    }

    for (Entity* rabbit : rabbits) {
        if (rabbit->x >= 0 && rabbit->x < R && rabbit->y >= 0 && rabbit->y < C && m1[rabbit->x][rabbit->y] != '*') {
            m1[rabbit->x][rabbit->y] = 'R';
            m2[rabbit->x][rabbit->y] = '0' + max(0, generation - rabbit->birthGen) % 10;
            m3[rabbit->x][rabbit->y] = 'R';
        }
    }

    for (Entity* fox : foxes) {
        if (fox->x >= 0 && fox->x < R && fox->y >= 0 && fox->y < C && m1[fox->x][fox->y] != '*') {
            m1[fox->x][fox->y] = 'F';
            m2[fox->x][fox->y] = '0' + max(0, generation - fox->birthGen) % 10;
            int hunger = max(0, generation - fox->lastEat);
            if (hunger >= 0 && hunger <= 9) {
                m3[fox->x][fox->y] = '0' + hunger;
            } else {
                m3[fox->x][fox->y] = '+';
            }
        }
    }

    for (int i = 0; i < R; i++) {
        cout << "|";
        for (int j = 0; j < C; j++) cout << m1[i][j];
        cout << "|   |";
        for (int j = 0; j < C; j++) cout << m2[i][j];
        cout << "| |";
        for (int j = 0; j < C; j++) cout << m3[i][j];
        cout << "|" << endl;
    }

    for (int i = 0; i < C + 2; i++) cout << "-";
    cout << "   ";
    for (int i = 0; i < C + 2; i++) cout << "-";
    cout << " ";
    for (int i = 0; i < C + 2; i++) cout << "-";
}

void cleanup() {
    for (auto& [pos, entity] : grid) delete entity;
    #pragma omp parallel sections
    {
        #pragma section
        {grid.clear();}
        #pragma section
        {rocks.clear();}
        #pragma section
        {rabbits.clear();}
        #pragma section
        {foxes.clear();}
    }
}

void printFinalState() {
    cout << GEN_PROC_RABBITS << " " << GEN_PROC_FOXES << " " << GEN_FOOD_FOXES << " 0 " << R << " " << C << " " << (rocks.size() + rabbits.size() + foxes.size()) << endl;
    for (int x = 0; x < R; x++) {
        for (int y = 0; y < C; y++) {
            auto it = grid.find({x, y});
            if (it != grid.end()) {
                Entity* entity = it->second;
                switch(entity->type) {
                    case ROCK:
                        cout << "ROCK " << x << " " << y << endl;
                        break;
                    case RABBIT:
                        cout << "RABBIT " << x << " " << y << endl;
                        break;
                    case FOX:
                        cout << "FOX " << x << " " << y << endl;
                        break;
                    case EMPTY:
                        break;
                }
            }
        }
    }
}

int main() {
    int x, y;
    string type;
    cin >> GEN_PROC_RABBITS >> GEN_PROC_FOXES >> GEN_FOOD_FOXES >> N_GEN >> R >> C >> N;
    
    for (int i = 0; i < N; i++) {
        cin >> type >> x >> y;
        if (type == "ROCK") addEntity(ROCK, x, y);
        else if (type == "RABBIT") addEntity(RABBIT, x, y);
        else if (type == "FOX") addEntity(FOX, x, y);
    }
    
    omp_set_num_threads(omp_get_max_threads());
    
    //printWorld(0);
    for (int i = 0; i < N_GEN; i++) {
        processRabbits(i);
        processFoxes(i);
        //printWorld(i + 1);
    }
    //cout << endl << endl;
    printFinalState(); 
    cleanup();
    return 0;
}
