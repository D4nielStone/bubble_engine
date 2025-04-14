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
 * @file main.cpp
 */


#include <iostream>
#include "registro_testes.hpp"
#include "depuracao/assert.hpp"
#include "util/vetor4.hpp"
#include "util/vetor3.hpp"
#include "util/vetor2.hpp"
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "nucleo/fase.hpp"
#include "sistemas/bubble_gui.hpp"
#include "componentes/fisica.hpp"

betest::registro_testes testes;

void testarVetor4()
{
    testes.classe("UTILS", "VETOR4");

    // Teste de soma
    testes.adicionar("soma", [](){
        vet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        vet4 v2(0.5f, 1.5f, -1.0f, 2.0f);
        vet4 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
        ASSERT_EQUAL(resultado.z, 2.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        vet4 v(1.0f, 2.0f, 3.0f, 4.0f);
        vet4 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        vet4 v1(3.0f, 5.0f, 7.0f, 9.0f);
        vet4 v2(1.0f, 2.0f, 3.0f, 4.0f);
        vet4 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
        ASSERT_EQUAL(resultado.w, 5.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        vet4 v(5.0f, 6.0f, 7.0f, 8.0f);
        vet4 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        vet4 v1(2.0f, 3.0f, 4.0f, 5.0f);
        vet4 v2(2.0f, 2.0f, 2.0f, 2.0f);
        vet4 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
        ASSERT_EQUAL(resultado.z, 8.0f);
        ASSERT_EQUAL(resultado.w, 10.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        vet4 v1(4.0f, 6.0f, 8.0f, 10.0f);
        vet4 v2(2.0f, 2.0f, 2.0f, 2.0f);
        vet4 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
        ASSERT_EQUAL(resultado.w, 5.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        vet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        vet4 v2(1.0f, 2.0f, 3.0f, 4.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        vet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        vet4 v2(1.0f, 2.0f, 3.0f, 5.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        vet4 v(1.0f, 2.0f, 2.0f, 1.0f);
        v.normalizar();
        float comprimento = v.tamanho();
        ASSERT_APPROX_EQUAL(comprimento, 1.0f, 0.0001f);
    });
}

void testarVetor3()
{
    testes.classe("UTILS", "VETOR3");

    // Teste de soma
    testes.adicionar("soma", [](){
        vet3 v1(1.0f, 2.0f, 3.0f);
        vet3 v2(0.5f, 1.5f, -1.0f);
        vet3 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
        ASSERT_EQUAL(resultado.z, 2.0f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        vet3 v(1.0f, 2.0f, 3.0f);
        vet3 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        vet3 v1(3.0f, 5.0f, 7.0f);
        vet3 v2(1.0f, 2.0f, 3.0f);
        vet3 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        vet3 v(5.0f, 6.0f, 7.0f);
        vet3 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        vet3 v1(2.0f, 3.0f, 4.0f);
        vet3 v2(2.0f, 2.0f, 2.0f);
        vet3 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
        ASSERT_EQUAL(resultado.z, 8.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        vet3 v1(4.0f, 6.0f, 8.0f);
        vet3 v2(2.0f, 2.0f, 2.0f);
        vet3 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        vet3 v1(1.0f, 2.0f, 3.0f);
        vet3 v2(1.0f, 2.0f, 3.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        vet3 v1(1.0f, 2.0f, 3.0f);
        vet3 v2(1.0f, 1.0f, 3.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        vet3 v(1.0f, 2.0f, 2.0f);
        v.normalizar();
        float comprimento = v.tamanho();
        ASSERT_APPROX_EQUAL(comprimento, 1.0f, 0.0001f);
    });
}

void testarVetor2()
{
    testes.classe("UTILS", "VETOR2");

    // Teste de soma
    testes.adicionar("soma", [](){
        vet2 v1(1.0f, 2.0f);
        vet2 v2(0.5f, 1.5f);
        vet2 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        vet2 v(1.0f, 2.0f);
        vet2 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        vet2 v1(3.0f, 5.0f);
        vet2 v2(1.0f, 2.0f);
        vet2 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        vet2 v(5.0f, 6.0f);
        vet2 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        vet2 v1(2.0f, 3.0f);
        vet2 v2(2.0f, 2.0f);
        vet2 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        vet2 v1(4.0f, 6.0f);
        vet2 v2(2.0f, 2.0f);
        vet2 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        vet2 v1(1.0f, 2.0f);
        vet2 v2(1.0f, 2.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        vet2 v1(1.0f, 2.0f);
        vet2 v2(1.0f, 3.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        vet2 v(3.0f, 4.0f);
        v.normalizar();
        float comprimento = v.tamanho();
        ASSERT_APPROX_EQUAL(comprimento, 1.0f, 0.0001f);
    });
}

void testarUtils()
{
    testarVetor4();
    testarVetor3();
    testarVetor2();
}

// componentes de teste
struct CompA : bubble::componente { 
    static constexpr bubble::componente::mascara mascara = static_cast<bubble::componente::mascara>(1);
};
struct CompB : bubble::componente { 
    static constexpr bubble::componente::mascara mascara = static_cast<bubble::componente::mascara>(1 << 1);
};

void testarFase()
{
    testes.classe("NUCLEO", "FASE");
    
    // Teste de construtor com diretório
    testes.adicionar("parse_nome", [](){
        
        const char* diretorio = "../assets/teste_fase.fase";

        bubble::fase fase(diretorio);
        fase.carregar();
    
        ASSERT_EQUAL(fase.nome(), "fase_teste");
    });
}

void testarRegistro()
{
    testes.classe("NUCLEO", "REGISTRO");
    
    // Teste de inicialização
    
    testes.adicionar("criando_entidades", [](){
        bubble::registro reg;
        bubble::entidade e1 = reg.criar();
        bubble::entidade e2 = reg.criar();
        
        ASSERT_TRUE(e1.id != e2.id);  // IDs devem ser diferentes
        ASSERT_EQUAL(e1.mascara, bubble::componente::COMPONENTE_TRANSFORMACAO);
        ASSERT_EQUAL(e2.mascara, bubble::componente::COMPONENTE_TRANSFORMACAO);
    });

    testes.adicionar("tem/nao_tem_componente", [](){
        bubble::registro reg;
        bubble::entidade e = reg.criar();
    
        ASSERT_TRUE(!reg.tem<bubble::fisica>(e.id));
        reg.adicionar<bubble::fisica>(e);
        ASSERT_TRUE(reg.tem<bubble::fisica>(e.id));
    });
    
    testes.adicionar("obtendo_componente", [](){
		bubble::registro reg;
		bubble::entidade e = reg.criar();
	
		ASSERT_TRUE(!reg.tem<bubble::fisica>(e.id));

        reg.adicionar<bubble::fisica>(e);

		ASSERT_TRUE(reg.obter<bubble::fisica>(e.id)!=nullptr);
    });
    
    testes.adicionar("removendo_componente_nulo", [](){
            bubble::registro reg;
            bubble::entidade e = reg.criar();
        
            ASSERT_TRUE(!reg.tem<bubble::fisica>(e.id));
            reg.remover<bubble::fisica>(e.id);
            ASSERT_TRUE(!reg.tem<bubble::fisica>(e.id));
            ASSERT_TRUE(!reg.obter<bubble::fisica>(e.id));
        });
        
        testes.adicionar("adicionando_removento_componentes", [](){
            bubble::registro reg;
            bubble::entidade e = reg.criar();
            
            reg.adicionar<CompA>(e);
            ASSERT_TRUE(reg.tem<CompA>(e.id));
        
            auto componente = reg.obter<CompA>(e.id);
            ASSERT_TRUE(componente != nullptr);
            ASSERT_EQUAL(componente->meu_objeto, e.id);
        
            reg.remover<CompA>(e.id);
            ASSERT_TRUE(!reg.tem<CompA>(e.id));
        });
        
        testes.adicionar("iteracao_com_multiplos_componentes", []() {
            bubble::registro reg;
            
            // cria combinações de entidades
            auto e1 = reg.criar();
            auto e2 = reg.criar();
            auto e3 = reg.criar();
            
            reg.adicionar<CompA>(e1);
            reg.adicionar<CompB>(e1);  // tem ambos
            
            reg.adicionar<CompA>(e2);   // apenas CompA
            
            reg.adicionar<CompB>(e3);   // apenas CompB
        
            // testa iterações para entidades com ambos
            int count = 0;
            reg.cada<CompA, CompB>([&](uint32_t entity) {
                count++;
                ASSERT_EQUAL(entity, e1.id);
            });
            ASSERT_EQUAL(count, 1);
        });
        
        testes.adicionar("iteracao_sem_matches", []() {
            bubble::registro reg;
            
            // cria entidades sem componentes
            auto e = reg.criar();
        
            int count = 0;
            reg.cada<bubble::fisica>([&](uint32_t) { count++; });
            ASSERT_EQUAL(count, 0);
        });
        
        testes.adicionar("limpeza_entidade_sem_componentes", []() {
            bubble::registro reg;
            auto e = reg.criar();
        
            // adiciona e remove componente
            reg.adicionar<bubble::fisica>(e, false, 0.0f, btVector3(0.0f, 0.0f, 0.0f));
            reg.remover<bubble::fisica>(e.id);
            reg.remover<bubble::transformacao>(e.id);
        
            // entidade deve ser removida do mapa
            ASSERT_TRUE(reg.obterComponentes(e.id) == bubble::componente::COMPONENTE_NONE);
            ASSERT_TRUE(reg.entidades.find(e.id) == reg.entidades.end());
        });
        
        testes.adicionar("multiplos_componentes_mascara", []() {
            bubble::registro reg;
            auto e = reg.criar();
        
            reg.adicionar<CompA>(e);
            reg.adicionar<CompB>(e);
            reg.remover<bubble::transformacao>(e.id);

            ASSERT_EQUAL(reg.obterComponentes(e.id), (CompA::mascara | CompB::mascara));
        
            reg.remover<CompA>(e.id);
            ASSERT_EQUAL(reg.obterComponentes(e.id), CompB::mascara);
        
            reg.adicionar<CompA>(e);
            ASSERT_EQUAL(reg.obterComponentes(e.id), (CompA::mascara | CompB::mascara));
        });
}

void testarNucleo()
{
    testarRegistro();
    testarFase();
}

void testarBubbleGUI()
{
    testes.classe("SISTEMAS", "BUBBLE_GUI");

    // Teste 1: obtenção nula
    testes.adicionar("obtencao_nula", []() {
        bubble::bubble_gui gui;
        auto ptr = gui.obterElemento(""); // Não deve crashar também
        ASSERT_EQUAL(ptr, nullptr);
    });

    // Teste 2: Parente inexistente
    testes.adicionar("parente_vazio", []() {
        bubble::bubble_gui gui;
        gui.adicionarElemento<bubble::caixa>("", "child_test"); // Não deve crashar também
    });
    
    // Teste 3: Caixa nula
    testes.adicionar("atualizarFilhos_caixa_nula", []() {
        bubble::bubble_gui gui;
        gui.atualizarFilhos(nullptr); // Não deve causar crash
    });

    // Teste 4: Caixa inativa
    testes.adicionar("atualizarFilhos_caixa_inativa", []() {
        bubble::bubble_gui gui;
        gui.adicionarElemento<bubble::caixa>("raiz", "parent_test");
        bubble::caixa* parent = gui.obterElemento("parent_test");
        parent->m_ativo = false;
        parent->adicionarFilho<bubble::caixa>("filho");
        gui.atualizarFilhos(gui.obterElemento("raiz"));
        ASSERT_EQUAL(parent->obterFilho("filho")->m_limites, vet4(0,0,20,20)); // Valores padrão
    });

    // Teste 3: Layout horizontal com dimensões fixas
    testes.adicionar("atualizarFilhos_horizontal_fixo", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_orientacao_modular = bubble::caixa::orientacao::horizontal;
        parent.m_limites = {0, 0, 100, 50};
        parent.m_padding_geral = {5, 0};

        auto* child1 = parent.adicionarFilho<bubble::caixa>("child1");
        child1->m_largura = 30;
        child1->m_padding.x = 2;

        auto* child2 = parent.adicionarFilho<bubble::caixa>("child2");
        child2->m_largura = 40;
        child2->m_padding.x = 3;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child1->m_limites.x == 7);  // 0 +5 +2
        ASSERT(child1->m_limites.z == 30);
        ASSERT(child2->m_limites.x == 47); // 39 +5 +3
        ASSERT(child2->m_limites.z == 40);
    });

    // Teste 4: Layout horizontal com porcentagem
    testes.adicionar("atualizarFilhos_horizontal_percentual", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_orientacao_modular = bubble::caixa::orientacao::horizontal;
        parent.m_limites.z = 200;

        auto* child = parent.adicionarFilho<bubble::caixa>("child");
        child->m_flag_estilo |= bubble::flag_estilo::largura_percentual;
        child->m_largura = 0.5f;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child->m_limites.z == 100); // 200 * 0.5
    });

    // Teste 5: Crescimento modular horizontal
    testes.adicionar("atualizarFilhos_horizontal_crescimento", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_orientacao_modular = bubble::caixa::orientacao::horizontal;
        parent.m_limites.z = 100;

        auto* child1 = parent.adicionarFilho<bubble::caixa>("child1");
        child1->m_largura = 30;
        child1->m_crescimento_modular = 1;

        auto* child2 = parent.adicionarFilho<bubble::caixa>("child2");
        child2->m_largura = 40;
        child2->m_crescimento_modular = 1;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child1->m_limites.z == 45); // 30 + (100-70)/2
        ASSERT(child2->m_limites.z == 55); // 40 + (100-70)/2
    });

    // Teste 6: Layout vertical com dimensões fixas e percentual
    testes.adicionar("atualizarFilhos_vertical_misto", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_orientacao_modular = bubble::caixa::orientacao::vertical;
        parent.m_limites = {0, 0, 80, 150};
        parent.m_padding_geral = {10, 5};

        auto* child = parent.adicionarFilho<bubble::caixa>("child");
        child->m_flag_estilo |= bubble::flag_estilo::largura_percentual;
        child->m_largura = 0.5f;
        child->m_altura = 60;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        // Largura esperada: (80 * 0.5) - 2*10 = 40 - 20 = 20
        ASSERT(child->m_limites.z == 20);
        ASSERT(child->m_limites.w == 60);
    });

    // Teste 7: Atualização recursiva de filhos
    testes.adicionar("atualizarFilhos_recursivo", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;

        auto* child = parent.adicionarFilho<bubble::caixa>("child");
        child->m_flag_estilo = bubble::flag_estilo::modular;
        child->m_orientacao_modular = bubble::caixa::orientacao::vertical;

        auto* grandchild = child->adicionarFilho<bubble::caixa>("grandchild");
        grandchild->m_altura = 30;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(grandchild->m_limites.w == 30); // Verifica se o neto foi atualizado
    });

    // Teste 8: Espaço insuficiente para crescimento
    testes.adicionar("atualizarFilhos_espaco_insuficiente", []() {
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_orientacao_modular = bubble::caixa::orientacao::horizontal;
        parent.m_limites.z = 50;

        auto* child1 = parent.adicionarFilho<bubble::caixa>("child1");
        child1->m_largura = 30;
        child1->m_crescimento_modular = 1;

        auto* child2 = parent.adicionarFilho<bubble::caixa>("child2");
        child2->m_largura = 30;
        child2->m_crescimento_modular = 1;

        bubble::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child1->m_limites.z == 30); // Sem crescimento (espaço negativo)
        ASSERT(child2->m_limites.z == 30);
    });
    
    // Teste 9: 
    testes.adicionar("atualizarFilhos_padding_deral", []() {
        bubble::bubble_gui gui;
        
        bubble::caixa parent;
        parent.m_flag_estilo = bubble::flag_estilo::modular;
        parent.m_padding_geral = {5, 5};
        auto* child1 = parent.adicionarFilho<bubble::caixa>("child1");
        auto* child2 = parent.adicionarFilho<bubble::caixa>("child2");
        gui.atualizarFilhos(&parent);
        // Horizontal
        // Assegura que o padding foi aplicado aos filhos
        ASSERT_EQUAL(child1->m_limites.x, 5)
        ASSERT_EQUAL(child2->m_limites.x, 30) // Largura do child1 mais o padding
        parent.m_orientacao_modular = bubble::caixa::orientacao::vertical;

        gui.atualizarFilhos(&parent);
        ASSERT_EQUAL(child1->m_limites.y, 5)
        ASSERT_EQUAL(child2->m_limites.y, 30) // Largura do child1 mais o padding
    });
}
void testarSistemas()
{
    testarBubbleGUI();
}

int main()
{
    depuracao::debug_ativo = false;
	testarUtils();
    testarNucleo();
    testarSistemas();
    testes.rodarTestes();
    return 0;
}
