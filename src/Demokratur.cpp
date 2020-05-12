//============================================================================
// Name        : Demokratur.cpp
// Author      : Johannes Uttecht
// Description : Demokratur-Simulation
//============================================================================

#include <iostream>
#include <Windows.h>
#include <assert.h>
#include "Citizen.h"
using namespace std;


void getBoard(Citizen citizens[], int size, string stringSize) {

    for (int i = 0; i < size; i++) {
        if (i % std::stoi(stringSize) == 0) {
            cout << "\n";
        }
        cout << citizens[i].party << " ";
    }
    cout << "\n";
}

bool check(Citizen citizens[], int size) {
    for (int i = 0; i < size; i++) {
        if (citizens[i].party != citizens[0].party) {
            return false;
        }
    }
    return true;
}


int get_neighbor(int citizen_choice, int size, string stringSize, bool wrap) {
	int width = std::stoi(stringSize);
    int neighbor_choice;
    int c = rand() % 4;

    switch (c) {
        case 0: // S
            neighbor_choice = citizen_choice + width;
            if (neighbor_choice > size-1) {
            	neighbor_choice = wrap ? (neighbor_choice - size) : citizen_choice;
            }
        break;

        case 1: //N
            neighbor_choice = citizen_choice - width;
            if (neighbor_choice < 0) {
            	neighbor_choice = wrap ? (neighbor_choice + size) : citizen_choice;
            }
        break;
        case 2: // W
            neighbor_choice = citizen_choice - 1;
            if (neighbor_choice % width == width-1) {
            	neighbor_choice = wrap ? (neighbor_choice + (width-1)) : citizen_choice;
            } if (neighbor_choice == -1) {
                neighbor_choice = wrap ? (size-1) : citizen_choice;
            } else if (neighbor_choice >= size) {
            	neighbor_choice = wrap ? (neighbor_choice - size) : citizen_choice;
            }
        break;
        case 3: // E
            neighbor_choice = citizen_choice + 1;
            if (neighbor_choice % width == 0) {
            	neighbor_choice = wrap ? (neighbor_choice - (width-1)) : citizen_choice;
            } else if (neighbor_choice == size) {
            	neighbor_choice = wrap ? 0 : citizen_choice;
            } else if (neighbor_choice < 0) {
            	neighbor_choice = wrap ? (neighbor_choice + size) : citizen_choice;
            }
        break;
    }

    return neighbor_choice;
}


int main() {
	srand(42);

	string colors;
	string info;
	string stringSize;
	bool corona;
	bool half;
	bool wrap;

	// INIT
	cout << "Welcher Modus? (demokratur, halb, corona): ";
	cin >> info;

	corona = (info == "corona");
	half = (info == "halb");

	cout << "Wrapbedingung einschalten? (y/n): ";
	cin >> info;
	wrap = (info == "y");

	cout << "Wie gross soll das Feld sein? (x mal x): ";
	cin >> stringSize;

	if (!corona && !half) {
		cout << "Wieviele Farben sollen enthalten sein? ";
		cin >> colors;

	} else {
		colors = "2";
	}

	const int size = std::stoi(stringSize) * std::stoi(stringSize);

	int icolors = std::stoi(colors);
	int color_count[icolors];
	for (int i = 0; i < icolors; i++) {
		color_count[i] = 0;
	}

	// assign persuasion skills to the parties
	int persuasions[icolors];
	if (!corona) {
		int sum = 0;
		for (int i = 0; i < icolors; i++) {
			cout << "Welche Überzeugungswahrscheinlichkeit hat Farbe " << i << ": ";
			cin >> persuasions[i];
			sum += persuasions[i];
		}

		// assert if the sum of persuasions is 100 or less
		assert(sum <= 100);
	}

	Citizen citizens[size];

	// Assign the field
	if (half) {
		for (int i = 0; i < size/2; i++) {
			citizens[i].party = 0;
			citizens[i].persuasion = persuasions[0];

			citizens[i+size/2].party = 1;
			citizens[i+size/2].persuasion = persuasions[1];
			color_count[1] = size/2;
			color_count[0] = size/2;
		}
	} else if (corona) {
		for (int i = 0; i < size; i++) {
			citizens[i].persuasion = 0;
		}

		int choice = rand() % size;
		citizens[choice].party = 1;
		citizens[choice].persuasion = 20;
		color_count[1] = 1;
		color_count[0] = size - 1;
	} else {
		for (int i = 0; i < size; i++) {
			int color = rand() % icolors;
			citizens[i].party = color;
			citizens[i].persuasion = persuasions[color];
			color_count[color] += 1;
		}
	}

	// start the game
	int runs = 0;
	int final = 0;

	getBoard(citizens, size, stringSize);
	for (int i = 0; i < icolors; i++) {
		cout << color_count[i] << endl;
	}
	cout << final << endl;

	while(true) {
		final++; runs++;

		if (runs % 1000 == 0) {
			system("cls");
			getBoard(citizens, size, stringSize);

			for (int i = 0; i < icolors; i++) {
				cout << color_count[i] << endl;
			}
			cout << final << endl;
			Sleep(100);
		}

		// get one citizen and one of its neighbor
		int citizen_choice = rand() % size;
		int neighbor_choice = get_neighbor(citizen_choice, size, stringSize, wrap);

		// skip loop if both have same colors
		if (citizens[citizen_choice].party == citizens[neighbor_choice].party) {
			continue;
		}

		// see if one persuades the other
		int combined_probability = citizens[citizen_choice].persuasion + citizens[neighbor_choice].persuasion;
		int draw_probability = 100 - combined_probability;
		if (rand() % 100 < draw_probability) {
			continue;
		}
		if (rand() % combined_probability <= citizens[citizen_choice].persuasion) {
			// citizen wins
			color_count[citizens[neighbor_choice].party] -= 1;
			color_count[citizens[citizen_choice].party] += 1;
			citizens[neighbor_choice].party = citizens[citizen_choice].party;
			citizens[neighbor_choice].persuasion = citizens[citizen_choice].persuasion;
		} else {
			color_count[citizens[citizen_choice].party] -= 1;
			color_count[citizens[neighbor_choice].party] += 1;
			citizens[citizen_choice].party = citizens[neighbor_choice].party;
			citizens[citizen_choice].persuasion = citizens[neighbor_choice].persuasion;
		}


		// exit condition
		if (check(citizens, size)) {
			system("cls");
			getBoard(citizens, size, stringSize);
			cout << runs << endl;
			break;
		}
	}

	return 0;
}
