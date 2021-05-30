#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "ComponentMatch.h"
#include "EntityManager.h"

namespace railguard::core
{


    class ComponentManager
    {
    private:
        // === Manager data ===

        // Map that can be used for fast search of a component given an entity
        // Entity -> Component
        std::unordered_map<eid_t, size_t> _entityLookUpMap;

        // === Component data ===

        // For a given component, the entity linked to it
        // Component -> Entity
        std::vector<Entity> _entities;

    protected:
        // Create a new component for the given entity
        // It is protected because "real" component managers will add additional parameters
        ComponentMatch RegisterComponent(const Entity &entity);
    public:
        // Inits the manager and preallocate space in the vectors and maps
        explicit ComponentManager(const size_t defaultComponentCapacity);
        // Destroys the given component
        void DestroyComponent(const ComponentMatch &match);
        virtual void DestroyComponent(size_t index);
        // Finds the component
        [[nodiscard]] const ComponentMatch FindComponentOfEntity(const Entity &entity);
        // Finds the entity linked to this component
        const Entity GetCorrespondingEntity(const ComponentMatch &match);
        // Checks random components to check if the corresponding entities are still alive
        // Can be useful when there are a lot of entities
        void RunGarbageCollection(const EntityManager &em);
    };

}