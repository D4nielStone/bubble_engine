/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "nucleo/sistema.hpp"
#include <unordered_map>
#include <memory>
#include "util/vetor4.hpp"

/**
 *
 * @file bubble_gui.hpp
 * @struct Manuseia a interface do editor
 *
 **/

typedef uint32_t ID;
namespace bubble
{
    /*
     * @struct caixa
     * @brief funciona como uma div em css
     *
     **/
    struct caixa
    {
        enum flag_tam
        {
            nenhum,
            conter,         //< Imagem dentro da caixa
            automatico,     //< Imagem no tamanho original
            cobrir          //< Imagem preenche a caixa
        };
        flag_tam ajuste = automatico;
        vet4 limites {0, 0, 100, 100};
        caixa* filho{nullptr};
        bubble::imagem* imagem{nullptr};

        caixa& operator=(const caixa& outra)
        {
            if(this != &outra)
            {
                limites = outra.limites;
                imagem = outra.imagem;
                ajuste = outra.ajuste;
            }
            return *this;
        }
    };

    /*
     * @struct ancora
     * @brief Uma ancora é um espaço para caixas.
     * @brief Pode ser dividido horizontal, vertica e centralmente ( cria tabs )
     * */

    /*
                                  ancora B
                                     |
                              +- - - | - - -+ 
        +---------------------+-------------+
        |       ancora A      |   ancora C  |
        |                     |-------------|
        |                     |   ancora D  |
        +---------------------+-------------+
    */

    struct ancora
    {
        ancora() = default;
        
        enum orientacao
        {
            horizontal,
            vertical,
            central
        };
        
        caixa corpo;
        vet4 limites {0, 0, 100, 100};
        ID ancora_a{0}, ancora_b{0}; // Se igual à 0, não dividir
        orientacao ori_atual;
        float proporcao = 0.5f;
        ID id;
    };
    class bubble_gui : public bubble::sistema
    {
        private:
            void recursivo(const ID ancora);
            ID proximo_id{0};
        public:
            std::unordered_map<ID, std::shared_ptr<ancora>> ancoras;
            bubble_gui();
            void atualizar() override;
        /*
         * @brief Divide um ancora em uma orientacao
         * @param ancora id da ancora à ser dividida
         * @param orientacao orientação da ancora à ser dividida
         * @return id da nova ancora (ancora_b e c)
         * */
            std::pair<ID, ID> dividir(const ID ancora, const ancora::orientacao);
            
            ID ancora_principal;
    };
}
