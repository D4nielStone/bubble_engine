#include "commons_namespace.hpp"
#include "glad.h"
#include "util/mesh.hpp"
#include "util/material.hpp"
#include "debugging/debug.hpp"
#include "util/vertex.hpp"
#include "systems/render_system.hpp"

using namespace COMMONS_NS;


mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const material& material) :
    m_vertices(vertices), m_indices(indices), m_material(material) {
}

std::vector<vertex> mesh::getVertices() const {
    return m_vertices;
}

std::vector<transform> mesh::getInstances() const {
    return m_instances;
}

std::vector<unsigned int> mesh::getIndices() const  {
    return m_indices;
}

material mesh::getMaterial() const {
    return m_material;
}

unsigned int mesh::getVertexArray() const {
    return m_VAO;
}

unsigned int mesh::getElementBuffer() const {
    return m_EBO;
}

unsigned int mesh::getVertexBuffer() const {
    return m_VBO;
}

void mesh::set_vertices(const std::vector<vertex>& v) {
    m_vertices = v;
}

void mesh::setInstances(const std::vector<transform>& i) {
    m_instances = i;
}

void mesh::set_indices(const std::vector<unsigned int>& i) {
    m_indices = i;
}

void mesh::setMaterial(const material& m) {
    m_material = m;
}

void mesh::setSobreposition(const bool s) {
    m_overlayed = s;
}

void mesh::unload() {
    if(!m_loaded) return;
    debugging::emit(debug, "mesh", "descarregando");
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    m_loaded = false;
}

void mesh::load() {
    if(m_loaded) return;

    if (m_instances.empty()) {
        m_material.set_uniform("instance", false);
    } else {
        m_material.set_uniform("instance", true);
        for (size_t i = 0; i < m_instances.size(); i++) {
            render_system::calculateTransformacao(&m_instances[i]);
            m_material.set_uniform("transformacoes[" + std::to_string(i) + "]", m_instances[i].get_model_matrix());
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
        &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvcoords));

    glBindVertexArray(0);
    m_loaded = true;
}

void mesh::draw(shader &shader) {
    if(m_overlayed)
        glDepthFunc(GL_ALWAYS);

    m_material.usar(shader);

    glBindVertexArray(m_VAO);

    if (m_instances.empty()) {
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_instances.size());
    }
    glBindVertexArray(0);

    if(m_overlayed)
        glDepthFunc(GL_LESS);
}

bool mesh::isLoaded() const {
    return m_loaded;
}

bool mesh::isOverlayed?() const {
    return m_overlayed;
}

mesh::~mesh() {
    unload();
}
