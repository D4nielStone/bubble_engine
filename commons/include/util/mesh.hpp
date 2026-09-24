#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "commons_namespace.hpp"
#include "material.hpp"
#include "vertex.hpp"
#include "components/transform.hpp"
#include "loaders/shader.hpp"

namespace COMMONS_NS {
    /**
     * \class mesh
     * \brief Gerencia os vertices, indices, material e ect.
     * Usada para renderizar objects 3D.
     */
     class mesh {
     public:
        mesh() = default;
        mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int> &indices, const material& material);
        std::vector<vertex>         getVertices() const;
        std::vector<transform>      getInstances() const;
        std::vector<unsigned int>   getIndices() const;
        material                    getMaterial() const;
        unsigned int                getVertexArray() const;
        unsigned int                getElementBuffer() const;
        unsigned int                getVertexBuffer() const;
        void                        set_vertices(const std::vector<vertex>&);
        void                        setInstances(const std::vector<transform>&);
        void                        set_indices(const std::vector<unsigned int>&);
        void                        setMaterial(const material&);
        void                        setSobreposition(const bool);
        void                        unload();
        void                        load();
        void                        draw(shader& shader);
        bool                        isLoaded() const;
        bool                        isOverlayed() const;
        ~mesh();
     private:
        bool                        m_overlayed { false },
                                    m_loaded { false };
        std::vector<transform>      m_instances;
        std::vector<vertex>         m_vertices;
        std::vector<unsigned int>   m_indices;
        material                    m_material;
        unsigned int                m_VAO,
                                    m_VBO,
                                    m_EBO;

    };
} // namespace commons
