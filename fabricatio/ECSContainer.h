#ifndef ECSCONTAINER_H
#define ECSCONTAINER_H

#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cassert>
#include <typeindex>
#include <bitset>

using Entity = unsigned int;

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    void addComponentTo(Entity entity)
    {
        addComponentTo(entity, T());
    }

    void addComponentTo(Entity entity, T&& component)
    {
        assert(!m_entity_index_map.contains(entity) && "Component already added for entity");
        m_components.emplace_back(std::move(component));
        m_entity_index_map[entity] = m_components.size() - 1;
    }

    void modifyComponent(Entity entity, T&& component)
    {
        assert(m_entity_index_map.contains(entity));
        m_components[m_entity_index_map[entity]] = std::move(component);
    }

    T& getComponent(Entity entity)
    {
        assert(m_entity_index_map.contains(entity));
        return m_components[m_entity_index_map[entity]];
    }

    bool contains(Entity entity)
    {
        return m_entity_index_map.contains(entity);
    }

private:
    std::vector<T> m_components;
    std::map<Entity, size_t> m_entity_index_map;
};

class ECSContainer
{
public:
    Entity createEntity()
    {
        // TODO, maintain pool of entites
        return m_nextEntity++;
    }
    template <typename T>
    void addComponent(Entity entity)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->addComponentTo(entity);
    }

    template <typename T>
    void addComponent(Entity entity, T&& component)
    {
        ComponentArray<T>* componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->addComponentTo(entity, std::move(component));
    }

    template <typename T>
    void modifyComponent(Entity entity, T&& component)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->modifyComponent(entity, std::move(component));
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        return componentArray->getComponent(entity);
    }

    template <typename T>
    bool hasComponent(Entity entity)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        return componentArray->contains(entity);
    }


    template <typename T>
    void registerType()
    {
        assert(m_componentArrays.find(std::type_index(typeid(T))) == m_componentArrays.end() && "Component already registered");
        m_componentArrays[std::type_index(typeid(T))] = std::make_unique<ComponentArray<T>>();
    }

private:
    Entity m_nextEntity = 0;
    std::map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays;
    std::vector<std::bitset<32>> m_signatures;
};

class System
{
public:
    virtual ~System() = default;
    virtual void progress(float timeDelta, const std::set<Entity>& entities) = 0;
};

#endif
