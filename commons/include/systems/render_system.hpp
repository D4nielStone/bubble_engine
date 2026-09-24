#pragma once
#include "system.hpp"
#include "components/camera.hpp"
#include "components/transform.hpp"
#include <memory>

namespace COMMONS_NS {
  struct framebuffer {
    unsigned int id;
    unsigned int texture_id;
    unsigned int rbo_id;
    unsigned int width;
    unsigned int height;
  };
	class render_system : system {
      // @brief Lista de cameras ativas no sistema
    private:   
      std::vector<std::shared_ptr<camera>> m_cameras;
      // Último framebuffer renderizado
      framebuffer m_framebuffer;
	  public:
	    render_system() = default;
      void addCamera(std::shared_ptr<camera> cam) { m_cameras.push_back(cam); };
	    void removeCamera(std::shared_ptr<camera> cam) { 
        m_cameras.erase(std::remove(m_cameras.begin(), m_cameras.end(), cam), m_cameras.end()); 
      };
      // @brief Renderiza a cena usando a camera especificada
      // @param cam Ponteiro para a camera a ser usada na renderização
      // @return Retorna o framebuffer resultante da renderização
      COMMONS_NS::framebuffer render(std::shared_ptr<camera> cam);
      // @brief Retorna o framebuffer atual do sistema de renderização
      COMMONS_NS::framebuffer& getFrameBuffer() { return ; }
		  void setup() override;
	};
}
/** @see render_system.cpp */
