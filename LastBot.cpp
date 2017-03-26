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
		
		//pentru fiecare casuta
        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                home = presentMap.getSite({ b, a });
                //daca e a mea
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
    		// pe care ii pot cuceri; daca am mai multe optiuni, aleg pe cea
    		// cu Strength minim.
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

    		// daca am gasit o casuta dupa regula de mai sus,
			// o cuceresc
                        if(minStrength != 256){
                            move = poz;
                        }
						//daca nu pot cuceri niciunul din cei 4, stau pe loc
                        if(myNeighbours == 0 && minStrength == 256) {
                          move = 0;
                        }
						//daca am 1-3 vecini deja cuceriti, daca totusi
						//puterea mea se va dubla in 3 ture, aleg sa stau
                        if(myNeighbours > 0 && myNeighbours < 4) {
                          if(3*home.production > home.strength)
                          move = 0;
                        }

    		// daca home e inconjurat doar de casute care sunt tot ale mele,
    		// analizez vecinii lor si aplic doua criterii de alegere
                        if(myNeighbours == 4){
                            poz = 0;
                            myNeighboursN = 0;
                            maxStrengthN = 0;
                            maxPozN = 0;
                            minStrength = 256;
                            int plusMinStrength = 512;
                            int plusMinMove = -1;
                            /*maxStrength = 0;
                            maxPoz = 0;*/

							// pentru fiecare vecin
                            for(unsigned short i = 1; i < POSSIBLE_MOVEMENTS; i++){
                                neighbour = presentMap.getSite(presentMap.getLocation(location, i));
                                locationN.x = presentMap.getLocation(location, i).x;
                                locationN.y = presentMap.getLocation(location, i).y;
								
								
								//pentru fiecare dintre vecinii lui
                                for(unsigned short j = 1; j < POSSIBLE_MOVEMENTS; j++){
                                    neighbourN = presentMap.getSite(presentMap.getLocation(locationN,j));

                                    if(neighbourN.owner == myID){
                                        myNeighboursN++;
                                    }
									// daca nu il am si vecinul meu direct nu il poate cuceri, dar
									// primind puterea mea va putea,
									// atunci folosesc doua criterii de alegere a mutarii:
                                    if(neighbourN.owner != myID && neighbour.strength < neighbourN.strength &&
                                        home.strength + neighbour.strength >= neighbourN.strength) {
											//1. Daca puterea obtinuta prin cedarea puterii din
											//home unui vecin este minima
                                          if(home.strength + neighbour.strength < plusMinStrength) {
                                            plusMinMove = j;
                                            plusMinStrength = home.strength + neighbour.strength;
                                          }
										  //daca puterea vecinului ce trebuie cucerit este minima
                                          if(neighbourN.strength < minStrength) {
                                            minStrength = neighbourN.strength;
                                            poz = j;
                                          }

                                        }

                                }
                            }


    			// daca pe primul caz am obtinut o putere mai mica de 255, adica nu pierd putere
				// aleg aceasta mutare
				// daca nu, aleg mutarea spre vecinul cu Strength minim
                                if(minStrength != 256) {
                                    if(plusMinStrength < 255) {
                                      move = plusMinMove;
                                    } else {
                                      move = poz;
                                    }
                                }
								
								// daca detin toti vecinii directi si toti vecinii vecinilor mei directi
                                else if(myNeighboursN == 16){

                                    //move = rand() % 5;



                           //if(myNeighboursN == 16){
                                poz = 0;
                                distMin = 51;
								
								// Caut distanta minima pana la prima casuta pe care nu o detin
								// Prima data spre Nord
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
								// Dupa spre EST
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
								
								// Spre sud
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
								
								// Spre Vest
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
								//Daca am gasit o distanta minima spre N sau E sau S sau V, 
								//fac mutarea corespunzatoare directiei
                                if(distMin != 51){
                                    move = poz;
                                }
								// Altfel, parcurg matricea in intregime si caut casuta necucerita
								// la distanta cea mai mica de mine (adica de home.location)
                                else{
								
                                float distantaMin = 100;
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
								
								// in functie de valoarea unghiului dintre mine si casuta gasita,
								// voi alege directia de deplasare
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
					// Indiferent de algoritmul de mai sus, daca o casuta are puterea mai mica de 15
					// atunci mai bine sta pe loc
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
