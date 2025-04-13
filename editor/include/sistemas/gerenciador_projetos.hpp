/**
 * @file gerenciador_projetos.hpp
 *
 * @see gerenciador_projetos.cpp
 */
#include <string>
#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"

namespace EDITOR_NS
{
    class gerenciador_projetos
    {
        public:
            gerenciador_projetos(const std::string& DIR_PADRAO);
            void atualizarElementos(const std::string& Dir);
            void iniciar();
            void criarProjetoPadrao(const std::string& novo_diretorio, const char* nome);
        private:
            std::string DIR_PADRAO;
            bubble_gui gui;
            gerenciador_projetos() = default;
    };
}
