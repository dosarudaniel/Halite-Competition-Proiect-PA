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
    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap; // harta
    hlt::Location location; // locatia casutei pe care ma aflu
    hlt::Location locationN; // locatia unui vecin al casutei pe care ma aflu
    hlt::Location auxLocation;
    hlt::Site neighbour; // informatiile din casuta unui vecin
    hlt::Site neighbourN; // informatiile din casuta unui vecin al vecinului
    hlt::Site home; // informatile din casuta pentru care calculez mutarea
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

        //ca sa aiba timp sa creasca puterea vor fi runde la care ii voi spune
        //intregii harti sa stea
        if(rand() % 2 == 1){
            stay = true;
        }
        else{
            stay = false;
        }

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                home = presentMap.getSite({ b, a });

                //daca este casuta mea atunci trebuie sa imi pun problema in ce
                //parte sa o ia
                if (home.owner == myID) {
                    if(stay == false){
                        location.x = b;
                        location.y = a;

                        move = 0;
                        poz = 0;
                        prodMax = -1;
                        myNeighbours = 0;
                        minStrength = 256;

		                // analizez vecinii casutei in care ma aflu (home) si vad
		                // care vecin are "production" cel mai mare, cu conditia sa
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
                            }

                        }

                        //pentru "production" maxim pe care il obtin, tratez
                        //cazul in care productia maxima este avuta de
                        //mai multe casute;
                        //in cazul asta voi alege sa merg pe casuta cu
                        //"strength" mai mic
                        for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                            neighbour = presentMap.getSite(presentMap.getLocation(location, i));

                            if(neighbour.owner != myID &&
                                neighbour.strength <= home.strength &&
                                neighbour.production == prodMax && neighbour.strength < minStrength){
                                    minStrength = neighbour.strength;
                                    poz = i;
                            }
                        }

    		            // daca exista o casuta pe care pot sa o iau, merg pe ea;
                        // daca mai exista cel putin o casuta in jurul meu si daca
                        //nu pot sa mananc nimic atunci ii spun sa stea
                        if(prodMax != -1){
                            move = poz;
                        }
                        else{
                            move = 0;
                        }

    		            // daca home e inconjurat doar de casute care sunt tot ale mele,
    		            // analizez vecinii lor si aflu iar productivitatea maxima, cu
    		            // conditia ca puterea mea + puterea casutei dintre noi sa ii
    		            // depaseasca puterea.
                        if(myNeighbours == 4){
                            prodMax = -1;
                            poz = 0;
                            myNeighboursN = 0;
                            maxStrengthN = 0;
                            maxPozN = 0;
                            minStrength = 256;

                            //parcurg iar toti vecinii mei
                            for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                                neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                                locationN.x = presentMap.getLocation(location, i).x;
                                locationN.y = presentMap.getLocation(location, i).y;

                                //pentru fiecare vecin de-al meu, parcurg toti vecinii lui
                                for(unsigned short j = 1; j < POSSIBLE_MOVEMENTS; j++){
                                    neighbourN = presentMap.getSite(presentMap.getLocation(locationN,j));

                                    //numar cati din vecinii vecinilor sunt ai mei
                                    if(neighbourN.owner == myID){
                                        myNeighboursN++;
                                    }

                                    //calculez productia maxima pe vecinii vecinilor
                                    if(neighbourN.owner != myID &&
                                        home.strength + neighbour.strength >= neighbourN.strength &&
                                        neighbourN.production > prodMax){
                                            prodMax = neighbourN.production;
                                        }

                                    //verfic si care vecin are puterea cea mai mare
                                    //in situatia in care voi avea vecini ai
                                    //vecinilor care nu sunt mancati sa stiu sa
                                    //dau puterea vecinului care duce la casuta
                                    //cea mai puternica
                                    if(neighbourN.owner != myID && neighbourN.strength > maxStrengthN){
                                        maxStrengthN = neighbourN.strength;
                                        maxPozN = i;
                                    }

                                }
                            }

                            //pentru productia maxima calculata verific cazul in
                            //care pot fi mai multe casute cu aceeasi productie
                            for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                                neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                                locationN.x = presentMap.getLocation(location, i).x;
                                locationN.y = presentMap.getLocation(location, i).y;

                                for(unsigned short j = 1; j < POSSIBLE_MOVEMENTS; j++){
                                    neighbourN = presentMap.getSite(presentMap.getLocation(locationN,j));

                                    //pentru casutele cu aceeasi productie (maxima)
                                    //o voi alege pe cea cu puterea cea mai mica
                                    if(neighbourN.owner != myID &&
                                        home.strength + neighbour.strength >= neighbourN.strength &&
                                        neighbourN.production == prodMax && neighbourN.strength < minStrength){

                                            minStrength = neighbourN.strength;
                                            poz = i;

                                        }

                                }
                            }


                			// daca exista o casuta cu productivitate maxima pe
                            //care o pot cuceri, dau puterea vecinului care duce
                            //la ea
                            if(prodMax != -1){
                                move = poz;
                            }
                            else
                                //daca nu s-a putut calcula o productivitate maxima
                                //si exista vecini ai vecinilor care inca nu sunt
                                //ai mei, dau puterea spre vecinul care duce la
                                //casuta cea mai puternica dintre cele care nu
                                //sunt ale mele
                                if(myNeighboursN != 16){
                                    move = maxPozN;
                                }

                                //analizez cazul in care atat vecinii cat si vecinii
                                //vecinilor sunt ai mei
                                else{

                                    //vreau sa gasesc cea mai apropiata casuta,
                                    //care nu este a mea, fata de casuta curenta
                                    poz = 0;
                                    distMin = 51;

                                    //caut de la casuta curenta in sus
                                    for(short i = location.y - 1; i >= 0; i--){
                                        auxLocation.x = location.x;
                                        auxLocation.y = i;

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

                                    //caut de la casuta curenta in dreapta
                                    for(short i = location.x + 1; i < presentMap.width; i++){
                                        auxLocation.x = i;
                                        auxLocation.y = location.y;
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

                                    //caut de la casuta curenta in jos
                                    for(short i = location.y + 1; i < presentMap.height; i++){
                                        auxLocation.x = location.x;
                                        auxLocation.y = i;
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

                                    //caut de la casuta curenta in stanga
                                    for(short i = location.x - 1; i >= 0; i--){
                                        auxLocation.x = i;
                                        auxLocation.y = location.y;
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

                                    //daca pe linie sau coloana se gaseste o casuta
                                    //care inca nu este a mea si este cea mai
                                    //apropiata, merg spre ea
                                    if(distMin != 51){
                                        move = poz;
                                    }

                                    //tratez cazul in care toate casutele de pe
                                    //linie si coloana sunt ale mele

                                    else{

                                        float distantaMin = 51;
                                        float unghi = 0;
                                        hlt::Location locatiaMin;
                                        poz = -1;

                                        //cauta in restul hartii casuta cea mai
                                        //apropiata de casuta pe care ma aflu
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

                                        //verific in ce cadran se afla casuta cea
                                        //mai apropiata si aleg sa merg spre ea
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
                                }
                        }
                        //acutalizez variabla astfel incat daca tura curenta
                        //casutele se misca, tura viitoare sa stea toate pe loc
                        stay = true;
                    }
                    else{
                        //acutalizez variabila astfel incat daca tura curenta
                        //casutele nu s-au miscat, tura viitoare sa se miste
                        stay = false;
                        move = 0;
                    }

                    //adaug mutarea pentru casuta curenta
                    moves.insert({{b, a}, (unsigned char)(move) });
                }
            }
        }

        sendFrame(moves);
    }
    return 0;
}
