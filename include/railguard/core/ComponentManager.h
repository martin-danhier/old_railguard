#pragma once

#define REQUIRED_FOUND_ALIVE_TO_END_GC 5

#include <railguard/core/Entity.h>
#include <railguard/core/EntityManager.h>
#include <railguard/core/Match.h>
#include <unordered_map>
#include <vector>

namespace railguard::core
{
    /**
     * @brief Type representing an index of a component.
     */
    typedef size_t component_idx_t;

    template<typename T = std::nullptr_t>
    class ComponentManager
    {
      protected:
        // === Manager data ===

        // Map that can be used for fast search of a component given an entity
        // Entity -> Component
        std::unordered_map<eid_t, component_idx_t> _entityLookUpMap;

        // === Component data ===

        // For a given component, the entity linked to it
        // Component -> Entity
        std::vector<Entity> _entities;

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
            // Add the entity in the list
            _entities.push_back(entity);
            // Store the index of the new variable + 1
            // + 1 because the maps returns 0 if a value does not exist in the map
            // Thus, we reserve 0 for the "no entity" case
            _entityLookUpMap[entity.eid] = _entities.size();

            // Other managers should also push values to the respective vectors
            return Match(_entities.size());
        }

        virtual void Clear()
        {
            _entities.clear();
            _entityLookUpMap.clear();
        }

        virtual /**
                 * @brief Destroy a component but keep everything tightly packed in the vectors.
                 *
                 * Meant to be used from derived classes. Call this method to execute the boilerplate,
                 * then move the last item of the custom vectors at the location of the destroyed item, then pop_back
                 * every custom vectors.
                 *
                 * @param index index of the component.
                 */
            void
            DestroyComponent(const Match &match)
        {
            auto index = match.GetIndex();

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
                _entityLookUpMap[movedEntity.eid] = index + 1;
            }
            _entities.pop_back();
        }

      public:
        /**
         * @brief Inits the vectors to the default capacity.
         *
         * @param storage Optional storage required by the derived class
         * @param defaultComponentCapacity Number of items that will have pre allocated space in the vectors
         */
        ComponentManager(T storage, component_idx_t defaultComponentCapacity)
        {
            // Pre allocate the given size
            _entities.reserve(defaultComponentCapacity);
            _entityLookUpMap.reserve(defaultComponentCapacity);

            // Save the storage
            _storage = storage;
        }

        /**
         * @brief Searches for an component attached to the given entity.
         *
         * @param entity Entity for the search.
         * @return Match a match representing the position of the component in the arrays.
         */
        [[nodiscard]] Match FindComponentOfEntity(const Entity &entity)
        {
            component_idx_t index = _entityLookUpMap[entity.eid];
            return Match(index);
        }

        /**
         * @brief Get the entity attached to the given component.
         *
         * @param match Match pointing to the component of which the entity is queried.
         * @return The Entity attached to the given component.
         */
        Entity GetCorrespondingEntity(const Match &match)
        {
            return _entities[match.GetIndex()];
        }

        // Checks random components to check if the corresponding entities are still alive
        // Can be useful when there are a lot of entities
        void RunGarbageCollection(const EntityManager &em)
        {
            uint8_t foundAliveInARow = 0;

            while (foundAliveInARow < REQUIRED_FOUND_ALIVE_TO_END_GC && !_entities.empty())
            {
                // Select an entity
                auto i   = std::rand() % _entities.size();
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

} // namespace railguard::core
