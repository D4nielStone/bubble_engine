/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see bubble_gui.cpp
 */

#include <unordered_map>
#include <memory>
#include <set>
#include "becommons.hpp"

namespace BECOMMONS_NS
{ 
	inline static unsigned int ret_VAO= 0, ret_VBO = 0, ret_EBO = 0, text_VBO = 0, text_VAO = 0;
    inline static void renderizarImagem(elementos::imagem*);
    inline static void renderizarFundo(caixa*, shader*);
    inline static void renderizarTexto(elementos::texto*);
    class bubble_gui : public sistema
    {
        private:
            std::unique_ptr<caixa> raiz;
            std::unordered_map<std::string, caixa*> caixas;
            std::set<std::string> estilo_atual;
            shader* quad_shader{nullptr};
            void desenhar_caixa(caixa* c);
        public:
            bubble_gui();
            ~bubble_gui();

            void atualizarFilhos(caixa*);
            void definirBuffers();
            void adicionarFlags(const std::string& id, flags_caixa); 
            void inicializar(fase*) override;
            void atualizar() override;

            template <typename T, typename ...Args>
            void adicionarElemento(const std::string& pai_id, const std::string& nova_id, Args&&... args) {
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
