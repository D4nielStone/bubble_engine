local gatilho = false

function iniciar()
    entidade(4).fisica:defFriccao(0)
    entidade(4).fisica:defRestituicao(1)
    entidade(4).fisica:defFatorAngular(vetor3(1, 0, 0))
    entidade(4).fisica:defFatorLinear(vetor3(0, 0, 0))
    eu.fisica:defFriccao(0)
    eu.fisica:defRestituicao(0)
    eu.fisica:defFatorAngular(vetor3(1, 0, 0))
    eu.fisica:defFatorLinear(vetor3(0, 0, 0))

    eu.fisica:defRaioCcd(0.1)
    eu.fisica:defVelocidade(vetor3(0,0,0))
    entidade(4).fisica:defVelocidade(vetor3(0,0,0))
end

function atualizar()
    entidade(4).fisica:defPosicao(vetor3(entidade(4).transformacao.posicao.x, 0, 0))
    if not gatilho and inputs.pressionada("Scrl") then
   
    
   eu.fisica:defVelocidade(vetor3(-3, 0, 0))
    
   entidade(4).fisica:defVelocidade(vetor3(-15, 0, 0))
        gatilho = true

    end
end
