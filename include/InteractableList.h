#ifndef INTERACT_LIST_H
#define INTERACT_LIST_H

#include "Utility.h"
#include "Interactable.h"

#include <memory>
#include <vector>


class InteractableList : public Interactable {
public:
    InteractableList() {}
    InteractableList(shared_ptr<Interactable> object) { add(object); }
    
    void clear() { objects.clear(); }
    void add(shared_ptr<Interactable> object) { objects.push_back(object); }
    
    virtual bool interact(const Ray& r, double tMin, double tMax, Interaction& interaction) const override;
    
public:
    std::vector<shared_ptr<Interactable>> objects;
};

bool InteractableList::interact(const Ray& r, double tMin, double tMax, Interaction& interaction) const {
    Interaction tempInteraction;
    bool interactionOccured = false;
    auto closestSoFar = tMax;
    
    for (const auto& object : objects) {
        if(object->interact(r, tMin, closestSoFar, tempInteraction)) {
            interactionOccured = true;
            closestSoFar = tempInteraction.t;
            interaction = tempInteraction;
        }
    }
    
    return interactionOccured;
};


#endif