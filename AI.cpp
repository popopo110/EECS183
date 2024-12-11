/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#khaloi ho, Jack Vegter, Isabella Sanchez-Baratto, Halbert#>
 * <#khaho, jvegter,isasanch,halbertr#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core
string getAIMoveString(const BuildingState& buildingState) {
    // determining when the elevator should pass
    bool passCondition = false;
    bool floorCondition = false;
    bool pickUpCondition = false;
    int sumangerLevel = 0;
    int floorAnger[10];/*Array store anger level of each floor*/
    bool floorFilled[10] = {0}; //boolean array storing what elevator going to what floor
    int floorWanted = 0;
    string empty;
    string elevatorId = "";
    string floorToGo = "";
    
    /*get the sum anger level of every floor then store it in the floorAnger array*/
            /*iterating through every floors in the building*/
    for (int a = 0; a < NUM_FLOORS; a++)
    {
        for (int i = 0; i < buildingState.floors[a].numPeople; i++)
        {
            sumangerLevel += buildingState.floors[a].people[i].angerLevel;
        }
        floorAnger[a] = sumangerLevel;
        sumangerLevel = 0;
    }
    int highestAnger = 0;
    int temp1 = 0;
    //Checking the floor with the highest anger level
    for (int b = 0; b < 10; b++)
    {
        temp1 = floorAnger[b];
        if (highestAnger < temp1)
        {
            highestAnger = temp1;
        }
    }
    int e = 0;
    /*iterating through floorAnger array to see which floor has highest anger level,
             the floor with highest anger level will be prioritize to get the elevator first*/
    for(e = 0; e < 10; e++)
    {
        for(int i = 0; i < NUM_ELEVATORS; i++)
        {
            if(buildingState.elevators[i].targetFloor == e )
            {
                floorFilled[e] = true;
            }
        }
        
    }
    
    for (int c = 0; c < 10; c++)
    {/*if the floor with the highest anger level is already a target floor of another elevator then we find the floor second to highest anger level*/
        if(floorAnger[c] == highestAnger)
        {
            floorWanted = c;
        }
        else if(floorAnger[c] == highestAnger && floorFilled[c] == true)
        {
            floorAnger[c] = 0;
            for (int d = 0; d < 10; d++)
            {
                temp1 = floorAnger[d];
                if (highestAnger < temp1)
                {
                    highestAnger = temp1;
                }
            }
            for(int e = 0; e < 10; e++)
            {
                if(floorAnger[e] == highestAnger && floorFilled[e] == false)
                {
                    floorWanted = e;
                }
            }
        }
    }
    
    floorToGo = to_string(floorWanted);
    int minDistance = 100;
    int diff = 0;
    int elevatorNumber = 0;
    //Checking to see which elevator is nearest to the wanted floor and not currently servicing
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        if (!buildingState.elevators[i].isServicing) {
            diff = abs(floorWanted - buildingState.elevators[i].currentFloor);
            
            if (minDistance > diff) {
                minDistance = diff;
                elevatorNumber = i;
            }
        }
    }
    
    
    int counter = 0; //tracking amount of elevator that is not servicing
    /*floorWanted = 1;
     floorToGo = to_string(floorWanted);*/
    for(int i = 0; i < NUM_ELEVATORS; i++)
    {
        if(!buildingState.elevators[i].isServicing)
        {
            counter++;
        }
    }
    /*If there at least 1 elevator that is not servicing,
         then we can procced to check the next condition */
    if(counter > 0 && buildingState.elevators[elevatorNumber].currentFloor != floorWanted && floorFilled[floorWanted] == false)
    {
        floorCondition = true;
    }
    else if(counter > 0 && buildingState.floors[buildingState.elevators[elevatorNumber].currentFloor].numPeople != 0)
    {
        pickUpCondition = true;
    }
    /*else if(counter > 0 && buildingState.elevators[elevatorNumber].currentFloor == floorWanted && buildingState.floors[floorWanted].numPeople != 0)
     {
     floorCondition = true;
     }*/
    
    else
    {
        passCondition = true;
    }
    
    if(floorCondition)
    {
        int temp = -1;
        int greatestFloor = 0;
        int maxFloor = 0;
        for(int i = 0; i < NUM_FLOORS; i++)
        {
            // determines which floor has the greatest amount of ppl
            if(buildingState.floors[i].numPeople > temp )
            {
                temp = buildingState.floors[i].numPeople;
                maxFloor = i;
                
            }
        }
        greatestFloor = maxFloor;
        
        empty = "e" + to_string(elevatorNumber) + "f" + floorToGo;
        return empty;
    }
    else if(pickUpCondition)
    {
        empty = "e" + to_string(elevatorNumber) + "p";
        return empty;
    }
    else
    {
        empty = "";
        return empty;
    }}
    
    string getAIPickupList(const Move& move, const BuildingState& buildingState,
                           const Floor& floorToPickup)
    {
        
        string Command = "";
        int sumAngerUp = -1;
        int sumAngerDown = -1;
        int currentFloorPeopleNum = floorToPickup.getNumPeople();
        string demandUp = "";
        string demandDown = "";
        int countUp = 0;
        int countDown = 0;
        
        //goes through each person on a given floor
        for (int i = 0; i < currentFloorPeopleNum; i++)
        {
            int targetFloorPeople = floorToPickup.getPersonByIndex(i).getTargetFloor();
            int currentFloorPeople =
            floorToPickup.getPersonByIndex(i).getCurrentFloor();
            //takes target floor of one person
            
            // floors[floorToPickup].people[i].angerlevel;
            //if target floor is greater (above) current floor
            if ( targetFloorPeople > currentFloorPeople)
            {
                //add the anger of the people who want to go up
                sumAngerUp += floorToPickup.getPersonByIndex(i).getAngerLevel() + 1;
                //gets index of people going up
                demandUp += to_string(i);
                countUp++;
                
            }
            //if people are going down
            else
            {
                //adds tha anger of people who want to go down
                sumAngerDown += floorToPickup.getPersonByIndex(i).getAngerLevel() + 1;
                //gets index of people going down
                demandDown += to_string(i);
                countDown++;
                
            }
        }
        
        
        if(sumAngerUp > sumAngerDown && countUp < countDown)
        {
            return demandDown;
        }
        else if(sumAngerUp < sumAngerDown && countUp > countDown)
        {
            return demandUp;
        }
        else if(sumAngerUp > sumAngerDown && countUp > countDown)
        {
            return demandUp;
        }
        else if(sumAngerUp < sumAngerDown && countUp < countDown)
        {
            return demandDown;
        }
        else if(sumAngerUp == sumAngerDown && countUp > countDown)
        {
            return demandUp;
        }
        else if(sumAngerUp == sumAngerDown && countUp < countDown)
        {
            return demandDown;
        }
        else if(countUp == 0)
        {
            return demandDown;
        }
        else if(countDown == 0)
        {
            return demandUp;
        }
        else
        {
            return demandUp;
        }
        
        
        
    }
