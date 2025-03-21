/** @copyright Copyright (c) 2025 Daniel Oliveira */

#include <iostream>
#include "registro_testes.hpp"
#include "depuracao/assert.hpp"
#include "util/vetor4.hpp"
#include "util/vetor3.hpp"
#include "util/vetor2.hpp"
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "nucleo/fase.hpp"
#include "componentes/fisica.hpp"

betest::registro_testes testes;

void testarVetor4()
{
    testes.classe("BUBBLE", "VETOR4");

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
    testes.classe("BUBBLE", "VETOR3");

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
    testes.classe("BUBBLE", "VETOR2");

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
    testes.classe("BUBBLE", "FASE");
    
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
    testes.classe("BUBBLE", "REGISTRO");
    
    // Teste de inicialização
    
    testes.adicionar("criando_entidades", [](){
        bubble::registro reg;
        bubble::entidade e1 = reg.criar();
        bubble::entidade e2 = reg.criar();
        
        ASSERT_TRUE(e1.id != e2.id);  // IDs devem ser diferentes
        ASSERT_EQUAL(e1.mascara, bubble::componente::COMPONENTE_NONE);
        ASSERT_EQUAL(e2.mascara, bubble::componente::COMPONENTE_NONE);
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
        
            // entidade deve ser removida do mapa
            ASSERT_TRUE(reg.obterComponentes(e.id) == bubble::componente::COMPONENTE_NONE);
            ASSERT_TRUE(reg.entidades.find(e.id) == reg.entidades.end());
        });
        
        testes.adicionar("multiplos_componentes_mascara", []() {
            bubble::registro reg;
            auto e = reg.criar();
        
            reg.adicionar<CompA>(e);
            reg.adicionar<CompB>(e);
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

int main()
{
    depuracao::debug_ativo = false;
	testarUtils();
    testarNucleo();
    testes.rodarTestes();
    return 0;
}
