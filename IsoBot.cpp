#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>

#include "hlt.hpp"
#include "networking.hpp"

#define POSSIBLE_MOVEMENTS 5

int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    hlt::Location location;
    hlt::Location locationN;
    hlt::Site neighbour;
    hlt::Site neighbourN;
    hlt::Site home;

    int move = 0;
    int poz = 0;
    int prodMax = 0;
    int myNeighbours = 0;

    getInit(myID, presentMap);
    sendInit("RandomC++Bot");

    std::set<hlt::Move> moves;

    while(true) {

        moves.clear();
        getFrame(presentMap);

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                home = presentMap.getSite({ b, a });
                if (home.owner == myID) {
                    location.x = b;
                    location.y = a;

                    move = 0;
                    poz = 0;
                    prodMax = 0;
                    myNeighbours = 0;
                    //moves.insert({ { b, a }, (unsigned char)(rand() % 5) });
			
		// analizez vecinii casutei in care ma aflu (home) si vad
		// care vecin are production cel mai mare, cu conditia sa
		// il pot si manca.
                    for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                        neighbour = presentMap.getSite(presentMap.getLocation(location, i));

                        if(neighbour.owner == myID){
                            myNeighbours++;
                        }

                        if(neighbour.owner != myID &&
                            neighbour.strength <= home.strength &&
                            neighbour.production > prodMax){
                            prodMax = neighbour.production;
                            poz = i;
                        }
                    }
		
		// daca il gasesc, il mananc
                    if(prodMax != 0){
                        move = poz;
                    }

		// daca home e inconjurat doar de casute care sunt tot ale mele,
		// analizez vecinii lor si aflu iar productivitatea maxima, cu
		// conditia ca puterea mea + puterea casutei dintre noi sa ii 
		// depaseasca puterea.
                    if(myNeighbours == 4){
                        prodMax = 0;
                        poz = 0;
                        for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                            neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                            locationN.x = presentMap.getLocation(location, i).x;
                            locationN.y = presentMap.getLocation(location, i).y;

                            for(unsigned short j = 1; j < POSSIBLE_MOVEMENTS; j++){
                                neighbourN = presentMap.getSite(presentMap.getLocation(locationN,j));

                                if(neighbourN.owner != myID &&
                                    home.strength + neighbour.strength >= neighbourN.strength &&
                                    neighbourN.production > prodMax){
                                        prodMax = neighbourN.production;
                                        poz = i;
                                    }
                            }
	
			// daca gasesc asa ceva, ma mut pe casuta dintre noi,
			// adica donez puterea catre casuta cu productivitatea maxima
                            if(prodMax != 0){
                                move = i;
                            }
                        }

                    }

                    moves.insert({{b, a}, (unsigned char)(move) });
                }
            }
        }

        sendFrame(moves);
    }

    return 0;
}
