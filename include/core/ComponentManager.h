#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Match.h"
#include "EntityManager.h"

namespace railguard::core
{
    /**
     * @brief Type representing an index of a component.
     */
    typedef size_t component_id_t;

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

    protected:
        // Create a new component for the given entity
        // It is protected because "real" component managers will add additional parameters
        Match RegisterComponent(const Entity &entity);
    public:
        // Inits the manager and preallocate space in the vectors and maps
        explicit ComponentManager(const component_id_t defaultComponentCapacity);
        // Destroys the given component
        void DestroyComponent(const Match &match);
        virtual void DestroyComponent(component_id_t index);
        // Finds the component
        [[nodiscard]] const Match FindComponentOfEntity(const Entity &entity);
        // Finds the entity linked to this component
        const Entity GetCorrespondingEntity(const Match &match);
        // Checks random components to check if the corresponding entities are still alive
        // Can be useful when there are a lot of entities
        void RunGarbageCollection(const EntityManager &em);
    };

}