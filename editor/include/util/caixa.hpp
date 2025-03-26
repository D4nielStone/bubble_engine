/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "util/vetor4.hpp"
    
namespace bubble
{
    /*
     * @struct caixa
     * @brief funciona como uma div em css
     *
     **/
    struct caixa
    {
        /*
         * @enum flag_tam
         * @brief controlo o ajuste de limites
         **/
        enum flag_tam : uint8_t
        {
            nenhum,         //< Sem ajuste de limites
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
}
