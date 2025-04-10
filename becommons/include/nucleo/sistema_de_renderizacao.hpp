/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema_de_renderizacao.hpp
 * @brief Gerencia a renderizacao
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see sistema_de_renderizacao.cpp
 */

#pragma once
#include "sistema.hpp"
#include "componentes/camera.hpp"

namespace BECOMMONS_NS
{
	class sistema_renderizacao : public sistema
	{
        private:
            void atualizarCamera(camera*);
	    public:
            camera* camera_principal{nullptr};
	        void definirCamera(camera*);
	    	sistema_renderizacao() = default;
		    void atualizar() override;
		    void inicializar(fase* fase_ptr) override;
	};
}
