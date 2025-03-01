
-- Autor Daniel Oliveira (c) 2025 --

function iniciar()
    print("Iniciando o jogo...")
end

function atualizar()
    -- Suaviza a rotação da câmera com base no movimento do mouse
    local sensibilidade = 0.1
    
    local deltaX = inputs:tamanhoTela().x/2 - inputs:mouse().x
    local deltaY = inputs:tamanhoTela().y/2 - inputs:mouse().y
    inputs.cursor(inputs:tamanhoTela().x/2,inputs:tamanhoTela().y/2)

    eu.transformacao.rotacao.y = eu.transformacao.rotacao.y - deltaX * sensibilidade
    eu.transformacao.rotacao.x = eu.transformacao.rotacao.x + deltaY * sensibilidade
    
    if inputs.pressionada("W") then
        eu.camera:mover(vetor3(0, 0, 1))
    end if inputs.pressionada("S") then
        eu.camera:mover(vetor3(0, 0, -1))
    end if inputs.pressionada("A") then
        eu.camera:mover(vetor3(-1, 0, 0))
    end if inputs.pressionada("D") then
        eu.camera:mover(vetor3(1, 0, 0))
end
   
end
