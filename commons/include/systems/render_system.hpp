#pragma once
#include "system.hpp"
#include <memory>

namespace COMMONS_NS {
  /**
   * @brief Classe responsável por gerenciar o sistema de renderização do jogo.
   * @brief Atualiza os framebuffers das câmeras, configura as luzes e renderiza os objetos na tela.
   */
	class render_system : public system {
	  public:
	    render_system() = default;
		  void setup(const std::shared_ptr<ecs>&) override;
      void update(const std::shared_ptr<ecs>&) override;
	};
}
/** @see render_system.cpp */
