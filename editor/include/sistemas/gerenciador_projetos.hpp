/**
 * @file gerenciador_projetos.hpp
 *
 * @see gerenciador_projetos.cpp
 */
#include <string>
#include "becommons/becommons.hpp"
#include "editor_namespace.hpp"

using namespace becommons;

/// namespace beeditor
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
            becommons::bubble_gui gui;
            gerenciador_projetos() = default;
    };
}
