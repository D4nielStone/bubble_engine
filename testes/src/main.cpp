/** \copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * \file main.cpp
 */


#include <iostream>
#include "registro_testes.hpp"
#include "becommons/becommons.hpp"

using namespace becommons;

betest::registro_testes testes;

void testarVetor4()
{
    testes.classe("utils", "vetor4");

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
    testes.classe("utils", "vetor3");

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
    testes.classe("utils", "vetor2");

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
    testes.classe("nucleo", "fase");
    
    // Teste de construtor com diretório
    testes.adicionar("parse_nome", [](){
        //- Preparação:
        becommons::fase fase(R"(
        {
            "nome": "fase_teste"
        }
        )");
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
            
            reg.adicionar<CompA>(1);
            reg.adicionar<CompB>(1);  // tem ambos
            
            reg.adicionar<CompA>(2);   // apenas CompA
            
            reg.adicionar<CompB>(3);   // apenas CompB
        
            // testa iterações para entidades com ambos
            int count = 0;
            reg.cada<CompA, CompB>([&](uint32_t entity) {
                count++;
                ASSERT_EQUAL(entity, 1);
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
            reg.adicionar<becommons::fisica>(e);
            reg.remover<becommons::fisica>(e.id);
            reg.remover<becommons::transformacao>(e.id);
        
            // entidade deve ser removida do mapa
            ASSERT_TRUE(reg.obterComponentes(e.id) == becommons::componente::COMPONENTE_NONE);
            ASSERT_TRUE(reg.entidades.find(e.id) == reg.entidades.end());
        });
        
        testes.adicionar("multiplos_componentes_mascara", []() {
            becommons::registro reg;
        
            reg.remover<becommons::transformacao>(1);
            reg.adicionar<CompA>(1);
            reg.adicionar<CompB>(1);

            ASSERT_EQUAL(reg.obterComponentes(1), (CompA::mascara | CompB::mascara));
        
            reg.remover<CompA>(1);
            ASSERT_EQUAL(reg.obterComponentes(1), CompB::mascara);
        
            reg.adicionar<CompA>(1);
            ASSERT_EQUAL(reg.obterComponentes(1), (CompA::mascara | CompB::mascara));
        });
}

void testarMotor()
{
    testes.classe("nucleo", "motor");

    // Teste 1: instancia global (singleton-like) é atualizada no construtor
    testes.adicionar("singleton_instance", []() {
        motor m1;
        // obter() deve referenciar a última instância construída
        ASSERT_TRUE(&becommons::motor::obter() == &m1);
    });

    // Teste 2: criação de múltiplas instâncias atualiza a referência global
    testes.adicionar("instance_updates_on_multiple", []() {
        motor m1;
        motor m2;
        ASSERT_TRUE(&becommons::motor::obter() == &m2); // última instância
    });

    // Teste 3: criarJanela() não deve lançar (caminho sem projeto)
    testes.adicionar("criar_janela_sem_projeto_nao_lanca", []() {
        motor m;
        m.setJanelaFactory([]() -> std::shared_ptr<ijanela>{
            return std::make_shared<ijanela>();
                });
        ASSERT_NO_THROW(m.criarJanela());
    });

    // Teste 4: criarJanela() com projeto (construtor com directory) não deve lançar
    testes.adicionar("criar_janela_com_projeto_nao_lanca", []() {
        // Prep
        motor m("dummy_project_dir");
        m.setJanelaFactory([]() -> std::shared_ptr<ijanela>{
            return std::make_shared<ijanela>();
                });
        // Assert
        ASSERT_NO_THROW(m.criarJanela());
    });

    // Teste 5: iniciar em modo editor deve setar o tipo e não lançar
    testes.adicionar("iniciar_editor_sets_mode", []() {
        motor m;
        m.setJanelaFactory([]() -> std::shared_ptr<ijanela>{
            return std::make_shared<ijanela>();
                });
        ASSERT_NO_THROW(m.iniciar(exec::editor));
        ASSERT_EQUAL(m.obterTipo(), exec::editor);
    });

    // Teste 6: iniciar em modo jogo — tenta iniciar; pode tocar levelmanager.
    // usamos ASSERT_NO_THROW para proteger o runner caso haja I/O não mockado.
    testes.adicionar("iniciar_jogo_sets_mode", []() {
        motor m("dummy_project_dir");
        m.setJanelaFactory([]() -> std::shared_ptr<ijanela>{
            return std::make_shared<ijanela>();
                });
        ASSERT_NO_THROW(m.iniciar(exec::jogo)); // se implementar levelmanager real, pode falhar — protegido
        ASSERT_EQUAL(m.obterTipo(), exec::jogo);
    });

    // Teste 7: rodando() padrão (espera-se falso / não inicializado)
    testes.adicionar("rodando_padrao_false", []() {
        motor m;
        ASSERT_EQUAL(m.rodando(), false);
    });

    // Teste 8: finalizar() não deve lançar
    testes.adicionar("finalizar_nao_lanca", []() {
        motor m;
        ASSERT_NO_THROW(m.finalizar());
    });

    // Teste 9: obter() referencia a instância ativa
    testes.adicionar("obter_referencia_valida", []() {
        motor m;
        motor& got = becommons::motor::obter();
        ASSERT_TRUE(&got == &m);
    });
}

void testarNucleo()
{
    testarRegistro();
    testarFase();
    testarMotor();
}

void testarBubbleGUI()
{
    testes.classe("sistemas", "interface");
    // Teste 1: obtenção / adição.
    testes.adicionar("obtencao_e_adicao", []() {
        //- Preparação:
        becommons::interface ui(nullptr);

        // adiciona pai
        auto* box = ui.m_raiz->adicionar<becommons::caixa>();

        auto* child = box->adicionar<becommons::caixa>();
        
        //- Verificação:
        ASSERT(child != nullptr);
    });
    
    // Teste 2: Caixa nula
    testes.adicionar("atualizar_filhos_caixa_nula", []() {
        //- Preparação:
        becommons::interface ui(nullptr);
        
        //- Verificação:
        // deve retornar exceção de atualização.
        ASSERT_THROW(ui.atualizarFilhos(nullptr), std::runtime_error);
    });

    // \name Teste 3: Caixa inativa
    // \brief
    testes.adicionar("atualizar_filhos_caixa_inativa", []() {
        //- Preparação:
        becommons::interface ui(nullptr);

        // adiciona pai
        auto* parent = ui.m_raiz->adicionar<becommons::caixa>();
        
        // adiciona filho
        auto* child  = parent->adicionar<becommons::caixa>();
        
        // desativa caixa 
        parent->m_estilo.m_ativo = false;
        
        //- Verificação:
        ASSERT_NO_THROW(ui.atualizarFilhos(parent));

        // confirma valoras inalterados
        // Valores padrão = 0, 0, 20, 20.
        ASSERT_EQUAL(child->m_estilo.m_limites, fvet4(0.f,0.f,20.f,20.f)); 
    });

    // Teste 4: Layout horizontal com dimensões fixas
    testes.adicionar("atualizar_filhos_horizontal_fixo", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites = {0, 0, 100, 50};
        parent.m_estilo.m_padding_geral = {5, 0};

        auto* child1 = parent.adicionar<becommons::caixa>();
        child1->m_estilo.m_largura = 30;
        child1->m_estilo.m_padding.x = 2;

        auto* child2 = parent.adicionar<becommons::caixa>();
        child2->m_estilo.m_largura = 40;
        child2->m_estilo.m_padding.x = 3;

        becommons::interface ui(nullptr);
        ui.atualizarFilhos(&parent);

        ASSERT(child1->m_estilo.m_limites.x == 7);  // 0 +5 +2
        ASSERT(child1->m_estilo.m_limites.z == 30);
        ASSERT(child2->m_estilo.m_limites.x == 45); // 37 + 5 + 3
        ASSERT(child2->m_estilo.m_limites.z == 40);
    });

    // Teste 4: Layout horizontal com porcentagem
    testes.adicionar("atualizar_filhos_horizontal_percentual", []() {
        //- Preparação:
        // prepara instâncias
        becommons::interface ui(nullptr);
        becommons::caixa parent;

        // edita o esitilo
        parent.m_estilo.m_limites.z = 200;

        // adiciona e obtêm filho
        auto* child = parent.adicionar<becommons::caixa>();
        child->m_estilo.m_flag_estilo |= becommons::flag_estilo::largura_percentual;
        child->m_estilo.m_largura = 2;

        ui.atualizarFilhos(&parent);

        //- Verificação:
        // largura do filho deve ser igual da largura do pai
        ASSERT_EQUAL(child->m_estilo.m_limites.z, 200);
    });

    // Teste 5: Crescimento modular horizontal
    testes.adicionar("atualizar_filhos_horizontal_crescimento", []() {
        //- Preparação:
        // prepara instâncias
        becommons::caixa parent;

        // edita o esitilo
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites.z = 100;

        auto* child1 = parent.adicionar<becommons::caixa>();
        child1->m_estilo.m_largura = 1;
        child1->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;

        auto* child2 = parent.adicionar<becommons::caixa>();
        child2->m_estilo.m_largura = 3;
        child2->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;

        becommons::interface ui(nullptr);
        ui.atualizarFilhos(&parent);

        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites.z, 25); // 1/4 * 100
        ASSERT_EQUAL(child2->m_estilo.m_limites.z, 75); // 3/4 * 100
    });

    // Teste 6: Layout vertical com dimensões fixas e percentual
    testes.adicionar("atualizar_filhos_vertical_misto", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::vertical;
        parent.m_estilo.m_limites = {0, 0, 80, 150};
        parent.m_estilo.m_padding_geral = {10, 5};

        auto* child = parent.adicionar<becommons::caixa>();
        child->m_estilo.m_flag_estilo |= becommons::flag_estilo::largura_percentual;
        child->m_estilo.m_largura = 1.f;
        child->m_estilo.m_altura = 60;

        becommons::interface ui(nullptr);
        ui.atualizarFilhos(&parent);

        // Largura esperada: 80 - 2*10 = 80 - 20 = 60
        ASSERT(child->m_estilo.m_limites.z == 60);
        ASSERT(child->m_estilo.m_limites.w == 60);
    });

    // Teste 7: Atualização recursiva de filhos
    testes.adicionar("atualizar_filhos_recursivo", []() {
        becommons::interface ui(nullptr);

        becommons::caixa parent;
        auto* child = parent.adicionar<becommons::caixa>();
        child->m_estilo.m_altura = 200;
        auto* grandchild = child->adicionar<becommons::caixa>();

        grandchild->m_estilo.m_altura = 100;

        ui.atualizarFilhos(&parent);
        ASSERT(grandchild->m_estilo.m_limites.w == 100); // Verifica se o neto foi atualizado
    });

    // Teste 8: Espaço insuficiente para crescimento
    testes.adicionar("atualizar_filhos_espaco_negativo", []() {
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_limites.z = 50;

        auto* child1 = parent.adicionar<becommons::caixa>();
        child1->m_estilo.m_largura = 1;
        child1->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;

        auto* child2 = parent.adicionar<becommons::caixa>();
        child2->m_estilo.m_largura = 1;
        child2->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;

        becommons::interface ui(nullptr);
        ui.atualizarFilhos(&parent);

        ASSERT(child1->m_estilo.m_limites.z == 25);
        ASSERT(child2->m_estilo.m_limites.z == 25);
    });
    
    // Teste 9: 
    testes.adicionar("padding_deral_fixo", []() {
        becommons::interface ui(nullptr);
        
        becommons::caixa parent;
        parent.m_estilo.m_flag_estilo = becommons::flag_estilo::modular;
        parent.m_estilo.m_padding_geral = {5, 5};
        auto* child1 = parent.adicionar<becommons::caixa>();
        auto* child2 = parent.adicionar<becommons::caixa>();
        ui.atualizarFilhos(&parent);
        // Horizontal
        // Assegura que o padding foi aplicado aos filhos
        ASSERT_EQUAL(child1->m_estilo.m_limites.x, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.x, 30) // Largura do child1 mais o padding
        ASSERT_EQUAL(child1->m_estilo.m_limites.y, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.y, 5) // padding
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::vertical;

        ui.atualizarFilhos(&parent);
        ASSERT_EQUAL(child1->m_estilo.m_limites.y, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.y, 30) // Largura do child1 mais o padding
        ASSERT_EQUAL(child1->m_estilo.m_limites.x, 5)
        ASSERT_EQUAL(child2->m_estilo.m_limites.x, 5) // padding
    });
    
    // Teste 10 : Crescimento modular com padding e pai de 100x100
    testes.adicionar("padding_modular_crescimento_completo", []() {
        becommons::interface ui(nullptr);
    
        becommons::caixa parent;
        parent.m_estilo.m_orientacao_modular = becommons::estilo::orientacao::horizontal;
        parent.m_estilo.m_padding_geral = {5, 5};
        parent.m_estilo.m_limites = {0, 0, 100, 100}; 
    
        auto* child1 = parent.adicionar<becommons::caixa>();
        auto* child2 = parent.adicionar<becommons::caixa>();
    
        child1->m_estilo.m_largura = 1;
        child1->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;
        child2->m_estilo.m_largura = 1;
        child2->m_estilo.m_flag_estilo = becommons::flag_estilo::largura_percentual;
    
        ui.atualizarFilhos(&parent);
    
        // Área total disponível para largura: 100 - (padding esquerdo + padding entre filhos + padding direito) = 100 - (5 + 5 + 5) = 85
        // Como ambos têm crescimento 1.0, cada um recebe 85 / 2 = 42.5
        
        ASSERT_EQUAL(child1->m_estilo.m_limites.x,   5.f);       // começa no padding esquerdo
        ASSERT_EQUAL(child1->m_estilo.m_limites.z, 42.5f);      // metade do espaço disponível
        ASSERT_EQUAL(child2->m_estilo.m_limites.x, 52.5f);      // 5 (padding) + 42 (child1) + 5 (padding)
        ASSERT_EQUAL(child2->m_estilo.m_limites.z, 42.5f);      // mesma largura
    });
    // Teste 11 : Testando flag "quebrar_linha"
    testes.adicionar("flag_quebrar_linha", []() {
        //- Preparação:
        becommons::interface ui(nullptr);
        becommons::caixa box; 
        box.m_estilo.m_altura = 40; // 20*40
        auto* child1 = box.adicionar<becommons::caixa>();
        auto* child2 = box.adicionar<becommons::caixa>();
        child1->m_estilo.m_flag_estilo =  becommons::flag_estilo::quebrar_linha;
        ui.atualizarFilhos(&box);
        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites.y, 0);
        ASSERT_EQUAL(child2->m_estilo.m_limites.y, 20);
    });
}
void testarSistemas()
{
    testarBubbleGUI();
}

