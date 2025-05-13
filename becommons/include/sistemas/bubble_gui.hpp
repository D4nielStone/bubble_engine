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
 */

#include "glad.h"
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
#include "util/vetor4.hpp"
#include <functional>
#include <queue>

/// namespace becommons
namespace BECOMMONS_NS { 
	inline static unsigned int ret_VAO= 0,  ///< Vertex Array Object do retângulo
	       ret_VBO = 0,                     ///< Vertex Buffer Object do retângulo
	       ret_EBO = 0,                     ///< Element Buffer Object do texto
	       text_VBO = 0,                    ///< Vertex Buffer Object do texto
	       text_VAO = 0;                    ///< Vertex Array Object do texto
    /**
     * @brief Renderiza uma imagem do elemento.
     * @param img Ponteiro para o objeto imagem a ser renderizado.
     */
    static void renderizarImagem(elementos::imagem* img);

    /**
     * @brief Renderiza o fundo de uma caixa utilizando um shader específico.
     * @param c Ponteiro para a caixa a ser renderizada.
     * @param s Shader utilizado para o fundo.
     */
    static void renderizarFundo(caixa* c, shader* s);

    /**
     * @brief Renderiza um elemento de texto.
     * @param txt Ponteiro para o objeto texto.
     */
    static void renderizarTexto(elementos::texto* txt);
    /// @class bubble_gui 
    /// @brief Gerencia e organiza as caixas flexíveis da interface
    class bubble_gui : public sistema
    {
        private:
            glm::mat4 proj { 1.f };
            bool m_novo_estilo { false };
            std::queue<std::function<void()>> funcoes;      ///< Fila de funções para serem executadas no fim do loop
            std::unique_ptr<caixa> raiz;                    ///< Elemento raiz ( tem as dimensões da janela )
            std::unordered_map<std::string, caixa*> caixas; ///< Vetor de elementos
            std::unordered_map<std::string, caixa*> estilo_atual;             ///< Todos os elementos presentes no estilo atual
            shader* quad_shader{nullptr};                   ///< Shader do quadrado usado no fundo do elemento
            /// Desenha a caixa
            /// Desenha e decide que tipo de elemento ela é
            /// @param c Ponteiro da caixa
            inline void desenhar_caixa(caixa* c);
            inline static void atualizarLJ(caixa*);
            inline static void atualizarAJ(caixa*);
            inline static void atualizarHDTF(caixa*, std::function<void(caixa*)>);
            inline static bool deveAtualizar(caixa*);
            inline void configOpenglState() const;
            inline void deconfigOpenglState() const;
            inline void popFuncoes();
            inline void processarModular(caixa*);
            inline void processarDimensaoPercentual(caixa*, bool, float&, fvet2&, fvet2&);
            inline void aplicarLayoutModular(caixa*, bool, float, const fvet2&);

            inline void renderizarImagem(elementos::imagem*) const;
            inline void renderizarFundo(caixa*, BECOMMONS_NS::shader*) const;
            inline void renderizarTexto(elementos::texto* tex) const;
        public:
        /**
         * @brief Construtor padrão da interface.
         */
        bubble_gui();

        /**
         * @brief Destrutor da interface.
         */
        ~bubble_gui();

        /**
         * @brief Atualiza as posições e tamanhos dos filhos de uma caixa.
         * @param c Caixa a ser atualizada.
         */
        void atualizarFilhos(caixa* c);

        /**
         * @brief Define os buffers de OpenGL necessários para a renderização.
         */
        void definirBuffers();

        /**
         * @brief Adiciona uma flag de estilo a uma caixa específica.
         * @param id ID da caixa.
         * @param f Flag de estilo a ser aplicada.
         */
        void adicionarFlags(const std::string& id, flag_estilo f);

        /**
         * @brief Inicializa o sistema gráfico dentro de uma fase.
         * @param f Ponteiro para a fase.
         */
        void inicializar(fase* f) override;

        /**
         * @brief Atualiza o sistema gráfico a cada quadro.
         */
        void atualizar() override;
    
