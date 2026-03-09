#include "Researcher.h"


Researcher::Researcher()
    : fullName(""),
      capacity(0),
      assignedArtifacts(nullptr),
      numAssigned(0)
{
}

Researcher::Researcher(const std::string &name, int cap)
    : fullName(name),
      capacity(cap),
      assignedArtifacts(nullptr),
      numAssigned(0)
{
    if (capacity > 0)
    {
        assignedArtifacts = new int[capacity];
    }
}


Researcher::Researcher(const Researcher &other)
    : fullName(other.fullName),
      capacity(other.capacity),
      numAssigned(other.numAssigned),
      assignedArtifacts(nullptr)
{
    
    if (capacity > 0) {
        assignedArtifacts = new int[capacity];
        for (int i = 0; i < numAssigned; ++i) {
            assignedArtifacts[i] = other.assignedArtifacts[i];
        }
    }
}


Researcher &Researcher::operator=(const Researcher &other)
{
    if (this == &other){
         return *this; 
    }
    delete[] assignedArtifacts;

    
    fullName = other.fullName;
    capacity = other.capacity;
    numAssigned = other.numAssigned;
    assignedArtifacts = nullptr;

    
    if (capacity > 0) {
        assignedArtifacts = new int[capacity];
        for (int i = 0; i < numAssigned; ++i) {
            assignedArtifacts[i] = other.assignedArtifacts[i];
        }
    }

    return *this;
}



Researcher::~Researcher()
{
    delete[] assignedArtifacts;
    assignedArtifacts = nullptr;
}

bool Researcher::addArtifact(int artifactID)
{
    // TODO:
    // 1) If hasArtifact(artifactID) is true, return false (already supervised).
    // 2) If numAssigned >= capacity, return false (at full capacity).
    // 3) Otherwise, insert artifactID at assignedArtifacts[numAssigned] and increment numAssigned.
    // 4) Return true.
    
    if(hasArtifact(artifactID)){
        return false;
    }
    if(numAssigned>=capacity){
        return false;
    }
    assignedArtifacts[numAssigned]=artifactID;
    numAssigned++;
    return true;
}

bool Researcher::removeArtifact(int artifactID)
{
    // TODO:
    // 1) Find index i where assignedArtifacts[i] == artifactID.
    // 2) If not found, return false.
    // 3) Shift elements after i one step to the left.
    // 4) Decrement numAssigned and return true.
    
    int index=-1;
    for(int i=0; i<numAssigned; ++i){
        if(assignedArtifacts[i]==artifactID){
            index=i;
            break;
        }
    }
    if(index==-1){
        return false;
    }
    for(int i=index; i<numAssigned-1; ++i){
        assignedArtifacts[i]=assignedArtifacts[i+1];
    }
    numAssigned--;
    return true;
}

void Researcher::removeAllArtifacts()
{
    // TODO:
    // Simply reset numAssigned to 0.
    // (The actual artifacts' assignedToName in the AVL tree will be cleared by the controller.)
    numAssigned=0;
}

bool Researcher::hasArtifact(int artifactID) const
{
    // TODO:
    // Return true if artifactID appears in assignedArtifacts[0..numAssigned-1], false otherwise.
    
    for(int i=0; i<numAssigned; ++i){
        if(assignedArtifacts[i]==artifactID){
            return true;
        }
    }
    return false;
}
