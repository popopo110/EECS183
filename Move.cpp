/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    //TODO: Implement non-default constructor
    if(commandString == "")
        {
            isPass = true;
        }
        else if(commandString == "S" || commandString == "s")
        {
            isSave = true;
        }
        else if(commandString == "Q" || commandString == "q")
        {
            isQuit = true;
        }
        else
        {
            string str = commandString;
            stringstream ss(str);
            char e;
            char temp;
            ss >> e >> elevatorId >> temp;
            if(temp == 'f')
            {
                ss >> targetFloor;
            }
            else if(temp == 'p')
            {
                isPickup = true;
            }
        }
}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    //TODO: Implement isValidMove
    bool isValid = false;
        //Pass, Quit, or Save move
        if(isPass == true || isQuit == true || isSave == true)
        {
            isValid = true;
        }
        else if(elevatorId >= 0 && elevatorId < NUM_ELEVATORS
                && !elevators[elevatorId].isServicing())
        {
            //Pickup move
            if(isPickup == true)
            {
                isValid = true;
            }
            //Servicing move
            else if(targetFloor >= 0 && targetFloor < NUM_FLOORS
                    && targetFloor != elevators[elevatorId].getCurrentFloor())
            {
                isValid = true;
            }
        }
        return isValid;
    //Returning false to prevent compilation error
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    //TODO: Implement setPeopleToPickup
    numPeopleToPickup = 0;
        totalSatisfaction = 0;
        for(int i = 0; i < pickupList.size(); i++)
        {
            peopleToPickup[i] = pickupList.at(i) - 48;
            numPeopleToPickup++;
        }
        
        int maxDiff = 0;
        int anger;
        int distance;
        for(int i = 0; i < numPeopleToPickup; i++)
        {
            anger = pickupFloor.getPersonByIndex(peopleToPickup[i]).getAngerLevel();
            totalSatisfaction += MAX_ANGER - anger;
            distance = abs(pickupFloor.getPersonByIndex
                           (peopleToPickup[i]).getTargetFloor() - currentFloor);
            if(maxDiff <= distance)
            {
                maxDiff = distance;
                targetFloor =
                pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor();
            }
        }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
