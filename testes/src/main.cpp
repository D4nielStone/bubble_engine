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
 *
 * @brief Sumário de testes
 *
 * @author Daniel Oliveira
 * @details
 * Resumo compacto das responsabilidades e API esperada para os módulos cobertos
 * pelos testes em `main.cpp`. Usar este bloco como referência no topo dos
 * módulos principais.
 *
 * @section utils Vetores (fvet2, fvet3, fvet4)
 * - Responsabilidade: operações componente-a-componente, comparações, normalizar e norma.
 * - API mínima: operadores + - * / (vetor, escalar), ==, !=, void normalizar(), float tamanho() const.
 * - Invariantes: normalizar trata comprimento ~0 (eps), comparar floats com eps.
 *
 * @section nucleo ECS (registro / entidade / componente)
 * - Responsabilidade: criar entidades, adicionar/obter/remover componentes tipados, iterar por combinação de componentes.
 * - API mínima: entidade criar(); template adicionar<T>(entidade/id); tem<T>(id); obter<T>(id); remover<T>(id); cada<Ts...>(callback).
 * - Invariantes: criar() gera id único; remover é idempotente; entidade sem componentes é removida do mapa.
 *
 * @section fase Level / parsing
 * - Responsabilidade: representar e carregar fase a partir de texto (JSON).
 * - API mínima: explicit fase(const std::string&); void carregar(); std::string nome() const.
 * - Invariantes: carregar() sinaliza erro em parse; nome() consistente após carregar().
 *
 * @section bubble_gui UI (interface / caixa)
 * - Responsabilidade: árvore de caixas, cálculo de limites (x,y,z=largura,w=altura), padding, layouts modular (h/v), percentuais, flags (modular, largura_percentual, quebrar_linha, ativo).
 * - API mínima: caixa::adicionar<T>(); interface::atualizarFilhos(caixa*).
 * - Regras-chave: atualizarFilhos(nullptr) lança; inativo não altera filhos; percentuais tratam m_largura como peso; atualizar é recursivo.
 *
 * @section test_runner Infra de testes
 * - Responsabilidade: registrar/rodar casos e prover asserts.
 * - API mínima: registro_testes::classe(cat, nome); adicionar(nome, lambda); int rodarTestes();
 * - Asserções: ASSERT_EQUAL, ASSERT_TRUE, ASSERT_APPROX_EQUAL, ASSERT_THROW, ASSERT_NO_THROW.
 *
 * @section main Orquestração
 * - Fluxo: desativar logs, registrar testes (testarUtils, testarNucleo, testarSistemas), return testes.rodarTestes().
 *
 * @note Recomendações rápidas:
 * - Usar float eps centralizado; evitar UAF ao remover componentes durante iterações (marcar-para-remover); usar enum class + operadores bitwise para flags.
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
        ASSERT_EQUAL(child1->m_estilo.m_limites, fvet4(0, 0, 20, 20));
        ASSERT_EQUAL(child2->m_estilo.m_limites, fvet4(0, 20, 20, 20));
        
        //- Preparação:
        box.m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
        ui.atualizarFilhos(&box);
        
        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites, fvet4(0, 0, 20, 20));
        ASSERT_EQUAL(child2->m_estilo.m_limites, fvet4(20, 0, 20, 20));
    });
    // Teste 12 : Teste do container
    testes.adicionar("contanier_horizontal", []() {
        //- Preparação:
        becommons::container box; 
        box.m_estilo.m_limites = {0, 0, 100, 100};
        auto [child1, child2] = box.split(0.2); // divisão horizontal
        box.atualizar();
        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites, fvet4(0, 0, 20, 100));
        ASSERT_EQUAL(child2->m_estilo.m_limites, fvet4(20, 0, 80, 100));
    });
    // Teste 13 : Teste do container
    testes.adicionar("contanier_vertical", []() {
        //- Preparação:
        becommons::container box; 
        box.m_estilo.m_limites = {0, 0, 100, 100};
        auto [child1, child2] = box.split(0.2, estilo::orientacao::vertical); // divisão horizontal
        box.atualizar();
        //- Verificação:
        ASSERT_EQUAL(child1->m_estilo.m_limites, fvet4(0, 0, 100, 20));
        ASSERT_EQUAL(child2->m_estilo.m_limites, fvet4(0, 20, 100, 80));
    });
    // Teste 14 : Teste do container
    testes.adicionar("contanier_triplo", []() {
        //- Preparação:
        becommons::container box; 
        box.m_estilo.m_limites = {0, 0, 100, 100};
        auto [up, down] = box.split(0.5, estilo::orientacao::vertical); // divisão vertical
        auto [u_left, u_right] = up->split(0.5); // divisão horizontal
        auto [d_left, d_right] = down->split(0.2); // divisão horizontal
        box.atualizar();
        //- Verificação:
        ASSERT_EQUAL(u_left->m_estilo.m_limites, fvet4(0, 0, 50, 50));
        ASSERT_EQUAL(u_right->m_estilo.m_limites, fvet4(50, 0, 50, 50));
        ASSERT_EQUAL(d_left->m_estilo.m_limites, fvet4(0, 50, 20, 50));
        ASSERT_EQUAL(d_right->m_estilo.m_limites, fvet4(20, 50, 80, 50));
    });
}
void testarSistemas()
{
    testarBubbleGUI();
}


int main()
{
	testarUtils();
    testarNucleo();
    testarSistemas();
    try {
        return testes.rodarTestes();
    } 
    catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
        return -1;
    }
}
