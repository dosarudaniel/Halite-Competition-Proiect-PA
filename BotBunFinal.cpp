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
#define PI 3.14159

int main() {
    srand(time(NULL));
    std::ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    hlt::Location location;
    hlt::Location locationN;
    hlt::Location auxLocation;
    hlt::Site neighbour;
    hlt::Site neighbourN;
    hlt::Site home;
    hlt::Site auxSite;

    int move = 0;
    int poz = 0;
    int prodMax = -1;
    int myNeighbours = 0;
    int myNeighboursN = 0;
    int minStrength = 0;
    int minPoz = 0;
    int maxStrength = 0;
    int maxPoz = 0;
    int maxStrengthN = 0;
    int maxPozN = 0;

    int distNorth = 0;
    int distWest = 0;
    int distSouth = 0;
    int distEast = 0;
    int distMin = 50;
    int distAux = 0;
    bool stay = false;

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
                            prodMax = -1;
                            myNeighbours = 0;
                            minStrength = 256;
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
                                    neighbour.strength <= home.strength){
                                      if(neighbour.strength < minStrength) {
                                        minStrength = neighbour.strength;
                                        poz = i;
                                      }
                                    }
                            }

    		// daca il gasesc, il mananc
                        if(minStrength != 256){
                            move = poz;
                        }
                        else if(myNeighbours == 0) {
                          move = 0;
                        }

                        if(myNeighbours > 0 && myNeighbours < 4) {
                          if(3*home.production > home.strength)
                          move = 0;
                        }

    		// daca home e inconjurat doar de casute care sunt tot ale mele,
    		// analizez vecinii lor si aflu iar productivitatea maxima, cu
    		// conditia ca puterea mea + puterea casutei dintre noi sa ii
    		// depaseasca puterea.
                        if(myNeighbours == 4){
                            poz = 0;
                            myNeighboursN = 0;
                            maxStrengthN = 0;
                            maxPozN = 0;
                            minStrength = 256;
                            int plusMinStrength = 1000;
                            int plusMinMove = -1;
                            /*maxStrength = 0;
                            maxPoz = 0;*/

                            for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                                neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                                locationN.x = presentMap.getLocation(location, i).x;
                                locationN.y = presentMap.getLocation(location, i).y;

                                for(unsigned short j = 1; j < POSSIBLE_MOVEMENTS; j++){
                                    neighbourN = presentMap.getSite(presentMap.getLocation(locationN,j));

                                    if(neighbourN.owner == myID){
                                        myNeighboursN++;
                                    }

                                    if(neighbourN.owner != myID && neighbour.strength < neighbourN.strength &&
                                        home.strength + neighbour.strength >= neighbourN.strength) {
                                          if(home.strength + neighbour.strength < plusMinStrength) {
                                            plusMinMove = j;
                                            plusMinStrength = home.strength + neighbour.strength;
                                          }
                                          if(neighbourN.strength < minStrength) {
                                            minStrength = neighbourN.strength;
                                            poz = j;
                                          }

                                        }

                                }
                            }


    			// daca gasesc asa ceva, ma mut pe casuta dintre noi,
    			// adica donez puterea catre casuta cu productivitatea maxima
                                if(minStrength != 256) {
                                  if(plusMinStrength != 1000) {
                                    if(plusMinStrength < 255) {
                                      move = plusMinMove;
                                    } else {
                                      move = poz;
                                    }
                                  }
                                }
                                else if(myNeighboursN == 16){

                                    //move = rand() % 5;



                           //if(myNeighboursN == 16){
                                poz = 0;
                                distMin = 51;

                                for(short i = location.y - 1; i >= 0; i--){
                                    auxLocation.x = location.x;
                                    auxLocation.y = i;

                                    myfile <<"North: " << auxLocation.y << " " << auxLocation.x << "\n";
                                    auxSite = presentMap.getSite(presentMap.getLocation(auxLocation,0));
                                    if(auxSite.owner != myID){
                                        distAux = location.y - i;
                                        if(distAux < distMin){
                                            distMin = distAux;
                                            poz = 1;
                                        }

                                        break;
                                    }
                                }

                                for(short i = location.x + 1; i < presentMap.width; i++){
                                    auxLocation.x = i;
                                    auxLocation.y = location.y;

                                    myfile <<"East: " << auxLocation.y << " " << auxLocation.x << "\n";
                                    auxSite = presentMap.getSite(presentMap.getLocation(auxLocation,0));

                                    if(auxSite.owner != myID){
                                        distAux = i - location.x;
                                        if(distAux < distMin){
                                            distMin = distAux;
                                            poz = 2;
                                        }
                                        break;
                                    }
                                }

                                for(short i = location.y + 1; i < presentMap.height; i++){
                                    auxLocation.x = location.x;
                                    auxLocation.y = i;

                                    myfile <<"South: " << auxLocation.y << " " << auxLocation.x << "\n";
                                    auxSite = presentMap.getSite(presentMap.getLocation(auxLocation,0));

                                    if(auxSite.owner != myID){
                                        distAux = i - location.y;
                                        if(distAux < distMin){
                                            distMin = distAux;
                                            poz = 3;
                                        }
                                        break;
                                    }
                                }

                                for(short i = location.x - 1; i >= 0; i--){
                                    auxLocation.x = i;
                                    auxLocation.y = location.y;

                                    myfile <<"West: " << auxLocation.y << " " << auxLocation.x << "\n";
                                    auxSite = presentMap.getSite(presentMap.getLocation(auxLocation, 0));

                                    if(auxSite.owner != myID){
                                        distAux = location.x - i;
                                        if(distAux < distMin){
                                            distMin = distAux;
                                            poz = 4;
                                        }
                                        break;
                                    }
                                }

                                if(distMin != 51){
                                    move = poz;
                                }
                                else{
// AICI ERA DOAR LINIA CU RANDOM
                                //    move = rand() % 4 + 1;


                                float distantaMin = 51;
                                float unghi = 0;
                                hlt::Location locatiaMin;
                                poz = -1;

                                for(unsigned short a = 0; a < presentMap.height; a++) {
                                    for(unsigned short b = 0; b < presentMap.width; b++) {
                                        auxSite = presentMap.getSite({ b, a });
                                        auxLocation.x = b;
                                        auxLocation.y = a;

                                        if(auxSite.owner != home.owner){
                                            if(presentMap.getDistance(location, auxLocation) < distantaMin){
                                                distantaMin = presentMap.getDistance(location, auxLocation);
                                                locatiaMin.x = b;
                                                locatiaMin.y = a;
                                            }
                                        }

                                    }
                                }

                                unghi = presentMap.getAngle(location, locatiaMin);

                                if(unghi < 3 * PI / 4 && unghi >= PI / 4){
                                    poz = 1;
                                }

                                if((unghi < PI && unghi >= 3 * PI / 4 ) || (unghi < - 3 * PI / 4 && unghi >= - PI)){
                                    poz = 2;
                                }

                                if(unghi < - PI / 4 && unghi >= - 3 * PI / 4){
                                    poz = 3;
                                }

                                if(unghi < PI / 4 && unghi >= - PI / 4){
                                    poz = 4;
                                }

                                if(poz != -1){
                                    move = poz;
                                }
                                else{
                                    move = 0;
                                }


                                }

                        //    }

                        }
                    }
                    if(home.strength < 15) {
                      move = 0;
                    }
                    moves.insert({{b, a}, (unsigned char)(move) });
                }
            }
        }

        sendFrame(moves);
    }
    myfile.close();
    return 0;
}