        /**
         * @brief Adiciona um novo elemento à interface, como botão, texto ou imagem.
         * @tparam T Tipo do elemento a ser adicionado (deve herdar de `caixa`).
         * @tparam Args Argumentos para o construtor do elemento.
         * @param pai_id ID do elemento pai.
         * @param nova_id ID do novo elemento.
         * @param args Argumentos para construção do novo elemento.
         */
        template <typename T, typename ...Args>
        void adicionar(const std::string& pai_id, const std::string& nova_id, Args&&... args) {
                if (elementoExiste(pai_id)) {
                    m_novo_estilo = false;
                    auto pai = obterElemento(pai_id);
                    auto nova_caixa = pai->adicionarFilho<T>(nova_id, std::forward<Args>(args)...);
                    estilo_atual[nova_id] = nova_caixa;
                } else if(estilo_atual.find(pai_id) != estilo_atual.end()) {
                    auto pai = estilo_atual[pai_id];
                    auto nova_caixa = pai->adicionarFilho<T>(nova_id, std::forward<Args>(args)...);
                    estilo_atual[nova_id] = nova_caixa;
                } else {
                    throw std::runtime_error("parente da caixa não existe!");
                }
                m_novo_estilo = true;
            }
        void adicionar(const std::string& pai_id, const std::string& nova_id, std::unique_ptr<caixa> ptr) {
                if (elementoExiste(pai_id)) {
                    m_novo_estilo = false;
                    auto pai = obterElemento(pai_id);
                    ptr->m_id = nova_id;
                    estilo_atual[nova_id] = ptr.get();
                    pai->m_filhos.push_back(std::move(ptr));
                } else if(estilo_atual.find(pai_id) != estilo_atual.end()) {
                    auto pai = estilo_atual[pai_id];
                    ptr->m_id = nova_id;
                    estilo_atual[nova_id] = ptr.get();
                    pai->m_filhos.push_back(std::move(ptr));
                } else {
                    throw std::runtime_error("parente da caixa não existe!");
                }
                m_novo_estilo = true;
            }

        /**
         * @brief Remove um elemento da interface, junto com todos os seus filhos.
         * @param id ID do elemento a ser removido.
         */
        void removerElemento(const std::string& id);

        /**
         * @brief Remove apenas os filhos de uma determinada caixa.
         * @param id ID da caixa cujos filhos serão removidos.
         */
        void removerFilhos(const std::string& id);

        /**
         * @brief Recupera uma caixa da interface pelo seu ID.
         * @param id ID do elemento.
         * @return Ponteiro para a caixa, ou nullptr se não for encontrada.
         */
        caixa* obterElemento(const std::string& id);

        bool elementoExiste(const std::string id) const;

        // === Funções de Estilo ===

        /**
         * @brief inicia a raiz com um id específico
         * @param nome
         */
        void iniciarRaiz(const std::string& nome);
        /**
         * @brief Limpa o estilo atual.
         */
        void fimEstilo();

        /**
         * @brief Define uma flag de estilo para todos os elementos do estilo atual.
         * @param v Flag a ser aplicada.
         */
        void defFlags(const flag_estilo v);

        /**
         * @brief Define largura percentual para todos os elementos do estilo atual.
         * @param v Valor percentual (0.0 a 1.0).
         */
        void defLargura(const double v);

        /**
         * @brief Define altura percentual para todos os elementos do estilo atual.
         * @param v Valor percentual (0.0 a 1.0).
         */
        void defAltura(const double v);

        /**
         * @brief Define largura absoluta (em pixels) para os elementos.
         * @param v Valor inteiro em pixels.
         */
        void defLargura(const int v);

        /**
         * @brief Define largura lingando-a à altura
         * @param b Booleano (true linka )
         */
        void defLarguraAltura(const bool b);
        
        /**
         * @brief Define estado de ativação
         * @param b Booleano (true = ativado)
         */
        void defAtivo(const bool b);

        /**
         * @brief Define altura absoluta (em pixels) para os elementos.
         * @param v Valor inteiro em pixels.
         */
        void defAltura(const int v);

        /**
         * @brief Define a orientação modular das caixas.
         * @param v Orientação (horizontal ou vertical).
         */
        void defOrientacao(const estilo::orientacao v);

        /**
         * @brief Define o padding geral (externo) das caixas.
         * @param v1 Padding horizontal.
         * @param v2 Padding vertical.
         */
        void defPaddingG(const int v1, const int v2);

        /**
         * @brief Define o padding interno das caixas.
         * @param v1 Padding horizontal.
         * @param v2 Padding vertical.
         */
        void defPadding(const int v1, const int v2);

        /**
         * @brief Define a cor da borda das caixas.
         * @param v Cor da borda.
         */
        void defCorBorda(const cor v);

        /**
         * @brief Define a cor do fundo das caixas.
         * @param v Cor do fundo.
         */
        void defCorFundo(const cor v);

        /**
         * @brief Define o crescimento modular (escala proporcional) das caixas.
         * @param v Fator de crescimento.
         */
        void defCrescimentoM(const float v);

        /**
         * @brief Define a espessura da borda.
         * @param v Valor em pixels.
         */
        void defTamanhoBorda(const int v);
    };
}
/// @see bubble_gui.cpp
