#pragma once

#include <concepts>
#include <unordered_map>
#include <vector>
#include "railguard/core/Match.h"

namespace railguard::core
{

    /**
     * @brief A generic class containing the boilerplate code of standalone managers.
     * A standalone manager is a data structure that allows the storage of items in separate vectors
     * (struct of arrays instead of an array of struct).
     *
     * The "standalone" part means that it is not used to define a component used with ids.
     * Instead, the manager uses its own ids.
     * This can be useful for internal structures that doesn't have a lot of meaning in the game world.
     * For example, a shader is not directly located in the world, thus it is stored in a standalone manager.
     *
     * @tparam T type that will be used to represent ids. Must be integral, but the size may vary depending on the needs.
     * @tparam U optional type that can be used to store a struct of values with the init function.
     * For example, the vulkan device is often required in methods, storing it here removes the need to require it everywhere.
     */
    template <std::integral T, typename U = nullptr_t>
    class StandaloneManager
    {
    protected:
        /**
         * @brief Optional storage for data that is passed in the Init method.
         */
        U _storage;

        /**
         * @brief Allows to quickly get the index of a item from its id
         */
        std::unordered_map<T, size_t> _idLookupMap;

        /**
         * @brief Keeps track of the last used ID for an item.
         * Incremented each time a new item is created.
         *
         * We start at 1, so that we can use 0 as the "NULL" id.
         */
        T _lastUsedId = 1;

        /**
         * @brief Allows to quickly retrieve the id of an item from its index.
         */
        std::vector<T> _ids;


        /**
        * @brief Create a new Item. Derived function should append values to the vectors.
        *
        * @return CompleteMatch<T> a match with its ID and index
        */
        CompleteMatch<T> CreateItem()
        {

            // Get new id for the item
            T newId = _lastUsedId++;

            // Push new id and save it in the map
            _ids.push_back(newId);
            _idLookupMap[newId] = _ids.size();

            return CompleteMatch(_ids.size(), newId);
        }

        /**
         * @brief Destroy an item but keep everything tightly packed in the vectors.
         *
         * Meant to be used from derived classes. Call this method to execute the boilerplate,
         * then move the last item of the custom vectors at the location of the destroyed item, then pop_back
         * every custom vectors.
         *
         * Here is an example of code for a derived function:
         *
         * @snippet ShaderModuleManager.cpp Example of derivation of StandaloneManager::DestroyItem
         *
         * @param match Match pointing to the item
         */
        void DestroyItem(const Match &match)
        {

            // Get index
            auto index = match.GetIndex();

            // Remove entry from map
            _idLookupMap.erase(_ids[index]);

            // Move the last item of the vectors
            size_t lastIndex = _ids.size() - 1;

            // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
            // the destroyed item was
            if (index < lastIndex)
            {
                T movedId   = _ids[lastIndex];
                _ids[index] = movedId;
                // Update id in map
                _idLookupMap[movedId] = index + 1;
            }

            // Remove last item
            _ids.pop_back();
        }

        /**
         * @brief Inits the vectors to the default capacity.
         *
         * @param storage Optional storage required by the derived class
         * @param defaultCapacity Number of items that will have pre allocated space in the vectors
         */
        StandaloneManager(U storage, size_t defaultCapacity)
        {
            // Reserve space in the vector
            _ids.reserve(defaultCapacity);
            _idLookupMap.reserve(defaultCapacity);

            // Save the storage
            _storage = storage;
        }

      public:
        virtual /**
                 * @brief Destroys every remaining item.
                 */
            void
            Clear()
        {
            _ids.clear();
            _idLookupMap.clear();
        }

        /**
         * @brief Searches for an item with the given id.
         *
         * @param id Id of the item to look for
         * @return core::Match a match representing the position of the item in the arrays.
         */
        [[nodiscard]] Match LookupId(T id) const
        {
            size_t index = _idLookupMap.at(id);
            return core::Match(index);
        }

        [[nodiscard]] T GetId(const Match &match) const
        {
            return _ids[match.GetIndex()];
        }
    };
}