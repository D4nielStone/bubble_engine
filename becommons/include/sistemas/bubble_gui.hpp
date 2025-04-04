/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "nucleo/sistema.hpp"
#include <unordered_map>
#include <memory>
#include "util/vetor4.hpp"
#include "elementos/botao.hpp"
#include "elementos/texto.hpp"
#include <set>

/**
 *
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 **/

namespace bubble
{ 
	inline static unsigned int ret_VAO= 0, ret_VBO = 0, ret_EBO = 0, text_VBO = 0, text_VAO = 0;
    inline static void renderizarImagem(elementos::imagem*);
    inline static void renderizarFundo(caixa*);
    inline static void renderizarTexto(elementos::texto*);
    class bubble_gui : public bubble::sistema
    {
        private:
            std::unique_ptr<caixa> raiz;
            std::unordered_map<std::string, caixa*> caixas;
            std::set<std::string> estilo_atual;
            
            void atualizarFilhos(caixa*);
        public:
            bubble_gui();
            void adiFlags(const std::string& id, flags_caixa); 
            void atualizar() override;
            template <typename T, typename ...Args>
            void adiElemento(const std::string& pai_id, const std::string& nova_id, Args&&... args) {
                estilo_atual.insert(nova_id);
                if (auto pai = obterElemento(pai_id)) {
                    auto nova_caixa = pai->adicionarFilho<T>(nova_id, std::forward<Args>(args)...);
                    caixas[nova_id] = nova_caixa;
                }
            }
            void removerElemento(const std::string& id)
            {
                if(caixas.find(id) == caixas.end()) return;
                for(auto& filho : caixas[id]->m_filhos)
                {
                    caixas.erase(filho->m_id);
                }
                caixas[id]->m_filhos.clear();
                caixas.erase(id);
            }
            void removerFilhos(const std::string& id)
            {
                if(caixas.find(id) == caixas.end()) return;
                for(auto& filho : caixas[id]->m_filhos)
                {
                    caixas.erase(filho->m_id);
                }
                caixas[id]->m_filhos.clear();
            }

            caixa* obterElemento(const std::string& id) {
            auto it = caixas.find(id);
            if (it != caixas.end()) {
                return it->second;
            }
            return nullptr;
            }

            // Funções de estilo
            void novoEstilo(){
                estilo_atual.clear();
            };
            void defFlags(const flags_caixa v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_flags_caixa = v;
                }
            }
            void defLargura(const double v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_largura = v;
                    obterElemento(id)->m_flags_caixa |= flags_caixa::largura_percentual;
                }
            }
            void defAltura(const double v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_altura = v;
                    obterElemento(id)->m_flags_caixa |= flags_caixa::altura_percentual;
                }
            }
            void defLargura(const int v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_largura = v;
                }
            }
            void defAltura(const int v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_altura = v;
                }
            }
            void defOrientacao(const caixa::orientacao v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_orientacao_modular = v;
                }
            }
            void defPaddingG(const int v1, const int v2){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_padding_geral = {v1, v2};
                }
            }
            void defPadding(const int v1, const int v2){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_padding = {v1, v2};
                }
            }
            void defCorBorda(const cor v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_cor_borda = v;
                }
            }
            void defCorFundo(const cor v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_cor_fundo = v;
                }
            }
            void defCrescimentoM(const float v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_crescimento_modular = v;
                }
            }
            void defTamanhoBorda(const int v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_espessura_borda = v;
                }
            }
    };
}
