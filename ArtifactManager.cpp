
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known
    std::string tokens[10];
    int tokenCount = tokenize(line, tokens,10 );
    if (tokenCount == 0) {
        return;
    }
    std::string command=tokens[0];
    if (command.size() > 0 && command[0] == '#') {
        return;
    }
    if(command=="ADD_ARTIFACT"){
        handleAddArtifact(tokens,tokenCount);
    }else if(command=="REMOVE_ARTIFACT"){
        handleRemoveArtifact(tokens,tokenCount);

    }else if(command=="HIRE_RESEARCHER"){
        handleHireResearcher(tokens,tokenCount);
        
    }else if(command=="FIRE_RESEARCHER"){
        handleFireResearcher(tokens,tokenCount);

    }else if(command=="REQUEST"){
        handleRequest(tokens,tokenCount);

    }else if(command=="RETURN"){
        handleReturn(tokens,tokenCount);
        
    }else if(command=="RETURN_ALL"){
        handleReturnAll(tokens,tokenCount);
    }else if(command=="RESEARCHER_LOAD"){
        handleResearcherLoad(tokens,tokenCount);

    }else if(command=="MATCH_RARITY"){
        handleMatchRarity(tokens,tokenCount);

    }else if(command=="PRINT_UNASSIGNED"){
        handlePrintUnassigned(tokens,tokenCount);

    }else if(command=="PRINT_STATS"){
        handlePrintStats(tokens,tokenCount);

    }else if(command=="CLEAR"){
        handleClear(tokens,tokenCount);
    }else{
        std::cout<<"Error: Unknown command '"<<command<<"'."<<std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    if(count!= 5){
        std::cout<<"Error: Invalid command format."<<std::endl;
        return;
    }
    int id=std::stoi(tokens[1]);
    std::string name=tokens[2];
    int rarity=std::stoi(tokens[3]);
    int value=std::stoi(tokens[4]);

    Artifact newArtifact(id,name,rarity,value);
    bool insertSucces=artifactTree.insertArtifact(newArtifact);
    if(insertSucces){
        std::cout<<"Artifact "<<id<<" added."<<std::endl;
    }else{
        std::cout << "Error: Artifact " << id << " already exists." << std::endl;
    }
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.
    if (count != 2) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    int artifactId = std::stoi(tokens[1]);
    ArtifactNode* node = artifactTree.findArtifact(artifactId);
    if(node==nullptr){
        std::cout << "Error: Artifact " << artifactId << " not found." << std::endl;
        return;
    }
    if(node->data.assignedToName!=""){
        std::string researcherName = node->data.assignedToName;
        ResearcherNode* researcher = researcherTree.findResearcher(researcherName);
        if (researcher != nullptr) {
            researcher->data.removeArtifact(artifactId);
        }
    }
    bool isRemoved = artifactTree.removeArtifact(artifactId);
    if (isRemoved) {
        std::cout<<"Artifact "<<artifactId<<" removed."<<std::endl;
    } else {
        
        std::cout<<"Error: Artifact "<<artifactId<<" not found."<<std::endl;
    }
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.
    if (count != 3) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string name = tokens[1];
    int capacity = std::stoi(tokens[2]);
    Researcher newResearcher(name, capacity);
    bool insertSuccess = researcherTree.insertResearcher(newResearcher);
    if (insertSuccess) {
        std::cout << "Researcher " << name << " hired." << std::endl;
    } else {
        std::cout << "Error: Researcher " << name << " already exists." << std::endl;
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.
    if (count != 2) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string researcherName = tokens[1];
    ResearcherNode* researcher = researcherTree.findResearcher(researcherName);
    if (researcher == nullptr) {
        std::cout << "Error: Researcher " << researcherName << " not found." << std::endl;
        return;
    }

    for (int i = 0; i < researcher->data.numAssigned; ++i){
        int artifactID = researcher->data.assignedArtifacts[i];
        artifactTree.clearAssignedTo(artifactID);
    }

    bool isRemoved = researcherTree.removeResearcher(researcherName);
    if (isRemoved) {
        std::cout << "Researcher " << researcherName << " fired." << std::endl;
    } else {
        std::cout << "Error: Could not remove researcher." << std::endl;
    }
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."
    if(count!=3){
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string researcherName=tokens[1];
    int artifactID=std::stoi(tokens[2]);
    ResearcherNode* researcher=researcherTree.findResearcher(researcherName);
    if(researcher==nullptr){
        std::cout << "Error: Researcher " << researcherName << " not found." << std::endl;
        return;
    }
    ArtifactNode* artifact=artifactTree.findArtifact(artifactID);
    if(artifact==nullptr){
        std::cout << "Error: Artifact " << artifactID << " not found." << std::endl;
        return;
    }
    if(artifact->data.assignedToName!=""){
        std::cout << "Error: Artifact " << artifactID << " is already assigned." << std::endl;
        return;
    }
    if(researcher->data.numAssigned >= researcher->data.capacity){
        std::cout << "Error: Researcher " << researcherName << " is at full capacity." << std::endl;
        return;
    }
    bool isAdded=researcher->data.addArtifact(artifactID);
    if(isAdded){
        artifactTree.setAssignedTo(artifactID,researcherName);
        artifact->data.updateValueBasedOnUsage();
        std::cout << "Artifact " << artifactID << " assigned to " << researcherName << "." << std::endl;
    
    }
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."
    if(count!=3){
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string researcherName = tokens[1];
    int artifactID = std::stoi(tokens[2]);
    ResearcherNode* researcher = researcherTree.findResearcher(researcherName);
    if(researcher==nullptr){
        std::cout << "Error: Researcher " << researcherName << " not found." << std::endl;
        return;
    }
    ArtifactNode* artifact=artifactTree.findArtifact(artifactID);
    if(artifact==nullptr){
        std::cout << "Error: Artifact " << artifactID << " not found." << std::endl;
        return;
    }
    if(artifact->data.assignedToName!=researcherName){
        std::cout << "Error: Artifact " << artifactID << " not assigned to " << researcherName << "." << std::endl;
        return;
    }

    bool isRemoved=researcher->data.removeArtifact(artifactID);
    if(isRemoved){
        artifactTree.clearAssignedTo(artifactID);
        std::cout << "Artifact " << artifactID << " returned by " << researcherName << "." << std::endl;
    }else{
        std::cout << "Error: Artifact could not be removed from researcher's list." << std::endl;
    }
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.
    if(count!=2){
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string researcherName=tokens[1];
    ResearcherNode* researcher=researcherTree.findResearcher(researcherName);
    if(researcher==nullptr){
        std::cout << "Error: Researcher " << researcherName << " not found." << std::endl;
        return;
    }
    for(int i=0; i<researcher->data.numAssigned;++i){
        int artifactID=researcher->data.assignedArtifacts[i];
        artifactTree.clearAssignedTo(artifactID);

    }
    researcher->data.removeAllArtifacts();
    std::cout << "All artifacts returned by " << researcherName << "." << std::endl;

}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.
    if(count!=2){
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    std::string researcherName=tokens[1];
    ResearcherNode* researcher=researcherTree.findResearcher(researcherName);
    if(researcher==nullptr){
        std::cout << "Error: Researcher " << researcherName << " not found." << std::endl;
        return;
    }
    int loadCount=researcher->data.numAssigned;
    std::cout << "Researcher " << researcherName << " is supervising " << loadCount << " artifacts." << std::endl;
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.
    if (count != 2) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    int minRarity = 0;
    try {
        minRarity = std::stoi(tokens[1]);
    } catch (...) {
        std::cout << "Error: Invalid rarity level." << std::endl;
        return;
    }
    
    std::cout <<"=== MATCH_RARITY "<<minRarity<<" ==="<< std::endl;
    artifactTree.printMatchingRarity(minRarity);
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().
    if (count != 1) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    
    std::cout << "Unassigned artifacts:" << std::endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)
    if (count != 1) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    int totalArtifacts = artifactTree.getArtifactCount();
    int totalResearchers = researcherTree.getResearcherCount();
    int totalRarity = artifactTree.getTotalRarity();
    int totalLoad = researcherTree.getTotalLoad();

    double averageRarity = (totalArtifacts == 0) ? 0.0 : totalRarity / totalArtifacts;
    double averageLoad = (totalResearchers == 0) ? 0.0 : totalLoad / totalResearchers;

    std::cout << "=== SYSTEM STATISTICS ===" << std::endl;
    std::cout << "Artifacts: " << totalArtifacts << std::endl;
    std::cout << "Researchers: " << totalResearchers << std::endl;
    std::cout << "Average rarity: " << averageRarity << std::endl;
    std::cout << "Average load: " << averageLoad << std::endl;
    std::cout << "Researchers:" << std::endl;
    researcherTree.traversePreOrderForStats();
    std::cout << "Artifacts:"<< std::endl;
    artifactTree.traversePostOrderForStats();
    
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    if (count != 1) {
        std::cout << "Error: Invalid command format." << std::endl;
        return;
    }
    artifactTree.clear();
    researcherTree.clear();
    // e.g. "All data cleared."
    std::cout << "All data cleared." << std::endl;
}
