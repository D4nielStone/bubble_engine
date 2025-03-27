/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "nucleo/sistema.hpp"
#include <unordered_map>
#include <memory>
#include "util/vetor4.hpp"
#include "util/caixa.hpp"

/**
 *
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 **/

namespace bubble
{ 
    class bubble_gui : public bubble::sistema
    {
        private:
            std::unique_ptr<caixa> raiz;
            std::unordered_map<std::string, caixa*> caixas;
            void atualizarFilhos(caixa*);
        public:
            bubble_gui();
            void adicionarFlags(const std::string& id, flags); 
            void atualizar() override;

            void adicionarCaixa(const std::string& pai_id, const std::string& nova_id) {
                if (auto pai = obterCaixa(pai_id)) {
                    auto nova_caixa = pai->adicionarFilho(nova_id);
                    caixas[nova_id] = nova_caixa;
                }
            }

            caixa* obterCaixa(const std::string& id) {
            auto it = caixas.find(id);
            if (it != caixas.end()) {
                return it->second;
            }
            return nullptr;
        }
    };
}
