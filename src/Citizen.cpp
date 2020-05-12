/*
 * Citizen.cpp
 *
 *  Created on: 12.05.2020
 *      Author: juttecht
 */

#include "Citizen.h"

Citizen::Citizen(int x, int y) : party(x), persuasion(y) { }
Citizen::Citizen() : party(0), persuasion(50) { }
