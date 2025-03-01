--[[
	Copyright (c) 2025 Daniel Oliveira dos Santos. Todos os direitos reservados.
	Este código é parte do projeto BubbleEngine.
]]
-- arrastando.lua
local M = {}

-- Variáveis internas do módulo
local arrastando = false
local gatilho = false
local tempoDecorrido = 0

function M.arrastando(raio)
    -- Atualizar o tempo decorrido com base no deltaTime
    tempoDecorrido = tempoDecorrido + tempo.obterDeltaTime()

    -- Verificar se o mouse está dentro do raio e o botão esquerdo está pressionado
    if util.distanciaV2(inputs:mouse(), vetor2d(
            inputs:tamanhoTela().x / 2,
            inputs:tamanhoTela().y / 2)) < raio and
            inputs.pressionada("MouseE") and not gatilho
           then
        arrastando = true
        tempoDecorrido = 0 -- Reiniciar o tempo quando começar a arrastar
    end

    -- Atualizar o estado do gatilho e do arrastando
    if not inputs.pressionada("MouseE") then
        gatilho = false
        arrastando = false
    else
        gatilho = true
    end

    -- Retornar o estado do arrastando e o tempo decorrido
    return arrastando, tempoDecorrido
end

return M
