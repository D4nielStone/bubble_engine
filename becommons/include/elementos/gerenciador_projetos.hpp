/**
 * @file gerenciador_projetos.hpp
 *
 * @see gerenciador_projetos.cpp
 */
#include <string>
#include "elementos/painel.hpp"

using namespace becommons;

/// namespace beeditor
namespace EDITOR_NS
{
    class gerenciador_projetos : public painel {
        public:
            gerenciador_projetos(const std::string& DIR_PADRAO);
            void buscarProjetos();
            void abrirProjeto(const std::string&);
            void removerProjeto(const std::string&);
            void criarProjeto(const std::string& novo_diretorio, const char* nome, const bool padrao = false);
        private:
            std::map<std::string, std::string> projetos;
            std::string m_projeto_selecionado {"nenhum"}, buffer_projeto {""};
            std::string DIR_PADRAO;
            gerenciador_projetos() = default;
    };
}
