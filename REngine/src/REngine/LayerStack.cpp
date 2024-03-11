#include "repch.h"
#include "LayerStack.h"

namespace REngine
{
    REngine::LayerStack::LayerStack()
    {
        m_layerInsert = m_layers.begin();
    }

    REngine::LayerStack::~LayerStack()
    {
        for (Layer* layer : m_layers)
            delete layer;
    }

    void REngine::LayerStack::PushLayer(Layer* layer)
    {
        m_layerInsert = m_layers.emplace(m_layerInsert, layer);
    }

    void REngine::LayerStack::PushOverlay(Layer* overlay)
    {
        m_layers.emplace_back(overlay);
    }

    void REngine::LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end())
        {
            m_layers.erase(it);
            m_layerInsert--;
        }
    }

    void REngine::LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if (it != m_layers.end())
            m_layers.erase(it);
    }
}