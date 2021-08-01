#pragma once

#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Match.h"
#include "EntityManager.h"
#include "../utils/AdvancedCheck.h"

#define REQUIRED_FOUND_ALIVE_TO_END_GC 5

namespace railguard::core
{
    /**
     * @brief Type representing an index of a component.
     */
    typedef size_t component_id_t;

    template <typename T = nullptr_t>
    class ComponentManager
    {
    private:
        // === Manager data ===

        // Map that can be used for fast search of a component given an entity
        // Entity -> Component
        std::unordered_map<eid_t, component_id_t> _entityLookUpMap;

        // === Component data ===

        // For a given component, the entity linked to it
        // Component -> Entity
        std::vector<Entity> _entities;

#ifdef USE_ADVANCED_CHECKS
        /**
         * @brief In debug mode, keep track of the init status to ensure that Init is called first
         */
        bool _initialized = false;
#endif

    protected:
        /**
         * @brief Optional storage for data that is passed in the Init method.
         */
        T _storage;

        /**
         * @brief Creates a new component. Derived function should append values to the vectors.
         *
         * @param entity Entity to which the new component should be linked.
         * @return Match mapping the new component with its index in the component.
         */
        Match RegisterComponent(const Entity &entity)
        {
            ADVANCED_CHECK(_initialized, "ComponentManager should be initialized with Init before calling this method.");

            // Add the entity in the list
            _entities.push_back(entity);
            // Store the index of the new variable + 1
            // + 1 because the maps returns 0 if a value does not exist in the map
            // Thus, we reserve 0 for the "no entity" case
            _entityLookUpMap[entity.eid] = _entities.size();

            // Other managers should also push values to the respective vectors
            return Match(_entities.size());
        }

    public:
        /**
         * @brief Inits the vectors to the default capacity.
         *
         * @param storage Optional storage required by the derived class
         * @param defaultComponentCapacity Number of items that will have pre allocated space in the vectors
         */
        void Init(T storage = nullptr, const component_id_t defaultComponentCapacity = 10)
        {
            ADVANCED_CHECK(!_initialized, "ComponentManager should not be initialized twice.");

            // Pre allocate the given size
            _entities.reserve(defaultComponentCapacity);
            _entityLookUpMap.reserve(defaultComponentCapacity);

            // Save the storage
            _storage = storage;

#ifdef USE_ADVANCED_CHECKS
            _initialized = true;
#endif
        }

        /**
         * @brief Destroy a component but keep everything tightly packed in the vectors.
         *
         * Meant to be used from derivated classes. Call this method to execute the boilerplate,
         * then move the last item of the custom vectors at the location of the destroyed item, then pop_back
         * every custom vectors.
         *
         * @param match Match pointing to the component.
         */
        void DestroyComponent(const Match &match)
        {
            component_id_t index = match.GetIndex();
            DestroyComponent(index);
        }

        /**
         * @brief Destroy a component but keep everything tightly packed in the vectors.
         *
         * Meant to be used from derivated classes. Call this method to execute the boilerplate,
         * then move the last item of the custom vectors at the location of the destroyed item, then pop_back
         * every custom vectors.
         *
         * @param index index of the component.
         */
        virtual void DestroyComponent(component_id_t index)
        {
            ADVANCED_CHECK(_initialized, "ComponentManager should be initialized with Init before calling this method.");
            ADVANCED_CHECK(index < _entities.size(), "The index should exist in the vectors.");

            // Remove the match from the lookup map
            _entityLookUpMap.erase(_entities[index].eid);

            // Remove the component data when it is not at the end
            size_t lastIndex = _entities.size() - 1;
            if (index < lastIndex)
            {
                // To keep everything tightly packed, we move the last element to the deleted slot
                auto movedEntity = _entities[lastIndex];
                _entities[index] = movedEntity;
                // Update the map for the updated index
                _entityLookUpMap[movedEntity.eid] = index;
            }
            _entities.pop_back();
        }

        /**
         * @brief Searches for an component attached to the given entity.
         *
         * @param entity Entity for the search.
         * @return Match a match representing the position of the component in the arrays.
         */
        [[nodiscard]] const Match FindComponentOfEntity(const Entity &entity)
        {
            ADVANCED_CHECK(_initialized, "ComponentManager should be initialized with Init before calling this method.");

            component_id_t index = _entityLookUpMap[entity.eid];
            return Match(index);
        }

        /**
         * @brief Get the entity attached to the given component.
         *
         * @param match Match pointing to the component of which the entity is queried.
         * @return The Entity attached to the given component.
         */
        const Entity GetCorrespondingEntity(const Match &match)
        {
            ADVANCED_CHECK(_initialized, "ComponentManager should be initialized with Init before calling this method.");

            return _entities[match.GetIndex()];
        }

        // Checks random components to check if the corresponding entities are still alive
        // Can be useful when there are a lot of entities
        void RunGarbageCollection(const EntityManager &em)
        {
            ADVANCED_CHECK(_initialized, "ComponentManager should be initialized with Init before calling this method.");
            
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
    };

}