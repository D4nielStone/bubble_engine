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
#include "becommons/becommons.hpp"

betest::registro_testes testes;

void testarVetor4()
{
    testes.classe("UTILS", "VETOR4");

    // Teste de soma
    testes.adicionar("soma", [](){
        fvet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        fvet4 v2(0.5f, 1.5f, -1.0f, 2.0f);
        fvet4 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
        ASSERT_EQUAL(resultado.z, 2.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        fvet4 v(1.0f, 2.0f, 3.0f, 4.0f);
        fvet4 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        fvet4 v1(3.0f, 5.0f, 7.0f, 9.0f);
        fvet4 v2(1.0f, 2.0f, 3.0f, 4.0f);
        fvet4 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
        ASSERT_EQUAL(resultado.w, 5.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        fvet4 v(5.0f, 6.0f, 7.0f, 8.0f);
        fvet4 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
        ASSERT_EQUAL(resultado.w, 6.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        fvet4 v1(2.0f, 3.0f, 4.0f, 5.0f);
        fvet4 v2(2.0f, 2.0f, 2.0f, 2.0f);
        fvet4 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
        ASSERT_EQUAL(resultado.z, 8.0f);
        ASSERT_EQUAL(resultado.w, 10.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        fvet4 v1(4.0f, 6.0f, 8.0f, 10.0f);
        fvet4 v2(2.0f, 2.0f, 2.0f, 2.0f);
        fvet4 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
        ASSERT_EQUAL(resultado.w, 5.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        fvet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        fvet4 v2(1.0f, 2.0f, 3.0f, 4.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        fvet4 v1(1.0f, 2.0f, 3.0f, 4.0f);
        fvet4 v2(1.0f, 2.0f, 3.0f, 5.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        fvet4 v(1.0f, 2.0f, 2.0f, 1.0f);
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
        fvet3 v1(1.0f, 2.0f, 3.0f);
        fvet3 v2(0.5f, 1.5f, -1.0f);
        fvet3 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
        ASSERT_EQUAL(resultado.z, 2.0f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        fvet3 v(1.0f, 2.0f, 3.0f);
        fvet3 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        fvet3 v1(3.0f, 5.0f, 7.0f);
        fvet3 v2(1.0f, 2.0f, 3.0f);
        fvet3 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        fvet3 v(5.0f, 6.0f, 7.0f);
        fvet3 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
        ASSERT_EQUAL(resultado.z, 5.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        fvet3 v1(2.0f, 3.0f, 4.0f);
        fvet3 v2(2.0f, 2.0f, 2.0f);
        fvet3 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
        ASSERT_EQUAL(resultado.z, 8.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        fvet3 v1(4.0f, 6.0f, 8.0f);
        fvet3 v2(2.0f, 2.0f, 2.0f);
        fvet3 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
        ASSERT_EQUAL(resultado.z, 4.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        fvet3 v1(1.0f, 2.0f, 3.0f);
        fvet3 v2(1.0f, 2.0f, 3.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        fvet3 v1(1.0f, 2.0f, 3.0f);
        fvet3 v2(1.0f, 1.0f, 3.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        fvet3 v(1.0f, 2.0f, 2.0f);
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
        fvet2 v1(1.0f, 2.0f);
        fvet2 v2(0.5f, 1.5f);
        fvet2 resultado = v1 + v2;
        ASSERT_EQUAL(resultado.x, 1.5f);
        ASSERT_EQUAL(resultado.y, 3.5f);
    });

    // Teste de soma com escalar
    testes.adicionar("soma_escalar", [](){
        fvet2 v(1.0f, 2.0f);
        fvet2 resultado = v + 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
    });

    // Teste de subtração
    testes.adicionar("subtracao", [](){
        fvet2 v1(3.0f, 5.0f);
        fvet2 v2(1.0f, 2.0f);
        fvet2 resultado = v1 - v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
    });

    // Teste de subtração com escalar
    testes.adicionar("subtracao_escalar", [](){
        fvet2 v(5.0f, 6.0f);
        fvet2 resultado = v - 2.0f;
        ASSERT_EQUAL(resultado.x, 3.0f);
        ASSERT_EQUAL(resultado.y, 4.0f);
    });

    // Teste de multiplicação
    testes.adicionar("multiplicacao", [](){
        fvet2 v1(2.0f, 3.0f);
        fvet2 v2(2.0f, 2.0f);
        fvet2 resultado = v1 * v2;
        ASSERT_EQUAL(resultado.x, 4.0f);
        ASSERT_EQUAL(resultado.y, 6.0f);
    });

    // Teste de divisão
    testes.adicionar("divisao", [](){
        fvet2 v1(4.0f, 6.0f);
        fvet2 v2(2.0f, 2.0f);
        fvet2 resultado = v1 / v2;
        ASSERT_EQUAL(resultado.x, 2.0f);
        ASSERT_EQUAL(resultado.y, 3.0f);
    });

    // Teste de igualdade
    testes.adicionar("igualdade", [](){
        fvet2 v1(1.0f, 2.0f);
        fvet2 v2(1.0f, 2.0f);
        ASSERT_TRUE(v1 == v2);
    });

    // Teste de desigualdade
    testes.adicionar("desigualdade", [](){
        fvet2 v1(1.0f, 2.0f);
        fvet2 v2(1.0f, 3.0f);
        ASSERT_TRUE(v1 != v2);
    });

    // Teste de normalização
    testes.adicionar("normalizacao", [](){
        fvet2 v(3.0f, 4.0f);
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
struct CompA : becommons::componente { 
    static constexpr becommons::componente::mascara mascara = static_cast<becommons::componente::mascara>(1);
};
struct CompB : becommons::componente { 
    static constexpr becommons::componente::mascara mascara = static_cast<becommons::componente::mascara>(1 << 1);
};

void testarFase()
{
    testes.classe("NUCLEO", "FASE");
    
    // Teste de construtor com diretório
    testes.adicionar("parse_nome", [](){
        //- Preparação:
        const std::string diretorio = becommons::obterExecDir() + "/../assets/teste_fase.fase";
        becommons::fase fase(diretorio);
        fase.carregar();
    
        //- Verificação:
        // nome da fase deve ser igual ao do pasing.
        ASSERT_EQUAL(fase.nome(), "fase_teste");
    });
}

void testarRegistro()
{
    testes.classe("NUCLEO", "REGISTRO");
    
    // Teste de inicialização
    
    testes.adicionar("criando_entidades", [](){
        becommons::registro reg;
        becommons::entidade e1 = reg.criar();
        becommons::entidade e2 = reg.criar();
        
        ASSERT_TRUE(e1.id != e2.id);  // IDs devem ser diferentes
        ASSERT_EQUAL(e1.mascara, becommons::componente::COMPONENTE_TRANSFORMACAO);
        ASSERT_EQUAL(e2.mascara, becommons::componente::COMPONENTE_TRANSFORMACAO);
    });

    testes.adicionar("tem/nao_tem_componente", [](){
        becommons::registro reg;
        becommons::entidade e = reg.criar();
    
        ASSERT_TRUE(!reg.tem<becommons::fisica>(e.id));
        reg.adicionar<becommons::fisica>(e);
        ASSERT_TRUE(reg.tem<becommons::fisica>(e.id));
    });
    
    testes.adicionar("obtendo_componente", [](){
		becommons::registro reg;
		becommons::entidade e = reg.criar();
	
		ASSERT_TRUE(!reg.tem<becommons::fisica>(e.id));

        reg.adicionar<becommons::fisica>(e);

		ASSERT_TRUE(reg.obter<becommons::fisica>(e.id)!=nullptr);
    });
    
    testes.adicionar("removendo_componente_nulo", [](){
            becommons::registro reg;
            becommons::entidade e = reg.criar();
        
            ASSERT_TRUE(!reg.tem<becommons::fisica>(e.id));
            reg.remover<becommons::fisica>(e.id);
            ASSERT_TRUE(!reg.tem<becommons::fisica>(e.id));
            ASSERT_TRUE(!reg.obter<becommons::fisica>(e.id));
        });
        
        testes.adicionar("adicionando_removento_componentes", [](){
            becommons::registro reg;
            becommons::entidade e = reg.criar();
            
            reg.adicionar<CompA>(e);
            ASSERT_TRUE(reg.tem<CompA>(e.id));
        
            auto componente = reg.obter<CompA>(e.id);
            ASSERT_TRUE(componente != nullptr);
            ASSERT_EQUAL(componente->meu_objeto, e.id);
        
            reg.remover<CompA>(e.id);
            ASSERT_TRUE(!reg.tem<CompA>(e.id));
        });
        
        testes.adicionar("iteracao_com_multiplos_componentes", []() {
            becommons::registro reg;
            
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
            becommons::registro reg;
            
            // cria entidades sem componentes
            auto e = reg.criar();
        
            int count = 0;
            reg.cada<becommons::fisica>([&](uint32_t) { count++; });
            ASSERT_EQUAL(count, 0);
        });
        
        testes.adicionar("limpeza_entidade_sem_componentes", []() {
            becommons::registro reg;
            auto e = reg.criar();
        
            // adiciona e remove componente
            reg.adicionar<becommons::fisica>(e, false, 0.0f, btVector3(0.0f, 0.0f, 0.0f));
            reg.remover<becommons::fisica>(e.id);
            reg.remover<becommons::transformacao>(e.id);
        
            // entidade deve ser removida do mapa
            ASSERT_TRUE(reg.obterComponentes(e.id) == becommons::componente::COMPONENTE_NONE);
            ASSERT_TRUE(reg.entidades.find(e.id) == reg.entidades.end());
        });
        
        testes.adicionar("multiplos_componentes_mascara", []() {
            becommons::registro reg;
            auto e = reg.criar();
        
            reg.adicionar<CompA>(e);
            reg.adicionar<CompB>(e);
            reg.remover<becommons::transformacao>(e.id);

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
    // Teste 0: obtenção / adição.
    testes.adicionar("obtencao_e_adicao", []() {
        //- Preparação 1:
        becommons::bubble_gui gui;

        gui.iniciarRaiz("raiz");
        gui.fimEstilo();
        // adiciona pai
        gui.adicionar<becommons::caixa>("raiz", "parent_test");
         
        //- Verificação 1:
        // captura exceção do estilo do pai.
        ASSERT_THROW(gui.obterElemento("parent_test"), std::runtime_error); 
        
        //- Preparação 2:
        // corrige exceção de estilo
        gui.fimEstilo();
        
        // adiciona filho de forma incorreta
        gui.obterElemento("parent_test")->adicionarFilho<becommons::caixa>("filho");
        
        //- Verificação 2:
        // sem exceção do estilo
        ASSERT_NO_THROW(gui.obterElemento("parent_test"));
        
        // captura exceção de filho não encontrado.
        ASSERT_THROW(gui.obterElemento("filho"), std::runtime_error); 
        
        //- Preparação 3:
        // adiciona o filho da maneira correta.
        gui.adicionar<becommons::caixa>("parent_test", "filho");

        // corrige exceção
        gui.fimEstilo();

        //- Verificação 3:
        // sem exceção do filho não encontrado.
        ASSERT_NO_THROW(gui.obterElemento("filho"));
    });


    // Teste 1: obtenção vazia
    testes.adicionar("obtencao_vazia", []() {
        //- Preparação:
        becommons::bubble_gui gui;

        //- Verificação:
        // deve retornar exceção de caixa não encontrada.
        ASSERT_THROW(gui.obterElemento("empty_element"), std::runtime_error);
    });

    // Teste 2: Parente inexistente
    testes.adicionar("parente_vazio", []() {
        //- Preparação:
        becommons::bubble_gui gui;
        
        //- Verificação:
        // deve retornar exceção.
        ASSERT_THROW(gui.adicionar<becommons::caixa>("empty_parent", "child_test"), std::runtime_error); 
    });
    
    // Teste 3: Caixa nula
    testes.adicionar("atualizar_filhos_caixa_nula", []() {
        //- Preparação:
        becommons::bubble_gui gui;
        
        //- Verificação:
        // deve retornar exceção de atualização.
        ASSERT_THROW(gui.atualizarFilhos(nullptr), std::runtime_error);
    });

    // @name Teste 4: Caixa inativa
    // @brief
    testes.adicionar("atualizar_filhos_caixa_inativa", []() {
        //- Preparação:
        becommons::bubble_gui gui;

        gui.iniciarRaiz("raiz");
        gui.fimEstilo();
        // adiciona pai
        gui.adicionar<becommons::caixa>("raiz", "parent_test");
        
        // adiciona filho
        gui.adicionar<becommons::caixa>("parent_test", "filho");
        
        gui.fimEstilo();

        // desativa caixa de maneira bruta
        gui.obterElemento("parent_test")->m_estilo.m_ativo = false;
        
        //- Verificação:
        ASSERT_NO_THROW(gui.atualizarFilhos(gui.obterElemento("parent_test")));

        // confirma valoras inalterados
        // Valores padrão = 0, 0, 20, 20.
        ASSERT_EQUAL(gui.obterElemento("filho")->m_estilo.m_limites, fvet4(0.f,0.f,20.f,20.f)); 
    });

    // Teste 3: Layout horizontal com dimensões fixas
    testes.adicionar("atualizar_filhos_horizontal_fixo", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites = {0, 0, 100, 50};
        parent.m_estilo.m_padding_geral = {5, 0};

        auto* child1 = parent.adicionarFilho<becommons::caixa>("child1");
        child1->m_estilo.m_largura = 30;
        child1->m_estilo.m_padding.x = 2;

        auto* child2 = parent.adicionarFilho<becommons::caixa>("child2");
        child2->m_estilo.m_largura = 40;
        child2->m_estilo.m_padding.x = 3;

        becommons::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child1->m_estilo.m_limites.x == 7);  // 0 +5 +2
        ASSERT(child1->m_estilo.m_limites.z == 30);
        ASSERT(child2->m_estilo.m_limites.x == 47); // 39 +5 +3
        ASSERT(child2->m_estilo.m_limites.z == 40);
    });

    // Teste 4: Layout horizontal com porcentagem
    testes.adicionar("atualizar_filhos_horizontal_percentual", []() {
        //- Preparação:
        // prepara instâncias
        becommons::caixa parent;
        becommons::bubble_gui gui;

        // edita o esitilo
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites.z = 200;

        // adiciona e obtêm filho
        auto* child = parent.adicionarFilho<becommons::caixa>("child");
        child->m_estilo.m_flag_estilo |= becommons::flag_estilo::largura_percentual;
        child->m_estilo.m_largura = 0.5f;

        gui.atualizarFilhos(&parent);

        //- Verificação:
        // largura do filho deve ser 50% da largura do pai
        ASSERT_EQUAL(child->m_estilo.m_limites.z, 100); // 200 * 0.5
    });

    // Teste 5: Crescimento modular horizontal
    testes.adicionar("atualizar_filhos_horizontal_crescimento", []() {
        //- Preparação:
        // prepara instâncias
        becommons::caixa parent;

        // edita o esitilo
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites.z = 100;

        auto* child1 = parent.adicionarFilho<becommons::caixa>("child1");
        child1->m_estilo.m_largura = 30;
        child1->m_estilo.m_crescimento_modular = 1;

        auto* child2 = parent.adicionarFilho<becommons::caixa>("child2");
        child2->m_estilo.m_largura = 40;
        child2->m_estilo.m_crescimento_modular = 1;

        becommons::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites.z, 45); // 30 + (100-70)/2
        ASSERT_EQUAL(child2->m_estilo.m_limites.z, 55); // 40 + (100-70)/2
    });

    // Teste 6: Layout vertical com dimensões fixas e percentual
    testes.adicionar("atualizar_filhos_vertical_misto", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::vertical;
        parent.m_estilo.m_limites = {0, 0, 80, 150};
        parent.m_estilo.m_padding_geral = {10, 5};

        auto* child = parent.adicionarFilho<becommons::caixa>("child");
        child->m_estilo.m_flag_estilo |= becommons::flag_estilo::largura_percentual;
        child->m_estilo.m_largura = 0.5f;
        child->m_estilo.m_altura = 60;

        becommons::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        // Largura esperada: (80 * 0.5) - 2*10 = 40 - 20 = 20
        ASSERT(child->m_estilo.m_limites.z == 20);
        ASSERT(child->m_estilo.m_limites.w == 60);
    });

    // Teste 7: Atualização recursiva de filhos
    testes.adicionar("atualizar_filhos_recursivo", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;

        auto* child = parent.adicionarFilho<becommons::caixa>("child");
        child->m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        child->m_estilo.m_orientacao_modular = becommons::estilo::orientacao::vertical;

        auto* grandchild = child->adicionarFilho<becommons::caixa>("grandchild");
        grandchild->m_estilo.m_altura = 30;

        becommons::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(grandchild->m_estilo.m_limites.w == 30); // Verifica se o neto foi atualizado
    });

    // Teste 8: Espaço insuficiente para crescimento
    testes.adicionar("atualizar_filhos_espaco_insuficiente", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites.z = 50;

        auto* child1 = parent.adicionarFilho<becommons::caixa>("child1");
        child1->m_estilo.m_largura = 30;
        child1->m_estilo.m_crescimento_modular = 1;

        auto* child2 = parent.adicionarFilho<becommons::caixa>("child2");
        child2->m_estilo.m_largura = 30;
        child2->m_estilo.m_crescimento_modular = 1;

        becommons::bubble_gui gui;
        gui.atualizarFilhos(&parent);

        ASSERT(child1->m_estilo.m_limites.z == 30); // Sem crescimento (espaço negativo)
        ASSERT(child2->m_estilo.m_limites.z == 30);
    });
    
    // Teste 9: 
    testes.adicionar("padding_deral_fixo", []() {
        becommons::bubble_gui gui;
        
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_padding_geral = {5, 5};
        auto* child1 = parent.adicionarFilho<becommons::caixa>("child1");
        auto* child2 = parent.adicionarFilho<becommons::caixa>("child2");
        gui.atualizarFilhos(&parent);
        // Horizontal
        // Assegura que o padding foi aplicado aos filhos
        ASSERT_EQUAL(child1->m_estilo.m_limites.x, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.x, 30) // Largura do child1 mais o padding
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::vertical;

        gui.atualizarFilhos(&parent);
        ASSERT_EQUAL(child1->m_estilo.m_limites.y, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.y, 30) // Largura do child1 mais o padding
    });
    
    // Teste 10 : Crescimento modular com padding e pai de 100x100
    testes.adicionar("padding_modular_crescimento_completo", []() {
        becommons::bubble_gui gui;
    
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_padding_geral = {5, 5};
        parent.m_estilo.m_largura = 100;
        parent.m_estilo.m_altura = 100; 
    
        auto* child1 = parent.adicionarFilho<becommons::caixa>("child1");
        auto* child2 = parent.adicionarFilho<becommons::caixa>("child2");
    
        child1->m_estilo.m_crescimento_modular = 1.0f;
        child2->m_estilo.m_crescimento_modular = 1.0f;
    
        gui.atualizarFilhos(&parent);
    
        // Área total disponível para largura: 100 - (padding esquerdo + padding entre filhos + padding direito) = 100 - (5 + 5 + 5) = 85
        // Como ambos têm crescimento 1.0, cada um recebe 85 / 2 = 42.5
        
        ASSERT_EQUAL(child1->m_estilo.m_limites.x,   5.f);       // começa no padding esquerdo
        ASSERT_EQUAL(child1->m_estilo.m_limites.z, 42.5f);      // metade do espaço disponível
        ASSERT_EQUAL(child2->m_estilo.m_limites.x, 52.5f);      // 5 (padding) + 42 (child1) + 5 (padding)
        ASSERT_EQUAL(child2->m_estilo.m_limites.z, 42.5f);      // mesma largura
    
        // Também podemos verificar altura, se estiver fixa em 100
        ASSERT_EQUAL(child1->m_estilo.m_limites.w, 100);
        ASSERT_EQUAL(child2->m_estilo.m_limites.w, 100);
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
    return testes.rodarTestes();
}
