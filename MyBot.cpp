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
    float quality;
    float max = 0;
    int move = 0;

    hlt::Location location;
    hlt::Site neighbour;
    hlt::Site home;


    
    hlt::GameMap presentMap;
    getInit(myID, presentMap);
    sendInit("MyC++Bot");

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
                    quality = 0;
                    max = 0;
                    move = 0;

                    // Calculate the best place to go:
                    // Best place to go == max production/strength raport and strength less than the present place 

                    for (int i = 1; i < POSSIBLE_MOVEMENTS; i++) {
                        neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                        if(neighbour.owner != myID && neighbour.strength < home.strength){
                            if(neighbour.strength > 0){
                                quality = (float) neighbour.production / neighbour.strength;
                            }
                            else{
                                quality = (float)9999999;
                                move = i;
                            }
                            if (quality > max){
                                move = i;
                            }
                        }
                        
                    }

                     
                    //TO DO: Find a better strategy for pieces in the center of my set of pieces,
                    //      such as: a special "depth traversal"
                    //      or move pieces from center to the edges.

                    // If I just decided to stay home (STILL),
                    if(rand()%5 == 1){
                        for (int i = 1; i < POSSIBLE_MOVEMENTS; i++) {
                            neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                            if(move == STILL && neighbour.owner == myID){
                                //Combine the strength of two pieces
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
