/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 * @see bubble_gui.cpp
 */

#include <glad/glad.h>
#include <unordered_map>
#include <memory>
#include <set>
#include "namespace.hpp"
#include "nucleo/sistema.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/fonte.hpp"
#include "util/caixa.hpp"
#include "elementos/botao.hpp"
#include "elementos/imagem.hpp"
#include "elementos/texto.hpp"
#include <functional>
#include <queue>

namespace BECOMMONS_NS { 
	inline static unsigned int ret_VAO= 0, ret_VBO = 0, ret_EBO = 0, text_VBO = 0, text_VAO = 0;
    static void renderizarImagem(elementos::imagem*);
    static void renderizarFundo(caixa*, shader*);
    static void renderizarTexto(elementos::texto*);
    class bubble_gui : public sistema
    {
        private:
            std::queue<std::function<void()>> funcoes;
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
            void adicionarFlags(const std::string& id, flag_estilo); 
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
            void defFlags(const flag_estilo v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_flag_estilo = v;
                }
            }
            void defLargura(const double v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_largura = v;
                    obterElemento(id)->m_flag_estilo |= flag_estilo::largura_percentual;
                }
            }
            void defAltura(const double v){
                for(auto& id : estilo_atual){
                    obterElemento(id)->m_altura = v;
                    obterElemento(id)->m_flag_estilo |= flag_estilo::altura_percentual;
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
