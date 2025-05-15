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
            inline void configurarUI(const std::string&);
            inline void atualizarElementos(const std::string&);
            inline void abrirProjeto(const std::string&);
            inline void removerProjeto(const std::string&);
            void criarProjetoPadrao(const std::string& novo_diretorio, const char* nome);
            void iniciar();
        private:
            std::map<std::string, std::string> projetos;
            std::string m_projeto_selecionado {"n/a"};
            becommons::bubble_gui gui;
            inline static std::string DIR_PADRAO;
            gerenciador_projetos() = default;
    };
}