/**
 * @brief Sumário de responsabilidades da engine
 *
 * @author Daniel Oliveira
 * @copyright MIT License
 *
 * @details
 * Comentários Doxygen a serem colocados no topo do projeto / módulos principais.
 * Contém: Responsibilities, Invariantes, Assinaturas sugeridas e Observações
 * para as partes testadas pelos testes em `main.cpp`.
 *
 * ======================================================================
 * 1) UTILS — Vetores (fvet2, fvet3, fvet4)
 * ======================================================================
 *
 * @section utils_vectors Responsabilidade
 * Fornecer tipos de vetores (fvet2, fvet3, fvet4) com operações aritméticas
 * básicas, comparações, normalização e cálculo de norma euclidiana.
 *
 * @subsection utils_vectors_funcs Operações esperadas / API sugerida
 * - Operadores binários componente-a-componente:
 *   - fvetN operator+(const fvetN& a, const fvetN& b);
 *   - fvetN operator-(const fvetN& a, const fvetN& b);
 *   - fvetN operator*(const fvetN& a, const fvetN& b);
 *   - fvetN operator/(const fvetN& a, const fvetN& b);
 * - Operadores com escalar (float):
 *   - fvetN operator+(const fvetN& a, float s);
 *   - fvetN operator-(const fvetN& a, float s);
 *   - fvetN operator*(const fvetN& a, float s);
 *   - fvetN operator/(const fvetN& a, float s);
 * - Comparações:
 *   - bool operator==(const fvetN& a, const fvetN& b);
 *   - bool operator!=(const fvetN& a, const fvetN& b);
 * - Normalização e norma:
 *   - void normalizar();           ///< normaliza (com proteção contra zero)
 *   - float tamanho() const;       ///< norma Euclidiana
 *
 * @subsection utils_vectors_invariants Invariantes e cuidados
 * - Operações devem ser definidas componente-a-componente.
 * - `normalizar()` deve tratar vetor de comprimento ~0 (usar eps).
 * - Usar comparações aproximadas (float_equal) quando necessário.
 * - Evitar UB para NaN/Inf; documentar comportamento.
 *
 * @note
 * Recomenda-se utilitário: `inline bool float_equal(float a, float b, float eps = 1e-6f);`
 *
 * ======================================================================
 * 2) NÚCLEO — Registro / Entidade / Componente (ECS simplificado)
 * ======================================================================
 *
 * @section nucleo_ecs Responsabilidade
 * Implementar um ECS básico que permita:
 * - criar entidades únicas,
 * - adicionar/obter/remover componentes tipados,
 * - iterar sobre entidades que tenham um conjunto específico de componentes,
 * - manter máscaras (bitfield) de componentes por entidade,
 * - limpar (remover) entidades sem componentes restantes.
 *
 * @subsection nucleo_ecs_funcs API sugerida (assinaturas)
 * @code
 * struct entidade { uint32_t id; uint32_t mascara; };
 *
 * class registro {
 * public:
 *   entidade criar();
 *
 *   template<typename T> void adicionar(entidade e);        // adiciona componente T
 *   template<typename T> bool tem(uint32_t id) const;      // verifica presença
 *   template<typename T> T* obter(uint32_t id);            // retorna ponteiro ou nullptr
 *   template<typename T> void remover(uint32_t id);        // idempotente
 *
 *   template<typename... Ts>
 *   void cada(std::function<void(uint32_t)> f);            // itera entidades com TODOS Ts
 *
 *   uint32_t obterComponentes(uint32_t id) const;          // máscara combinada
 *
 *   std::unordered_map<uint32_t, entidade> entidades;
 * };
 * @endcode
 *
 * @subsection nucleo_ecs_invariants Invariantes e expectativas
 * - `criar()` deve gerar `id` único; máscara inicial inclui TRANSFORMACAO.
 * - `adicionar<T>` deve inicializar `meu_objeto = id` (conforme testes).
 * - `remover<T>` é segura mesmo quando componente inexistente.
 * - `cada<CompA,CompB>` itera apenas entidades que possuam todas as máscaras (bitwise &).
 * - Quando todos os componentes são removidos, a entidade deve ser removida do mapa `entidades`.
 *
 * @warning
 * Evitar use-after-free ao remover componentes durante iterações; documentar política de reuso de IDs.
 *
 * ======================================================================
 * 3) FASE
 * ======================================================================
 *
 * @section fase Responsabilidade
 * Representar e carregar dados de uma fase/level a partir de texto (JSON ou similar).
 *
 * @subsection fase_funcs API sugerida
 * @code
 * class fase {
 * public:
 *   explicit fase(const std::string& source);
 *   void carregar();               // parseia `source` e popula membros
 *   std::string nome() const;      // retorna nome da fase
 * private:
 *   std::string m_source;
 *   std::string m_nome;
 * };
 * @endcode
 *
 * @subsection fase_invariants Invariantes e cuidados
 * - `carregar()` deve tratar erros de parse (lançar ou retornar estado de erro).
 * - `nome()` deve retornar valor consistente se `carregar()` foi bem-sucedido.
 *
 * ======================================================================
 * 4) SISTEMAS — INTERFACE (interface / caixa)
 * ======================================================================
 *
 * @section bubble_gui Responsabilidade
 * Sistema de UI com hierarquia de caixas (containers) capaz de:
 * - manter uma árvore (interface -> raiz -> caixas filhas),
 * - calcular/atualizar limites (x, y, z=largura, w=altura) dos filhos,
 * - aplicar padding geral e padding por filho,
 * - suportar layout modular (orientação horizontal/vertical),
 * - suportar larguras/alturas fixas e percentuais (peso / crescimento),
 * - flags de estilo: modular, largura_percentual, quebrar_linha, ativo/inativo,
 * - atualização recursiva (netos também são atualizados).
 *
 * @subsection bubble_gui_data Estruturas de estilo/caixa (exemplo)
 * @code
 * struct estilo {
 *   fvet4 m_limites;         // x, y, z=largura, w=altura
 *   fvet2 m_padding;         // padding individual (left/top)
 *   fvet2 m_padding_geral;   // padding geral do container
 *   float m_largura;         // valor fixo ou peso quando largura_percentual
 *   float m_altura;
 *   uint32_t m_flag_estilo;  // bitflags (modular, largura_percentual, quebrar_linha)
 *   enum orientacao { horizontal, vertical } m_orientacao_modular;
 *   bool m_ativo;
 * };
 *
 * class caixa {
 * public:
 *   template<typename T> T* adicionar();   // cria e retorna ponteiro para filho
 *   estilo m_estilo;
 *   std::vector<caixa*> filhos;
 * };
 *
 * class interface {
 * public:
 *   caixa* m_raiz;
 *   void atualizarFilhos(caixa* parent);
 * };
 * @endcode
 *
 * @subsection bubble_gui_layout Regras e exemplos verificados por testes
 * - `atualizarFilhos(nullptr)` deve lançar `std::runtime_error`.
 * - Se `parent->m_estilo.m_ativo == false`, `atualizarFilhos(parent)` não altera os limites dos filhos.
 * - Layout horizontal com dimensões fixas:
 *   - posição X do primeiro filho = padding_geral.x + child->m_estilo.m_padding.x
 *   - largura do filho = child->m_estilo.m_largura
 * - Percentuais / crescimento modular:
 *   - Se `flag_estilo & largura_percentual` então `m_largura` é tratada como peso.
 *   - espaço_disponivel = parent.largura - somatório(padding e paddings entre filhos)
 *   - largura_filho = peso_filho / soma_pesos * espaço_disponivel
 * - Layout vertical espelha regras para alturas (limites.w).
 * - Padding geral e padding por filho afetam posições x/y esperadas.
 * - `quebrar_linha` força avanço na coordenada Y (ex.: próximo filho começa em Y + linha_size).
 * - Atualização é recursiva: ao atualizar parent, filhos e netos são atualizados.
 *
 * @subsection bubble_gui_invariants Cuidados / notas de implementação
 * - Trabalhar com floats e preservar precisões (ex.: 42.5f).
 * - Evitar divisões por zero quando soma de pesos == 0 (tratar como distribuição igual).
 * - Definir comportamento quando padding excede tamanho do pai (clamping ou overflow controlado).
 * - `atualizarFilhos` deve ser determinística e sem efeitos colaterais não documentados.
 *
 * ======================================================================
 * 5) INFRAESTRUTURA DE TESTES (registro_testes, ASSERT_*)
 * ======================================================================
 *
 * @section test_runner Responsabilidade
 * Registrar e executar testes organizados por classes e nomes, prover asserções:
 * - testes.classe(categoria, nome);
 * - testes.adicionar(nomeCaso, lambda);
 * - testes.rodarTestes() -> int (exit code)
 *
 * Macros/helpers esperados:
 * - ASSERT_EQUAL(a, b)
 * - ASSERT_TRUE(expr)
 * - ASSERT_APPROX_EQUAL(a, b, eps)
 * - ASSERT_THROW(expr, exception_type)
 * - ASSERT_NO_THROW(expr)
 *
 * @note
 * Mensagens descritivas em falhas facilitam debugging; o runner pode continuar rodando casos mesmo após falhas.
 *
 * ======================================================================
 * 6) main() — Orquestração
 * ======================================================================
 *
 * @section main Orquestração de execução de testes
 * - desativa `depuracao::debug_ativo` se aplicável;
 * - chama as funções que registram os testes:
 *   - testarUtils();
 *   - testarNucleo();
 *   - testarSistemas();
 * - retorna `testes.rodarTestes();` como código de saída do processo.
 *
 * ======================================================================
 * Observações gerais e recomendações de implementação
 * ======================================================================
 * - Use enum class para flags + operadores bitwise inline para clareza.
 * - Centralizar constantes (ex.: limites padrão 20x20, epsilon para floats).
 * - Documentar política de IDs (reuso / incremento).
 * - Garantir que remover componentes durante laços não cause UAF; definir política
 *   (ex.: marcação para remoção e limpeza pós-iteração).
 * - Implementar comparações de float com epsilon em utilitário comum.
 * - Validar e documentar comportamento com valores especiais (NaN / Inf).
 *
 * ======================================================================
 */


int main()
{
    depuracao::debug_ativo = false;
	testarUtils();
    testarNucleo();
    testarSistemas();
    return testes.rodarTestes();
}
