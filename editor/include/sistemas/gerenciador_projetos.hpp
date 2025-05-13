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
            inline static void atualizarElementos(const std::string&);
            inline static void abrirProjeto(const std::string&);
            inline static void removerProjeto(const std::string&);
            void criarProjetoPadrao(const std::string& novo_diretorio, const char* nome);
            void iniciar();
        private:
            std::string m_projeto_selecionado {"nenhum"};
            becommons::bubble_gui gui;
            inline static std::string DIR_PADRAO;
            gerenciador_projetos() = default;
    };
}
