#ifndef ECSCONTAINER_H
#define ECSCONTAINER_H

#pragma once

#include <vector>
#include <map>
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
    void AddComponentTo(Entity entity)
    {
        AddComponentTo(entity, T());
    }

    void AddComponentTo(Entity entity, T&& component)
    {
        assert(!m_entity_index_map.contains(entity) && "Component already added for entity");
        m_components.emplace_back(std::move(component));
        m_entity_index_map[entity] = m_components.size() - 1;
    }

    void ModifyComponent(Entity entity, T&& component)
    {
        assert(m_entity_index_map.contains(entity));
        m_components[m_entity_index_map[entity]] = std::move(component);
    }

    T& GetComponent(Entity entity)
    {
        assert(m_entity_index_map.contains(entity));
        return m_components[m_entity_index_map[entity]];
    }

private:
    std::vector<T> m_components;
    std::map<Entity, size_t> m_entity_index_map;
};

class ECSContainer
{
public:
    Entity CreateEntity()
    {
        // TODO, maintain pool of entites
        return m_nextEntity++;
    }
    template <typename T>
    void AddComponent(Entity entity)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->AddComponentTo(entity);
    }

    template <typename T>
    void AddComponent(Entity entity, T&& component)
    {
        ComponentArray<T>* componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->AddComponentTo(entity, std::move(component));
    }

    template <typename T>
    void ModifyComponent(Entity entity, T&& component)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        componentArray->ModifyComponent(entity, std::move(component));
    }

    template <typename T>
    T& GetComponent(Entity entity)
    {
        ComponentArray<T> *componentArray = dynamic_cast<ComponentArray<T> *>(m_componentArrays[std::type_index(typeid(T))].get());
        return componentArray->GetComponent(entity);
    }

    template <typename T>
    void Register()
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
    virtual void Progress(float timeDelta) = 0;
};

#endif