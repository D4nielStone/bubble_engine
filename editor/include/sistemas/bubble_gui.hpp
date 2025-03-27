/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "nucleo/sistema.hpp"
#include <unordered_map>
#include <memory>
#include "util/vetor4.hpp"
#include "elementos/imagem.hpp"

/**
 *
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 **/

namespace bubble
{ 
	inline static unsigned int ret_VAO= 0, ret_VBO = 0, ret_EBO = 0;
    inline static void renderizarImagem(shader*, const unsigned int, const bool, const vet4&);
    class bubble_gui : public bubble::sistema
    {
        private:
            std::unique_ptr<caixa> raiz;
            std::unordered_map<std::string, caixa*> caixas;
            void atualizarFilhos(caixa*);
        public:
            bubble_gui();
            void adicionarFlags(const std::string& id, flags_caixa); 
            void atualizar() override;
            
            template <typename T, typename ...Args>
            void adiElemento(const std::string& pai_id, const std::string& nova_id, Args&&... args) {
                if (auto pai = obterElemento(pai_id)) {
                    auto nova_caixa = pai->adicionarFilho<T>(nova_id, std::forward<Args>(args)...);
                    caixas[nova_id] = nova_caixa;
                }
            }

            caixa* obterElemento(const std::string& id) {
            auto it = caixas.find(id);
            if (it != caixas.end()) {
                return it->second;
            }
            return nullptr;
        }
    };
}
