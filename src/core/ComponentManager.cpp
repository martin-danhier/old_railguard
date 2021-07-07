#include <cassert>
#include <random>
#include <iostream>
#include "../../include/core/ComponentManager.h"

#define REQUIRED_FOUND_ALIVE_TO_END_GC 5

namespace railguard::core
{

    ComponentManager::ComponentManager(const component_id_t defaultComponentCapacity)
    {
        // Pre allocate the given size
        _entities.reserve(defaultComponentCapacity);
        _entityLookUpMap.reserve(defaultComponentCapacity);
    }

    Match ComponentManager::RegisterComponent(const Entity &entity)
    {
        // Add the entity in the list
        _entities.push_back(entity);
        // Store the index of the new variable + 1
        // + 1 because the maps returns 0 if a value does not exist in the map
        // Thus, we reserve 0 for the "no entity" case
        _entityLookUpMap[entity.eid] = _entities.size();

        // Other managers should also push values to the respective vectors
        return Match(_entities.size());
    }

    const Match ComponentManager::FindComponentOfEntity(const Entity &entity)
    {
        component_id_t index = _entityLookUpMap[entity.eid];
        return Match(index);
    }

    const Entity ComponentManager::GetCorrespondingEntity(const Match &match)
    {
        return _entities[match.GetIndex()];
    }

    void ComponentManager::DestroyComponent(const Match &match)
    {
        component_id_t index = match.GetIndex();
        DestroyComponent(index);
    }
    void ComponentManager::DestroyComponent(component_id_t index)
    {
        assert(index < _entities.size());

        // Remove the match from the lookup map
        _entityLookUpMap.erase(_entities[index].eid);

        // Remove the component data when it is not at the end
        if (index < _entities.size() - 1)
        {
            // To keep everything tightly packed, we move the last element to the deleted slot
            auto movedEntity = _entities[_entities.size() - 1];
            _entities[index] = movedEntity;
            // Update the map for the updated index
            _entityLookUpMap[movedEntity.eid] = index;
        }
        _entities.pop_back();

    }

    void ComponentManager::RunGarbageCollection(const EntityManager &em)
    {
        uint8_t foundAliveInARow = 0;

        while (foundAliveInARow < REQUIRED_FOUND_ALIVE_TO_END_GC && _entities.size() > 0)
        {
            // Select an entity
            auto i = std::rand() % _entities.size();
            Entity e = _entities[i];

            // Check if it is still alive
            if (em.IsEntityAlive(e))
            {
                foundAliveInARow++;
            }
            // Not alive
            else
            {
                // Delete it
                this->DestroyComponent(i);
                foundAliveInARow = 0;
            }
        }
    }

} // namespace core
